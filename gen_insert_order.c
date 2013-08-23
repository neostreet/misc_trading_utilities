#include <stdio.h>
#include <string.h>

#define TAB 0x09

#define MAX_LINE_LEN 1024
static char line[MAX_LINE_LEN];

static char usage[] = "usage: gen_insert_order filename\n";
static char couldnt_open[] = "couldn't open %s\n";

static void GetLine(FILE *fptr,char *line,int *line_len,int maxllen);
int gen_insert_order(char *line,int line_len,int line_no);

int main(int argc,char **argv)
{
  FILE *fptr;
  int line_len;
  int line_no;
  int retval;

  if (argc != 2) {
    printf(usage);
    return 1;
  }

  printf("use trading\n\n");

  if ((fptr = fopen(argv[1],"r")) == NULL) {
    printf(couldnt_open,argv[1]);
    return 2;
  }

  line_no = 0;

  for ( ; ; ) {
    GetLine(fptr,line,&line_len,MAX_LINE_LEN);

    if (feof(fptr))
      break;

    line_no++;

    retval = gen_insert_order(line,line_len,line_no);

    if (retval) {
      printf("gen_insert_order failed on line %d: %d\n",line_no,retval);
      return 3;
    }
  }

  fclose(fptr);

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

int gen_insert_order(char *line,int line_len,int line_no)
{
  int n;
  int tab_count;
  int tab_ix;
  char *entered_ptr;
  char *filled_ptr;
  char *symbol_ptr;
  char *type_ptr;
  char *quantity_ptr;
  char *qty_filled_ptr;
  char *qty_left_ptr;
  char *stop_ptr;
  char *order_limit_ptr;
  char *filled_price_ptr;
  char *order_status_ptr;
  char *commission_ptr;

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
          entered_ptr = &line[n+1];

          break;
        case 2:
          filled_ptr = &line[n+1];

          break;
        case 3:
          symbol_ptr = &line[n+1];

          break;
        case 5:
          type_ptr = &line[n+1];

          break;
        case 6:
          quantity_ptr = &line[n+1];

          break;
        case 7:
          qty_filled_ptr = &line[n+1];

          break;
        case 8:
          qty_left_ptr = &line[n+1];

          break;
        case 9:
          stop_ptr = &line[n+1];

          break;
        case 10:
          order_limit_ptr = &line[n+1];

          break;
        case 11:
          filled_price_ptr = &line[n+1];

          break;
        case 12:
          order_status_ptr = &line[n+1];

          break;
        case 21:
          commission_ptr = &line[n+2];

          break;
      }
    }
  }

  printf("insert into tradestation_order(\n");

  if (!strlen(qty_filled_ptr))
    printf("  entered,filled,symbol,type,quantity,qty_left,\n");
  else
    printf("  entered,filled,symbol,type,quantity,qty_filled,qty_left,\n");

  if (!strlen(stop_ptr))
    printf("  order_limit,\n");
  else
    printf("  stop,order_limit,\n");

  if (!strlen(filled_price_ptr))
    printf("  order_status,commission\n");
  else
    printf("  filled_price,order_status,commission\n");

  printf(") values (\n");

  if (!strlen(qty_filled_ptr)) {
    printf("  '%s','%s','%s','%s',%s,%s,\n",entered_ptr,filled_ptr,symbol_ptr,
      type_ptr,quantity_ptr,qty_left_ptr);
  }
  else {
    printf("  '%s','%s','%s','%s',%s,%s,%s,\n",entered_ptr,filled_ptr,symbol_ptr,
      type_ptr,quantity_ptr,qty_filled_ptr,qty_left_ptr);
  }

  if (!strlen(stop_ptr))
    printf("  '%s',\n",order_limit_ptr);
  else
    printf("  %s,'%s',\n",stop_ptr,order_limit_ptr);

  if (!strlen(filled_price_ptr))
    printf("  '%s',%s\n",order_status_ptr,commission_ptr);
  else
    printf("  %s,'%s',%s\n",filled_price_ptr,order_status_ptr,commission_ptr);

  printf(");\n");

  return 0;
}
