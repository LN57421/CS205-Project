#include "../calculator_h/CNumber.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>

using namespace std;

CNumber::CNumber() { initialToZero(); }

CNumber::CNumber(const char *szNum) {
    initialToZero();
    strToCNumber(szNum);
}

CNumber::~CNumber() { clear(); }

void CNumber::clear() {
  if (m_sz)
    delete[] m_sz;
  m_sz = NULL;
}

void CNumber::initialToZero() { memset(this, 0, sizeof(CNumber)); }

int CNumber::strLenByCNumber() {
  int len = floatbits + intbits + 1;
  if (intbits == 0)
    len++; /* .1 --> 0.1*/
  if (floatbits)
    len++; /* '.'*/
  if (sign)
    len++; /* '-'*/
  if (infinite)
    return 11; /* #INFINITE */
  return len;
}

void CNumber::strToCNumber(const char *arr) {
  const char *point;
    initialToZero();
  if (*arr == '-') /* 如果是负数*/
  {
    arr++;
    sign = 1;
  }

  point = strchr(arr, '.');

  if (point) /* 找到小数点 */
  {
    int n = intbits = point - arr; /* 计算出整数数位 ？？？？？*/

    while (n) /* 整数数位不==0则循环 */
    {
      intpart[intbits - n] = arr[n - 1] - '0'; /* 将数字低位存在低下标元素*/
      n--;
    }

    while (*++point) {
      floatpart[floatbits] = *point - '0';
      floatbits++;
    }
  } else /* 说明没写小数点,全是整数.*/
  {
    int n = intbits = strlen(arr);
    while (n) {
      intpart[intbits - n] = arr[n - 1] - '0';
      n--;
    }
  }
    clear_zero();

  /* 处理-0 和0的情况*/

  if (floatbits == 0) {
    if (intbits == 0 || intbits == 1 && intpart[0] == 0)
      sign = 0;
  }
}

void CNumber::CNumberToStr(char *szBuf) {
  int n = intbits, c;
  memset(szBuf, 0, strLenByCNumber());
  if (sign) /* 如果是负数*/
  {
    *szBuf++ = '-';
  }
  if (infinite) {
    strcat(szBuf, "#INFINITE");
    return;
  }
  while (n) {
    szBuf[intbits - n] = intpart[n - 1] + '0';
    n--;
  }
  c = 0; /* 是否加了0*/
  if (intbits == 0) {
    strcat(szBuf, "0");
    c = 1;
  }
  if (floatbits)
    strcat(szBuf, ".");
  n = 0;
  while (n < floatbits) {
    szBuf[intbits + 1 + n + c] = floatpart[n] + '0';
    n++;
  }
}
void CNumber::clear_zero() {
  while (intbits > 1 && intpart[intbits - 1] == 0)
    intbits--;
  while (floatbits && floatpart[floatbits - 1] == 0)
    floatbits--;
}
void CNumber::moveFloatPoint(int delta) {
  /* delta<0则往左移动小数点,delta>0则向右移动小数点 */
  if (delta) {
    CNumber n = *this;
      initialToZero();
    sign = n.sign;
    if (delta < 0) {
      int i;
      delta = -delta;
      for (i = delta; i < n.intbits; i++) {
        intpart[intbits++] = n.intpart[i];
      }
      for (i = delta - 1; i >= 0; i--) {
        floatpart[floatbits++] = n.intpart[i];
      }
      for (i = 0; i < n.floatbits; i++) {
        floatpart[floatbits++] = n.floatpart[i];
      }
    } else {
      int i;
      for (i = delta; i < n.floatbits; i++) /* 处理小数部分*/
      {
        floatpart[floatbits++] = n.floatpart[i];
      }
      for (i = delta - 1; i >= 0; i--) /* 小数到整数的部分*/
      {
        intpart[intbits++] = n.floatpart[i];
      }
      for (i = 0; i < n.intbits; i++) /* 整数部分*/
      {
        intpart[intbits++] = n.intpart[i];
      }
    }
  }
    clear_zero();
}
void CNumber::makeInfinite() { infinite = 1; }

int CNumber::compare(const CNumber &n) const {
  /* 首先是比较符号*/
  if (sign == 0 && n.sign != 0)      /* pn1是正数,pn2是负数*/
    return 1;                        /* >*/
  else if (sign != 0 && n.sign == 0) /* pn1是负数,pn2是正数*/
    return -1;                       /* <*/
  else                               /* 同号状态*/
  {
    /* 比较整数部分*/
    if (intbits > n.intbits) /* pn1整数数位多*/
      return sign ? -1 : 1;
    else if (intbits < n.intbits)
      return sign ? 1 : -1;
    else /* 整数数位相同*/
    {
      int i = intbits - 1; /*指到最高位*/
      while (i >= 0) {
        if (intpart[i] > n.intpart[i])
          return sign ? -1 : 1;
        else if (intpart[i] < n.intpart[i])
          return sign ? 1 : -1;
        else
          i--;
      }
      /* 整数部分相同,比较小数部分*/
      for (i = 0; i < floatbits && i < n.floatbits;) {
        if (floatpart[i] > n.floatpart[i])
          return sign ? -1 : 1;
        else if (floatpart[i] < n.floatpart[i])
          return sign ? 1 : -1;
        else
          i++;
      }
      if (i < floatbits)
        return sign ? -1 : 1;
      if (i < n.floatbits)
        return sign ? 1 : -1;
      return 0; /* 相等*/
    }
  }
}
int CNumber::isZero() const {
  if (floatbits == 0 && intbits == 0)
    return 1;
  if (floatbits == 0 && intbits == 1 && intpart[0] == 0)
    return 1;
  return 0;
}

void CNumber::add(const CNumber &n1, const CNumber &n2, CNumber &Res) {
    Res.initialToZero();
  if (n1.sign ^ n2.sign) /*异号*/
  {
    CNumber rn2 = n2;
    rn2.sign = n1.sign;
    subtract(n1, rn2, Res);
  } else /*同号*/
  {
    int maxfloatbits =
        n1.floatbits > n2.floatbits ? n1.floatbits : n2.floatbits;
    int addbit = 0; /* 进位值*/
    int i, j;
    for (i = maxfloatbits - 1; i >= 0; i--) {
      int value = n1.floatpart[i] + n2.floatpart[i] + addbit;
      addbit = value / 10; /* 看看是否超过10. 设置进位值*/
      Res.floatpart[i] = value % 10;
    }
    Res.floatbits = maxfloatbits;
    /* 到此,小数部分计算完毕.*/
    for (j = 0; j < n1.intbits || j < n2.intbits; j++) {
      int value = n1.intpart[j] + n2.intpart[j] + addbit;
      addbit = value / 10;
      Res.intpart[j] = value % 10;
      Res.intbits++;
    }
    if (addbit > 0) {
      Res.intpart[j] = addbit;
      Res.intbits++;
    }
    Res.sign = n1.sign; /*决定符号*/
      Res.clear_zero();
  }
}

void CNumber::multiply(const CNumber &n1, const CNumber &n2, CNumber &Res) {
  CNumber z1 = n1, z2 = n2;
  CNumber sum;
  int i, j;
    sum.initialToZero();
    Res.initialToZero();
    z1.moveFloatPoint(z1.floatbits);
    z2.moveFloatPoint(z2.floatbits);
  /* 计算z1*z2 */
  for (i = 0; i < z2.intbits; i++) {
    CNumber tmp; /* 存放临时乘积*/
    int addbit = 0;
    tmp.intbits = z1.intbits + i;
    for (j = 0; j < z1.intbits; j++) {
      int value = z2.intpart[i] * z1.intpart[j] + addbit;
      addbit = value / 10;
      tmp.intpart[j + i] = value % 10;
    }
    if (addbit) {
      tmp.intpart[j + i] = addbit;
      tmp.intbits++;
    }
    add(sum, tmp, Res);
    sum = Res;
  }
  Res = sum;
    Res.moveFloatPoint(-(n1.floatbits + n2.floatbits));
  /* 判断符号,异号为负*/
  if (n1.sign ^ n2.sign)
    Res.sign = 1;
}

void CNumber::subtract(const CNumber &n1, const CNumber &n2, CNumber &Res) {
    Res.initialToZero();
  if (n1.sign ^ n2.sign) /* 异号情况*/
  {
    CNumber rn2 = n2;
    rn2.sign = n1.sign;
    add(n1, rn2, Res);
  } else /* 同号情况*/
  {
    int cmp = n1.compare(n2);
    int swapflag, i, maxfloatbits, subtractbit;
    if (cmp == 0)
      return; /* 相等就没必要再减了.*/
    swapflag = n1.sign == 0 ? cmp == -1 : cmp == 1;
    const CNumber *pn1 = &n1;
    const CNumber *pn2 = &n2;
    if (swapflag) {
      const CNumber *t = pn1;
      pn1 = pn2;
      pn2 = t;
    }
    maxfloatbits =
        pn1->floatbits > pn2->floatbits ? pn1->floatbits : pn2->floatbits;
    subtractbit = 0; /* 退位值*/
    /* 先计算小数部分*/
    for (i = maxfloatbits - 1; i >= 0; i--) {
      if (pn1->floatpart[i] - subtractbit < pn2->floatpart[i]) {
        int value = pn1->floatpart[i] - pn2->floatpart[i] - subtractbit + 10;
        subtractbit = 1;
        Res.floatpart[i] = value;
      } else {
        int value = pn1->floatpart[i] - pn2->floatpart[i] - subtractbit;
        subtractbit = 0;
        Res.floatpart[i] = value;
      }
    }
    Res.floatbits = maxfloatbits;
    /* 至此小数部分计算完毕.*/
    for (i = 0; i < pn1->intbits || i < pn2->intbits; i++) {
      if (pn1->intpart[i] - subtractbit < pn2->intpart[i]) {
        int value = pn1->intpart[i] - pn2->intpart[i] - subtractbit + 10;
        subtractbit = 1;
        Res.intpart[i] = value;
      } else {
        int value = pn1->intpart[i] - pn2->intpart[i] - subtractbit;
        subtractbit = 0;
        Res.intpart[i] = value;
      }
      Res.intbits++;
    }
    Res.sign = swapflag ? !n1.sign : n1.sign; /*决定符号*/
      Res.clear_zero();
  }
}
void CNumber::divide(const CNumber &n1, const CNumber &n2, CNumber &Res) {
  CNumber z1 = n1, z2 = n2;
  int deta = z2.floatbits - z1.floatbits;
    z1.moveFloatPoint(z1.floatbits);
    z2.moveFloatPoint(z2.floatbits);
    Res.initialToZero();
  if (n1.isZero()) {
    return;
  }
  if (n2.isZero()) {
    Res.sign = n1.sign;
      Res.makeInfinite();
    return;
  }
  z1.sign = z2.sign = 0;             /*统一符号,便于比较大小*/
  while (z1.intbits != z2.intbits) { /*确保数位相等,这步耗费很多时间*/
    if (z1.intbits < z2.intbits) {
        z1.moveFloatPoint(1);
      deta--;
    } else {
        z2.moveFloatPoint(1);
      deta++;
    }
  }
  while (Res.floatbits < (INT_BIT_MAX / 2)) {
    int cmp = z1.compare(z2);
    int n = 10;
    CNumber mulres, subres;
    if (cmp == -1) { /*z1<z2*/
        z1.moveFloatPoint(1);
      Res.floatpart[Res.floatbits++] = 0;
      continue;
    } else if (cmp == 0) { /*z1==z2*/
      Res.floatpart[Res.floatbits++] = 1;
      break;
    }
    do { /*找商*/
      CNumber tmp;
      tmp.intpart[0] = --n;
      tmp.intbits = 1;
      multiply(z2, tmp, mulres);
    } while ((cmp = mulres.compare(z1)) == 1);
    Res.floatpart[Res.floatbits++] = n;
    if (cmp == 0)
      break;
    subtract(z1, mulres, subres);
      subres.moveFloatPoint(1);
    z1 = subres;
  }
    Res.moveFloatPoint(1);
    Res.moveFloatPoint(deta);
  /* 判断符号,异号为负*/
  if (n1.sign ^ n2.sign)
    Res.sign = 1;
}
char *CNumber::Result(const char *val1, const char *val2, CALMATH pfnCalc) {
  CNumber n1, n2, res;
    n1.strToCNumber(val1);
    n2.strToCNumber(val2);
  pfnCalc(n1, n2, res);
  return res.toString();
}

char *CNumber::toString() {
    clear();
  m_sz = new char[strLenByCNumber()];
    CNumberToStr(m_sz);
  return m_sz;
}

CNumber &CNumber::operator=(const CNumber &n) {
  if (this != &n) {
      clear();
    memcpy(this, &n, sizeof(CNumber));
    if (n.m_sz) {
      m_sz = strdup(n.m_sz);
    }
  }
  return *this;
}

CNumber CNumber::operator+(const CNumber &n) {
  CNumber res;
  CNumber::add(*this, n, res);
  return res;
}
CNumber CNumber::operator-(const CNumber &n) {
  CNumber res;
  CNumber::subtract(*this, n, res);
  return res;
}
CNumber CNumber::operator*(const CNumber &n) {
  CNumber res;
  CNumber::multiply(*this, n, res);
  return res;
}
CNumber CNumber::operator/(const CNumber &n) {
  CNumber res;
  CNumber::divide(*this, n, res);
  return res;
}
CNumber &CNumber::operator+=(const CNumber &n) {
  CNumber n1 = *this, n2 = n;
  CNumber::add(n1, n2, *this);
  return *this;
}
CNumber &CNumber::operator-=(const CNumber &n) {
  CNumber n1 = *this, n2 = n;
  CNumber::subtract(n1, n2, *this);
  return *this;
}
CNumber &CNumber::operator*=(const CNumber &n) {
  CNumber n1 = *this, n2 = n;
  CNumber::multiply(n1, n2, *this);
  return *this;
}
CNumber &CNumber::operator/=(const CNumber &n) {
  CNumber n1 = *this, n2 = n;
  CNumber::divide(n1, n2, *this);
  return *this;
}
bool CNumber::operator>(const CNumber &n) { return compare(n) == 1; }
bool CNumber::operator>=(const CNumber &n) { return compare(n) != -1; }
bool CNumber::operator<(const CNumber &n) { return compare(n) == -1; }
bool CNumber::operator<=(const CNumber &n) { return compare(n) != 1; }
bool CNumber::operator==(const CNumber &n) { return compare(n) == 0; }
bool CNumber::operator!=(const CNumber &n) { return compare(n) != 0; }

