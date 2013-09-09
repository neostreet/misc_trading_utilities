#include <vector>
#include <stdio.h>

using namespace std;

#define MAX_LINE_LEN 1024
static char line[MAX_LINE_LEN];

static char usage[] = "usage: sma num_periods filename\n";
static char couldnt_open[] = "couldn't open %s\n";

static void GetLine(FILE *fptr,char *line,int *line_len,int maxllen);

int main(int argc,char **argv)
{
  int m;
  int n;
  int num_periods;
  FILE *fptr;
  int line_len;
  int line_no;
  int value;
  vector<int> values;
  int sma_count;
  double dwork;

  if (argc != 3) {
    printf(usage);
    return 1;
  }

  sscanf(argv[1],"%d",&num_periods);

  if ((fptr = fopen(argv[2],"r")) == NULL) {
    printf(couldnt_open,argv[2]);
    return 2;
  }

  line_no = 0;

  for ( ; ; ) {
    GetLine(fptr,line,&line_len,MAX_LINE_LEN);

    if (feof(fptr))
      break;

    line_no++;

    sscanf(line,"%d",&value);
    values.push_back(value);
  }

  fclose(fptr);

  if (line_no < num_periods) {
    printf("not enough data points\n");
    return 3;
  }

  sma_count = line_no - num_periods + 1;

  for (n = 0; n < sma_count; n++) {
    dwork = (double)0;

    for (m = 0; m < num_periods; m++)
      dwork += values[n+m];

    dwork /= (double)num_periods;
    printf("%lf\n",dwork);
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
