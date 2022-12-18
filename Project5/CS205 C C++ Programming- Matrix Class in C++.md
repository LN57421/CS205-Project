# CS205 C/ C++ Programming- Matrix Class in C++

### name：刘家宝 (Liu Jiabao)

### SID: 12110416



## Part 1 - 总体目标与细节实现分析

1. 设计一个矩阵类，其中包含矩阵的基本数据信息（行数、列数、 channel数...）
2.  矩阵类支持不同的数据类型，即元素的数据类型应该多元化 
3. soft copy实现，使用引用传值，以及完成很好的内存管理，避免内存溢出以及动态分配与释放内存 
4. 重载运算符完成矩阵相关操作（赋值运算符，判断相等运算符，矩阵加 法运算符，矩阵减法运算符，以及矩阵乘法运算符等等）
5. 实现ROI （OpenCV中感兴趣区域） 
6.  在X86架构以及ARM服务器中运行程序，并且比较差异

### 1.1 模板类

### 基本属性

对于本次project中的nums，我们使用模板类智能指针以及来实现不同数据类型的支持，其他还包含的属性有：

1.矩阵列 

2.矩阵行 

3.矩阵通道数 

4.是否进行ROI处理的bool值

5.ROI行数

6.ROI列数

7.指向ROI的首地址的模板类智能指针

```c++
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
  
  ...
	
}	
```

### 1.2 内存管理

### 参数检查

对于方法体内传入的参数，我们需要进行鲁棒性判断：

1.所有指针在操作之前需检查所指向对象是否为空

2.如果遇到错误，进行try-catch操作进行捕获，并进行打印

3.每个矩阵分开进行检查，防止混乱

4.对参数进行检查，进行相应的报错

具体实现可见后续代码

### 智能指针

在实现nums的访问的时候，我们为了让指针的内存管理更加简单，使用shared_ptr<T>  nums智能指针

**智能指针基本定义**：

了解 `Objective-C`/`Swift` 的程序员应该知道引用计数的概念。引用计数这种计数是为了防止内存泄露而产生的。 基本想法是对于动态分配的对象，进行引用计数，每当增加一次对同一个对象的引用，那么引用对象的引用计数就会增加一次， 每删除一次引用，引用计数就会减一，当一个对象的引用计数减为零时，就自动删除指向的堆内存。

在传统 C++ 中，『记得』手动释放资源，总不是最佳实践。因为我们很有可能就忘记了去释放资源而导致泄露。 所以通常的做法是对于一个对象而言，我们在构造函数的时候申请空间，而在析构函数（在离开作用域时调用）的时候释放空间， 也就是我们常说的 RAII 资源获取即初始化技术。

凡事都有例外，我们总会有需要将对象在自由存储上分配的需求，在传统 C++ 里我们只好使用 `new` 和 `delete` 去 『记得』对资源进行释放。而 C++11 引入了智能指针的概念，使用了引用计数的想法，让程序员**不再需要关心手动释放内存**。 这些智能指针包括 `std::shared_ptr`/`std::unique_ptr`/`std::weak_ptr`，使用它们需要包含头文件 `<memory>`。

在本次project中我们使用到的`std::shared_ptr` 是一种智能指针，它能够记录多少个 `shared_ptr` 共同指向一个对象，从而消除显式的调用 `delete`，当引用计数变为零的时候就会将对象自动删除。

**如何访问**：

类似于普通指针，有 `*` 和 `->` 操作，但不支持下标操作[]，只能通过 `get()` 去访问数组的元素，**shared_ptr 在默认情况下是不能指向数组的**，原因是因为我们的 shared_ptr 默认的删除器是使用 Delete 对智能指针中的对象进行删除，而 delete 要求 new 时是单一指针 Delete时也应该是指针 new时是数组 delete 也应该用数组类型去delete

`std::shared_ptr` 可以通过 `get()` 方法来获取原始指针，通过 `reset()` 来减少一个引用计数， 并通过`use_count()`来查看一个对象的引用计数。

### 1.3 具体实现

### 1.3.1 默认构造器

用户可以通过使用默认参数的方式，定义默认构造函数，对所有成员进行初始化， 确保所有成员一开始就有已知的合理的值。对于nums中的每一个数赋值为0 。

```c++
Matrix<int> matrix;
```

默认构造器的具体代码如下：

```c++
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
```

### 1.3.2 复制构造函数

### 基本思路

如果没有显示地重载一个类的复制构造器，编译器便会自动地生成复制构造函数，但是这种默认生成的复制构造函数只会对对象中非静态的成员进行浅复制，即仅仅复制成员的值，对于指针变量来说，即直接将地址复制给该对象，这将导致只要两个对象当中的一个被释 放，该地址的内存就将会被释放，由于我们使用了智能指针所以不会有这个问题，但是重定义复制构造函数依然十分必要

在复制构造函数当中，传入的变量是一个同类的对象变量的引用，因为使用按值传递的方法将耗费一定的时间和内存去将传入的对象复制给一个临时变量，而使用引用传递，形参就是实参的一个别名，避免了大段内存的复制， 使用的是原始数据而非副本。 同样地，需要对传入的变量进行检测。

### 软拷贝

在具体实现软拷贝的过程中，还有一些细节需要考虑，就是我们在设置第二个指针的时候，需要直接指向智能指针本身。具体使用方法类似如下：

```C++
auto pointer = std::make_shared<int>(10);
auto pointer2 = pointer; // 引用计数+1
auto pointer3 = pointer; // 引用计数+1
int *p = pointer.get();  // 这样不会增加引用计数
```

具体实现的demo如下：

```c++
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
```


### 1.3.3 数组构造函数

矩阵的构建也可以依赖于指针数组，用户输入指针，行，列，通道数构建矩阵，实现思路比较简单，代码如下：

```C++
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
```

### 1.3.4 文件构造函数

实际工程中，矩阵的构建依赖于文件指针从文件中读取数据，用户输入文件名，行，列，通道数构建矩阵，实现思路比较简单，代码如下：

```C++
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
```

### 1.3.6 ROI矩阵构造函数

对于ROI的实现，将于后续进行分析。

### 1.3.7 赋值运算符=

### 基本思路

首先需要判断传入的**引用对象是否为空**，即进行程序安全性的检查。

确认传入对象有效之后，为了防止自身给自身赋值带来不必要的开销，在真正开始赋值之前**先判断两个对象是否完全相同**，如果完全相同，则返回原对象，结束程序。如果不相同，实现软拷贝。

具体代码如下：

```C++
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
```

### 1.3.8 析构函数

对于矩阵类的析构函数，由于使用了智能指针，所以不需要手动对nums进行释放，但是我们需要对于计数进行处理,

### reset方法

reset()包含两个操作。当智能指针中有值的时候，调用reset()会使引用计数减1.当调用reset（new xxx())重新赋值时，智能指针首先是生成新对象，然后将就对象的引用计数减1（当然，如果发现引用计数为0时，则析构旧对象），然后将新对象的指针交给智能指针保管。那么我们的析构函数要做的就很简单，调用reset方法

```c++
	~Matrix ()
	{
		elements.reset();
		elements_ROI.reset();
	}
```

### 1.3.9 ROI分析与实现

对于图像处理方面，ROI 代表一幅图像中感兴趣区域的信息，在矩阵当中， ROI 区域可以看作是一个矩阵的子矩阵，可以在矩阵当中维护相关的ROI 信息。

同时，在本次project中的ROI实现，应该也同样避免硬拷贝。

### 实现思路

对于普通矩阵，大致的内存结构如下，智能指针指向同一块内存，matrix中包含了其他属性。

![image-20221217195933506](C:\Users\nian5\AppData\Roaming\Typora\typora-user-images\image-20221217195933506.png)

对于matrix而言，若想实现ROI，应该让指针的起点处于想要操作的矩阵的起点

![image-20221217200050687](C:\Users\nian5\AppData\Roaming\Typora\typora-user-images\image-20221217200050687.png)

### 具体操作

首先，我们实现了通过普通矩阵构建得出ROI，构建ROI矩阵的方法如下：

```C++
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
```

而后再对于加减乘中的矩阵进行判断，判断进行运算的两个矩阵是否是ROI矩阵，根据相应的情况更改运算方式，具体实现可见后续加减乘除运算。

需要注意的是，我们在析构函数进行操作的时候，也会对ROI对应的element指针进行reset操作。

### 1.3.a 多维矩阵运算 + - *（包含ROI）运算

### 矩阵乘法运算符：*

在实现多维矩阵乘法的时候其实只要满足两个要求：

1.两个矩阵的通道数相同

2.两个矩阵的行数和列数满足第一个矩阵的列数等于第二个矩阵的行数

然后在实现的过程中需要进行考虑，先判断该矩阵是否为ROI矩阵，然后再进行运算，注意到ROI矩阵的访问内存并非连续的，所以乘法不同于原本乘法的加速乘法，要做相应调整。

首先是矩阵数乘：

```C++
template <typename T>
Matrix<T> Matrix<T>::operator*(const T number) {
  try {
    Matrix<T> MatrixOut = Matrix<T>(this->row, this->column, this->channel);
    if (&MatrixOut == NULL) {
      throw Exception("The result matrix is too large", __FILE__, __LINE__);
    }
    size_t size = this->row * this->column * this->channel;
    for (size_t i = 0; i < size;  ++i) {
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
```

其次是两个矩阵相乘：如果均为普通矩阵，借助project4中的矩阵乘法进行加速，如果有ROI矩阵，更改矩阵乘法实现

```C++
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
```

### 矩阵加法减法运算符：+ 、-

加法减法实现思路如下：

1. 加法减法的鲁棒性检测，如维数对应等
2. 考虑该矩阵是否为ROI矩阵
3. 进行相应的加法操作，同样应该注意到ROI矩阵的访问内存并非连续的

### 1.3.b 其他运算符

### !=运算符

此运算符的实现思路较为简单，实现思路如下

1. 进行鲁棒性判断

2. 判断该矩阵是否为ROI矩阵

3. 判断行、列、通道数等参数是否相等，只要有不同则返回true

4. 进行指针指向数据的判断，运用memcmp，只要有不同返回true，若都相等则返回false

### ==运算符

此运算符的实现思路较为简单，实现思路如下

1. 进行鲁棒性判断
2. 判断该矩阵是否为ROI矩阵
3. 判断行、列、通道数等参数是否相等，只要有不同则返回false
4. 进行指针指向数据的判断，运用memcmp，只要有不同返回false，若都相等则返回true

两个方法实现如下：

```C++
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
```

### 1.3.c 友元函数

友元是一种定义在类外部的普通函数或类，但它需要在类体内进行说明，为了与该类的成员函数加以区别，在说明时前面加以关键字friend。类的友元函数是定义在类外部，但有权访问类的所有私有（private）成员和保护（protected）成员。尽管友元函数的原型有在类的定义中出现过，但是友元函数并不是成员函数。通过友元函数能够使类对象的使用更加方便。

### 基本思路

友元函数的声明虽然位于类内部，可以访问并操作类内部的成员变量，即与 成员函数的访问权限相同，但是其并不属于类的成员函数。 那么友元函数是否违法面向对象编程的原则——封装呢？

一种解释是友元函数并不悖于面向对象的原则，因为决定哪一个函数是友元函 数是由类的声明决定的，因此类声明依然控制了函数的访问权限，这样就可以将友元函数看作是表达类接口的另一种不同机制。

在友元函数原型声明时，使用friend关键字，将非本类的想作为其第一个操作数，就可以通过友元函数实现操作数顺序的反转。

在本次project中，对于运算符的重载，为了适配首项不是目标类的运算情况， 都统一地使用了友元函数实现。

1.标准输出流<<，打印矩阵内容

```C++
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
            os<<matrix.nums.get()[(i * matrix.column * matrix.row) +
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
                    os<<matrix.nums_ROI.get()[(i * matrix.row * matrix.column) +
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
```

2.矩阵数乘

```C++
//矩阵乘法
template <typename T>
Matrix<T> operator*(const T number, const Matrix<T> &matrix) {
    return matrix * number;
}
```

## Part 2 - 其他实现

### 2.1 关于模板类函数定义在哪里实现的问题

在学习cvMat实现的过程中，发现其实没有使用模板类进行实现，进行了解后发现，由于模板类本质上是在编译时生成，cvMat的builder认为使用模板类会导致模板类的实现都放在头文件导致每次运行时用户编译时间过长导致效率降低的问题。

对于模板类函数函数而言，有两种实现方式，有一种是将所有的实现放在头文件；另一种是放在cpp文件，同时将实例化语句放入，两种各有利弊。主要参考了以下文章：
[微软关于模板类组织的看法](https://learn.microsoft.com/en-us/cpp/cpp/source-code-organization-cpp-templates?view=msvc-170)

[如何组织编写模板类](https://www.codeproject.com/Articles/3515/How-To-Organize-Template-Source-Code)

![image-20221218145807479](C:\Users\nian5\AppData\Roaming\Typora\typora-user-images\image-20221218145807479.png)

由于在实际测试中，编译时间并非问题，并且提前实例化会导致代码的参数类型有局限性，最终确定的模板类函数仍为定义在头文件中

### 2.2 CMakeList进行代码管理

本次project的实现利用CMakeList进行代码的管理。要注意的是，在CMakeLists.txt中添加符合omp和avx2的编译选项。

## Part3 - 代码

### 代码结构

本次用到的文件如下

![image-20221218234935017](C:\Users\nian5\AppData\Roaming\Typora\typora-user-images\image-20221218234935017.png)

期中，Exception.h包含了抛出异常所需要的demo

### Github获取所有代码

您可以通过点击这里获取所有代码

## Part4 - 正确性检验 

### 测试环境：x86

- 操作系统：Windows 11 Home China x86_64 
- 运行环境：Linux -- ubuntu 20.04
- 硬盘：SAMSUNG MZVLB1T0HBLR-000L2 953GB
- 处理器：AMD Ryzen 9 5900HX with Radeon graphics
- 内存：Micron Technology DDR4 3200MHZ
- OpenBLAS：0.3.21-x64

### 测试环境：ARM

- 产品型号：EAIDK-310-P0
- 主芯片：Rockchip RK3228H 4*A53
- 储存容量：1GB LPDDR3 / 8GB eMMC
- 输入电源：5V micro USB

### 4.1 方法正确性检验

### 默认构造器

打印并输出默认构造的，row = 10， col = 10，channel = 3的全为0的矩阵, 参数选择为int类型

![image-20221218230419040](C:\Users\nian5\AppData\Roaming\Typora\typora-user-images\image-20221218230419040.png)

![image-20221218223258087](C:\Users\nian5\AppData\Roaming\Typora\typora-user-images\image-20221218223258087.png)

进一步选择参数为float类型，由os流输出的结果相同

![image-20221218224534289](C:\Users\nian5\AppData\Roaming\Typora\typora-user-images\image-20221218224534289.png)

### 指针构造与赋值重载

使用指针构造矩阵并赋值，使用int类型检验其正确性

![image-20221218230506699](C:\Users\nian5\AppData\Roaming\Typora\typora-user-images\image-20221218230506699.png)

![image-20221218223957708](C:\Users\nian5\AppData\Roaming\Typora\typora-user-images\image-20221218223957708.png)

### 矩阵基本运算

基本运算

![image-20221218232535134](C:\Users\nian5\AppData\Roaming\Typora\typora-user-images\image-20221218232535134.png)结果如下：

![image-20221218232420999](C:\Users\nian5\AppData\Roaming\Typora\typora-user-images\image-20221218232420999.png)

乘法运算：

![image-20221218232608640](C:\Users\nian5\AppData\Roaming\Typora\typora-user-images\image-20221218232608640.png)

运算结果如下：

![image-20221218231611551](C:\Users\nian5\AppData\Roaming\Typora\typora-user-images\image-20221218231611551.png)

随后通过文件读取的方式对16 * 16 的单通道矩阵相乘，同时对两个矩阵进行左上角2 * 2的ROI矩阵进行验证，使用matlab进行结果验证：

![image-20221218234135459](C:\Users\nian5\AppData\Roaming\Typora\typora-user-images\image-20221218234135459.png)

matlab结果：

![image-20221218233904064](C:\Users\nian5\AppData\Roaming\Typora\typora-user-images\image-20221218233904064.png)

### 判等符号== 与!=

沿用上部分代码得到的矩阵结果

![image-20221218232900720](C:\Users\nian5\AppData\Roaming\Typora\typora-user-images\image-20221218232900720.png)

![image-20221218232955754](C:\Users\nian5\AppData\Roaming\Typora\typora-user-images\image-20221218232955754.png)

### 4.2 X86系统与ARM开发版运行结果对比

### 计算速度

以单通道矩阵乘法为例

| 矩阵规模    | t1：X86   | t2:ARM    |
| ----------- | --------- | --------- |
| **16*16**   | 0.000014s | 0.000098s |
| **32*32**   | 0.000036s | 0.000427s |
| **64*64**   | 0.000162s | 0.001624s |
| **128*128** | 0.000239s | 0.125496s |
| **256*256** | 0.073775s | 0.214851  |
| **512*512** | 0.456798s | 1.73221   |
| **1k*1k**   | 3.581013s | 14.1996   |

总体结果为，x86系统运行速度为ARM开发板的4.5倍左右

## Part5 - 总结 

在本次项目当中，我觉得又比前一个项目更有了经验以及收获，首先是更加懂得了一个类的应该如何更完美地设计，保证内存不泄露的同时还要使得类的运行效率更高。也通过参考OpenCV文档更加了解了一个矩阵类应当如何设计。由于在现实生活当中一个矩阵类需要支持多种数据类型的矩阵，所以通过模板类的使用，可以达到这一目的，在这一过程当中，我也不懂模板类到熟练操纵模板类完成自己想要做的事情。在ARM开发板上进行开发也帮助我更加理解了计算机的本质。
