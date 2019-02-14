#include <stdio.h>

#define K 8
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;
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
int main()
{
    int M = 8, N = 8;
    int A[N][M], B[M][N];
    for(int i = 0; i < N; i++)
        for(int j = 0; j < M; j++) {
            B[i][j] = 0;
            A[i][j] = i * M + j; 
        }
    transpose_submit(M, N, A, B);
    for(int i = 0; i < N; i++) {
        printf("\n");
        for(int j = 0; j < M; j++) {
            printf("%d ", B[i][j]);
        }
    }
    return 0;
}
