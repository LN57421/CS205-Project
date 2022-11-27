#include <math.h>
#include "OpenBLAS/include/cblas.h"
#include "matrix.h"
//#pragma GCC optimize(2)

int main(int argc, char *argv[])
{
    int test_size = 32000;
    struct timeval t1, t2, t3, t4, t5, t6;
    Matrix *mat1 = createRandomMatrix(test_size,test_size);
    Matrix *mat2 = createRandomMatrix(test_size,test_size);
    Matrix *res_plain = createMatrix(test_size,test_size);
    Matrix *res_improved = createMatrix(test_size, test_size);
    Matrix *res_openBLAS = createMatrix(test_size,test_size);

    gettimeofday(&t1, NULL);

    matmul_plain(res_plain, mat1, mat2);

    gettimeofday(&t2, NULL);

    double time_serial1 = (t2.tv_sec - t1.tv_sec) + ((double)(t2.tv_usec - t1.tv_usec) / 1000000.0);

    printf("plain matrix mul takes %.6f second\n", time_serial1);

    gettimeofday(&t3, NULL);

    matmul_improved(res_improved, mat1, mat2);

    gettimeofday(&t4, NULL);

    double time_serial2 = (t4.tv_sec - t3.tv_sec) + ((double)(t4.tv_usec - t3.tv_usec) / 1000000.0);

    printf("improved matrix mul takes %.6f second\n", time_serial2);

    gettimeofday(&t5, NULL);

    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, mat1->row, mat1->col, mat2->row, 1, mat1->nums, mat1->col, mat2->nums, mat2->col, 0, res_openBLAS->nums, mat2->col);

    gettimeofday(&t6, NULL);

    double time_serial3 = (t6.tv_sec - t5.tv_sec) + ((double)(t6.tv_usec - t5.tv_usec) / 1000000.0);

    printf("OpenBLAS matrix mul takes %.6f second\n", time_serial3);

    // 计算误差并输出
    float cnt = 0.f;
    size_t length = test_size * test_size;

    #pragma omp parallel for
    for (size_t i = 0; i < length; ++i) {
        cnt += fabsf(res_openBLAS->nums[i] - res_improved->nums[i]) / res_openBLAS->nums[i];
    }
    printf("cnt = %f\n", cnt / (float)(test_size * test_size));



//  Matrix *mat1 = (Matrix *)malloc(sizeof(Matrix));
//  Matrix *mat2 = (Matrix *)malloc(sizeof(Matrix));

//  //文件读取矩阵
//  FILE *fp1 = NULL, *fp2 = NULL;
//  int size = 8000;//
//
//  int mat1_row = size;
//  int mat1_col = size;
//  int mat2_row = size;
//  int mat2_col = size;
//
//  fp1 = fopen("input_data/A8k.txt", "r");//
//
//  if (fp1 == NULL)
//  {
//    printf("Error in opening the first file\n");
//    exit(1);
//  }
//  else
//  {
//    mat1 = createMatrixByFile(mat1, mat1_row, mat1_col, fp1);
//  }
//
//  fp2 = fopen("input_data/B8k.txt", "r");//
//
//  if (fp2 == NULL)
//  {
//    printf("Error in opening the second file\n");
//    exit(1);
//  }
//  else
//  {
//    mat2 = createMatrixByFile(mat2, mat2_row, mat2_col, fp2);
//  }
//
//  FILE *fout_plain = fopen("output_plain/out8k.txt", "w");//
//  FILE *fout_improved = fopen("output_improved/out8k.txt", "w");//
//  FILE *fout_openBLAS = fopen("output_openBLAS/out8k.txt", "w");//
//
//  if (fout_plain == NULL || fout_improved == NULL)
//  {
//    printf("Cannot create the file\n");
//    exit(1);
//  }
//
//  Matrix *res_plain = (Matrix *)malloc(sizeof(Matrix));
//  Matrix *res_improved = (Matrix *)malloc(sizeof(Matrix));
//  Matrix *res_openBLAS = (Matrix *)malloc(sizeof(Matrix));
//
//  res_plain = createMatrix(res_plain, mat1_row, mat2_col);
//  res_improved = createMatrix(res_improved, mat1_row, mat2_col);
//  res_openBLAS = createMatrix(res_openBLAS, mat1_row, mat2_col);

//  struct timeval t1, t2, t3, t4, t5, t6;

//  gettimeofday(&t1, NULL);
//
//  matmul_plain(res_plain, mat1, mat2);
//
//  gettimeofday(&t2, NULL);
//
//  double time_serial1 = (t2.tv_sec - t1.tv_sec) + ((double)(t2.tv_usec - t1.tv_usec) / 1000000.0);
//
//  printf("plain matrix mul takes %.10f second\n", time_serial1);

//  gettimeofday(&t3, NULL);
//
//    matmul_improved(res_improved, mat1, mat2);
//
//  gettimeofday(&t4, NULL);
//
//  double time_serial2 = (t4.tv_sec - t3.tv_sec) + ((double)(t4.tv_usec - t3.tv_usec) / 1000000.0);
//
//  printf("improved matrix mul takes %.10f second\n", time_serial2);
//
//  gettimeofday(&t5, NULL);
//
//  cblas_sgemm(CblasRowMajor, CblasTrans, CblasTrans, mat1_row, mat1_col, mat2_row, 1, mat1->nums, mat1_col, mat2->nums, mat2_col, 0, res_openBLAS->nums, mat2_col);
//
//  gettimeofday(&t6, NULL);
//
//  double time_serial3 = (t6.tv_sec - t5.tv_sec) + ((double)(t6.tv_usec - t5.tv_usec) / 1000000.0);
//
//  printf("OpenBLAS matrix mul takes %.10f second\n", time_serial3);
//
//
//  int row = size;
//  int col = size;
//  char *str_res1 = (char *)malloc(10 * sizeof(char));
//  char *str_res2 = (char *)malloc(10 * sizeof(char));
//  char *str_res3 = (char *)malloc(10 * sizeof(char));
//
//  for (int i = 0; i < row; i++)
//  {
//    for (int j = 0; j < col; j++)
//    {
//      sprintf(str_res1, "%.2f", res_plain->nums[i * col + j]);
//      fputs(str_res1, fout_plain);
//      fputc(' ', fout_plain);
//    }
//    fputs("\n", fout_plain);
//  }
//
//  for (int i = 0; i < row; i++)
//  {
//      for (int j = 0; j < col; j++)
//      {
//          sprintf(str_res2,"%.2f", res_improved->nums[i * col + j]);
//          fputs(str_res2, fout_improved);
//          fputc(' ', fout_improved);
//      }
//      fputs("\n", fout_improved);
//  }
//
//  for (int i = 0; i < row; i++)
//  {
//      for (int j = 0; j < col; j++)
//      {
//          sprintf(str_res3,"%.2f", res_openBLAS->nums[i * col + j]);
//          fputs(str_res3, fout_openBLAS);
//          fputc(' ', fout_openBLAS);
//      }
//      fputs("\n", fout_openBLAS);
//  }
//
//    // 计算误差并输出
//    float cnt = 0.f;
//    //打开两个文件
//    fp1 = fopen("output_improved/out8k.txt", "r");//
//    fp2 = fopen("output_openBLAS/out8k.txt", "r");//
//    //检测文件是否安全打开
//    if(fp1 == NULL || fp2 == NULL){
//        printf("Wrong in openning files.");
//        exit(1);
//    }else{
//        for(int i = 0; i < size; i ++){
//            for(int j = 0; j < size; j++){
//                float f1, f2;
//                fscanf(fp1, "%f", &f1);
//                fscanf(fp2, "%f", &f2);
//                cnt += fabsf(f1 - f2) / f1;
//            }
//        }
//    }
//    printf("cnt = %f\n", cnt / ((float) size * size));
//    Matrix *mat1 = (Matrix *)malloc(sizeof(Matrix));
//    Matrix *mat2 = (Matrix *)malloc(sizeof(Matrix));
//
//    //文件读取矩阵
//    FILE *fp1 = NULL, *fp2 = NULL;
//    int size = 64000;
//
//    FILE *fout = fopen("input_data/B64k.txt", "w");
//
//    if (fout == NULL)
//    {
//        printf("Cannot create the file\n");
//        exit(1);
//    }
//
//    int row = size;
//    int col = size;
//    char *str_res = (char *)malloc(10 * sizeof(char));
//    srand((unsigned)time(NULL));
//    for (int i = 0; i < row; i++)
//    {
//        for (int j = 0; j < col; j++)
//        {
//            float temp =  (rand() % (100 * 10 - 1)) / 100.0;
////      sprintf(str_res, "%.6f", res->nums[i * col + j]);
//            sprintf(str_res,"%.1f",temp);
//            fputs(str_res, fout);
//            fputc(' ', fout);
//        }
//        fputs("\n", fout);
//    }
//
//    fclose(fout);

    return 0;
}
