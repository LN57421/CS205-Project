#include "matrix.h"

int main(int argc, char *argv[]) {
  Matrix *mat1 = (Matrix *)malloc(sizeof(Matrix));
  Matrix *mat2 = (Matrix *)malloc(sizeof(Matrix));

     //创建矩阵测试
//      mat1 = createMatrix(mat1, 10, 10);
//      mat2 = createMatrix(mat2, 10, 10);
//      float count = 1.f;
//      for (int i = 0; i < 10; ++i) {
//        for (int j = 0; j < 10; ++j) {
//          mat1->nums[i * 10 + j] = count;
//          mat2->nums[i * 10 + j] = count;
//          count++;
//        }
//      }
//      Matrix *res;
//      res = createMatrix(res, 10, 10);
//
//
//      multiplyTwoMatrix(res, mat1, mat2);
//
//      printf("The result of is:\n");
//      for (int i = 0; i < 10; ++i) {
//        for (int j = 0; j < 10; ++j) {
//          printf("%.6f ", res->nums[i * 10 + j]);
//        }
//        printf("\n");
//      }
//    int rank = 0;
//    int determination = 0;
//
//    if(rankOfMatrix(&rank,mat1)){
//        printf("The rank of mat1 is: &d\n", rank);
//    }
//    if(deterOfMatrix(&determination,mat2)){
//        printf("The determination of mat2 is: &d\n", determination);
//    }


  //文件读取矩阵
  FILE *fp1, *fp2;
  int mat1_row = 10;
  int mat1_col = 10;
  int mat2_row = 10;
  int mat2_col = 10;
  fp1 = fopen("../data/A1.txt", "r");

  if (fp1 == NULL) {
    printf("Error in opening the first file\n");
    exit(1);
  } else {
    mat1 = createMatrixByFile(mat1, mat1_row, mat1_col, fp1);
  }

  fp2 = fopen("../data/B1.txt", "r");

  if (fp2 == NULL) {
    printf("Error in opening the second file\n");
    exit(1);
  } else {
    mat2 = createMatrixByFile(mat2, mat2_row, mat2_col, fp2);
  }

  FILE *fout = fopen("../data/out.txt", "w");

  if (fout == NULL) {
    printf("Cannot create the file\n");
    exit(1);
  }

  Matrix *res = (Matrix *)malloc(sizeof(Matrix));
  res = createMatrix(res, 10, 10);

//   multiplyTwoMatrix(res, mat1, mat2);
//   addTwoMatrix(res,mat1,mat2);
//   subtractMatrix(res,mat1,mat2);
//    if (copyMatrix(res, mat1)) {
//      printf("The Matrix copy is finish.\n");
//    }
//    if (addByScalar(mat1, 3.14f)) {
//      printf("The Matrix add 3.14 is finish.\n");
//    }
//    if (subByScalar(mat1, 3.14f)) {
//      printf("The Matrix subtract 3.14 is finish.\n");
//    }
//    if (multiplyByScalar(mat1, 3.14f)) {
//      printf("The Matrix subtract 3.14 is finish.\n");
//    }
//    if (divideByScalar(mat1, 3.14f)) {
//      printf("The Matrix  3.14 is finish.\n");
//    }

    int row = res->row;
    int col = res->col;
    char *str_res = (char *)malloc(10 * sizeof(char));
    for (int i = 0; i < row; i++) {
      for (int j = 0; j < col; j++) {
        sprintf(str_res, "%.6f", res->nums[i * col + j]);
        fputs(str_res, fout);
        fputc(' ', fout);
      }
      fputs("\n", fout);
    }


//  float max = 2e9 + 1;
//  float min = -2e9 - 1;
//
//  if (getMax(&max, mat1)) {
//    printf("The max value of mat1: %f\n", max);
//  }
//  if (getMin(&min, mat1)) {
//    printf("The min value of mat2: %f\n", min);
//  }
//  int row = 5;
//  int col = 5;
//  float answer = 0;
//  if (getValue(&answer,mat1,row,col)) {
//    printf("The value of index is: %f\n", answer);
//  }

  fclose(fout);

  if (deleteMatrix((mat1))) {
    printf("matrix1 has been successfully deleted\n");
  }

  if (deleteMatrix((mat2))) {
    printf("matrix2 has been successfully deleted\n");
  }

  if (deleteMatrix((res))) {
    printf("result matrix has been successfully deleted\n");
  }

  return 0;
}
