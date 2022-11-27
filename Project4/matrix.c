#include "matrix.h"
#include <math.h>
#include <string.h>
#include <immintrin.h>

#define MIN(i, j) (((i) < (j)) ? (i) : (j))
#define MAX(i, j) (((i) > (j)) ? (i) : (j))

Matrix * createMatrix(size_t row, size_t col){
    Matrix *matrix = NULL;
    if (row == 0 || col == 0) {
        fprintf(stderr, "Row or Col is zero.\n");
        return NULL;
    }
    matrix = (Matrix *) malloc(sizeof(Matrix));
    if (matrix == NULL) {
        fprintf(stderr, "Original matrix point is NULL\n");
        return NULL;
    }
    //为矩阵结构各结构体变量赋值
    matrix->row = row;
    matrix->col = col;
    matrix->nums = (float *) aligned_alloc(256,sizeof(float) * row * col);
    if(matrix->nums == NULL){
        fprintf(stderr, "Failed to allocate memory for the matrix data.\n");
        free(matrix->nums);
        return NULL;
    }
    return matrix;
}

Matrix *createMatrixByFile(size_t row, size_t col, FILE *fp) {
    Matrix *matrix = NULL;

    if (row == 0 || col == 0) {
        fprintf(stderr, "Row or Col is zero.\n");
        return NULL;
    }
    if (fp == NULL) {
        printf("File point is NULL.\n");
        return NULL;
    }
    matrix = (Matrix *) malloc(sizeof(Matrix));
    if (matrix == NULL) {
        fprintf(stderr, "Original matrix point is NULL\n");
        return NULL;
    }
    //为矩阵结构各结构体变量赋值
    matrix->row = row;
    matrix->col = col;
    matrix->nums = (float *) aligned_alloc(256,sizeof(float) * row * col);
    if(matrix->nums == NULL){
        fprintf(stderr, "Failed to allocate memory for the matrix data.\n");
        free(matrix->nums);
        return NULL;
    }
    for (size_t i = 0; i < row; i++) {
        for (size_t j = 0; j < col; j++) {
            fscanf(fp, "%f", &(matrix->nums[i * col + j]));
        }
    }
    fclose(fp);  //关闭文件
    return matrix;
}

Matrix *createRandomMatrix(size_t row, size_t col) {
    Matrix *matrix = NULL;
    if (row == 0 || col == 0) {
        fprintf(stderr, "Row or Col is zero.\n");
        return NULL;
    }
    matrix = (Matrix *) malloc(sizeof(Matrix));
    if (matrix == NULL) {
        fprintf(stderr, "Original matrix point is NULL\n");
        return NULL;
    }
    //为矩阵结构各结构体变量赋值
    matrix->row = row;
    matrix->col = col;
    matrix->nums = (float *) aligned_alloc(256,sizeof(float) * row * col);
    if(matrix->nums == NULL){
        fprintf(stderr, "Failed to allocate memory for the matrix data.\n");
        free(matrix->nums);
        return NULL;
    }
    for (size_t i = 0; i < row; i++) {
        for (size_t j = 0; j < col; j++) {
            matrix->nums[i * col + j] =  (float) ((rand() % 1000) / 10.0);
        }
    }
    return matrix;
}

bool matmul_improved(Matrix *answer, const Matrix *matrix1, const Matrix *matrix2){
    if(matrix1 == NULL){
        fprintf(stderr,"File %s, Line %d, Function %s(): The matrix1 is NULL.\n",
                __FILE__,__LINE__,__FUNCTION__ );
        return false;
    }
    else if(matrix1->nums == NULL){
        fprintf(stderr,"%s(): The matrix1 parameter is NULL.\n",__FUNCTION__ );
        return false;
    }
    if(matrix2 == NULL){
        fprintf(stderr,"File %s, Line %d, Function %s(): The matrix2 is NULL.\n",
                __FILE__,__LINE__,__FUNCTION__ );
        return false;
    }
    else if(matrix2->nums == NULL){
        fprintf(stderr,"%s(): The matrix2 parameter is NULL.\n",__FUNCTION__ );
        return false;
    }
    if(answer == NULL){
        fprintf(stderr,"File %s, Line %d, Function %s(): The answer matrix is NULL.\n",
                __FILE__,__LINE__,__FUNCTION__ );
        return false;
    }
    else if(answer->nums == NULL){
        fprintf(stderr,"%s(): The answer parameter is NULL.\n",__FUNCTION__ );
        return false;
    }

    if( matrix1->col != matrix2->row ||
        matrix1->row != answer->row || matrix2->col != answer->col){
        fprintf(stderr,"The input and output don't match. They should have same size");
        fprintf(stderr, "Their sizes are (%zu, %zu), (%zu, %zu) and (%zu, %zu)",
                matrix1->row, matrix1->col,
                matrix2->row, matrix2->col,
                answer->row, answer->col);
        return false;
    }

    if(matrix1->row <= 128){
        matmul_plain(answer, matrix1, matrix2);
    }

    size_t row = matrix1 -> row;
    size_t col = matrix1 -> col;

    #pragma omp parallel for
    for (size_t m = 0; m < row; m += 8) {
        for (size_t k = 0; k < col; k += 8) {
            __m256 C0v, C1v, C2v, C3v, C4v, C5v, C6v, C7v, B0v, B1v, B2v, B3v, B4v, B5v, B6v, B7v;
            for (size_t n = 0; n < matrix1->col; n += 8) {
                C0v = _mm256_loadu_ps(&answer->nums[ m * answer->col + k]);
                C1v = _mm256_loadu_ps(&answer->nums[ (m+1) * answer->col + k]);
                C2v = _mm256_loadu_ps(&answer->nums[ (m+2) * answer->col + k]);
                C3v = _mm256_loadu_ps(&answer->nums[ (m+3) * answer->col + k]);
                C4v = _mm256_loadu_ps(&answer->nums[ (m+4) * answer->col + k]);
                C5v = _mm256_loadu_ps(&answer->nums[ (m+5) * answer->col + k]);
                C6v = _mm256_loadu_ps(&answer->nums[ (m+6) * answer->col + k]);
                C7v = _mm256_loadu_ps(&answer->nums[ (m+7) * answer->col + k]);

                B0v = _mm256_loadu_ps(&matrix2->nums[n * matrix2->col + k]);

                C0v += matrix1->nums[ m * row + n] * B0v;
                C1v += matrix1->nums[ (m+1) * row + n] * B0v;
                C2v += matrix1->nums[ (m+2) * row + n] * B0v;
                C3v += matrix1->nums[ (m+3) * row + n] * B0v;
                C4v += matrix1->nums[ (m+4) * row + n] * B0v;
                C5v += matrix1->nums[ (m+5) * row + n] * B0v;
                C6v += matrix1->nums[ (m+6) * row + n] * B0v;
                C7v += matrix1->nums[ (m+7) * row + n] * B0v;

                B1v = _mm256_loadu_ps(&matrix2->nums[(n + 1) * matrix2->col + k]);

                C0v += matrix1->nums[ m * row + (n+1)] * B1v;
                C1v += matrix1->nums[ (m+1) * row + (n+1)] * B1v;
                C2v += matrix1->nums[ (m+2) * row + (n+1)] * B1v;
                C3v += matrix1->nums[ (m+3) * row + (n+1)] * B1v;
                C4v += matrix1->nums[ (m+4) * row + (n+1)] * B1v;
                C5v += matrix1->nums[ (m+5) * row + (n+1)] * B1v;
                C6v += matrix1->nums[ (m+6) * row + (n+1)] * B1v;
                C7v += matrix1->nums[ (m+7) * row + (n+1)] * B1v;

                B2v = _mm256_loadu_ps(&matrix2->nums[(n + 2) * matrix2->col + k]);

                C0v += matrix1->nums[ m * row + (n+2)] * B2v;
                C1v += matrix1->nums[ (m+1) * row + (n+2)] * B2v;
                C2v += matrix1->nums[ (m+2) * row + (n+2)] * B2v;
                C3v += matrix1->nums[ (m+3) * row + (n+2)] * B2v;
                C4v += matrix1->nums[ (m+4) * row + (n+2)] * B2v;
                C5v += matrix1->nums[ (m+5) * row + (n+2)] * B2v;
                C6v += matrix1->nums[ (m+6) * row + (n+2)] * B2v;
                C7v += matrix1->nums[ (m+7) * row + (n+2)] * B2v;

                B3v = _mm256_loadu_ps(&matrix2->nums[(n + 3) * matrix2->col + k]);

                C0v += matrix1->nums[ m * row + (n+3)] * B3v;
                C1v += matrix1->nums[ (m+1) * row + (n+3)] * B3v;
                C2v += matrix1->nums[ (m+2) * row + (n+3)] * B3v;
                C3v += matrix1->nums[ (m+3) * row + (n+3)] * B3v;
                C4v += matrix1->nums[ (m+4) * row + (n+3)] * B3v;
                C5v += matrix1->nums[ (m+5) * row + (n+3)] * B3v;
                C6v += matrix1->nums[ (m+6) * row + (n+3)] * B3v;
                C7v += matrix1->nums[ (m+7) * row + (n+3)] * B3v;

                B4v = _mm256_loadu_ps(&matrix2->nums[(n + 4) * matrix2->col + k]);

                C0v += matrix1->nums[ m * row + (n+4)] * B4v;
                C1v += matrix1->nums[ (m+1) * row + (n+4)] * B4v;
                C2v += matrix1->nums[ (m+2) * row + (n+4)] * B4v;
                C3v += matrix1->nums[ (m+3) * row + (n+4)] * B4v;
                C4v += matrix1->nums[ (m+4) * row + (n+4)] * B4v;
                C5v += matrix1->nums[ (m+5) * row + (n+4)] * B4v;
                C6v += matrix1->nums[ (m+6) * row + (n+4)] * B4v;
                C7v += matrix1->nums[ (m+7) * row + (n+4)] * B4v;

                B5v = _mm256_loadu_ps(&matrix2->nums[(n + 5) * matrix2->col + k]);

                C0v += matrix1->nums[ m * row + (n+5)] * B5v;
                C1v += matrix1->nums[ (m+1) * row + (n+5)] * B5v;
                C2v += matrix1->nums[ (m+2) * row + (n+5)] * B5v;
                C3v += matrix1->nums[ (m+3) * row + (n+5)] * B5v;
                C4v += matrix1->nums[ (m+4) * row + (n+5)] * B5v;
                C5v += matrix1->nums[ (m+5) * row + (n+5)] * B5v;
                C6v += matrix1->nums[ (m+6) * row + (n+5)] * B5v;
                C7v += matrix1->nums[ (m+7) * row + (n+5)] * B5v;

                B6v = _mm256_loadu_ps(&matrix2->nums[(n + 6) * matrix2->col + k]);

                C0v += matrix1->nums[ m * row + (n+6)] * B6v;
                C1v += matrix1->nums[ (m+1) * row + (n+6)] * B6v;
                C2v += matrix1->nums[ (m+2) * row + (n+6)] * B6v;
                C3v += matrix1->nums[ (m+3) * row + (n+6)] * B6v;
                C4v += matrix1->nums[ (m+4) * row + (n+6)] * B6v;
                C5v += matrix1->nums[ (m+5) * row + (n+6)] * B6v;
                C6v += matrix1->nums[ (m+6) * row + (n+6)] * B6v;
                C7v += matrix1->nums[ (m+7) * row + (n+6)] * B6v;


                B7v = _mm256_loadu_ps(&matrix2->nums[(n + 7) * matrix2->col + k]);

                C0v += matrix1->nums[ m * row + (n+7)] * B7v;
                C1v += matrix1->nums[ (m+1) * row + (n+7)] * B7v;
                C2v += matrix1->nums[ (m+2) * row + (n+7)] * B7v;
                C3v += matrix1->nums[ (m+3) * row + (n+7)] * B7v;
                C4v += matrix1->nums[ (m+4) * row + (n+7)] * B7v;
                C5v += matrix1->nums[ (m+5) * row + (n+7)] * B7v;
                C6v += matrix1->nums[ (m+6) * row + (n+7)] * B7v;
                C7v += matrix1->nums[ (m+7) * row + (n+7)] * B7v;

                _mm256_storeu_ps(&answer->nums[m * answer->col + k], C0v);
                _mm256_storeu_ps(&answer->nums[(m+1) * answer->col + k], C1v);
                _mm256_storeu_ps(&answer->nums[(m+2) * answer->col + k], C2v);
                _mm256_storeu_ps(&answer->nums[(m+3) * answer->col + k], C3v);
                _mm256_storeu_ps(&answer->nums[(m+4) * answer->col + k], C4v);
                _mm256_storeu_ps(&answer->nums[(m+5) * answer->col + k], C5v);
                _mm256_storeu_ps(&answer->nums[(m+6) * answer->col + k], C6v);
                _mm256_storeu_ps(&answer->nums[(m+7) * answer->col + k], C7v);
            }
        }
    }
    return true;
}

bool mulMatrix_strassen(Matrix *answer, const Matrix *matrix1, const Matrix *matrix2, size_t size){
    if(matrix1 == NULL){
        fprintf(stderr,"File %s, Line %d, Function %s(): The matrix1 is NULL.\n",
                __FILE__,__LINE__,__FUNCTION__ );
        return false;
    }
    else if(matrix1->nums == NULL){
        fprintf(stderr,"%s(): The matrix1 parameter is NULL.\n",__FUNCTION__ );
        return false;
    }
    if(matrix2 == NULL){
        fprintf(stderr,"File %s, Line %d, Function %s(): The matrix2 is NULL.\n",
                __FILE__,__LINE__,__FUNCTION__ );
        return false;
    }
    else if(matrix2->nums == NULL){
        fprintf(stderr,"%s(): The matrix2 parameter is NULL.\n",__FUNCTION__ );
        return false;
    }
    if(answer == NULL){
        fprintf(stderr,"File %s, Line %d, Function %s(): The answer matrix is NULL.\n",
                __FILE__,__LINE__,__FUNCTION__ );
        return false;
    }
    else if(answer->nums == NULL){
        fprintf(stderr,"%s(): The answer parameter is NULL.\n",__FUNCTION__ );
        return false;
    }

    if( matrix1->col != matrix2->row ||
        matrix1->row != answer->row || matrix2->col != answer->col){
        fprintf(stderr,"The input and output don't match. They should have same size");
        fprintf(stderr, "Their sizes are (%zu, %zu), (%zu, %zu) and (%zu, %zu)",
                matrix1->row, matrix1->col,
                matrix2->row, matrix2->col,
                answer->row, answer->col);
        return false;
    }

    size_t row = matrix1 -> row;
    size_t col = matrix1 -> col;
    size_t newSize = (size_t) size / 2;

    if(row <= 32){
        matmul_plain(answer, matrix1, matrix2);
        return true;
    }

    Matrix* A11 = (Matrix*) malloc(sizeof(Matrix));
    Matrix* A12 = (Matrix*) malloc(sizeof(Matrix));
    Matrix* A21 = (Matrix*) malloc(sizeof(Matrix));
    Matrix* A22 = (Matrix*) malloc(sizeof(Matrix));

    A11 = createMatrix(newSize, newSize);
    A12 = createMatrix(newSize, newSize);
    A21 = createMatrix(newSize, newSize);
    A22 = createMatrix(newSize, newSize);


    Matrix* B11 = (Matrix*) malloc(sizeof(Matrix));
    Matrix* B12 = (Matrix*) malloc(sizeof(Matrix));
    Matrix* B21 = (Matrix*) malloc(sizeof(Matrix));
    Matrix* B22 = (Matrix*) malloc(sizeof(Matrix));

    B11 = createMatrix(newSize, newSize);
    B12 = createMatrix(newSize, newSize);
    B21 = createMatrix(newSize, newSize);
    B22 = createMatrix(newSize, newSize);

    Matrix* C11 = (Matrix*) malloc(sizeof(Matrix));
    Matrix* C12 = (Matrix*) malloc(sizeof(Matrix));
    Matrix* C21 = (Matrix*) malloc(sizeof(Matrix));
    Matrix* C22 = (Matrix*) malloc(sizeof(Matrix));

    C11 = createMatrix(newSize, newSize);
    C12 = createMatrix(newSize, newSize);
    C21 = createMatrix(newSize, newSize);
    C22 = createMatrix(newSize, newSize);

    Matrix* resA = (Matrix*) malloc(sizeof(Matrix));
    Matrix* resB = (Matrix*) malloc(sizeof(Matrix));

    resA = createMatrix(newSize, newSize);
    resB = createMatrix(newSize, newSize);


    Matrix* P1 = (Matrix*) malloc(sizeof(Matrix));
    Matrix* P2 = (Matrix*) malloc(sizeof(Matrix));
    Matrix* P3 = (Matrix*) malloc(sizeof(Matrix));
    Matrix* P4 = (Matrix*) malloc(sizeof(Matrix));
    Matrix* P5 = (Matrix*) malloc(sizeof(Matrix));
    Matrix* P6 = (Matrix*) malloc(sizeof(Matrix));
    Matrix* P7 = (Matrix*) malloc(sizeof(Matrix));


    P1 = createMatrix(newSize, newSize);
    P2 = createMatrix(newSize, newSize);
    P3 = createMatrix(newSize, newSize);
    P4 = createMatrix(newSize, newSize);
    P5 = createMatrix(newSize, newSize);
    P6 = createMatrix(newSize, newSize);
    P7 = createMatrix(newSize, newSize);

    Matrix* S1 = (Matrix*) malloc(sizeof(Matrix));
    Matrix* S2 = (Matrix*) malloc(sizeof(Matrix));
    Matrix* S3 = (Matrix*) malloc(sizeof(Matrix));
    Matrix* S4 = (Matrix*) malloc(sizeof(Matrix));
    Matrix* S5 = (Matrix*) malloc(sizeof(Matrix));
    Matrix* S6 = (Matrix*) malloc(sizeof(Matrix));
    Matrix* S7 = (Matrix*) malloc(sizeof(Matrix));
    Matrix* S8 = (Matrix*) malloc(sizeof(Matrix));
    Matrix* S9 = (Matrix*) malloc(sizeof(Matrix));
    Matrix* S10 = (Matrix*) malloc(sizeof(Matrix));

    S1 = createMatrix(newSize, newSize);
    S2 = createMatrix(newSize, newSize);
    S3 = createMatrix(newSize, newSize);
    S4 = createMatrix(newSize, newSize);
    S5 = createMatrix(newSize, newSize);
    S6 = createMatrix(newSize, newSize);
    S7 = createMatrix(newSize, newSize);
    S8 = createMatrix(newSize, newSize);
    S9 = createMatrix(newSize, newSize);
    S10 = createMatrix(newSize, newSize);

    //对矩阵mat1 和 mat2进行分割，并减少多余的运算
    for(size_t i = 0; i < newSize; i++){
        for(size_t j = 0; j < newSize; j++){
            size_t temp = i * newSize + j;
            A11 -> nums[temp] = matrix1 -> nums[i * col + j];
            A12 -> nums[temp] = matrix1 -> nums[i * col + j + row / 2];
            A21 -> nums[temp] = matrix1 -> nums[(i + newSize) * col + j];
            A22 -> nums[temp] = matrix1 -> nums[(i + newSize) * col + j + newSize];

            B11 -> nums[temp] = matrix2 -> nums[i * col + j];
            B12 -> nums[temp] = matrix2 -> nums[i * col + j + row / 2];
            B21 -> nums[temp] = matrix2 -> nums[(i + newSize) * col + j];
            B22 -> nums[temp] = matrix2 -> nums[(i + newSize) * col + j + row / 2];
        }
    }
    addTwoMatrix(S1, B12, B22);

    addTwoMatrix(S2, A11, A12);

    addTwoMatrix(S3, A21, A22);

    subtractMatrix(S4, B21, B11);

    addTwoMatrix(S5, A11, A22);

    addTwoMatrix(S6, B11, B22);

    subtractMatrix(S7, A12, A22);

    addTwoMatrix(S8, B21, B22);

    subtractMatrix(S9, A11, A21);

    addTwoMatrix(S10, B11, B12);

    mulMatrix_strassen(P1, A11, S1, newSize);

    mulMatrix_strassen(P2, S2, B22, newSize);

    mulMatrix_strassen(P3, S3, B11, newSize);

    mulMatrix_strassen(P4, A22, S4, newSize);

    mulMatrix_strassen(P5, S5, S6, newSize);

    mulMatrix_strassen(P6, S7, S8, newSize);

    mulMatrix_strassen(P7, S9, S10, newSize);

    addTwoMatrix(resA, P4, P5);

    subtractMatrix(resB, resA, P2);

    addTwoMatrix(C11, resB, P6);

    addTwoMatrix(C12, P1, P2);

    addTwoMatrix(C21, P3, P4);

    addTwoMatrix(resA, P5, P1);

    subtractMatrix(resB, resA, P3);

    subtractMatrix(C22, resB, P7);

    for(size_t i = 0; i < newSize; i++){
        for(size_t j = 0; j < newSize; j++){
            answer -> nums[i * col + j] = C11 -> nums[i * newSize + j];
            answer -> nums[i * col + j + row / 2] = C12 -> nums[i * newSize + j];
            answer -> nums[(i + row / 2) * col + j] = C21 -> nums[i * newSize + j];
            answer -> nums[(i + row / 2) * col + j + row / 2] = C22 -> nums[i * newSize + j];
        }
    }

    deleteMatrix(A11);
    deleteMatrix(A12);
    deleteMatrix(A21);
    deleteMatrix(A22);

    deleteMatrix(B11);
    deleteMatrix(B12);
    deleteMatrix(B21);
    deleteMatrix(B22);

    deleteMatrix(C11);
    deleteMatrix(C12);
    deleteMatrix(C21);
    deleteMatrix(C22);

    deleteMatrix(P1);
    deleteMatrix(P2);
    deleteMatrix(P3);
    deleteMatrix(P4);
    deleteMatrix(P5);
    deleteMatrix(P6);
    deleteMatrix(P7);

    deleteMatrix(S1);
    deleteMatrix(S2);
    deleteMatrix(S3);
    deleteMatrix(S4);
    deleteMatrix(S5);
    deleteMatrix(S6);
    deleteMatrix(S7);
    deleteMatrix(S8);
    deleteMatrix(S9);
    deleteMatrix(S10);

    deleteMatrix(resA);
    deleteMatrix(resB);

    return true;
}
bool deleteMatrix(Matrix *matrix) {
    //先检查是否有效
    if (matrix == NULL) {
        fprintf(stderr, "The matrix have been NULL, no need to delete\n");
        return false;
    }
    if (matrix->nums != NULL) {
        free(matrix->nums);
    }
    free(matrix);
    matrix = NULL;
    return true;
}

bool matmul_plain(Matrix *answer, const Matrix *matrix1,const Matrix *matrix2) {

    if(matrix1 == NULL){
        fprintf(stderr,"File %s, Line %d, Function %s(): The matrix1 is NULL.\n",
                __FILE__,__LINE__,__FUNCTION__ );
        return false;
    }
    else if(matrix1->nums == NULL){
        fprintf(stderr,"%s(): The matrix1 parameter is NULL.\n",__FUNCTION__ );
        return false;
    }
    if(matrix2 == NULL){
        fprintf(stderr,"File %s, Line %d, Function %s(): The matrix2 is NULL.\n",
                __FILE__,__LINE__,__FUNCTION__ );
        return false;
    }
    else if(matrix2->nums == NULL){
        fprintf(stderr,"%s(): The matrix2 parameter is NULL.\n",__FUNCTION__ );
        return false;
    }
    if(answer == NULL){
        fprintf(stderr,"File %s, Line %d, Function %s(): The answer matrix is NULL.\n",
                __FILE__,__LINE__,__FUNCTION__ );
        return false;
    }
    else if(answer->nums == NULL){
        fprintf(stderr,"%s(): The answer parameter is NULL.\n",__FUNCTION__ );
        return false;
    }

    if( matrix1->col != matrix2->row ||
        matrix1->row != answer->row || matrix2->col != answer->col){
        fprintf(stderr,"The input and output don't match. They should have same size");
        fprintf(stderr, "Their sizes are (%zu, %zu), (%zu, %zu) and (%zu, %zu)",
                matrix1->row, matrix1->col,
                matrix2->row, matrix2->col,
                answer->row, answer->col);
        return false;
    }

    size_t row = matrix1->row;
    size_t col = matrix2->col;

    for (size_t i = 0; i < row; i++) {
        for (size_t j = 0; j < col; j++) {
            for (size_t k = 0; k < row; k++) {
                answer->nums[i * col + j] +=
                        matrix1->nums[i * col + k] * matrix2->nums[k * col + j];
            }
        }
    }

    return true;
}

bool copyMatrix(Matrix *target, const Matrix *source) {
    if (source == NULL) {
        fprintf(stderr, "Source matrix is NULL.\n");
        return false;
    }
    if (source->row == 0 || source->col == 0) {
        fprintf(stderr, "Source matrix have zero col & row.\n");
        return false;
    }
    if (source->nums == NULL) {
        fprintf(stderr, "Source matrix have NULL nums.\n");
        return false;
    }
    int row = source->row;
    int col = source->col;

    // 使用hard copy进行处理
    if (target!=NULL || target->row != row || target->col != col) {
        //原来的matrix
        deleteMatrix(target);
        target = createMatrix(row,col);
    }
    memcpy(target->nums,source->nums,row * col);
    return true;
}

bool addTwoMatrix(Matrix *answer, const Matrix *matrix1,const Matrix *matrix2) {
    if(matrix1 == NULL){
        fprintf(stderr,"File %s, Line %d, Function %s(): The matrix1 is NULL.\n",
                __FILE__,__LINE__,__FUNCTION__ );
        return false;
    }
    else if(matrix1->nums == NULL){
        fprintf(stderr,"%s(): The matrix1 parameter is NULL.\n",__FUNCTION__ );
        return false;
    }
    if(matrix2 == NULL){
        fprintf(stderr,"File %s, Line %d, Function %s(): The matrix2 is NULL.\n",
                __FILE__,__LINE__,__FUNCTION__ );
        return false;
    }
    else if(matrix2->nums == NULL){
        fprintf(stderr,"%s(): The matrix2 parameter is NULL.\n",__FUNCTION__ );
        return false;
    }
    if(answer == NULL){
        fprintf(stderr,"File %s, Line %d, Function %s(): The answer matrix is NULL.\n",
                __FILE__,__LINE__,__FUNCTION__ );
        return false;
    }
    else if(answer->nums == NULL){
        fprintf(stderr,"%s(): The answer parameter is NULL.\n",__FUNCTION__ );
        return false;
    }

    if( matrix1->col != matrix2->col || matrix1->row != matrix2->row ||
        matrix1->row != answer->row || matrix2->col != answer->col){
        fprintf(stderr,"The input and output don't match. They should have same size");
        fprintf(stderr, "Their sizes are (%zu, %zu), (%zu, %zu) and (%zu, %zu)",
                matrix1->row, matrix1->col,
                matrix2->row, matrix2->col,
                answer->row, answer->col);
        return false;
    }

    size_t row = matrix1->row;
    size_t col = matrix1->col;

    size_t length = matrix1->row * matrix1->col;
    const float * temp1 = matrix1->nums;
    const float * temp2 = matrix2->nums;
    float * temp3 = answer->nums;
    for (size_t i = 0; i < length; ++i) {
        *(temp3++) = *(temp1++) + *(temp2++);
    }
    return true;
}

bool subtractMatrix(Matrix *answer, const Matrix *matrix1,
                    const Matrix *matrix2) {
    if(matrix1 == NULL){
        fprintf(stderr,"File %s, Line %d, Function %s(): The matrix1 is NULL.\n",
                __FILE__,__LINE__,__FUNCTION__ );
        return false;
    }
    else if(matrix1->nums == NULL){
        fprintf(stderr,"%s(): The matrix1 parameter is NULL.\n",__FUNCTION__ );
        return false;
    }
    if(matrix2 == NULL){
        fprintf(stderr,"File %s, Line %d, Function %s(): The matrix2 is NULL.\n",
                __FILE__,__LINE__,__FUNCTION__ );
        return false;
    }
    else if(matrix2->nums == NULL){
        fprintf(stderr,"%s(): The matrix2 parameter is NULL.\n",__FUNCTION__ );
        return false;
    }
    if(answer == NULL){
        fprintf(stderr,"File %s, Line %d, Function %s(): The answer matrix is NULL.\n",
                __FILE__,__LINE__,__FUNCTION__ );
        return false;
    }
    else if(answer->nums == NULL){
        fprintf(stderr,"%s(): The answer parameter is NULL.\n",__FUNCTION__ );
        return false;
    }

    if( matrix1->col != matrix2->col || matrix1->row != matrix2->row ||
        matrix1->row != answer->row || matrix2->col != answer->col){
        fprintf(stderr,"The input and output don't match. They should have same size");
        fprintf(stderr, "Their sizes are (%zu, %zu), (%zu, %zu) and (%zu, %zu)",
                matrix1->row, matrix1->col,
                matrix2->row, matrix2->col,
                answer->row, answer->col);
        return false;
    }

    size_t row = matrix1->row;
    size_t col = matrix1->col;

    size_t length = matrix1->row * matrix1->col;
    const float * temp1 = matrix1->nums;
    const float * temp2 = matrix2->nums;
    float * temp3 = answer->nums;
    for (size_t i = 0; i < length; ++i) {
        *(temp3++) = *(temp1++) - *(temp2++);
    }
    return true;
}

bool multiplyByScalar(Matrix *matrix, float scalar) {
    if(matrix == NULL){
        fprintf(stderr,"File %s, Line %d, Function %s(): The matrix is NULL.\n",
                __FILE__,__LINE__,__FUNCTION__ );
        return false;
    }
    else if(matrix->nums == NULL){
        fprintf(stderr,"%s(): The matrix parameter is NULL.\n",__FUNCTION__ );
        return false;
    }
    size_t row = matrix->row;
    size_t col = matrix->col;

    size_t length = matrix->row * matrix->col;

    float * temp = matrix->nums;
    for (size_t i = 0; i < length; ++i) {
        *(temp) *= scalar;
    }
    return true;
}

bool addByScalar(Matrix *matrix, float scalar) {
    if(matrix == NULL){
        fprintf(stderr,"File %s, Line %d, Function %s(): The matrix is NULL.\n",
                __FILE__,__LINE__,__FUNCTION__ );
        return false;
    }
    else if(matrix->nums == NULL){
        fprintf(stderr,"%s(): The matrix parameter is NULL.\n",__FUNCTION__ );
        return false;
    }
    size_t row = matrix->row;
    size_t col = matrix->col;

    size_t length = matrix->row * matrix->col;

    float * temp = matrix->nums;
    for (size_t i = 0; i < length; ++i) {
        *(temp) += scalar;
    }
    return true;
}

bool subByScalar(Matrix *matrix, float scalar) {
    if(matrix == NULL){
        fprintf(stderr,"File %s, Line %d, Function %s(): The matrix is NULL.\n",
                __FILE__,__LINE__,__FUNCTION__ );
        return false;
    }
    else if(matrix->nums == NULL){
        fprintf(stderr,"%s(): The matrix parameter is NULL.\n",__FUNCTION__ );
        return false;
    }
    size_t row = matrix->row;
    size_t col = matrix->col;

    size_t length = matrix->row * matrix->col;

    float * temp = matrix->nums;
    for (size_t i = 0; i < length; ++i) {
        *(temp) -= scalar;
    }
    return true;
}

bool divideByScalar(Matrix *matrix, float scalar) {
    if(matrix == NULL){
        fprintf(stderr,"File %s, Line %d, Function %s(): The matrix is NULL.\n",
                __FILE__,__LINE__,__FUNCTION__ );
        return false;
    }
    else if(matrix->nums == NULL){
        fprintf(stderr,"%s(): The matrix parameter is NULL.\n",__FUNCTION__ );
        return false;
    }
    size_t row = matrix->row;
    size_t col = matrix->col;

    size_t length = matrix->row * matrix->col;

    float * temp = matrix->nums;
    for (size_t i = 0; i < length; ++i) {
        *(temp) /= scalar;
    }
    return true;
}

bool replaceCharValue(Matrix *matrix, size_t row, size_t col, float target) {
    if(matrix == NULL){
        fprintf(stderr,"File %s, Line %d, Function %s(): The matrix is NULL.\n",
                __FILE__,__LINE__,__FUNCTION__ );
        return false;
    }
    else if(matrix->nums == NULL){
        fprintf(stderr,"%s(): The matrix parameter is NULL.\n",__FUNCTION__ );
        return false;
    }

    matrix->nums[(row - 1) * col + (col - 1)] = target;

    return true;
}

bool getValue(float *value, const Matrix *matrix, size_t row, size_t col) {
    if(matrix == NULL){
        fprintf(stderr,"File %s, Line %d, Function %s(): The matrix is NULL.\n",
                __FILE__,__LINE__,__FUNCTION__ );
        return false;
    }
    else if(matrix->nums == NULL){
        fprintf(stderr,"%s(): The matrix parameter is NULL.\n",__FUNCTION__ );
        return false;
    }
    else if(value == NULL){
        fprintf(stderr,"File %s, Line %d, Function %s(): The output value is NULL.\n",
                __FILE__,__LINE__,__FUNCTION__ );
        return false;
    }

    *value = matrix->nums[(row - 1) * col + (col - 1)];

    return true;
}
bool getMax(float *value, const Matrix *matrix) {
    if(matrix == NULL){
        fprintf(stderr,"File %s, Line %d, Function %s(): The matrix is NULL.\n",
                __FILE__,__LINE__,__FUNCTION__ );
        return false;
    }
    else if(matrix->nums == NULL){
        fprintf(stderr,"%s(): The matrix parameter is NULL.\n",__FUNCTION__ );
        return false;
    }
    else if(value == NULL){
        fprintf(stderr,"File %s, Line %d, Function %s(): The value is NULL.\n",
                __FILE__,__LINE__,__FUNCTION__ );
        return false;
    }
    size_t row = matrix->row;
    size_t col = matrix->col;

    size_t length = matrix->row * matrix->col;

    float * temp = matrix->nums;
    for (size_t i = 0; i < length; ++i) {
        *value = MAX(*value, temp[i]);
    }
    return true;
}

bool getMin(float *value, const Matrix *matrix) {
    if(matrix == NULL){
        fprintf(stderr,"File %s, Line %d, Function %s(): The matrix is NULL.\n",
                __FILE__,__LINE__,__FUNCTION__ );
        return false;
    }
    else if(matrix->nums == NULL){
        fprintf(stderr,"%s(): The matrix parameter is NULL.\n",__FUNCTION__ );
        return false;
    }
    else if(value == NULL){
        fprintf(stderr,"File %s, Line %d, Function %s(): The value is NULL.\n",
                __FILE__,__LINE__,__FUNCTION__ );
        return false;
    }

    size_t row = matrix->row;
    size_t col = matrix->col;

    size_t length = matrix->row * matrix->col;

    float * temp = matrix->nums;
    for (size_t i = 0; i < length; ++i) {
        *value = MIN(*value, temp[i]);
    }
    return true;
}











