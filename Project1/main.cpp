#include "mathod.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

typedef long long ll;

int main(int argc, char **argv) {

  string first_string, second_string; // 用两个字符串储存起始输入的两个数
  bool first_loop = true;             // 对是否第一次进入程序做标记
  int judge1, judge2;

  // 如果有科学计数法需要的话 以下变量将储存变量信息
  int E1 = 0;
  int point1 = -1;
  int first_part1 = 0;
  int second_part1 = 0;

  int E2 = 0;
  int point2 = -1;
  int first_part2 = 0;
  int second_part2 = 0;

loop:
  // 1.用命令行运行程序时加载数据的输入
  if (first_loop) {
    if (argc <= 1) {
      cout << "You haven't put any number in command" << endl;
      first_loop = false; // 将first_loop设置为false
      goto loop;
    } else if (argc != 3) {
      cout << "Sorry, you should enter two proper numbers, please try again or "
              "enter \" quit \" to quit the loop:"
           << endl;
      first_loop = false;
      goto loop;
    }

    first_string = argv[1];
    second_string = argv[2];
    first_loop = false;

    if ((!isNumber(first_string) || !isNumber(second_string))) {
      cout << "Wrong input! Please inter two Numbers, or enter \" quit \" to "
              "quit the loop:"
           << endl;
      goto loop;
    }
  } else {
    // 2.用命令行输入参数计算
    cout << "Please inter two Numbers, or enter \" quit \" to quit the loop:"
         << endl;
    cin >> first_string;

    // 无论是否为第一次输入程序 当输入的第一个字符串为quit时，退出程序
    if (first_string == "quit") {
      return 0;
    }
    cin >> second_string;
  }

  // 无论是否为第一次输入程序 当输入的第一个字符串为quit时，退出程序
  if (first_string == "quit") {
    return 0;
  }

  // 判断两个字符串的数据类型
  judge1 = judgeType(first_string);
  judge2 = judgeType(second_string);

  // 任意一个数字并非数字
  if (judge1 == -1 || judge2 == -1) {
    cout << "Wrong input! Please inter two Numbers, or enter \" quit \" to "
            "quit the loop:"
         << endl;
    goto loop;
  }

  first_string = doubleToString(stringToDouble(first_string));
  second_string = doubleToString(stringToDouble(second_string));

  // 判断两个字符串的数据类型
  judge1 = judgeType(first_string);
  judge2 = judgeType(second_string);

  // 在经过处理以后仍然是科学计数法表达形式
  if (judge1 == 2 && judge2 == 2) {
    int carry = 0;
    int new_first_part = 0;
    int new_second_part = 0;

    for (int i = 0; i < first_string.size(); ++i) {
      if (first_string[i] == '.') {
        point1 = i;
      }
      if (first_string[i] == 'E' || first_string[i] == 'e') {
        E1 = i;
      }
    }
    first_part1 = stoi(first_string.substr(0, E1));
    second_part1 = stoi(first_string.substr(E1 + 1));

    for (int i = 0; i < first_string.size(); ++i) {
      if (first_string[i] == '.') {
        point2 = i;
      }
      if (first_string[i] == 'E' || first_string[i] == 'e') {
        E2 = i;
      }
    }
    first_part2 = stoi(first_string.substr(0, E2));
    second_part2 = stoi(first_string.substr(E2 + 1));

    new_second_part = second_part1 + second_part2;
    new_first_part = first_part1 * first_part2;

    if (new_first_part >= 10) {
      new_first_part %= 10;
      carry++;
    }
    new_second_part += carry;
    cout << first_string + " * " + second_string + " = ";
    cout << new_first_part << 'e' << new_second_part << endl;
    goto loop;
  } else {
    cout << first_string + " * " + second_string + " = ";
    // 判断结果的正负
    bool isNegative = false;
    if ((first_string[0] == '-' && second_string[0] != '-') ||
        (first_string[0] != '-' && second_string[0] == '-')) {
      isNegative = true;
    }

    ll first_string_size = first_string.size();
    ll second_string_size = second_string.size();

    if (first_string[0] == '-') {
      first_string_size--;
      first_string = first_string.substr(
          1, first_string_size - 1); // 截取除了负号后面的一段字符串做之后的计算
    }
    if (second_string[0] == '-') {
      second_string_size--;
      second_string = second_string.substr(
          1,
          second_string_size - 1); // 截取除了负号后面的一段字符串做之后的计算
    }

    judge1 = judgeType(first_string);
    judge2 = judgeType(second_string);
    string res;

    if (judge1 == 3 && judge2 == 3) {
      res = multiply1(first_string, second_string);
      if (isNegative) {
        res = '-' + res;
      }

      cout << res << endl;
    }

    if (judge1 == 1 || judge2 == 1) {
      int PointA = 0;
      int PointB = 0;
      int sum;
      for (int i = 0; i < first_string_size; ++i) {
        if (first_string[i] == '.') {
          // 记录PointA的下标
          PointA = first_string_size - (i + 1);
          first_string.erase(i, 1);
        }
      }
      for (int i = 0; i < second_string_size; ++i) {
        if (second_string[i] == '.') {
          // 记录PointA的下标
          PointB = second_string_size - (i + 1);
          second_string.erase(i, 1);
        }
      }

      sum = PointA + PointB;

      res = multiply1(first_string, second_string);

      if (res.size() - sum != 0)
        res.insert(res.size() - sum, 1, '.');

      if (isNegative) {
        res = '-' + res;
      }
      cout << res << endl;
    }

    goto loop; // 运算完成之后回到程序起始，再次进行新一轮运算
  }
}

bool isInt(const char *str) {
  bool isInteger = false;

  int index = 0;
  while (*str != '\0') {
    switch (*str) {
    case '0' ... '9':
      isInteger = true;
      break;
    case '+':
    case '-':
      if (index != 0) {
        return false;
      }
      break;
    default:
      return false;
    }
    str++;
    index++;
  }

  if (isInteger) {
    return true;
  }
  return false;
}

bool isFloatE(const char *str) {
  int isE = 0;
  int isPoint = -1;
  int symbol = 1;
  bool numBefore = false;
  bool numBehind = false;

  int index = 0;
  while (*str != '\0') {
    switch (*str) {
    case '.':
      isPoint = index;
      break;
    case '0' ... '9':
      if (isE) {
        numBehind = true;
      } else {
        numBefore = true;
      }
      break;
    case '+':
    case '-':
      if (index != 0 && index != isE + 1) {
        symbol = 0;
      }
      break;
    case 'e':
    case 'E':
      if (isE || !numBefore) {
        return false;
      } else {
        isE = index;
      }
      break;
    default:
      return 0;
    }
    index++;
    str++;
  }

  if (!numBefore) {
    return 0;
  } else if (!symbol) {
    return 0;
  } else if (isE && !numBehind) {
    return 0;
  }

  return isE;
}

bool isFloatPoint(const char *str) {
  int isPoint = 0;
  bool numBefore = false;
  bool numBehind = false;

  int index = 0;
  while (*str != '\0') {
    switch (*str) {
    case '0' ... '9':
      if (isPoint) {
        numBehind = true;
      } else {
        numBefore = true;
      }
      break;
    case '.':
      if (isPoint || !numBefore) {
        return false;
      } else {
        isPoint = index;
      }
      break;
    default:
      return 0;
    }
    index++;
    str++;
  }

  if (!numBefore) {
    return 0;
  } else if (isPoint && !numBehind) {
    return 0;
  }

  return isPoint;
}

// 封装方法 判断字符串是否为一个数
bool isNumber(const string &s) {
  if (isInt(s.c_str()) || isFloatPoint(s.c_str()) || isFloatE(s.c_str())) {
    return true;
  }

  return false;
}

vector<int> mul(vector<int> A, vector<int> B) {
  vector<int> C(A.size() + B.size(), 0);

  for (int i = 0; i < A.size(); ++i) {
    for (int j = 0; j < B.size(); ++j) {
      C[i + j] += A[i] * B[j];
    }
  }

  int temp = 0;
  for (int &i : C) {
    temp += i;
    i = temp % 10;
    temp /= 10;
  }

  while (C.size() > 1 && C.back() == 0) {
    C.pop_back();
  }
  return C;
}

string multiply1(string num1, string num2) {
  string res;
  vector<int> A, B, C;

  for (int i = num1.size() - 1; i >= 0; i--) {
    A.push_back(num1[i] - '0');
  }
  for (int i = num2.size() - 1; i >= 0; i--) {
    B.push_back(num2[i] - '0');
  }

  C = mul(A, B);
  for (int i = C.size() - 1; i >= 0; i--) {
    res += to_string(C[i]);
  }
  return res;
}

string doubleToString(double d) {
  string s;
  stringstream ss;
  ss << setprecision(16) << d;
  s = ss.str();
  ss.clear();
  return s;
}

double stringToDouble(string s) {
  double d;
  stringstream ss;
  ss << s;
  ss >> setprecision(16) >> d;
  ss.clear();
  return d;
}

int judgeType(const string &str) {
  if (isFloatPoint(str.c_str()))
    return 1;
  if (isFloatE(str.c_str()))
    return 2;
  if (isInt(str.c_str()))
    return 3;

  return -1;
}