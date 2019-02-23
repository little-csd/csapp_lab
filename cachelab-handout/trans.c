/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
#define K 8
#define K1 16
#define K2 8
#define _K 4
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;
    if (N == 32) {
        int t1, t2, t3, t4, t5, t6, t7, t0;
        for (i = 0; i < N; i+=K)
        for (j = 0; j < M; j+=K)
        for(int l = i; l < i+K && l < N; l++) {
            t0 = A[l][j];
            t1 = A[l][j+1];
            t2 = A[l][j+2];
            t3 = A[l][j+3];
            t4 = A[l][j+4];
            t5 = A[l][j+5];
            t6 = A[l][j+6];
            t7 = A[l][j+7];
            B[j][l] = t0;
            B[j+1][l] = t1;
            B[j+2][l] = t2;
            B[j+3][l] = t3;
            B[j+4][l] = t4;
            B[j+5][l] = t5;
            B[j+6][l] = t6;
            B[j+7][l] = t7;
        }
    } else if (N == 61 || M == 61) {
        int t1, t2, t3, t4, t5, t6, t7, t0;
        for (i = 0; i < N; i+=K1)
        for (j = 0; j < M; j+=K2)
        for (int l = i; l < N && l < i + K1; l++) {
            if (j + K2 < M) {
                t0 = A[l][j];
                t1 = A[l][j+1];
                t2 = A[l][j+2];
                t3 = A[l][j+3];
                t4 = A[l][j+4];
                t5 = A[l][j+5];
                t6 = A[l][j+6];
                t7 = A[l][j+7];
                B[j][l] = t0;
                B[j+1][l] = t1;
                B[j+2][l] = t2;
                B[j+3][l] = t3;
                B[j+4][l] = t4;
                B[j+5][l] = t5;
                B[j+6][l] = t6;
                B[j+7][l] = t7;
            } else {
                for (int r = j; r < M; r++)
                    B[r][l] = A[l][r];
            }
        }
    } else {
        int t1, t2, t3, t4, t5, t6, t7, t0;
        for (i = 0; i < N; i+=K) {
            for (j = 0; j < M; j+=K) {
                // 以下循环将A左上转置后移动到B坐上
                // 将A右上转置后移动到B右上
                for(int ii = i; ii < i + 4; ii++) {
                    t0 = A[ii][j];
                    t1 = A[ii][j+1];
                    t2 = A[ii][j+2];
                    t3 = A[ii][j+3];
                    t4 = A[ii][j+4];
                    t5 = A[ii][j+5];
                    t6 = A[ii][j+6];
                    t7 = A[ii][j+7];
                    B[j][ii] = t0;
                    B[j+1][ii] = t1;
                    B[j+2][ii] = t2;
                    B[j+3][ii] = t3;
                    B[j][ii+4] = t4;
                    B[j+1][ii+4] = t5;
                    B[j+2][ii+4] = t6;
                    B[j+3][ii+4] = t7;
                }
                
                // 将A左下角转置并移动到B右上角
                // 将B右上角移动到B左下角
                for(int jj = j; jj < j + 4; jj++) {
                    t0 = A[i+4][jj];
                    t1 = A[i+5][jj];
                    t2 = A[i+6][jj];
                    t3 = A[i+7][jj];
                    t4 = B[jj][i+4];
                    t5 = B[jj][i+5];
                    t6 = B[jj][i+6];
                    t7 = B[jj][i+7];
                    B[jj][i+4] = t0;
                    B[jj][i+5] = t1;
                    B[jj][i+6] = t2;
                    B[jj][i+7] = t3;
                    B[jj+4][i] = t4;
                    B[jj+4][i+1] = t5;
                    B[jj+4][i+2] = t6;
                    B[jj+4][i+3] = t7;
                }
                
                // 将A右下转置后移动到B右下
                t0 = A[i+4][j+4];
                t1 = A[i+4][j+5];
                t2 = A[i+4][j+6];
                t3 = A[i+4][j+7];
                t4 = A[i+5][j+4];
                t5 = A[i+5][j+5];
                t6 = A[i+5][j+6];
                t7 = A[i+5][j+7];
                B[j+4][i+4] = t0;
                B[j+5][i+4] = t1;
                B[j+6][i+4] = t2;
                B[j+7][i+4] = t3;
                B[j+4][i+5] = t4;
                B[j+5][i+5] = t5;
                B[j+6][i+5] = t6;
                B[j+7][i+5] = t7;
                
                t0 = A[i+6][j+4];
                t1 = A[i+6][j+5];
                t2 = A[i+6][j+6];
                t3 = A[i+6][j+7];
                t4 = A[i+7][j+4];
                t5 = A[i+7][j+5];
                t6 = A[i+7][j+6];
                t7 = A[i+7][j+7];
                B[j+4][i+6] = t0;
                B[j+5][i+6] = t1;
                B[j+6][i+6] = t2;
                B[j+7][i+6] = t3;
                B[j+4][i+7] = t4;
                B[j+5][i+7] = t5;
                B[j+6][i+7] = t6;
                B[j+7][i+7] = t7;
            }
        }
    }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

