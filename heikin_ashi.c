#include <stdio.h>
#include <string.h>

#define MAX_LINE_LEN 1024
static char line[MAX_LINE_LEN];

typedef struct ohlc {
  double open;
  double high;
  double low;
  double close;
} ohlc;

static char usage[] = "usage: heikin_ashi (-first_ha_open_is_open) filename\n";
static char couldnt_open[] = "couldn't open %s\n";

static void GetLine(FILE *fptr,char *line,int *line_len,int maxllen);

int main(int argc,char **argv)
{
  int curr_arg;
  bool bFirstHAOpenIsOpen;
  FILE *fptr;
  int line_len;
  int line_no;
  ohlc regular_candle;
  ohlc prev_ha_candle;
  ohlc ha_candle;

  if ((argc < 2) || (argc > 3)) {
    printf(usage);
    return 1;
  }

  bFirstHAOpenIsOpen = false;

  for (curr_arg = 1; curr_arg < argc; curr_arg++) {
    if (!strcmp(argv[curr_arg],"-first_ha_open_is_open"))
      bFirstHAOpenIsOpen = true;
    else
      break;
  }

  if (argc - curr_arg != 1) {
    printf(usage);
    return 2;
  }

  if ((fptr = fopen(argv[curr_arg],"r")) == NULL) {
    printf(couldnt_open,argv[curr_arg]);
    return 3;
  }

  line_no = 0;

  for ( ; ; ) {
    GetLine(fptr,line,&line_len,MAX_LINE_LEN);

    if (feof(fptr))
      break;

    sscanf(line,"%lf %lf %lf %lf",
      &regular_candle.open,
      &regular_candle.high,
      &regular_candle.low,
      &regular_candle.close);

    if (!line_no) {
      if (bFirstHAOpenIsOpen)
        ha_candle.open = regular_candle.open;
      else
        ha_candle.open = (regular_candle.open + regular_candle.close) /
          (double)2;
    }
    else {
      ha_candle.open = (prev_ha_candle.open + prev_ha_candle.close) /
        (double)2;
    }

    ha_candle.close = (regular_candle.open + regular_candle.high +
      regular_candle.low + regular_candle.close) / (double)4;

    if (ha_candle.open < ha_candle.close) {
      if (regular_candle.high > ha_candle.close)
        ha_candle.high = regular_candle.high;
      else
        ha_candle.high = ha_candle.close;

      if (regular_candle.low < ha_candle.open)
        ha_candle.low = regular_candle.low;
      else
        ha_candle.low = ha_candle.open;
    }
    else {
      if (regular_candle.high > ha_candle.open)
        ha_candle.high = regular_candle.high;
      else
        ha_candle.high = ha_candle.open;

      if (regular_candle.low < ha_candle.close)
        ha_candle.low = regular_candle.low;
      else
        ha_candle.low = ha_candle.close;
    }

    printf("%lf %lf %lf %lf\n",
      ha_candle.open,
      ha_candle.high,
      ha_candle.low,
      ha_candle.close);

    prev_ha_candle = ha_candle;

    line_no++;
  }

  fclose(fptr);

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
