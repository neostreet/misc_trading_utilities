#include <string>
#include <vector>
#include <stdio.h>
#include <string.h>

using namespace std;

enum enumTransType {
 TRANS_TYPE_BUY,
 TRANS_TYPE_SELL
};

#define MAX_DATESTR_LEN 20

struct transaction {
  enumTransType eTransType;
  char entered[MAX_DATESTR_LEN+1];
  char filled[MAX_DATESTR_LEN+1];
  int stop;
  int order_limit;
  int filled_price;
  bool processed;
};

enum enumPos {
 POS_LONG,
 POS_SHORT
};

static char *position_abbrevs[] = {
  "L",
  "S"
};

#define TAB 0x09

#define MAX_LINE_LEN 1024
static char line[MAX_LINE_LEN];

static char usage[] =
"usage: gen_insert_trade multiplier commission filename\n";
static char couldnt_open[] = "couldn't open %s\n";

static void GetLine(FILE *fptr,char *line,int *line_len,int maxllen);
int get_transaction(
  char *line,
  int line_len,
  int line_no,
  char **type_ptr_ptr,
  char **entered_ptr_ptr,
  char **filled_ptr_ptr,
  char **stop_ptr_ptr,
  char **order_limit_ptr_ptr,
  char **filled_price_ptr_ptr
);
char *get_order_type(struct transaction transaction);

int main(int argc,char **argv)
{
  int m;
  int n;
  double multiplier;
  double commission;
  FILE *fptr;
  int line_len;
  int line_no;
  vector<string> lines;
  char *curr_line;
  int retval;
  char *type_ptr;
  char *entered_ptr;
  char *filled_ptr;
  char *stop_ptr;
  char *order_limit_ptr;
  char *filled_price_ptr;
  struct transaction work;
  vector<struct transaction> transactions;
  int ticks;
  double current;
  int trade_no;

  if (argc != 4) {
    printf(usage);
    return 1;
  }

  sscanf(argv[1],"%lf",&multiplier);
  sscanf(argv[2],"%lf",&commission);

  if ((fptr = fopen(argv[3],"r")) == NULL) {
    printf(couldnt_open,argv[3]);
    return 2;
  }

  printf("use trading\n\n");

  line_no = 0;

  for ( ; ; ) {
    GetLine(fptr,line,&line_len,MAX_LINE_LEN);

    if (feof(fptr))
      break;

    line_no++;

    if (line_no >= 3)
      lines.push_back(line);
  }

  fclose(fptr);

  line_no = lines.size();

  for (n = line_no - 1; (n >= 0); n--) {
    curr_line = (char *)lines[n].c_str();
    line_len = strlen(curr_line);
    line_no = lines.size() - n;

    retval = get_transaction(curr_line,line_len,line_no,
      &type_ptr,
      &entered_ptr,
      &filled_ptr,
      &stop_ptr,
      &order_limit_ptr,
      &filled_price_ptr);

    if (retval == -1)
      continue;

    if (retval) {
      printf("get_transaction failed on line %d: %d\n",line_no,retval);
      return 3;
    }

    if (!strcmp(type_ptr,"Buy"))
      work.eTransType = TRANS_TYPE_BUY;
    else if (!strcmp(type_ptr,"Sell"))
      work.eTransType = TRANS_TYPE_SELL;
    else {
      printf("invalid transaction type %s on line %d\n",type_ptr,line_no);
      return 4;
    }

    strcpy(work.entered,entered_ptr);
    strcpy(work.filled,filled_ptr);

    if (!strlen(stop_ptr))
      work.stop = -1;
    else
      sscanf(stop_ptr,"%d",&work.stop);

    if (!strcmp(order_limit_ptr,"Market"))
      work.order_limit = -1;
    else
      sscanf(order_limit_ptr,"%d",&work.order_limit);

    sscanf(filled_price_ptr,"%d",&work.filled_price);
    work.processed = false;
    transactions.push_back(work);
  }

  trade_no = 0;

  for (n = 0; n < transactions.size(); n++) {
    if (!transactions[n].processed) {
      if (transactions[n].eTransType == TRANS_TYPE_BUY) {
        for (m = n + 1; m < transactions.size(); m++) {
          if (!transactions[m].processed && transactions[m].eTransType == TRANS_TYPE_SELL)
            break;
        }
      }
      else {
        for (m = n + 1; m < transactions.size(); m++) {
          if (!transactions[m].processed && transactions[m].eTransType == TRANS_TYPE_BUY)
            break;
        }
      }

      if (m == transactions.size()) {
        printf("couldn't find ending transaction for transaction %d\n",n);
        return 5;
      }

      trade_no++;

      if (transactions[n].eTransType == TRANS_TYPE_BUY)
        ticks = transactions[m].filled_price - transactions[n].filled_price;
      else
        ticks = transactions[n].filled_price - transactions[m].filled_price;

      current = ((double)-2 * commission) + multiplier * (double)ticks;

      printf("insert into trade(type,"
        "entry_order_type,entry_entered,entry_stop,entry_order_limit,entry_filled,entry_filled_price,"
        "exit_order_type,exit_entered,exit_stop,exit_order_limit,exit_filled,exit_filled_price,"
        "ticks,delta)\n");
      printf("values('%s',"
        "'%s','%s',%d,%d,'%s',%d,"
        "'%s','%s',%d,%d,'%s',%d,"
        "%d,%lf);\n",
        ((transactions[n].eTransType == TRANS_TYPE_BUY) ?
          position_abbrevs[POS_LONG] : position_abbrevs[POS_SHORT]),
        get_order_type(transactions[n]),
        transactions[n].entered,
        transactions[n].stop,
        transactions[n].order_limit,
        transactions[n].filled,
        transactions[n].filled_price,
        get_order_type(transactions[m]),
        transactions[m].entered,
        transactions[m].stop,
        transactions[m].order_limit,
        transactions[m].filled,
        transactions[m].filled_price,
        ticks,
        current);

      transactions[m].processed = true;
      transactions[n].processed = true;
    }
  }

  printf("\nquit\n");

  return 0;
}

static void GetLine(FILE *fptr,char *line,int *line_len,int maxllen)
{
  int chara;
  int local_line_len;

  local_line_len = 0;

  for ( ; ; ) {
    chara = fgetc(fptr);

    if (feof(fptr))
      break;

    if (chara == '\n')
      break;

    if (local_line_len < maxllen - 1)
      line[local_line_len++] = (char)chara;
  }

  line[local_line_len] = 0;
  *line_len = local_line_len;
}

int get_transaction(
  char *line,
  int line_len,
  int line_no,
  char **type_ptr_ptr,
  char **entered_ptr_ptr,
  char **filled_ptr_ptr,
  char **stop_ptr_ptr,
  char **order_limit_ptr_ptr,
  char **filled_price_ptr_ptr
)
{
  int n;
  int tab_count;
  int tab_ix;
  char *order_status_ptr;

  tab_count = 0;

  for (n = 0; n < line_len; n++) {
    if (line[n] == TAB)
      tab_count++;
  }

  if (tab_count != 22)
    return 1;

  tab_ix = 0;

  for (n = 0; n < line_len; n++) {
    if (line[n] == TAB) {
      tab_ix++;

      if (tab_ix > 1)
        line[n] = 0;

      switch(tab_ix) {
        case 1:
          *entered_ptr_ptr = &line[n+1];

          break;
        case 2:
          *filled_ptr_ptr = &line[n+1];

          break;
        case 5:
          *type_ptr_ptr = &line[n+1];

          break;
        case 9:
          *stop_ptr_ptr = &line[n+1];

          break;
        case 10:
          *order_limit_ptr_ptr = &line[n+1];

          break;
        case 11:
          *filled_price_ptr_ptr = &line[n+1];

          break;
        case 12:
          order_status_ptr = &line[n+1];

          break;
      }
    }
  }

  if (strcmp(order_status_ptr,"Filled"))
    return -1;

  return 0;
}

enum enumOrderType {
 ORDER_TYPE_MARKET,
 ORDER_TYPE_LIMIT,
 ORDER_TYPE_STOP_MARKET
};

static char *order_types[] = {
  "Market",
  "Limit",
  "Stop Market"
};

char *get_order_type(struct transaction transaction)
{
  if (transaction.stop == -1) {
    if (transaction.order_limit == -1)
      return order_types[ORDER_TYPE_MARKET];
    else
      return order_types[ORDER_TYPE_LIMIT];
  }
  else
    return order_types[ORDER_TYPE_STOP_MARKET];
}
