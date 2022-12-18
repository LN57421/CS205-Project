#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wout-of-line-declaration"

#ifndef PROJECT5_MATRIX_H
#define PROJECT5_MATRIX_H

#include <immintrin.h>

#include <cstdio>
#include <cstring>
#include <exception>
#include <iostream>
#include <memory>
#include <utility>

#include "Exception.h"

using namespace std;

static int cnt = 1;

template <class T>
class Matrix {
 private:
  size_t row;
  size_t column;
  size_t channel;

  // 储存数据
  std::shared_ptr<T> nums;

  // 进行标定，此矩阵是否为ROI矩阵
  bool isROI = false;
  // 支持ROI操作
  size_t row_ROI;               // ROI行数
  size_t column_ROI;            // ROI列数
  std::shared_ptr<T> nums_ROI;  // ROI指针

 public:
  // 默认构造器
  explicit Matrix(size_t row = 10, size_t column = 10, size_t channel = 3) {
    try {
      if (row == 0 || column == 0 || channel == 0) {
        throw Exception("The parameters are not valid", __FILE__, __LINE__);
      }
      this->row = row;
      this->column = column;
      this->channel = channel;
      nums = std::shared_ptr<T>(new T[row * column * channel],
                                std::default_delete<T[]>());
      memset(nums.get(), 0, sizeof(T) * row * column * channel);
    } catch (Exception &exception) {
      exception.errorHandle();
    } catch (...) {
      std::cerr << "other error at file: " << __FILE__ << ", line: " << __LINE__
                << "\n";
    }
  };

  //复制构造器
  Matrix(const Matrix &matrix) {
    try {
      if (&matrix == nullptr) {
        throw Exception("The origin matrix is null", __FILE__, __LINE__);
      }
      this->nums = matrix.nums;
      this->row = matrix.row;
      this->column = matrix.column;
      this->channel = matrix.channel;
      if (matrix.isROI == true) {
        this->nums_ROI = matrix.nums_ROI;
        this->row_ROI = matrix.row_ROI;
        this->column_ROI = matrix.column_ROI;
      }
    } catch (Exception &exception) {
      exception.errorHandle();
    }
  }

  //默认软拷贝赋值函数
  Matrix &operator=(const Matrix &matrix) {
    try {
      if (&matrix == nullptr) {
        throw Exception("The origin matrix is null", __FILE__, __LINE__);
      }
      if (this == &matrix) {
        return *this;
      }
      this->nums = matrix.nums;
      this->row = matrix.row;
      this->column = matrix.column;
      this->channel = matrix.channel;
      if (matrix.isROI == true) {
        this->nums_ROI = matrix.nums_ROI;
        this->row_ROI = matrix.row_ROI;
        this->column_ROI = matrix.column_ROI;
      }
    } catch (Exception &exception) {
      exception.errorHandle();
    }
  }

  //由数组构造矩阵
  Matrix(size_t row, size_t column, size_t channel, T *nums);

  //由文件读取矩阵
  Matrix(size_t row, size_t column, size_t channel, string &fileName);

  //构造函数，实现对ROI的支持
  Matrix(size_t row_ROI, size_t column_ROI, size_t row_Begin,
         size_t column_Begin, Matrix matrix);

  //将矩阵显示到屏幕上
  template <typename TT>
  friend std::ostream &operator<<(std::ostream &os, const Matrix<TT> &matrix);

  //矩阵乘法
  template <typename TT>
  friend Matrix<TT> operator*(TT number, const Matrix<TT> &matrix);

  Matrix operator*(T number);

  Matrix operator*(const Matrix<T> &matrix);

  //矩阵加法
  template <typename TT>
  friend Matrix<TT> &operator+(TT number, const Matrix<TT> &matrix);

  Matrix operator+(T number);

  Matrix<T> operator+(const Matrix<T> &matrix);

  //矩阵减法
  template <typename TT>
  friend Matrix<TT> &operator-(TT number, const Matrix<TT> &matrix);

  Matrix operator-(T number);

  Matrix operator-(const Matrix &matrix);

  //其他运算符
  bool operator==(const Matrix &matrix);

  bool operator!=(const Matrix &matrix);

  //析构函数
  ~Matrix() {
    nums.reset();
    nums_ROI.reset();
    std::cout << "matrix" << cnt++ << " is deconstructed.\n";
  }
};

// 由数组构造矩阵
template <typename T>
Matrix<T>::Matrix(size_t row, size_t column, size_t channel, T *nums) {
  try {
    T *new_nums = (T *)malloc(sizeof(T) * row * column * channel);
    if (new_nums == NULL) {
      throw Exception("the size is too large", __FILE__, __LINE__);
    }
    memcpy(new_nums, nums, sizeof(T) * row * column * channel);
    this->nums = std::shared_ptr<T>(new_nums, std::default_delete<T[]>());
    this->row = row;
    this->column = column;
    this->channel = channel;
  } catch (Exception &exceptionMatix) {
    exceptionMatix.errorHandle();
  } catch (...) {
    std::cerr << "other error at file: " << __FILE__ << ", line: " << __LINE__
              << "\n";
  }
}

// 由文件构造矩阵
template <typename T>
Matrix<T>::Matrix(size_t row, size_t column, size_t channel, string &fileName) {
  try {
    std::shared_ptr<FILE> fp(fopen(fileName.data(), "r"), fclose);
    if (fp.get() == nullptr) {
      throw Exception("File is not found", __FILE__, __LINE__);
    } else {
      string params;
      if (typeid(T) == typeid(1.f) || typeid(T) == typeid(1.0))
        params = "%f";
      else if (typeid(T) == typeid(1))
        params = "%d";
      else if (typeid(T) == typeid('d'))
        params = "%c";
      else
        throw Exception("Unknown file type", __FILE__, __LINE__);
      T num = 0;
      for (size_t i = 0; i < row * column * channel; i++) {
        if (fscanf(fp.get(), params, &num) != EOF) {
          this->nums.get()[i] = num;
        } else {
          throw Exception("File size not match", __FILE__, __LINE__);
        }
      }
    }
  } catch (Exception &exceptionMatix) {
    exceptionMatix.errorHandle();
  } catch (...) {
    std::cerr << "other error when open file: " << __FILE__
              << ", line: " << __LINE__ << "\n";
  }
}

//构造函数，实现对ROI的支持
template <typename T>
Matrix<T>::Matrix(size_t row_ROI, size_t column_ROI, size_t row_Begin,
                  size_t column_Begin, Matrix matrix) {
  try {
    if (row_ROI > matrix.row || column_ROI > matrix.column ||
        (row_ROI + row_Begin) > matrix.row ||
        (column_ROI + column_Begin) > matrix.column) {
      throw Exception(
          "Error: can not construct a ROI matrix, the parameters are wrong",
          __FILE__, __LINE__);
    }

    //设置ROI部分
    this->row_ROI = row_ROI;
    this->column_ROI = column_ROI;
    this->nums_ROI = std::shared_ptr<T>(
        matrix.nums.get()[(row_Begin - 1) * matrix.column + column_Begin - 1],
        std::default_delete<T[]>());

    //设置非ROI部分
    this->row = matrix.row;
    this->column = matrix.column;
    this->channel = matrix.channel;
    this->nums = matrix.nums;
    this->isROI = true;
  } catch (Exception &exceptionMatix) {
    exceptionMatix.errorHandle();
  } catch (...) {
    std::cerr << "other error at file: " << __FILE__ << ", line: " << __LINE__
              << "\n";
  }
}

//重载输出运算符
template <typename TT>
std::ostream &operator<<(std::ostream &os, const Matrix<TT> &matrix) {
  try {
    if (&matrix == NULL) {
      throw Exception("Null potinter exception", __FILE__, __LINE__);
    }
    if (!matrix.isROI) {
      os << "[";
      for (size_t i = 0; i < matrix.channel; ++i) {
        if (i != 0) os << "\n";
        os << "{";
        for (size_t j = 0; j < matrix.row; ++j) {
          for (size_t k = 0; k < matrix.column; ++k) {
            os << matrix.nums.get()[(i * matrix.column * matrix.row) +
                                    j * matrix.column + k];
            if (k != matrix.column - 1) os << ", ";
          }

          if (j != matrix.row - 1) os << "\n";
        }
        os << "}";
      }
      os << "]\n";
    } else {
      os << "[";
      for (size_t i = 0; i < matrix.channel; ++i) {
        if (i != 0) os << "\n";
        os << "{";
        for (size_t j = 0; j < matrix.row_ROI; ++j) {
          for (size_t k = 0; k < matrix.column_ROI; ++k) {
            os << matrix.nums_ROI.get()[(i * matrix.row * matrix.column) +
                                        j * (matrix.column_ROI) + k];
            if (k != matrix.column - 1) os << ", ";
          }

          if (j != matrix.row - 1) os << "\n";
        }
        os << "}";
      }
      os << "]\n";
    }
    return os;
  } catch (Exception &exceptionMatix) {
    exceptionMatix.errorHandle();
  } catch (...) {
    std::cerr << "other error at file: " << __FILE__ << ", line: " << __LINE__
              << "\n";
  }
}

//矩阵乘法
template <typename T>
Matrix<T> operator*(const T number, const Matrix<T> &matrix) {
  return matrix * number;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const T number) {
  try {
    Matrix<T> MatrixOut = Matrix<T>(this->row, this->column, this->channel);
    if (&MatrixOut == NULL) {
      throw Exception("The result matrix is too large", __FILE__, __LINE__);
    }
    size_t size = this->row * this->column * this->channel;
    for (size_t i = 0; i < size; ++i) {
      MatrixOut.nums.get()[i] = number * this->nums.get()[i];
    }
    return MatrixOut;
  } catch (Exception &exceptionMatix) {
    exceptionMatix.errorHandle();
  } catch (...) {
    std::cerr << "other error at file: " << __FILE__ << ", line: " << __LINE__
              << "\n";
  }
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T> &matrix) {
  try {
    // channel不匹配
    if (this->channel != matrix.channel) {
      throw Exception("The two matrix is not match", __FILE__, __LINE__);
    }

    // 若两者皆不是ROI矩阵
    if (!this->isROI || !matrix.isROI) {
      if (this->column != matrix.row) {
        throw Exception("The two matrix is not match", __FILE__, __LINE__);
      }
      Matrix<T> MatrixOut = Matrix<T>(this->row, matrix.column, this->channel);
      if (&MatrixOut == NULL) {
        throw Exception("The result matrix is too large", __FILE__, __LINE__);
      }
      size_t row = this->row;
      size_t col = this->column;
      size_t step = row * col;
      size_t channel = this->channel;
      for (size_t i = 0; i < channel; ++i) {
        #pragma omp parallel for
        for (size_t m = 0; m < row; m += 8) {
          for (size_t k = 0; k < col; k += 8) {
            __m256 C0v, C1v, C2v, C3v, C4v, C5v, C6v, C7v, B0v, B1v, B2v, B3v,
                B4v, B5v, B6v, B7v;
            for (size_t n = 0; n < this->col; n += 8) {
              C0v = _mm256_loadu_ps(
                  &MatrixOut->nums[m * MatrixOut->col + k + i * step]);
              C1v = _mm256_loadu_ps(
                  &MatrixOut->nums[(m + 1) * MatrixOut->col + k + i * step]);
              C2v = _mm256_loadu_ps(
                  &MatrixOut->nums[(m + 2) * MatrixOut->col + k + i * step]);
              C3v = _mm256_loadu_ps(
                  &MatrixOut->nums[(m + 3) * MatrixOut->col + k + i * step]);
              C4v = _mm256_loadu_ps(
                  &MatrixOut->nums[(m + 4) * MatrixOut->col + k + i * step]);
              C5v = _mm256_loadu_ps(
                  &MatrixOut->nums[(m + 5) * MatrixOut->col + k + i * step]);
              C6v = _mm256_loadu_ps(
                  &MatrixOut->nums[(m + 6) * MatrixOut->col + k + i * step]);
              C7v = _mm256_loadu_ps(
                  &MatrixOut->nums[(m + 7) * MatrixOut->col + k + i * step]);

              B0v = _mm256_loadu_ps(
                  &matrix->nums[n * matrix->col + k + i * step]);

              C0v += this->nums[m * row + n + i * step] * B0v;
              C1v += this->nums[(m + 1) * row + n + i * step] * B0v;
              C2v += this->nums[(m + 2) * row + n + i * step] * B0v;
              C3v += this->nums[(m + 3) * row + n + i * step] * B0v;
              C4v += this->nums[(m + 4) * row + n + i * step] * B0v;
              C5v += this->nums[(m + 5) * row + n + i * step] * B0v;
              C6v += this->nums[(m + 6) * row + n + i * step] * B0v;
              C7v += this->nums[(m + 7) * row + n + i * step] * B0v;

              B1v = _mm256_loadu_ps(
                  &matrix->nums[(n + 1) * matrix->col + k + i * step]);

              C0v += this->nums[m * row + (n + 1) + i * step] * B1v;
              C1v += this->nums[(m + 1) * row + (n + 1) + i * step] * B1v;
              C2v += this->nums[(m + 2) * row + (n + 1) + i * step] * B1v;
              C3v += this->nums[(m + 3) * row + (n + 1) + i * step] * B1v;
              C4v += this->nums[(m + 4) * row + (n + 1) + i * step] * B1v;
              C5v += this->nums[(m + 5) * row + (n + 1) + i * step] * B1v;
              C6v += this->nums[(m + 6) * row + (n + 1) + i * step] * B1v;
              C7v += this->nums[(m + 7) * row + (n + 1) + i * step] * B1v;

              B2v = _mm256_loadu_ps(
                  &matrix->nums[(n + 2) * matrix->col + k + i * step]);

              C0v += this->nums[m * row + (n + 2) + i * step] * B2v;
              C1v += this->nums[(m + 1) * row + (n + 2) + i * step] * B2v;
              C2v += this->nums[(m + 2) * row + (n + 2) + i * step] * B2v;
              C3v += this->nums[(m + 3) * row + (n + 2) + i * step] * B2v;
              C4v += this->nums[(m + 4) * row + (n + 2) + i * step] * B2v;
              C5v += this->nums[(m + 5) * row + (n + 2) + i * step] * B2v;
              C6v += this->nums[(m + 6) * row + (n + 2) + i * step] * B2v;
              C7v += this->nums[(m + 7) * row + (n + 2) + i * step] * B2v;

              B3v = _mm256_loadu_ps(
                  &matrix->nums[(n + 3) * matrix->col + k + i * step]);

              C0v += this->nums[m * row + (n + 3) + i * step] * B3v;
              C1v += this->nums[(m + 1) * row + (n + 3) + i * step] * B3v;
              C2v += this->nums[(m + 2) * row + (n + 3) + i * step] * B3v;
              C3v += this->nums[(m + 3) * row + (n + 3) + i * step] * B3v;
              C4v += this->nums[(m + 4) * row + (n + 3) + i * step] * B3v;
              C5v += this->nums[(m + 5) * row + (n + 3) + i * step] * B3v;
              C6v += this->nums[(m + 6) * row + (n + 3) + i * step] * B3v;
              C7v += this->nums[(m + 7) * row + (n + 3) + i * step] * B3v;

              B4v = _mm256_loadu_ps(
                  &matrix->nums[(n + 4) * matrix->col + k + i * step]);

              C0v += this->nums[m * row + (n + 4) + i * step] * B4v;
              C1v += this->nums[(m + 1) * row + (n + 4) + i * step] * B4v;
              C2v += this->nums[(m + 2) * row + (n + 4) + i * step] * B4v;
              C3v += this->nums[(m + 3) * row + (n + 4) + i * step] * B4v;
              C4v += this->nums[(m + 4) * row + (n + 4) + i * step] * B4v;
              C5v += this->nums[(m + 5) * row + (n + 4) + i * step] * B4v;
              C6v += this->nums[(m + 6) * row + (n + 4) + i * step] * B4v;
              C7v += this->nums[(m + 7) * row + (n + 4) + i * step] * B4v;

              B5v = _mm256_loadu_ps(
                  &matrix->nums[(n + 5) * matrix->col + k + i * step]);

              C0v += this->nums[m * row + (n + 5) + i * step] * B5v;
              C1v += this->nums[(m + 1) * row + (n + 5) + i * step] * B5v;
              C2v += this->nums[(m + 2) * row + (n + 5) + i * step] * B5v;
              C3v += this->nums[(m + 3) * row + (n + 5) + i * step] * B5v;
              C4v += this->nums[(m + 4) * row + (n + 5) + i * step] * B5v;
              C5v += this->nums[(m + 5) * row + (n + 5) + i * step] * B5v;
              C6v += this->nums[(m + 6) * row + (n + 5) + i * step] * B5v;
              C7v += this->nums[(m + 7) * row + (n + 5) + i * step] * B5v;

              B6v = _mm256_loadu_ps(
                  &matrix->nums[(n + 6) * matrix->col + k + i * step]);

              C0v += this->nums[m * row + (n + 6) + i * step] * B6v;
              C1v += this->nums[(m + 1) * row + (n + 6) + i * step] * B6v;
              C2v += this->nums[(m + 2) * row + (n + 6) + i * step] * B6v;
              C3v += this->nums[(m + 3) * row + (n + 6) + i * step] * B6v;
              C4v += this->nums[(m + 4) * row + (n + 6) + i * step] * B6v;
              C5v += this->nums[(m + 5) * row + (n + 6) + i * step] * B6v;
              C6v += this->nums[(m + 6) * row + (n + 6) + i * step] * B6v;
              C7v += this->nums[(m + 7) * row + (n + 6) + i * step] * B6v;

              B7v = _mm256_loadu_ps(
                  &matrix->nums[(n + 7) * matrix->col + k + i * step]);

              C0v += this->nums[m * row + (n + 7) + i * step] * B7v;
              C1v += this->nums[(m + 1) * row + (n + 7) + i * step] * B7v;
              C2v += this->nums[(m + 2) * row + (n + 7) + i * step] * B7v;
              C3v += this->nums[(m + 3) * row + (n + 7) + i * step] * B7v;
              C4v += this->nums[(m + 4) * row + (n + 7) + i * step] * B7v;
              C5v += this->nums[(m + 5) * row + (n + 7) + i * step] * B7v;
              C6v += this->nums[(m + 6) * row + (n + 7) + i * step] * B7v;
              C7v += this->nums[(m + 7) * row + (n + 7) + i * step] * B7v;

              _mm256_storeu_ps(
                  &MatrixOut->nums[m * MatrixOut->col + k + i * step], C0v);
              _mm256_storeu_ps(
                  &MatrixOut->nums[(m + 1) * MatrixOut->col + k + i * step],
                  C1v);
              _mm256_storeu_ps(
                  &MatrixOut->nums[(m + 2) * MatrixOut->col + k + i * step],
                  C2v);
              _mm256_storeu_ps(
                  &MatrixOut->nums[(m + 3) * MatrixOut->col + k + i * step],
                  C3v);
              _mm256_storeu_ps(
                  &MatrixOut->nums[(m + 4) * MatrixOut->col + k + i * step],
                  C4v);
              _mm256_storeu_ps(
                  &MatrixOut->nums[(m + 5) * MatrixOut->col + k + i * step],
                  C5v);
              _mm256_storeu_ps(
                  &MatrixOut->nums[(m + 6) * MatrixOut->col + k + i * step],
                  C6v);
              _mm256_storeu_ps(
                  &MatrixOut->nums[(m + 7) * MatrixOut->col + k + i * step],
                  C7v);
            }
          }
        }
      }
      return MatrixOut;
    }

    // 若两者皆为ROI矩阵
    else if (this->isROI && matrix.isROI) {
      if (this->column_ROI != matrix.row_ROI) {
        throw Exception("The two matrix is not match", __FILE__, __LINE__);
      }
      Matrix<T> MatrixOut =
          Matrix<T>(this->row_ROI, matrix.column_ROI, this->channel);
      if (&MatrixOut == NULL) {
        throw Exception("The result matrix is too large", __FILE__, __LINE__);
      }

      T sum = 0;
      for (size_t s = 0; s < this->channel; ++s) {
        for (size_t i = 0; i < this->row_ROI; ++i) {
          for (size_t j = 0; j < matrix.column_ROI; ++j) {
            sum = 0;
            for (size_t k = 0; k < this->column_ROI; ++k) {
              sum += this->nums_ROI.get()[(s * this->column * this->row) +
                                          i * this->column + k] *
                     matrix.nums_ROI.get()[(s * matrix.column * matrix.row) +
                                           k * matrix.column + j];
            }
            MatrixOut.nums.get()[(s * MatrixOut.column * MatrixOut.row) +
                                 i * MatrixOut.column + j] = sum;
          }
        }
      }
      return MatrixOut;
    }
    // this为ROI矩阵,matrix不为ROI矩阵
    else if (this->isROI && !matrix.isROI) {
      if (this->column_ROI != matrix.row) {
        throw Exception("The two matrix is not match", __FILE__, __LINE__);
      }
      Matrix<T> MatrixOut =
          Matrix<T>(this->row_ROI, matrix.column, this->channel);
      if (&MatrixOut == NULL) {
        throw Exception("The result matrix is too large", __FILE__, __LINE__);
      }

      T sum = 0;
      for (size_t s = 0; s < this->channel; ++s) {
        for (size_t i = 0; i < this->row_ROI; ++i) {
          for (size_t j = 0; j < matrix.column; ++j) {
            sum = 0;
            for (size_t k = 0; k < this->column_ROI; ++k) {
              sum += this->nums_ROI.get()[(s * this->column * this->row) +
                                          i * this->column + k] *
                     matrix.nums.get()[(s * matrix.column * matrix.row) +
                                       k * matrix.column + j];
            }
            MatrixOut.nums.get()[(s * MatrixOut.column * MatrixOut.row) +
                                 i * MatrixOut.column + j] = sum;
          }
        }
      }
      return MatrixOut;
    }
    // this不为ROI矩阵，matrix为ROI矩阵
    else {
      if (this->column != matrix.row_ROI) {
        throw Exception("The two matrix is not match", __FILE__, __LINE__);
      }
      Matrix<T> MatrixOut = Matrix<T>(this->row, matrix.column, this->channel);
      if (&MatrixOut == NULL) {
        throw Exception("The result matrix is too large", __FILE__, __LINE__);
      }

      T sum = 0;
      for (size_t s = 0; s < this->channel; ++s) {
        for (size_t i = 0; i < this->row; ++i) {
          for (size_t j = 0; j < matrix.column_ROI; ++j) {
            for (size_t k = 0; k < this->column; ++k) {
              sum = 0;
              sum += this->nums.get()[(s * this->column * this->row) +
                                      i * this->column + k] *
                     matrix.nums_ROI.get()[(s * matrix.column * matrix.row) +
                                           k * matrix.column_ROI + j];
              MatrixOut.nums.get()[(s * MatrixOut.column * MatrixOut.row) +
                                   i * MatrixOut.column + j] = sum;
            }
          }
        }
      }
      return MatrixOut;
    }
  } catch (Exception &exceptionMatix) {
    exceptionMatix.errorHandle();
  } catch (...) {
    std::cerr << "other error at file: " << __FILE__ << ", line: " << __LINE__
              << "\n";
  }
}

template <typename T>
Matrix<T> &operator-(const T number, const Matrix<T> &matrix) {
  return matrix - number;
}

template <typename T>
Matrix<T> Matrix<T>::operator-(const T number) {
  try {
    Matrix<T> MatrixOut = new Matrix<T>(this->row, this->column, this->channel);
    if (&MatrixOut == NULL) {
      throw Exception("The result matrix is too large", __FILE__, __LINE__);
    }
    size_t size = this->row * this->column * this->channel;
    for (size_t i = 0; i < size; ++i) {
      MatrixOut.nums.get()[i] = this->nums.get()[i] - number;
    }
    return MatrixOut;
  } catch (Exception &exceptionMatix) {
    exceptionMatix.errorHandle();
  } catch (...) {
    std::cerr << "other error at file: " << __FILE__ << ", line: " << __LINE__
              << "\n";
  }
}

template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T> &matrix) {
  try {
    // channel不匹配
    if (this->channel != matrix.channel) {
      throw Exception("The two matrix is not match", __FILE__, __LINE__);
    }
    // 两者皆不是ROI矩阵
    if (!this->isROI || !matrix.isROI) {
      if (this->column != matrix.column || this->row != matrix.row) {
        throw Exception("The two matrix is not match", __FILE__, __LINE__);
      }
      Matrix<T> MatrixOut = Matrix<T>(this->row, this->column, this->channel);
      if (&MatrixOut == NULL) {
        throw Exception("The result matrix is too large", __FILE__, __LINE__);
      }
      size_t size = this->row * this->column * this->channel;
      for (size_t i = 0; i < size; ++i) {
        MatrixOut.nums.get()[i] = this->nums.get()[i] - matrix.nums.get()[i];
      }
      return MatrixOut;
    } else if (this->isROI || !matrix.isROI) {
      if (this->column_ROI != matrix.column || this->row_ROI != matrix.row) {
        throw Exception("The two matrix is not match", __FILE__, __LINE__);
      }
      Matrix<T> MatrixOut =
          Matrix<T>(this->row_ROI, this->column_ROI, this->channel);
      if (&MatrixOut == NULL) {
        throw Exception("The result matrix is too large", __FILE__, __LINE__);
      }
      for (size_t i = 0; i < channel; ++i) {
        size_t channel_step = i * row_ROI * column_ROI;
        for (size_t j = 0; j < row_ROI; ++j) {
          size_t row_step = i * column_ROI;
          for (size_t k = 0; k < column_ROI; ++k) {
            MatrixOut.nums.get()[channel_step + row_step + k] =
                this->nums_ROI.get()[row_step + k + k * column_ROI] -
                matrix.nums.get()[row_step + k];
          }
        }
      }
      return MatrixOut;
    } else if (!this->isROI || matrix.isROI) {
      if (this->column != matrix.column_ROI || this->row != matrix.row_ROI) {
        throw Exception("The two matrix is not match", __FILE__, __LINE__);
      }
      Matrix<T> MatrixOut = Matrix<T>(this->row, this->column, this->channel);
      if (&MatrixOut == NULL) {
        throw Exception("The result matrix is too large", __FILE__, __LINE__);
      }
      for (size_t i = 0; i < channel; ++i) {
        size_t channel_step = i * row * column;
        for (size_t j = 0; j < row_ROI; ++j) {
          size_t row_step = i * column;
          for (size_t k = 0; k < column_ROI; ++k) {
            MatrixOut.nums.get()[channel_step + row_step + k] =
                this->nums_ROI.get()[row_step + k + k * column] -
                matrix.nums.get()[row_step + k];
          }
        }
      }
      return MatrixOut;
    }
    // 两者均为ROI矩阵
    if (this->isROI || matrix.isROI) {
      if (this->column_ROI != matrix.column_ROI ||
          this->row_ROI != matrix.row_ROI) {
        throw Exception("The two matrix is not match", __FILE__, __LINE__);
      }
      Matrix<T> MatrixOut =
          Matrix<T>(this->row_ROI, this->column_ROI, this->channel);
      if (&MatrixOut == NULL) {
        throw Exception("The result matrix is too large", __FILE__, __LINE__);
      }
      for (size_t i = 0; i < channel; ++i) {
        size_t channel_step = i * row_ROI * column_ROI;
        for (size_t j = 0; j < row_ROI; ++j) {
          size_t row_step = i * column_ROI;
          for (size_t k = 0; k < column_ROI; ++k) {
            MatrixOut.nums.get()[channel_step + row_step + k] =
                this->nums_ROI.get()[row_step + k + k * column_ROI] -
                matrix.nums.get()[row_step + k];
          }
        }
      }
      return MatrixOut;
    }
  } catch (Exception &exceptionMatix) {
    exceptionMatix.errorHandle();
  } catch (...) {
    std::cerr << "other error at file: " << __FILE__ << ", line: " << __LINE__
              << "\n";
  }
}

template <typename T>
Matrix<T> &operator+(const T number, const Matrix<T> &matrix) {
  return matrix + number;
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const T number) {
  try {
    Matrix<T> MatrixOut = new Matrix<T>(this->row, this->column, this->channel);
    if (&MatrixOut == NULL) {
      throw Exception("The result matrix is too large", __FILE__, __LINE__);
    }
    size_t size = this->row * this->column * this->channel;
    for (size_t i = 0; i < size; ++i) {
      MatrixOut.nums.get()[i] = this->nums.get()[i] + number;
    }
    return MatrixOut;
  } catch (Exception &exceptionMatix) {
    exceptionMatix.errorHandle();
  } catch (...) {
    std::cerr << "other error at file: " << __FILE__ << ", line: " << __LINE__
              << "\n";
  }
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> &matrix) {
  try {
    // channel不匹配
    if (this->channel != matrix.channel) {
      throw Exception("The two matrix is not match", __FILE__, __LINE__);
    }
    // 两者皆不是ROI矩阵
    if (!this->isROI || !matrix.isROI) {
      if (this->column != matrix.column || this->row != matrix.row) {
        throw Exception("The two matrix is not match", __FILE__, __LINE__);
      }
      Matrix<T> MatrixOut = Matrix<T>(this->row, this->column, this->channel);
      if (&MatrixOut == NULL) {
        throw Exception("The result matrix is too large", __FILE__, __LINE__);
      }
      size_t size = this->row * this->column * this->channel;
      for (size_t i = 0; i < size; ++i) {
        MatrixOut.nums.get()[i] = this->nums.get()[i] + matrix.nums.get()[i];
      }
      return MatrixOut;
    } else if (this->isROI || !matrix.isROI) {
      if (this->column_ROI != matrix.column || this->row_ROI != matrix.row) {
        throw Exception("The two matrix is not match", __FILE__, __LINE__);
      }
      Matrix<T> MatrixOut =
          Matrix<T>(this->row_ROI, this->column_ROI, this->channel);
      if (&MatrixOut == NULL) {
        throw Exception("The result matrix is too large", __FILE__, __LINE__);
      }
      for (size_t i = 0; i < channel; ++i) {
        size_t channel_step = i * row_ROI * column_ROI;
        for (size_t j = 0; j < row_ROI; ++j) {
          size_t row_step = i * column_ROI;
          for (size_t k = 0; k < column_ROI; ++k) {
            MatrixOut.nums.get()[channel_step + row_step + k] =
                this->nums_ROI.get()[row_step + k + k * column_ROI] +
                matrix.nums.get()[row_step + k];
          }
        }
      }
      return MatrixOut;
    } else if (!this->isROI || matrix.isROI) {
      if (this->column != matrix.column_ROI || this->row != matrix.row_ROI) {
        throw Exception("The two matrix is not match", __FILE__, __LINE__);
      }
      Matrix<T> MatrixOut = Matrix<T>(this->row, this->column, this->channel);
      if (&MatrixOut == NULL) {
        throw Exception("The result matrix is too large", __FILE__, __LINE__);
      }
      for (size_t i = 0; i < channel; ++i) {
        size_t channel_step = i * row * column;
        for (size_t j = 0; j < row_ROI; ++j) {
          size_t row_step = i * column;
          for (size_t k = 0; k < column_ROI; ++k) {
            MatrixOut.nums.get()[channel_step + row_step + k] =
                this->nums_ROI.get()[row_step + k + k * column] +
                matrix.nums.get()[row_step + k];
          }
        }
      }
      return MatrixOut;
    }
    // 两者均为ROI矩阵
    if (this->isROI || matrix.isROI) {
      if (this->column_ROI != matrix.column_ROI ||
          this->row_ROI != matrix.row_ROI) {
        throw Exception("The two matrix is not match", __FILE__, __LINE__);
      }
      Matrix<T> MatrixOut =
          Matrix<T>(this->row_ROI, this->column_ROI, this->channel);
      if (&MatrixOut == NULL) {
        throw Exception("The result matrix is too large", __FILE__, __LINE__);
      }
      for (size_t i = 0; i < channel; ++i) {
        size_t channel_step = i * row_ROI * column_ROI;
        for (size_t j = 0; j < row_ROI; ++j) {
          size_t row_step = i * column_ROI;
          for (size_t k = 0; k < column_ROI; ++k) {
            MatrixOut.nums.get()[channel_step + row_step + k] =
                this->nums_ROI.get()[row_step + k + k * column_ROI] +
                matrix.nums.get()[row_step + k];
          }
        }
      }
      return MatrixOut;
    }
  } catch (Exception &exceptionMatix) {
    exceptionMatix.errorHandle();
  } catch (...) {
    std::cerr << "other error at file: " << __FILE__ << ", line: " << __LINE__
              << "\n";
  }
}

template <typename T>
bool Matrix<T>::operator==(const Matrix &matrix) {
  try {
    // channel不匹配
    if (this->channel != matrix.channel) {
      std::cerr << "The two matrix is not match in channel at file:" << __FILE__
                << " ,line: " << __LINE__ << "\n";
      return false;
    }
    // 两者皆不是ROI矩阵
    if (!this->isROI || !matrix->isROI) {
      if (this->column != matrix.column) {
        std::cerr << "The two matrix is not match in col at file:" << __FILE__
                  << " ,line: " << __LINE__ << "\n";
        return false;
      } else if (this->row != matrix->row) {
        std::cerr << "The two matrix is not match in row at file:" << __FILE__
                  << " ,line: " << __LINE__ << "\n";
        return false;
      }
      size_t size = this->row * this->column * this->channel;
      return memcmp(this->nums, matrix.nums, size * sizeof(T));
    } else if (this->isROI || !matrix->isROI) {
      if (this->column_ROI != matrix.column) {
        std::cerr << "The two matrix is not match in col at file:" << __FILE__
                  << " ,line: " << __LINE__ << "\n";
        return false;
      } else if (this->row_ROI != matrix->row) {
        std::cerr << "The two matrix is not match in row at file:" << __FILE__
                  << " ,line: " << __LINE__ << "\n";
        return false;
      }
      for (size_t i = 0; i < row_ROI; ++i) {
        if (!memcmp(this->nums_ROI.get()[i * column_ROI],
                    matrix.nums.get()[i * column_ROI]),
            column_ROI * sizeof(T)) {
          return false;
        }
      }
    } else if (!this->isROI || matrix->isROI) {
      if (this->column != matrix.column_ROI) {
        std::cerr << "The two matrix is not match in col at file:" << __FILE__
                  << " ,line: " << __LINE__ << "\n";
        return false;
      } else if (this->row != matrix.row_ROI) {
        std::cerr << "The two matrix is not match in row at file:" << __FILE__
                  << " ,line: " << __LINE__ << "\n";
        return false;
      }
      for (size_t i = 0; i < row; ++i) {
        if (!memcmp(this->nums.get()[i * column],
                    matrix.nums_ROI.get()[i * column], column * sizeof(T))) {
          return false;
        }
      }
    } else {
      if (this->column_ROI != matrix.column_ROI) {
        std::cerr << "The two matrix is not match in col at file:" << __FILE__
                  << " ,line: " << __LINE__ << "\n";
        return false;
      } else if (this->row_ROI != matrix.row_ROI) {
        std::cerr << "The two matrix is not match in row at file:" << __FILE__
                  << " ,line: " << __LINE__ << "\n";
        return false;
      }
      for (size_t i = 0; i < row_ROI; ++i) {
        if (!memcmp(this->nums_ROI.get()[i * column_ROI],
                    matrix.nums_ROI.get()[i * column_ROI],
                    column_ROI * sizeof(T))) {
          return false;
        }
      }
    }
    return true;
  } catch (Exception &exceptionMatix) {
    exceptionMatix.errorHandle();
  } catch (...) {
    std::cerr << "other error in '==' operation, file:" << __FILE__
              << " ,line: " << __LINE__ << "\n";
  }
}

template <typename T>
bool Matrix<T>::operator!=(const Matrix &matrix) {
  try {
    // channel不匹配
    if (this->channel != matrix.channel) {
      std::cerr << "The two matrix is not match in channel at file:" << __FILE__
                << " ,line: " << __LINE__ << "\n";
      return true;
    }
    // 两者皆不是ROI矩阵
    if (!this->isROI || !matrix->isROI) {
      if (this->column != matrix.column) {
        std::cerr << "The two matrix is not match in col at file:" << __FILE__
                  << " ,line: " << __LINE__ << "\n";
        return true;
      } else if (this->row != matrix->row) {
        std::cerr << "The two matrix is not match in row at file:" << __FILE__
                  << " ,line: " << __LINE__ << "\n";
        return true;
      }
      size_t size = this->row * this->column * this->channel;
      return !memcmp(this->nums, matrix.nums, size * sizeof(T));
    } else if (this->isROI || !matrix->isROI) {
      if (this->column_ROI != matrix.column) {
        std::cerr << "The two matrix is not match in col at file:" << __FILE__
                  << " ,line: " << __LINE__ << "\n";
        return true;
      } else if (this->row_ROI != matrix->row) {
        std::cerr << "The two matrix is not match in row at file:" << __FILE__
                  << " ,line: " << __LINE__ << "\n";
        return true;
      }
      for (size_t i = 0; i < row_ROI; ++i) {
        if (!memcmp(this->nums_ROI.get()[i * column_ROI],
                    matrix.nums.get()[i * column_ROI]),
            column_ROI * sizeof(T)) {
          return true;
        }
      }
    } else if (!this->isROI || matrix->isROI) {
      if (this->column != matrix.column_ROI) {
        std::cerr << "The two matrix is not match in col at file:" << __FILE__
                  << " ,line: " << __LINE__ << "\n";
        return true;
      } else if (this->row != matrix.row_ROI) {
        std::cerr << "The two matrix is not match in row at file:" << __FILE__
                  << " ,line: " << __LINE__ << "\n";
        return true;
      }
      for (size_t i = 0; i < row; ++i) {
        if (!memcmp(this->nums.get()[i * column],
                    matrix.nums_ROI.get()[i * column], column * sizeof(T))) {
          return true;
        }
      }
    } else {
      if (this->column_ROI != matrix.column_ROI) {
        std::cerr << "The two matrix is not match in col at file:" << __FILE__
                  << " ,line: " << __LINE__ << "\n";
        return true;
      } else if (this->row_ROI != matrix.row_ROI) {
        std::cerr << "The two matrix is not match in row at file:" << __FILE__
                  << " ,line: " << __LINE__ << "\n";
        return true;
      }
      for (size_t i = 0; i < row_ROI; ++i) {
        if (!memcmp(this->nums_ROI.get()[i * column_ROI],
                    matrix.nums_ROI.get()[i * column_ROI],
                    column_ROI * sizeof(T))) {
          return true;
        }
      }
    }
    return false;
  } catch (Exception &exceptionMatix) {
    exceptionMatix.errorHandle();
  } catch (...) {
    std::cerr << "other error in '!=' operation, file:" << __FILE__
              << " ,line: " << __LINE__ << "\n";
  }
}

#endif  // PROJECT5_MATRIX_H
