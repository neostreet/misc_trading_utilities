#include <vector>
#include <stdio.h>
#include <string.h>

using namespace std;

#define MAX_LINE_LEN 1024
static char line[MAX_LINE_LEN];

static char usage[] = "usage: sma (-truncate) num_periods filename\n";
static char couldnt_open[] = "couldn't open %s\n";

static void GetLine(FILE *fptr,char *line,int *line_len,int maxllen);

int main(int argc,char **argv)
{
  int m;
  int n;
  int curr_arg;
  bool bTruncate;
  int num_periods;
  FILE *fptr;
  int line_len;
  int line_no;
  int value;
  vector<int> values;
  int sma_count;
  double dwork;
  int work;

  if ((argc < 3) || (argc > 4)) {
    printf(usage);
    return 1;
  }

  bTruncate = false;

  for (curr_arg = 1; curr_arg < argc; curr_arg++) {
    if (!strcmp(argv[curr_arg],"-truncate"))
      bTruncate = true;
    else
      break;
  }

  if (argc - curr_arg != 2) {
    printf(usage);
    return 2;
  }

  sscanf(argv[curr_arg],"%d",&num_periods);

  if ((fptr = fopen(argv[curr_arg+1],"r")) == NULL) {
    printf(couldnt_open,argv[curr_arg+1]);
    return 3;
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
    return 4;
  }

  sma_count = line_no - num_periods + 1;

  for (n = 0; n < sma_count; n++) {
    if (!bTruncate)
      dwork = (double)0;
    else
      work = 0;

    for (m = 0; m < num_periods; m++) {
      if (!bTruncate)
        dwork += (double)values[n+m];
      else
        work += values[n+m];
    }

    if (!bTruncate) {
      dwork /= (double)num_periods;
      printf("%lf\n",dwork);
    }
    else {
      work /= num_periods;
      printf("%d\n",work);
    }
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
