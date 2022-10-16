#include <cmath>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

#include "../calculator_h/CNumber.h"
#include "../calculator_h/List.h"
#include "../calculator_h/mathod.h"

using namespace std;

template <typename string>
class ArrayList;

// 储存变量名
static ArrayList<string> varNameList;
// 储存变量值
static ArrayList<string> valueList;
// 常量0
const static CNumber Zero("0");

int main() {
  string firstLine;
  bool wantToLeave = false;
  Welcome();
  varNameList.add("PI");
  valueList.add("3.14159 26535 89793 23846 26433 83279 50288 41971 69399 37");
  varNameList.add("e");
  valueList.add("2.71828 18284 59045 23536 02874 71352 66249 77572 47093 69");

  while (getline(cin, firstLine)) {
    if (firstLine == "help") {
      Help();
      continue;
    }
    if (firstLine == "C") {
      C(varNameList);
      C(valueList);
      varNameList.add("PI");
      valueList.add(
          "3.14159 26535 89793 23846 26433 83279 50288 41971 69399 37");
      varNameList.add("e");
      valueList.add(
          "2.71828 18284 59045 23536 02874 71352 66249 77572 47093 69");
      cout << "C operation is done." << endl;
      continue;
    }
    if (firstLine == "quit") {
      cout << "Are you sure to exit? Please enter [Y]/N to leave or not."
           << endl;
      wantToLeave = true;
      continue;
    }
    if (wantToLeave) {
      if (firstLine == "Y") {
        break;
      } else {
        cout << "You choose to continue your calculate." << endl;
        wantToLeave = false;
        continue;
      }
    }

    firstLine = emptySpace(firstLine);

    if (!isBracketMatching(firstLine)) {
      cout << "The bracket is not matching." << endl;
      continue;
    }

    try {
      if (isAssigning(firstLine)) {
        addVariable(firstLine);
        continue;
      }
    } catch (exception e1) {
      cout << "The variable name is not valid." << endl;
      continue;
    }
    firstLine = replaceAllMathFunctions(firstLine);
    firstLine = replaceVar(firstLine);
    if (!checkUnknownVar(firstLine)) {
      cout << "The variable does not exist." << endl;
      continue;
    }

    cout << answer(firstLine) << endl;
  }
  return 0;
}


void Welcome() {
    cout << "Welcome to the simple calculator_V2." << endl;
    cout << "If you have any problem about how to use, please enter 'help' to "
            "get the using way."
         << endl;
    cout << "Please enter 'quit' to quit this calculator." << endl;
    cout
            << "You can enter the variable assgin or what you want to calculate now: "
            << endl;
}

void Help() {
    cout << "--------------------------------------------------------------------"
         << endl;
    cout << "This is simple calculator_V2 simple help." << endl;
    cout << "Supported functions:" << endl;
    cout << "1. Basic operator: + - * / %" << endl;
    cout << "2. calculation with brackets: ( )" << endl;
    cout << "3. Definition variable by digit, alpha and '_' tips: [the variable "
            "must begin with alpha]."
         << endl;
    cout << "4. Math functions: sqrt, pow, max, min, abs, exp, sin, cos, tan, "
            "ceil, log, floor, random."
         << endl;
    cout << "5. Arbitrary Precision is permitted." << endl;
    cout << "6. const variable set: PI,e." << endl;
    cout << "7. clear input: input 'C' to make the variable assigned clear.";
    cout << "--------------------------------------------------------------------"
         << endl;
}

static void C(ArrayList<string> list) {
    list.clear();
}

string emptySpace(const string &expression) {
  string newString;
  for (char i : expression) {
    if (i != ' ') {
      newString += i;
    }
  }
  return newString;
}

bool isBracketMatching(const string &expression) {
  int left = 0;
  int right = 0;
  for (char i : expression) {
    if (i == '(') {
      left++;
    } else if (i == ')') {
      right++;
    }
  }
  return left == right;
}

int priority(char op) {
  int priority;
  if (op == '+' || op == '-') {
    priority = 1;
  } else if (op == '*' || op == '/') {
    priority = 2;
  } else {
    priority = 0;
  }
  return priority;
}

vector<string> middleToBack(string expression) {
  stack<char> s;
  vector<string> vs;

  for (int i = 0; i < expression.size(); i++) {
    string temp;
    // 是数字的情况下直接输出
    if (expression[i] >= '0' && expression[i] <= '9' ||
        expression[i] >= 'a' && expression[i] <= 'z') {
      temp += expression[i];
      vs.push_back(temp);
      temp = "";
    } else  // 不是数字的情况分类讨论进行判断
    {
      // 栈为空时直接入栈
      if (s.empty()) {
        s.push(expression[i]);
      }
      // 左括号入栈
      else if (expression[i] == '(') {
        s.push(expression[i]);
      }
      // 如果是右括号，只要栈顶不是左括号，就弹出并输出
      else if (expression[i] == ')') {
        while (s.top() != '(') {
          temp += s.top();
          vs.push_back(temp);
          s.pop();
          temp = "";
        }
        // 弹出左括号，但不输出
        s.pop();
      } else {
        // 栈顶元素的优先级大于等于当前的运算符，就将其输出
        while (priority(expression[i]) <= priority(s.top())) {
          temp += s.top();
          vs.push_back(temp);
          s.pop();
          temp = "";
          // 栈为空，停止
          if (s.empty()) {
            break;
          }
        }
        s.push(expression[i]);
      }
    }
  }
  // 最后，如果不为空，就把所以的元素全部弹出
  while (!s.empty()) {
    string temp;
    temp += s.top();
    vs.push_back(temp);
    s.pop();
    temp = "";
  }
  return vs;
}
// 检查是否有未知变量
bool checkUnknownVar(const string &expression) {
  for (char i : expression) {
    if (!(isdigit(i) || i == '.' || i == '+' || i == '-' || i == '*' ||
          i == '/' || i == '(' || i == ')')) {
      return false;
    }
  }
  return true;
}
// 检查是否为赋值语句
bool isAssigning(const string &expression) {
  int indexOfAssign = expression.find('=');

  bool varNameValid = false;

  if (indexOfAssign != -1) {
    varNameValid = true;
    // 截取变量名 并做检查
    string var = expression.substr(0, indexOfAssign);
    // 符合变量名的命名规范
    // 检查变量名开头 我们设定只能以字母开头
    if (!(isalpha(var[0]) || var[0] == '_')) {
      varNameValid = false;
    }
    // 检查开头之外的部分 可以是下划线 字母 数字
    for (int i = 1; i < var.length(); ++i) {
      if (!(isdigit(var[i]) || isalpha(var[i]) || var[i] == '_')) {
        varNameValid = false;
        break;
      }
    }
    // 如果变量检查出错 抛出异常
    if (!varNameValid) {
      throw exception();
    }
    return varNameValid;
  }
  return varNameValid;
}
// 添加变量
void addVariable(const string &expression) {
  int index = expression.find('=');

  string varName = expression.substr(0, index);

  string valStr = expression.substr(index + 1);

  valStr = replaceAllMathFunctions(valStr);

  bool contains = false;

  for (int i = 0; i < varNameList.getSize(); ++i) {
    if (varNameList.get(i) == varName) {
      contains = true;
      break;
    }
  }

  if (contains) {
    int index = 0;
    for (int i = 0; i < varNameList.getSize(); ++i) {
      if (varNameList.get(i) == varName) {
        index = i;
        break;
      }
    }
    valueList.set(index, valStr);
  } else {
    varNameList.add(varName);
    valueList.add(valStr);
  }
}
// 替换将变量替换为真实数值
string replaceVar(string expression) {
  for (int i = 0; i < varNameList.getSize(); ++i) {
    while (expression.find(varNameList.get(i)) != -1) {
      int index = expression.find(varNameList.get(i));
      string before = expression.substr(0, index);
      string after = expression.substr(index + varNameList.get(i).length());
      expression = before + valueList.get(i) + after;
    }
  }
  return expression;
}
// 检查是否包含数学函数
bool containsCertainMathFunctions(const string &expression, string funName) {
  return expression.find(funName) != -1;
}
// 数学方法
string sqrt(string expression) {
  int first = expression.find("sqrt(");
  int second = first;

  for (int i = first; i < expression.length(); i++) {
    if (expression[i] == ')') {
      second = i;
      break;
    }
  }

  string string_first = expression.substr(0, first);
  string string_second = expression.substr(second + 1);
  // 这里的下标进行对不同函数需要进行对应的修改
  string temp = expression.substr(first + 4, second - first - 3);
  // 构建新的CNumber value
  CNumber value(answer(temp).c_str());

  if (value < Zero) {
    // 如果遇到无效输出的情况直接返回nan
    expression = "nan";
    return expression;
  }
  string string_middle = to_string(sqrt(stod(value.toString())));

  expression = string_first + string_middle + string_second;
  return expression;
}
string pow(string expression) {
  int first = expression.find("pow(");
  int second = first;
  int temp = first;
  for (int i = first; i < expression.length(); i++) {
    if (expression[i] == ',') {
      temp = i;
    } else if (expression[i] == ')') {
      second = i;
      break;
    }
  }
  string string_first = expression.substr(0, first);
  string string_second = expression.substr(second + 1);

  string temp1 = expression.substr(first + 4, temp - first - 4);
  string temp2 = expression.substr(temp + 1, second - temp - 1);

  CNumber value1(answer(temp1).c_str());
  CNumber value2(answer(temp2).c_str());

  string string_middle =
      to_string(pow(stod(value1.toString()), stod(value2.toString())));
  expression = string_first + string_middle + string_second;
  return expression;
}
string max(string expression) {
  int first = expression.find("max(");
  int second = first;
  int temp = first;
  for (int i = first; i < expression.length(); i++) {
    if (expression[i] == ',') {
      temp = i;
    } else if (expression[i] == ')') {
      second = i;
      break;
    }
  }
  string string_first = expression.substr(0, first);
  string string_second = expression.substr(second + 1);

  string temp1 = expression.substr(first + 4, temp - first - 4);
  string temp2 = expression.substr(temp + 1, second - temp - 1);

  string middle_string = CNumber(temp1.c_str()) > CNumber(temp2.c_str())
                             ? CNumber(temp1.c_str()).toString()
                             : CNumber(temp2.c_str()).toString();
  expression = string_first + middle_string + string_second;
  return expression;
}
string min(string expression) {
  int first = expression.find("max(");
  int second = first;
  int temp = first;
  for (int i = first; i < expression.length(); i++) {
    if (expression[i] == ',') {
      temp = i;
    } else if (expression[i] == ')') {
      second = i;
      break;
    }
  }
  string string_first = expression.substr(0, first);
  string string_second = expression.substr(second + 1);

  string temp1 = expression.substr(first + 4, temp - first - 4);
  string temp2 = expression.substr(temp + 1, second - temp - 1);

  string middle_string = CNumber(temp1.c_str()) < CNumber(temp2.c_str())
                             ? CNumber(temp1.c_str()).toString()
                             : CNumber(temp2.c_str()).toString();
  expression = string_first + middle_string + string_second;
  return expression;
  return expression;
}
string abs(string expression) {
  int first = expression.find("abs(");
  int second = first;
  for (int i = first; i < expression.length(); i++) {
    if (expression[i] == ')') {
      second = i;
      break;
    }
  }
  string string_first = expression.substr(0, first);
  string string_second = expression.substr(second + 1);

  string temp = expression.substr(first + 4, second - first - 4);

  CNumber value(answer(temp).c_str());
  string middle_string = value.toString();
  expression = string_first + middle_string + string_second;
  return expression;
}
string random(string expression) {
  int first = expression.find("abs(");
  int second = first;
  for (int i = first; i < expression.length(); i++) {
    if (expression[i] == ')') {
      second = i;
      break;
    }
  }
  string string_first = expression.substr(0, first);
  string string_second = expression.substr(second + 1);

  string middle_string = to_string(rand());
  expression = string_first + middle_string + string_second;
  return expression;
}
string exp(string expression) {
  int first = expression.find("abs(");
  int second = first;
  for (int i = first; i < expression.length(); i++) {
    if (expression[i] == ')') {
      second = i;
      break;
    }
  }
  string string_first = expression.substr(0, first);
  string string_second = expression.substr(second + 1);

  string temp = expression.substr(first + 4, second - first - 4);

  string middle_string = to_string(exp(stod(temp)));
  expression = string_first + middle_string + string_second;
  return expression;
}

string sin(string expression) {
  int first = expression.find("abs(");
  int second = first;
  for (int i = first; i < expression.length(); i++) {
    if (expression[i] == ')') {
      second = i;
      break;
    }
  }
  string string_first = expression.substr(0, first);
  string string_second = expression.substr(second + 1);

  string temp = expression.substr(first + 4, second - first - 4);

  string middle_string = to_string(sin(stod(answer(temp))));
  expression = string_first + middle_string + string_second;
  return expression;
}

string cos(string expression) {
  int first = expression.find("abs(");
  int second = first;
  for (int i = first; i < expression.length(); i++) {
    if (expression[i] == ')') {
      second = i;
      break;
    }
  }
  string string_first = expression.substr(0, first);
  string string_second = expression.substr(second + 1);

  string temp = expression.substr(first + 4, second - first - 4);

  string middle_string = to_string(cos(stod(answer(temp))));
  expression = string_first + middle_string + string_second;
  return expression;
}
string tan(string expression) {
  int first = expression.find("abs(");
  int second = first;
  for (int i = first; i < expression.length(); i++) {
    if (expression[i] == ')') {
      second = i;
      break;
    }
  }
  string string_first = expression.substr(0, first);
  string string_second = expression.substr(second + 1);

  string temp = expression.substr(first + 4, second - first - 4);

  string middle_string = to_string(tan(stod(answer(temp))));
  expression = string_first + middle_string + string_second;
  return expression;
}
string ceil(string expression) {
  int first = expression.find("abs(");
  int second = first;
  for (int i = first; i < expression.length(); i++) {
    if (expression[i] == ')') {
      second = i;
      break;
    }
  }
  string string_first = expression.substr(0, first);
  string string_second = expression.substr(second + 1);
  string temp = expression.substr(first + 5, second - first - 5);

  string middle_string = to_string(ceil(stod(answer(temp))));
  expression = string_first + middle_string + string_second;
  return expression;
}
string floor(string expression) {
  int first = expression.find("abs(");
  int second = first;
  for (int i = first; i < expression.length(); i++) {
    if (expression[i] == ')') {
      second = i;
      break;
    }
  }
  string string_first = expression.substr(0, first);
  string string_second = expression.substr(second + 1);
  string temp = expression.substr(first + 5, second - first - 6);

  string middle_string = to_string(floor(stod(answer(temp))));
  expression = string_first + middle_string + string_second;
  return expression;
}
string log(string expression) {
  int first = expression.find("abs(");
  int second = first;
  for (int i = first; i < expression.length(); i++) {
    if (expression[i] == ')') {
      second = i;
      break;
    }
  }
  string string_first = expression.substr(0, first);
  string string_second = expression.substr(second + 1);

  string temp = expression.substr(first + 4, second - first - 4);

  string middle_string = to_string(log(stod(answer(temp))));
  expression = string_first + middle_string + string_second;
  return expression;
}

// 将所有的数学函数替换成真实值
string replaceAllMathFunctions(string expression) {
  if (expression == "nan") {
    return expression;
  }

  expression = replaceVar(expression);

  while (containsCertainMathFunctions(expression, "sqrt(")) {
    expression = sqrt(expression);
  }
  while (containsCertainMathFunctions(expression, "pow(")) {
    expression = pow(expression);
  }
  while (containsCertainMathFunctions(expression, "max(")) {
    expression = max(expression);
  }
  while (containsCertainMathFunctions(expression, "min(")) {
    expression = min(expression);
  }
  while (containsCertainMathFunctions(expression, "abs(")) {
    expression = abs(expression);
  }
  while (containsCertainMathFunctions(expression, "random()")) {
    expression = random(expression);
  }
  while (containsCertainMathFunctions(expression, "exp(")) {
    expression = exp(expression);
  }
  while (containsCertainMathFunctions(expression, "sin(")) {
    expression = sin(expression);
  }
  while (containsCertainMathFunctions(expression, "cos(")) {
    expression = cos(expression);
  }
  while (containsCertainMathFunctions(expression, "tan(")) {
    expression = tan(expression);
  }
  while (containsCertainMathFunctions(expression, "ceil(")) {
    expression = ceil(expression);
  }
  while (containsCertainMathFunctions(expression, "floor(")) {
    expression = floor(expression);
  }
  while (containsCertainMathFunctions(expression, "log(")) {
    expression = log(expression);
  }
  return expression;
}

// 利用后缀表达式得出结果
string result(vector<string> bh) {
  stack<string> st;
  string num, op1, op2;

  for (int i = 0; i < bh.size(); i++) {
    string tmp = bh[i];
    if (tmp[0] >= '0' && tmp[0] <= '9') {
      st.push(tmp);
    } else if (bh[i] == "+") {
      CNumber cn1(st.top().c_str());
      st.pop();
      CNumber cn2(st.top().c_str());
      st.pop();
      st.push((cn1 + cn2).toString());
    } else if (bh[i] == "-") {
      CNumber cn2(st.top().c_str());
      st.pop();
      CNumber cn1(st.top().c_str());
      st.pop();
      st.push((cn1 - cn2).toString());  // 精髓啊
    } else if (bh[i] == "*") {
      CNumber cn1(st.top().c_str());
      st.pop();
      CNumber cn2(st.top().c_str());
      st.pop();
      st.push((cn1 * cn2).toString());
    } else if (bh[i] == "/") {
      CNumber cn2(st.top().c_str());
      st.pop();
      CNumber cn1(st.top().c_str());
      st.pop();
      st.push((cn1 / cn2).toString());
      if (cn2 == Zero) {
        return "#INFINITE";
      }
    }
  }
  return st.top();
}
// 封装方法
string answer(string expression) {
  if (expression == "nan") {
    return expression;
  }
  expression = replaceVar(expression);
  vector<string> bh = middleToBack(expression);
  string k = result(bh);
  return k;
}
