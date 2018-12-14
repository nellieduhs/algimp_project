#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <math.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <stddef.h>

#include "error.h"

bool fm(size_t rows, size_t cols, signed char a[rows][cols], signed char c[cols]);

int main(int argc, char** argv)
{
  int x;
  size_t		rows;
  size_t		rows0;
  size_t		cols;
  int		n;
  FILE*		afile;
  FILE*		cfile;
  FILE*		ofile;
  char		afilename[100];
  char		cfilename[100];
  char		ofilename[100];
  char		output[100];
  snprintf(afilename, sizeof afilename, "A1.3");
  snprintf(cfilename, sizeof cfilename, "c1.3");
  snprintf(ofilename, sizeof ofilename, "output1.3");

  chdir("input");
  afile = fopen(afilename, "r");
  cfile = fopen(cfilename, "r");
  ofile = fopen(ofilename, "r");

  if (afile == NULL) {
    error("failed to open %s", afilename);
  }

  n = fscanf(afile, "%zu %zu", &rows, &cols);
  if (n != 2) {
    error("reading matrix size failed");
  }

  signed char	a[rows][cols];
  signed char	c[cols];

  memset(a, 0, sizeof a);
  memset(c, 0, sizeof c);

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      n = fscanf(afile, "%d", &x);
      if (n != 1){
        error("reading a[%zu][%zu] of %s failed", i, j, argv[0]);
      }
      a[i][j] = x;
      printf("%d ", a[i][j]);
    }
    printf("\n");
  }

  n = fscanf(cfile, "%zu", &rows0);
  if (n != 1)
    error("reading vector size of %s failed", cfilename);

  for (int i = 0; i < cols; ++i) {
    fscanf(cfile, "%d", &x);
    c[i] = x;
  }


  fclose(afile);
  fclose(cfile);

  bool res = fm(rows, cols, a, c);

  fgets(output, sizeof output, ofile);
  fclose(ofile);
  bool differ = (strncmp(output, "False", 5) != 0) ^ res;

  printf("%s\n", res ? "true" : "false");
  printf("%s\n", differ ? "INCORRECT" : "CORRECT");


}
