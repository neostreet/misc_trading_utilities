#include <stdio.h>
#include <string.h>

static char usage[] =
"usage: position0 (-verbose) multiplier commission trades ticks\n";
static char couldnt_open[] = "couldn't open %s\n";

int main(int argc,char **argv)
{
  int curr_arg;
  bool bVerbose;
  int start_ix;
  double multiplier;
  double commission;
  int trades;
  int ticks;
  double position;

  if ((argc < 5) || (argc > 6)) {
    printf(usage);
    return 1;
  }

  bVerbose = false;

  for (curr_arg = 1; curr_arg < argc; curr_arg++) {
    if (!strcmp(argv[curr_arg],"-verbose"))
      bVerbose = true;
    else
      break;
  }

  if (argc - curr_arg != 4) {
    printf(usage);
    return 2;
  }

  sscanf(argv[curr_arg],"%lf",&multiplier);
  sscanf(argv[curr_arg+1],"%lf",&commission);
  sscanf(argv[curr_arg+2],"%d",&trades);
  sscanf(argv[curr_arg+3],"%d",&ticks);

  position = (double)-1 * commission * trades +
   (double)ticks * multiplier;

  printf("%lf\n",position);

  return 0;
}
