#include <stdio.h>
#include <string.h>

enum enumPos {
 POS_LONG,
 POS_SHORT
};

char *position_strings[] = {
  "Long ",
  "Short"
};

#define MAX_LINE_LEN 1024
static char line[MAX_LINE_LEN];

static char usage[] = "usage: position (-debug) (-verbose) (-reset_ixix) multiplier commission filename\n";
static char couldnt_open[] = "couldn't open %s\n";

static void GetLine(FILE *fptr,char *line,int *line_len,int maxllen);

int main(int argc,char **argv)
{
  int curr_arg;
  bool bDebug;
  bool bVerbose;
  bool bHaveResetIx;
  int reset_ix;
  double multiplier;
  double commission;
  char pos;
  enumPos ePos;
  int bought_at;
  int sold_at;
  int temp;
  double position;
  double reset_position;
  int points;
  int sum_points;
  double current;
  FILE *fptr;
  int line_len;
  int line_no;

  if ((argc < 4) || (argc > 7)) {
    printf(usage);
    return 1;
  }

  bDebug = false;
  bVerbose = false;
  bHaveResetIx = false;

  for (curr_arg = 1; curr_arg < argc; curr_arg++) {
    if (!strcmp(argv[curr_arg],"-debug"))
      bDebug = true;
    else if (!strcmp(argv[curr_arg],"-verbose"))
      bVerbose = true;
    else if (!strncmp(argv[curr_arg],"-reset_ix",9)) {
      sscanf(&argv[curr_arg][9],"%d",&reset_ix);
      bHaveResetIx = true;
    }
    else
      break;
  }

  if (argc - curr_arg != 3) {
    printf(usage);
    return 2;
  }

  sscanf(argv[curr_arg],"%lf",&multiplier);
  sscanf(argv[curr_arg+1],"%lf",&commission);

  if ((fptr = fopen(argv[curr_arg+2],"r")) == NULL) {
    printf(couldnt_open,argv[curr_arg+2]);
    return 3;
  }

  position = (double)0;

  if (bHaveResetIx)
    reset_position = (double)0;

  sum_points = 0;
  line_no = 0;

  for ( ; ; ) {
    GetLine(fptr,line,&line_len,MAX_LINE_LEN);

    if (feof(fptr))
      break;

    sscanf(line,"%c %d %d",&pos,&bought_at,&sold_at);

    if (pos == 'L')
      ePos = POS_LONG;
    else if (pos == 'S')
      ePos = POS_SHORT;
    else {
      printf("unknown position character %c at line %d\n",pos,line_no+1);
      return 4;
    }

    if (ePos == POS_SHORT)
      points = bought_at - sold_at;
    else
      points = sold_at - bought_at;

    sum_points += points;

    current = ((double)-2 * commission) + multiplier * (double)points;

    position += current;

    if (bHaveResetIx && (line_no >= reset_ix))
      reset_position += current;

    if (bDebug) {
      if (!bVerbose)
        printf("%lf\n",position);
      else if (!bHaveResetIx || (line_no < reset_ix))
        printf("%lf %s %d %d %d %d %lf\n",position,position_strings[ePos],bought_at,sold_at,sum_points,points,current);
      else
        printf("%lf %s %d %d %d %d %lf %lf\n",position,position_strings[ePos],bought_at,sold_at,sum_points,points,current,reset_position);
    }

    line_no++;
  }

  fclose(fptr);

  if (!bDebug) {
    if (!bVerbose)
      printf("%lf\n",position);
    else
      printf("%lf %s %d %d %d %d %lf\n",position,position_strings[ePos],bought_at,sold_at,sum_points,points,current);
  }

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
