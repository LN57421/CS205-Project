#include "matrix.h"

#include <math.h>
#define MIN(i, j) (((i) < (j)) ? (i) : (j))
#define MAX(i, j) (((i) > (j)) ? (i) : (j))

Matrix *createMatrix(Matrix *matrix, int row, int col) {
  if (row <= 0 || col <= 0) {
    printf("Row or Col is less than zero\n");
    return NULL;
  }
  if (row >= 2e9 || col >= 2e9) {
    printf("Row or Col is too large\n");
    return NULL;
  }
  if (matrix == NULL) {
    printf("Original matrix point is NULL\n");
    return NULL;
  }
  //为新的矩阵开辟空间
  matrix = (Matrix *)malloc(row * col * sizeof(float));
  //为矩阵结构各结构体变量赋值
  matrix->row = row;
  matrix->col = col;
  matrix->nums =
      (float *)malloc(row * col * sizeof(float));  //动态分配二维数组的内存
  printf("Create Matrix Finish\n");
  return matrix;
}

Matrix *createMatrixByFile(Matrix *matrix, int row, int col, FILE *fp) {
  if (row <= 0 || col <= 0) {
    printf("Row or Col is less than zero.\n");
    return NULL;
  }
  if (row >= 2e9 || col >= 2e9) {
    printf("Row or Col is too large.\n");
    return NULL;
  }
  if (matrix == NULL) {
    printf("Original matrix point is NULL.\n");
    return NULL;
  }
  if (fp == NULL) {
    printf("File point is NULL.\n");
    return NULL;
  }
  //为矩阵结构各结构体变量赋值
  matrix->row = row;
  matrix->col = col;
  matrix->nums =
      (float *)malloc(row * col * sizeof(float));  //动态分配二维数组的内存
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      fscanf(fp, "%f", &(matrix->nums[i * col + j]));
    }
  }
  fclose(fp);  //关闭文件
  printf("Create Matrix By File Finish.\n");
  return matrix;
}

bool deleteMatrix(Matrix *matrix) {
  if (matrix == NULL) {  // sizeof(*matrix) != sizeof(Matrix)
    return false;
  }
  if (matrix->nums == NULL) {
    return false;
  }
  free(matrix->nums);
  //内存已经释放，但指针仍然指向原有内存，应该置空
  matrix->nums = NULL;
  free(matrix);
  matrix = NULL;
  return true;
}

bool copyMatrix(Matrix *target, const Matrix *source) {
  if (source == NULL) {
    return false;
  }
  if (target == NULL) {
    return false;
  }
  if (source->row <= 0 || source->col <= 0) {
    return false;
  }
  if (source->nums == NULL) {
    return false;
  }
  int row = source->row;
  int col = source->col;

  if (target->row != row || target->col != col) {
    //原来的matrix
    deleteMatrix(target);
    Matrix *tar;
    tar = (Matrix *)malloc(sizeof(Matrix));
    tar->row = row;
    tar->col = col;
    tar->nums = (float *)malloc(row * col * sizeof(float));
    target = tar;
  }
  //复制的时候先row后col
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      target->nums[i * col + j] = source->nums[i * col + j];
    }
  }
  return true;
}

bool multiplyTwoMatrix(Matrix *answer, const Matrix *matrix1,
                       const Matrix *matrix2) {
  if (answer == NULL || matrix1 == NULL || matrix2 == NULL) {
    return false;
  }

  if (matrix1->row <= 0 || matrix2->row <= 0 || matrix1->col <= 0 ||
      matrix2->col <= 0) {
    return false;
  }

  if (matrix1->row != matrix2->col) {
    return false;
  }

  if (matrix1->nums == NULL || matrix2->nums == NULL) {
    return false;
  }

  int row = matrix1->row;
  int col = matrix2->col;

  if (answer->row != row || answer->col != col) {
    deleteMatrix(answer);
    Matrix *temp_ans;
    temp_ans = (Matrix *)malloc(sizeof(Matrix));
    temp_ans->row = row;
    temp_ans->col = col;
    temp_ans->nums = (float *)malloc(row * col * sizeof(float));
    answer = temp_ans;
    return false;
  }

  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      for (int k = 0; k < row; k++) {
        answer->nums[i * col + j] +=
            matrix1->nums[i * col + k] * matrix2->nums[k * col + j];
      }
    }
  }
  return true;
}

bool addTwoMatrix(Matrix *answer, const Matrix *matrix1,
                  const Matrix *matrix2) {
  if (answer == NULL || matrix1 == NULL || matrix2 == NULL) {
    return false;
  }

  if (matrix1->row <= 0 || matrix2->row <= 0 || matrix1->col <= 0 ||
      matrix2->col <= 0) {
    return false;
  }

  if (matrix1->row != matrix2->row || matrix1->col != matrix2->col) {
    return false;
  }

  if (matrix1->nums == NULL || matrix2->nums == NULL) {
    return false;
  }

  int row = matrix1->row;
  int col = matrix1->col;

  if (answer->row != row || answer->col != col) {
    deleteMatrix(answer);
    Matrix *temp_ans;
    temp_ans = (Matrix *)malloc(sizeof(Matrix));
    temp_ans->row = row;
    temp_ans->col = col;
    temp_ans->nums = (float *)malloc(row * col * sizeof(float));
    answer = temp_ans;
  }

  for (int i = 0; i < row; ++i) {
    for (int j = 0; j < col; ++j) {
      answer->nums[i * col + j] =
          matrix1->nums[i * col + j] + matrix2->nums[i * col + j];
    }
  }

  return true;
}

bool subtractMatrix(Matrix *answer, const Matrix *matrix1,
                    const Matrix *matrix2) {
  if (answer == NULL || matrix1 == NULL || matrix2 == NULL) {
    return false;
  }

  if (matrix1->row <= 0 || matrix2->row <= 0 || matrix1->col <= 0 ||
      matrix2->col <= 0) {
    return false;
  }

  if (matrix1->row != matrix2->row || matrix1->col != matrix2->col) {
    return false;
  }

  if (matrix1->nums == NULL || matrix2->nums == NULL) {
    return false;
  }

  int row = matrix1->row;
  int col = matrix1->col;

  if (answer->row != row || answer->col != col) {
    deleteMatrix(answer);
    Matrix *temp_ans;
    temp_ans = (Matrix *)malloc(sizeof(Matrix));
    temp_ans->row = row;
    temp_ans->col = col;
    temp_ans->nums = (float *)malloc(row * col * sizeof(float));
    answer = temp_ans;
  }

  for (int i = 0; i < row; ++i) {
    for (int j = 0; j < col; ++j) {
      answer->nums[i * col + j] =
          matrix1->nums[i * col + j] - matrix2->nums[i * col + j];
    }
  }

  return true;
}

bool multiplyByScalar(Matrix *matrix, float scalar) {
  if (matrix == NULL) {
    return false;
  }

  if (matrix->row <= 0 || matrix->col <= 0) {
    return false;
  }

  if (matrix->nums == NULL) {
    return false;
  }

  int row = matrix->row;
  int col = matrix->col;

  for (int i = 0; i < row; ++i) {
    for (int j = 0; j < col; ++j) {
      matrix->nums[i * col + j] = matrix->nums[i * col + j] * scalar;
    }
  }
  return true;
}

bool addByScalar(Matrix *matrix, float scalar) {
  if (matrix == NULL) {
    return false;
  }

  if (matrix->row <= 0 || matrix->col <= 0) {
    return false;
  }

  if (matrix->nums == NULL) {
    return false;
  }

  int row = matrix->row;
  int col = matrix->col;

  for (int i = 0; i < row; ++i) {
    for (int j = 0; j < col; ++j) {
      matrix->nums[i * col + j] = matrix->nums[i * col + j] + scalar;
    }
  }
  return true;
}

bool subByScalar(Matrix *matrix, float scalar) {
  if (matrix == NULL) {
    return false;
  }

  if (matrix->row <= 0 || matrix->col <= 0) {
    return false;
  }
  if (matrix->nums == NULL) {
    return false;
  }

  int row = matrix->row;
  int col = matrix->col;

  for (int i = 0; i < row; ++i) {
    for (int j = 0; j < col; ++j) {
      matrix->nums[i * col + j] = matrix->nums[i * col + j] - scalar;
    }
  }
  return true;
}

bool divideByScalar(Matrix *matrix, float scalar) {
  if (matrix == NULL) {
    return false;
  }

  if (matrix->row <= 0 || matrix->col <= 0) {
    return false;
  }
  if (matrix->nums == NULL) {
    return false;
  }

  if (scalar == 0) {
    return false;
  }

  int row = matrix->row;
  int col = matrix->col;

  for (int i = 0; i < row; ++i) {
    for (int j = 0; j < col; ++j) {
      matrix->nums[i * col + j] = matrix->nums[i * col + j] / scalar;
    }
  }
  return true;
}

bool replaceCharValue(Matrix *matrix, int row, int col, float target) {
  if (matrix == NULL) {
    return false;
  }

  if (matrix->row <= 0 || matrix->col <= 0) {
    return false;
  }

  if (matrix->row < row || matrix->col < col) {
    return false;
  }

  if (row >= 2e9 || col >= 2e9) {
    return false;
  }

  if (matrix->nums == NULL) {
    return false;
  }

  matrix->nums[(row - 1) * col + (col - 1)] = target;

  return true;
}

bool getValue(float *value, const Matrix *matrix, int row, int col) {
  if (matrix == NULL || value == NULL) {
    return false;
  }

  if (matrix->row <= 0 || matrix->col <= 0) {
    return false;
  }

  if (row >= 2e9 || col >= 2e9) {
    return false;
  }

  if (matrix->row < row || matrix->col < col) {
    return false;
  }
  if (matrix->nums == NULL) {
    return false;
  }

  *value = matrix->nums[(row - 1) * col + (col - 1)];

  return true;
}
bool getMax(float *value, const Matrix *matrix) {
  if (matrix == NULL || value == NULL) {
    return false;
  }

  if (matrix->row <= 0 || matrix->col <= 0) {
    return false;
  }

  if (matrix->nums == NULL) {
    return false;
  }

  float temp = -2e9 - 1;
  int row = matrix->row;
  int col = matrix->col;

  for (int i = 0; i < row; ++i) {
    for (int j = 0; j < col; ++j) {
      temp = MAX(temp, matrix->nums[col * i + j]);
    }
  }
  *value = temp;
  return true;
}

bool getMin(float *value, const Matrix *matrix) {
  if (matrix == NULL || value == NULL) {
    return false;
  }

  if (matrix->row <= 0 || matrix->col <= 0) {
    return false;
  }

  if (matrix->nums == NULL) {
    return false;
  }

  float temp = 2e9 + 1;
  int row = matrix->row;
  int col = matrix->col;

  for (int i = 0; i < row; ++i) {
    for (int j = 0; j < col; ++j) {
      temp = MIN(temp, matrix->nums[col * i + j]);
    }
  }
  *value = temp;
  return true;
}

bool deterOfMatrix(float *determinate, const Matrix *source) {
  if (source == NULL || determinate == NULL) {
    return false;
  }

  if (source->row <= 0 || source->col <= 0 || source->row != source->col) {
    return false;
  }

  if (source->nums == NULL) {
    return false;
  }
  int row = source->row;
  int col = source->col;

  Matrix *upper_matrix;
  upper_matrix = (Matrix *)malloc(sizeof(Matrix));
  upper_matrix->row = row;
  upper_matrix->col = col;
  upper_matrix->nums = (float *)malloc(row * col * sizeof(float));

  int i, j, k, p, is, js, l, v;
  double tmp, pivot;
  // 这里要使用最简阶梯型矩阵
  for (i = 0; i < row * col; i++) {
    upper_matrix->nums[i] = source->nums[i];
  }
  // 保存行和列两者的最小值
  p = row < col ? row : col;
  // 找到目前的最大主元
  for (k = 0; k < p; k++) {
    pivot = 0.0;
    for (i = k; i < row; i++) {
      for (j = k; j < col; j++) {
        tmp = fabs(upper_matrix->nums[i * col + j]);
        if (tmp > pivot) {
          pivot = tmp;
          is = i;
          js = j;
        }
      }
    }
    //没有找到不为 0 的元素，退出循环,秩数再继续增加
    if (pivot < p) {
      break;
    }
    // 进行初等行变换
    if (is != k) {
      for (j = k; j < col; j++) {
        l = k * col + j;
        v = is * col + j;
        tmp = upper_matrix->nums[l];
        upper_matrix->nums[l] = upper_matrix->nums[v];
        upper_matrix->nums[v] = tmp;
      }
    }
    if (js != k) {
      for (i = k; i < row; i++) {
        l = i * col + k;
        v = i * col + js;
        tmp = upper_matrix->nums[l];
        upper_matrix->nums[l] = upper_matrix->nums[v];
        upper_matrix->nums[v] = tmp;
      }
    }
    // 进行消元操作
    for (i = k + 1; i < row; i++) {
      tmp = upper_matrix->nums[i * col + k] / upper_matrix->nums[k * col + k];
    }

    for (j = k + 1; j < col; j++) {
      upper_matrix->nums[i * col + j] -= tmp * upper_matrix->nums[k * col + j];
    }
  }
  float deter = 0;
  for (int m = 0; m < row * col; m = m + col + 1) {
    deter *= upper_matrix->nums[m];
  }
  *determinate = deter;
  deleteMatrix(upper_matrix);
  return true;
}

bool rankOfMatrix(int *rank, const Matrix *source) {
  if (source == NULL || rank == NULL) {
    return false;
  }

  if (source->row <= 0 || source->col <= 0) {
    return false;
  }

  if (rank == NULL) {
    return false;
  }

  if (source->nums == NULL) {
    return false;
  }

  int row = source->row;
  int col = source->col;
  *rank = 0;

  Matrix *upper_matrix;
  upper_matrix = (Matrix *)malloc(sizeof(Matrix));
  upper_matrix->row = row;
  upper_matrix->col = col;
  upper_matrix->nums = (float *)malloc(row * col * sizeof(float));

  int i, j, k, p, is, js, l, v;
  double tmp, pivot;
  // 这里要使用最简阶梯型矩阵
  for (i = 0; i < row * col; i++) {
    upper_matrix->nums[i] = source->nums[i];
  }
  // 保存行和列两者的最小值
  p = row < col ? row : col;
  (*rank) = 0;
  // 找到目前的最大主元
  for (k = 0; k < p; k++) {
    pivot = 0.0;
    for (i = k; i < row; i++) {
      for (j = k; j < col; j++) {
        tmp = fabs(upper_matrix->nums[i * col + j]);
        if (tmp > pivot) {
          pivot = tmp;
          is = i;
          js = j;
        }
      }
    }
    //没有找到不为 0 的元素，退出循环,秩数再继续增加
    if (pivot < p) {
      break;
    }
    (*rank)++;
    // 进行初等行变换
    if (is != k) {
      for (j = k; j < col; j++) {
        l = k * col + j;
        v = is * col + j;
        tmp = upper_matrix->nums[l];
        upper_matrix->nums[l] = upper_matrix->nums[v];
        upper_matrix->nums[v] = tmp;
      }
    }
    if (js != k) {
      for (i = k; i < row; i++) {
        l = i * col + k;
        v = i * col + js;
        tmp = upper_matrix->nums[l];
        upper_matrix->nums[l] = upper_matrix->nums[v];
        upper_matrix->nums[v] = tmp;
      }
    }
    // 进行消元操作
    for (i = k + 1; i < row; i++) {
      tmp = upper_matrix->nums[i * col + k] / upper_matrix->nums[k * col + k];
    }

    for (j = k + 1; j < col; j++) {
      upper_matrix->nums[i * col + j] -= tmp * upper_matrix->nums[k * col + j];
    }
  }

  //释放空间
  deleteMatrix(upper_matrix);
  return true;
}

//得到两个矩阵的合并矩阵
bool concatMatrix(Matrix *cat_matrix, const Matrix *matrix1,
                  const Matrix *matrix2, bool by_row) {
  if (matrix1 == NULL || matrix2 == NULL || cat_matrix == NULL) {
    return false;
  }
  if (matrix1->nums == NULL || matrix2->nums == NULL) {
    return false;
  }
  if (by_row) {
    // 沿着行进行拼接
    if (matrix1->row != matrix2->row) {
      return false;
    }

    // 为用户开辟空间
    if (cat_matrix->row != matrix1->row ||
        cat_matrix->col != matrix1->col + matrix2->col) {
      //原来的matrix
      deleteMatrix(cat_matrix);
      Matrix *tar;
      tar = (Matrix *)malloc(sizeof(Matrix));
      tar->row = matrix1->row;
      tar->col = matrix1->col + matrix2->col;
      tar->nums = (float *)malloc(tar->row * tar->col * sizeof(float));
      cat_matrix = tar;
      int temp = 0;
      for (int i = 0; i < tar->row * tar->col; i++) {
        if (temp < matrix1->row) {
          cat_matrix->nums[i] = matrix1->nums[temp];
        } else {
          cat_matrix->nums[i] = matrix2->nums[temp - matrix1->row];
        }
        temp = 0;
      }
    }
  } else {
    // 沿着列进行拼接
    if (matrix1->col != matrix2->col) {
      return false;
    }
    // 为用户开辟空间
    if (cat_matrix->col != matrix1->col ||
        cat_matrix->row != matrix1->row + matrix2->row) {
      //原来的matrix
      deleteMatrix(cat_matrix);
      Matrix *tar;
      tar = (Matrix *)malloc(sizeof(Matrix));
      tar->row = matrix1->row + matrix2->row;
      tar->col = matrix1->col;
      tar->nums = (float *)malloc(tar->row * tar->col * sizeof(float));
      cat_matrix = tar;
      for (int i = 0; i < cat_matrix->row; i++) {
        for (int j = 0; j < cat_matrix->col; j++) {
          if (i <= matrix1->row) {
            cat_matrix->nums[i * cat_matrix->col + j] =
                matrix1->nums[i * cat_matrix->col + j];
          } else {
            cat_matrix->nums[i * cat_matrix->col + j] =
                matrix2->nums[i * cat_matrix->col + j -
                              (matrix1->row * matrix1->col)];
          }
        }
      }
    }
  }
  return true;
}

//得到子矩阵
bool subMatrix(Matrix *sub_matrix, const Matrix *source, int row, int col,
               int index_i, int index_j) {
  if (source == NULL || sub_matrix == NULL) {
    return false;
  }
  if (row >= 2e9 || col >= 2e9) {
    return false;
  }

  if (source->row <= 0 || source->col <= 0) {
    return false;
  }

  if (source->nums == NULL) {
    return false;
  }

  if (index_i + row > source->row || index_j + col > source->col) {
    return false;
  }

  if (sub_matrix->row != row || sub_matrix->col != col) {
    //原来的matrix
    deleteMatrix(sub_matrix);
    Matrix *tar;
    tar = (Matrix *)malloc(sizeof(Matrix));
    tar->row = row;
    tar->col = col;
    tar->nums = (float *)malloc(row * col * sizeof(float));
    sub_matrix = tar;
  }

  int row_dif = source->row - (row + index_i);
  int col_dif = source->col - (col + index_j);
  int temp_index = 0;
  //复制的时候先row后col
  for (int i = 0; i < row; i++) {
    temp_index += col_dif;
    for (int j = 0; j < col; j++) {
      temp_index += row_dif;
      sub_matrix->nums[i * col + j] = source->nums[temp_index];
      temp_index++;
    }
  }
  return true;
}
//得到转置矩阵
bool transposeMatrix(Matrix *tans_matrix, const Matrix *source) {
  if (source == NULL) {
    return false;
  }

  if (source->row <= 0 || source->col <= 0) {
    return false;
  }

  if (source->nums == NULL) {
    return false;
  }
  int row = source->row;
  int col = source->col;

  if (tans_matrix != NULL && tans_matrix->row != row ||
      tans_matrix->col != col) {
    //原来的matrix
    deleteMatrix(tans_matrix);
    Matrix *tar;
    tar = (Matrix *)malloc(sizeof(Matrix));
    tar->row = row;
    tar->col = col;
    tar->nums = (float *)malloc(row * col * sizeof(float));
    tans_matrix = tar;
  }
  //复制的时候先col后row
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      tans_matrix->nums[j * col + i] = source->nums[i * col + j];
    }
  }
  return true;
}
