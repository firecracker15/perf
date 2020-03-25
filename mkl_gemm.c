#include <stdio.h>
#include <stdlib.h>
#include "mkl.h"
#include <time.h>

int main(){
    #define num_batch 1
	#define BILLION 1000000000.0
	int print_flag = 0;

	int m = 27;
	int n = 27;
	int k = 128;
	int i, j;

	float a_data[m * k];
	float b_data[k * n];
	float c_data[m * n];
        
	for(i = 0; i < m * k; i++) {
		a_data[i] = (float)rand()/(float)(RAND_MAX) + 0.01;
	}

	for(i = 0; i < k * n; i++) {
		b_data[i] = (float)rand()/(float)(RAND_MAX) + 0.01;
	}

    for(i = 0; i < m * n; i++) {
		c_data[i] = 0.0;
	}
	
	MKL_INT m_mkl = m;
	MKL_INT n_mkl = n;
	MKL_INT k_mkl = k;

	MKL_INT lda = k;
	MKL_INT ldb = k;
	MKL_INT ldc = n;
        
	float alpha = 1.0;
	float beta = 0.0;

	void print_data(float data[], int r, int c, int ld) {
		if(print_flag) {
			int len = r * c;
			for(i = 0; i < len; i++) {
				printf("%f ", data[i]);
				if((i + 1) % ld == 0)
					printf("\n");
			}
			printf("\n");
		}
	}

	struct timespec start, end;
	int num_iter = 100000;

	clock_gettime(CLOCK_REALTIME, &start);
	// #pragma omp for
	for (i = 0 ; i < num_iter; i++) {
		cblas_sgemm (CblasRowMajor, CblasNoTrans, CblasTrans,
			m_mkl, n_mkl, k_mkl,
			alpha,
			a_data, lda,
			b_data, ldb,
			beta,
			c_data, ldc);
	}
	clock_gettime(CLOCK_REALTIME, &end);

	print_data(a_data, m, k, k);
	print_data(b_data, k, n, k);
	print_data(c_data, m, n, n);

	double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / BILLION;
	printf("time = %f\n", time_spent * 1000 / num_iter);	

	return 0;
}
