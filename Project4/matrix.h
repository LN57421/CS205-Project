#ifndef PROJECT4_MATRIX_H
#define PROJECT4_MATRIX_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct Matrix_{
    size_t row;
    size_t col;
    float *nums;
} Matrix;

//直接输入初始化矩阵
Matrix *createMatrix(size_t row, size_t col);
//读取文件初始化矩阵
Matrix *createMatrixByFile(size_t row, size_t col, FILE *fp);
//随机初始化矩阵
Matrix *createRandomMatrix(size_t row, size_t col);

//删除矩阵
bool deleteMatrix(Matrix *matrix);
//复制矩阵
bool copyMatrix(Matrix *target, const Matrix *source);

//朴素矩阵乘法
bool matmul_plain(Matrix *answer, const Matrix *matrix1,const Matrix *matrix2);
//SIMD和OpenMP加速朴素矩阵乘法
bool matmul_improved(Matrix *answer, const Matrix *matrix1, const Matrix *matrix2);
//使用Strassen算法计算矩阵乘法
bool mulMatrix_strassen(Matrix *answer,const Matrix *matrix1,const Matrix *matrix2, size_t size);

//矩阵乘以某个数
bool multiplyByScalar(Matrix *matrix, float scalar);
//矩阵加上某个数
bool addByScalar(Matrix *matrix, float scalar);
//矩阵减去某个数
bool subByScalar(Matrix *matrix, float scalar);
//矩阵除以某个数
bool divideByScalar(Matrix *matrix, float scalar);
//矩阵加法
bool addTwoMatrix(Matrix *answer, const Matrix *matrix1, const Matrix *matrix2);
//矩阵减法
bool subtractMatrix(Matrix *answer, const Matrix *matrix1,const Matrix *matrix2);

//得到某个位置的元素
bool getValue(float *value, const Matrix *matrix, size_t row, size_t col);
//更改某个位置的元素
bool replaceCharValue(Matrix *matrix, size_t row, size_t col, float target);
//得到最小值
bool getMin(float *value, const Matrix *matrix);
//得到最大值
bool getMax(float *value, const Matrix *matrix);

#endif //PROJECT4_MATRIX_H
