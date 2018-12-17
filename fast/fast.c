#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

bool fm(size_t rows, size_t cols, signed char a[rows][cols], signed char c[rows])
{
	int r = rows;
	int s = cols;
	int n1 = 0;
	int n2 = 0;

	//Looking at last column of A. Find
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

	double **t = malloc(r * sizeof(double*));
	for(int i = 0; i < r; i++){
		t[i] = malloc(s * sizeof(double));
	}
	double *q = malloc(s * sizeof(double));

	//memset(t, 0, sizeof t);
	//memset(q, 0, sizeof q);


	// Put in elements from a to t sorted by last column,
	// first positive, then negative and last zero.
	// q sorted in same manner with corresponding elements from c
	for (int i = 0; i < s; i++){
		if ((double) a[r-1][i] > 0){
			for(int k = 0; k < r; k++){
			t[k][pos] = a[k][i];
		//	printf("Positive : %d\n",a[i][k]);
			//printf("Positive t : %f\n",t[pos][k]);
			}
			q[pos] = c[i];
			pos++;

		} else if ( (double) a[r-1][i] <0) {
			for(int k = 0; k < r; k++){
			t[k][neg] = a[k][i];
			printf("Negative : %d\n",a[k][i]);
			}
			printf("T00 vid %d : %f\n",i , t[0][0]);
			q[neg] = c[i];
			printf("Ci : %d\n",c[i]);///////________ALERT
			neg++;
		} else {
			for(int k = 0; k < r; k++){
			t[k][z] = a[k][i];
			}
			q[z] = c[i];
			z++;
		}
		printf("T00 vid %d : %f\n",i , t[0][0]);
	}

	for(int i = 0; i < r; i ++){
		for(int k = 0; k < s; k++){
			printf("%f ", t[i][k]);
		}
		printf("\n");
	}


	// rows with absolute value of last element larger than zero
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
		int b = INT_MIN;
		for (int i = n1; i < n2; i++){
			if(q[i] > b){
				b = q[i];
			}
		}
		// find B1, i.e. min value of q for "positive equations"
		int B = INT_MAX;;
		for (int i = 0; i < n1; i++){
			if(q[i] < B){
				B = q[i];
			}
		}
		// find if any value of q for "zero equations" is less than 0
		bool q_neg = false;
		for (int i =n2; i < s; i++){
			if(q[i] < 0){
				q_neg = true;
				break;
			}
		}

		for (int i = 0; i < r; i++){
    //int* currentIntPtr = ptr[i];
    free(t[i]);
		}
		free(q);

		return false;
		//return (b < B) && !q_neg;
	} else {
		int snew = s - n2 + n1*(n2-n1);
		printf("snew: %d\n", snew);
		// Step 7
		if (snew > 0){
			printf("snew: %d\n", snew);
			double tnew[snew][r-1];
			double qnew[snew];
		//
			int j = 0;
			for(int k = 0; k < n1; k++){
				for(int l = n1; l < n2; l++){
					for(int i = 0; i < r-1 ; i++){
						tnew[i][j] = t[i][k] - t[i][l];
						qnew[j] = q[k] - q[l];
					}
					j++;
				}
			}

			// Zeros stay the same. s-n2 ineq put directly into eq-system
			for(int k = j; k < s; k++){
				for(int i = 0; i < r-1;i++){
					tnew[k][i] = t[k][i];
				}
				qnew[k] = q[k];
			}

			for (int i = 0; i < r; i++){
	    double* currentPtr = t[i];
	    free(currentPtr);
		}
			//free(q);
			printf("Q is the bov");

			return fm((size_t) r-1, (size_t) snew, tnew, qnew);
		} else {
			printf("T is the bov");
			for (int i = 0; i < r; i++){
			double* currentPtr = t[i];
	    free(currentPtr);
			}
			printf("Q is the bov");
			free(q);
			printf("FUCK");
			return true;
		}
	}
}
