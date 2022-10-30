#ifndef PROJECT3_MATRIX_H
#define PROJECT3_MATRIX_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int row;
  int col;
  float *nums;
} Matrix;

//直接输入初始化矩阵
Matrix *createMatrix(Matrix *matrix, int row, int col);
//读取文件初始化矩阵
Matrix *createMatrixByFile(Matrix *matrix, int row, int col, FILE *fp);

//删除矩阵
bool deleteMatrix(Matrix *matrix);
//复制矩阵
bool copyMatrix(Matrix *target, const Matrix *source);

//矩阵乘以某个数
bool multiplyByScalar(Matrix *matrix, float scalar);
//矩阵加上某个数
bool addByScalar(Matrix *matrix, float scalar);
//矩阵减去某个数
bool subByScalar(Matrix *matrix, float scalar);
//矩阵除以某个数
bool divideByScalar(Matrix *matrix, float scalar);
//矩阵乘法
bool multiplyTwoMatrix(Matrix *answer, const Matrix *matrix1,
                       const Matrix *matrix2);
//矩阵加法
bool addTwoMatrix(Matrix *answer, const Matrix *matrix1, const Matrix *matrix2);
//矩阵减法
bool subtractMatrix(Matrix *answer, const Matrix *matrix1,
                    const Matrix *matrix2);

//得到某个位置的元素
bool getValue(float *value, const Matrix *matrix, int row, int col);
//更改某个位置的元素
bool replaceCharValue(Matrix *matrix, int row, int col, float target);
//得到矩阵的秩
bool rankOfMatrix(int *rank, const Matrix *source);
//求行列式
bool deterOfMatrix(float *determinate, const Matrix *source);
//得到最小值
bool getMin(float *value, const Matrix *matrix);
//得到最大值
bool getMax(float *value, const Matrix *matrix);
//得到两个矩阵的合并矩阵
bool concatMatrix(Matrix *cat_matrix, const Matrix *matrix1,
                  const Matrix *matrix2, bool by_row);
//得到子矩阵
bool subMatrix(Matrix *sub_matrix, const Matrix *source, int row, int col,
               int index_i, int index_j);
//得到转置矩阵
bool transposeMatrix(Matrix *tans_matrix, const Matrix *source);

#endif  // PROJECT3_MATRIX_H
