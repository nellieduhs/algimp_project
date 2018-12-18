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

  printf("n1: %d, n2: %d\n", n1, n2);
  printf("r: %d, s: %d\n",r, s);

  //double t[r][s];

  int pos=0;
  int neg=n1;
  int z=n2;
  //double q[s];

  double **t = alloca(r * sizeof(double*));
  for(int i = 0; i < r; i++){
    t[i] = alloca(s * sizeof(double));
  }
  double *q = alloca(s * sizeof(double));

  //memset(t, 0, sizeof t);
  //memset(q, 0, sizeof q);


  // Put in elements from a to t, columns rearranged sorted by their last elem,
  // first positive, then negative and last zero.
  // q sorted in same manner with corresponding elements from c
  for (int i = 0; i < s; i++){
    if ((double) a[r-1][i] > 0){
      for(int k = 0; k < r; k++){
      t[k][pos] = a[k][i];
    //  printf("Positive : %d\n",a[i][k]);
      //printf("Positive t : %f\n",t[pos][k]);
      }
      q[pos] = c[i];
      pos++;

    } else if ( (double) a[r-1][i] <0) {
      for(int k = 0; k < r; k++){
      t[k][neg] = a[k][i];
      //printf("Negative : %d\n",a[k][i]);
      }
      //printf("T00 vid %d : %f\n",i , t[0][0]);
      q[neg] = c[i];
      //printf("Ci : %d\n",c[i]);///////________ALERT
      neg++;
    } else {
      for(int k = 0; k < r; k++){
      t[k][z] = a[k][i];
      }
      q[z] = c[i];
      z++;
    }
    //printf("T00 vid %d : %f\n",i , t[0][0]);
  }

  // Free input matrices
  for (int i = 0; i < r; i++){
  //int* currentIntPtr = ptr[i];
  free(a[i]);
  }
  free(c);

  //print for testing
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
  printf("\n");





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

   //print for testing
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
  printf("\n");

  // Step 6
  if (r == 1) {
    //find b1, i.e. max value of q for "negative equations"
    double b = INT_MIN;
    for (int i = n1; i < n2; i++){
      if(q[i] > b){
        b = q[i];
      }
    }
    printf("b: %f\n", b);

    // find B1, i.e. min value of q for "positive equations"
    double B = INT_MAX;;
    for (int i = 0; i < n1; i++){
      if(q[i] < B){
        B = q[i];
      }
    }
    printf("B: %f\n", B);

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

    for (int i = 0; i < r; i++){
    //int* currentIntPtr = ptr[i];
    free(t[i]);
    }
    free(q);

    return (b < B) && !q_neg;
  } else {
    int snew = s - n2 + n1*(n2-n1);
    printf("snew: %d\n", snew);
    // Step 7
    if (snew > 0){
      printf("snew: %d\n", snew);


      double **tnew = malloc((r-1) * sizeof(double*));
      for(int i = 0; i < (r-1); i++){
        t[i] = malloc(snew * sizeof(double));
      }
      double *qnew = malloc(snew * sizeof(double));

      int j = 0;
      for(int k = 0; k < n1; k++){
        for(int l = n1; l < n2; l++){
          for(int i = 0; i < (r-1) ; i++){
            tnew[i][j] = (t[i][k] - t[i][l]);
            printf("j: %d, k: %d, l: %d, i: %d\n", j, k, l, i);
          }
          qnew[j] = (q[k] - q[l]);
          j++;
          if(j > snew){
            printf("INDEX ERROR\n");
          }
        }
      }

      printf("Error in second matrix index, not allocation...\n");
      //print for testing
      printf("tnew: \n");
      for(int i = 0; i < r-1; i ++){
        for(int k = 0; k < snew; k++){
          printf("%f ", tnew[i][k]);
        }
        printf("\n");
      }

      printf("qnew: \n");
      for(int k = 0; k < snew; k++){
          printf("%f ", qnew[k]);
      }
      printf("\n");

      // Zeros stay the same. s-n2 ineq put directly into eq-system
      if (n2 < s) {
        for(int k = j; k < s; k++){
          for(int i = 0; i < r-1;i++){
            tnew[k][i] = t[k][i];
          }
          qnew[k] = q[k];
        }
      }

      /*for (int i = 0; i < r; i++){
      //double* currentPtr = t[i];
      free(t[i]);
      }
      free(q);*/

      return solve((size_t) r-1, (size_t) snew, tnew, qnew);
    } else {
      printf("T is the bov");
      /*for (int i = 0; i < r; i++){
      double* currentPtr = t[i];
      free(currentPtr);
      }
      printf("Q is the bov");
      free(q);*/
      printf("FUCK");
      return true;
    }
  }
}
