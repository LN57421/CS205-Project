#if !defined(AFX_CNUMBER_H__92E62B40_491A_4A75_AB89_FFB160DB2343__INCLUDED_)
#define AFX_CNUMBER_H__92E62B40_491A_4A75_AB89_FFB160DB2343__INCLUDED_

#define INT_BIT_MAX 255
#define FLOAT_BIT_MAX 255

class CNumber {
private:
  int intbits;                   /* 整数数位*/
  int floatbits;                 /* 小数有效数位*/
  char infinite;                 /* 无穷大*/
  char sign;                     /* 符号*/
  char intpart[INT_BIT_MAX];     /* 整数部分*/
  char floatpart[FLOAT_BIT_MAX]; /* 小数部分*/
  char *m_sz;

public:
  // 构造器
  CNumber();
  CNumber(const char *szNum);

  // 析构函数
  ~CNumber();

  // 转换成字符串
  char *toString();

  // 对应析构函数清空
  void clear();

  // 初始化CNumber为0
  void initialToZero();

  // 判断需要多少个字符空间存储CNumber转换的字符串
  int strLenByCNumber();

  // 从字符串转换到CNumber
  void strToCNumber(const char *arr);

  // 从CNumber转换到字符串
  void CNumberToStr(char *szBuf);

  // 删除多余的0
  void clear_zero();

  // 移动小数点,delta=0不移动,delta<0往左移动,delta>0往右移动
  void moveFloatPoint(int delta);

  // 使无穷大
  void makeInfinite();

  // 比较2个数大小
  int compare(const CNumber &n) const;

  // 判断是否为0
  int isZero() const;

  /* 赋值号重载*/
  CNumber &operator=(const CNumber &n);

  /* 运算符重载 */
  CNumber operator+(const CNumber &n);
  CNumber operator-(const CNumber &n);
  CNumber operator*(const CNumber &n);
  CNumber operator/(const CNumber &n);
  CNumber &operator+=(const CNumber &n);
  CNumber &operator-=(const CNumber &n);
  CNumber &operator*=(const CNumber &n);
  CNumber &operator/=(const CNumber &n);

  bool operator>(const CNumber &n);
  bool operator>=(const CNumber &n);
  bool operator<(const CNumber &n);
  bool operator<=(const CNumber &n);
  bool operator==(const CNumber &n);
  bool operator!=(const CNumber &n);

  // 加减乘除
  static void add(const CNumber &n1, const CNumber &n2, CNumber &Res);
  static void multiply(const CNumber &n1, const CNumber &n2, CNumber &Res);
  static void subtract(const CNumber &n1, const CNumber &n2, CNumber &Res);
  static void divide(const CNumber &n1, const CNumber &n2, CNumber &Res);

  // 算术函数指针类型
  typedef void (*CALMATH)(const CNumber &, const CNumber &, CNumber &);

  // 根据算术函数返回结果
  static char *Result(const char *val1, const char *val2, CALMATH pfnCalc);
};

#endif

