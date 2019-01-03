#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

bool solve(size_t rows, size_t cols,  double** a, double* c);

bool fm(size_t rows, size_t cols, signed char a[rows][cols], signed char c[rows])
{
  // Transpose input matrix to match algorithm xA = c. Currently on form Ax = c.
  // allocate memory for double matrix
  double **t = malloc(cols * sizeof(double*));
  for(int i = 0; i < cols; i++){
    t[i] = malloc(rows * sizeof(double));
  }
  double *q = malloc(rows * sizeof(double));

  for(int r = 0; r < rows; r++){
    for(int c = 0; c < cols; c++){
      t[c][r] = a[r][c];
    }
    q[r] = c[r];
  }
  return solve(cols, rows, t, q);
}

bool solve(size_t rows, size_t cols, double** a, double* c){

  int r = rows;
  int s = cols;
  int n1 = 0;
  int n2 = 0;



  //Looking at last row of A. Find
  //number of postive elements, n1
  //number of negative + positive elements, n2
  for (int i = 0; i < s; i++){
    if (a[r-1][i] > 0) {
      n1++;
      n2++;
    } else if (a[r-1][i] < 0)  n2++;
  }

  //printf("n1: %d, n2: %d\n", n1, n2);
  //printf("r: %d, s: %d\n",r, s);

  double t[r][s];
  double q[s];

  int pos=0;
  int neg=n1;
  int z=n2;


  // Put in elements from a to t, columns rearranged sorted by their last elem,
  // first positive, then negative and last zero.
  // q sorted in same manner with corresponding elements from c
  for (int i = 0; i < s; i++){
    if ((double) a[r-1][i] > 0){
      for(int k = 0; k < r; k++){
      t[k][pos] = a[k][i];
      }
      q[pos] = c[i];
      pos++;

    } else if ( (double) a[r-1][i] <0) {
      for(int k = 0; k < r; k++){
      t[k][neg] = a[k][i];
      }
      q[neg] = c[i];
      neg++;
    } else {
      for(int k = 0; k < r; k++){
      t[k][z] = a[k][i];
      }
      q[z] = c[i];
      z++;
    }
  }


  /*//print for testing
  printf("t: \n");
  for(int i = 0; i < r; i ++){
    for(int k = 0; k < s; k++){
      printf("%f ", t[i][k]);
    }
    printf("\n");
  }

  printf("q: \n");
  for(int k = 0; k < s; k++){
      printf("%f ", q[k]);
  }
  printf("\n");*/

  // Free input matrices
  for (int i = 0; i < r; i++){
    double* currentPtr = a[i];
    free(currentPtr);
  }
  free(a);
  free(c);

  // columns with last element nonzero, and corresponding elem in q,
  // divided by this element.
  // Note: inequality sign switches direction for negative values,
  // i.e. rows n1 <= r < n2
  for (int i = 0; i < n2; i++){
    double norm_factor = t[r-1][i];
    for (int k = 0; k < r; k++){
      t[k][i] = t[k][i]/norm_factor;
    }
    q[i] = q[i]/norm_factor;
  }

  // Step 6
  if (r == 1) {
    //find b1, i.e. max value of q for "negative equations"
    double b = INT_MIN;
    for (int i = n1; i < n2; i++){
      if(q[i] > b){
        b = q[i];
      }
    }

    // find B1, i.e. min value of q for "positive equations"
    double B = INT_MAX;;
    for (int i = 0; i < n1; i++){
      if(q[i] < B){
        B = q[i];
      }
    }

    // find if any value of q for "zero equations" is less than 0
    bool q_neg = false;
    if (n2 < s) {
      for (int i = n2; i < s; i++){
        if(q[i] < 0){
          q_neg = true;
          break;
        }
      }
    }

    return (b < B) && !q_neg;
  } else {
    int snew = s - n2 + n1*(n2-n1);
    // Step 7
    if (snew > 0){
      size_t rnew = r-1;

      double **tnew = malloc(rnew * sizeof(double*));
      for(int i = 0; i < rnew; i++){
        tnew[i] = malloc(snew * sizeof(double));
      }
      double *qnew = malloc(snew * sizeof(double));

      int j = 0;
      for(int k = 0; k < n1; k++){
        for(int l = n1; l < n2; l++){
          for(int i = 0; i < rnew ; i++){
            tnew[i][j] = (t[i][k] - t[i][l]);
          }
          qnew[j] = (q[k] - q[l]);
          j++;
          if(j > snew){
            printf("INDEX ERROR\n");
          }
        }
      }

      // Zeros stay the same. s-n2 ineq put directly into eq-system
      for(int k = n2; k < s; k++){
        for(int i = 0; i < rnew; i ++){
          tnew[i][j] = t[i][k];
         }
         qnew[j] = q[k];
         j++;
         if(j > snew){
           printf("INDEX ERROR\n");
         }
      }

      return solve((size_t) rnew, (size_t) snew, tnew, qnew);
    } else {
      return true;
    }
  }
}
