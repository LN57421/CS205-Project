# CS205 C/ C++ Programming- A Simple Calculator_V2

### name：刘家宝 (Liu Jiabao)

### SID: 12110416

[toc]

## Part 1 -Analysis

### 1.1 总体目标

实现一个更为优秀的计算器

### 1.2 具体需求

### CNumber类的创建

#### 为什么需要CNumber类

在project1中，笔者为输入的两个字符串分别设置了四个变量，记录E的下标，小数点的位置，小数点左边部分的子字符串，以及小数点右边部分的子字符串

```c++
  // 如果有科学计数法需要的话 以下变量将储存变量信息
  int E1 = 0;
  int point1 = -1;
  string first_part1 = 0;
  string second_part1 = 0;
```

然而这样做在project2会有两个问题：

1.输入的数据个数不定，不知道要设置多少个变量

2.变量全部写在main方法里，过于冗杂，难以管理

同时，在project1中我们只需要计算高精度的乘法，对于输入的数字本身而言，所用到的方法并不多，然而在project2中我们需要实现更多方法，这些方法同样需要更好的管理

那么，有没有一种方法可以解决上面的两个问题呢？这种方法就是构建一个类，这个类应该包含了并且拓展了更多的属性，包含project2中运算所需要的方法，在本次project中，体现为两个文件——CNumber.h、CNumber.cpp

#### CNumber类的实现

在本次project实现中，CNumber类的实现主要依赖于两个文件，CNumber.h，CNumber.cpp

**【声明】**这两个文件部分实现借鉴自博客园的一份源码，同时笔者对源码做了适当修改，以能适应本次project所需，您可以点击[这里](https://www.cnblogs.com/acbingo/p/4116206.html)

查看源代码，对源码的引用与改写已经得到授权

---

在CNumber.h以及CNumber.cpp中，主要满足了如下实现：

1.为CNmber类创建类变量，由于输入时变量已经确定，所以全部创建为private类型以防止任意修改

2.创建无参构造器以及通过const char*指针创建的含参构造器

3.创建析构函数以及clear函数，以在每次运算完毕后释放资源

4.构建了算数函数指针类型CALMATH，能适应任意长度的整数和小数的加减乘除以及取模运算

5.赋值、运算、操作符的重载

6.输入表达式多余0的清除

7.计算结果小数点移动的支持

8.将数学表达式对象转化为可读小数或整数以计算（1e-2转为0.02）

9.支持结果无穷大的表达（例如除0得到INFINITE）

10.高精度运算的实现

**CNumber类**的代码声明如下:

```c++
class CNumber {
private:
  int intbits;                   // 整数有效数位
  int floatbits;                 // 小数有效数位
  char infinite;                 // 无穷大
  char sign;                     // 符号
  char intpart[255];             // 整数部分
  char floatpart[255];           // 小数部分
  char *m_sz;					 // CNumber内置指针

public:
  // 无参构造器以及长度确定的构造器
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

  // 比较两个数大小
  int compare(const CNumber &n) const;

  // 判断是否为0
  int isZero() const;

  // 赋值号重载
  CNumber &operator=(const CNumber &n);

  // 运算符重载
  CNumber operator+(const CNumber &n);
  CNumber operator-(const CNumber &n);
  CNumber operator*(const CNumber &n);
  CNumber operator/(const CNumber &n);
  CNumber operator%(const CNumber &n); 
  
  CNumber &operator+=(const CNumber &n);
  CNumber &operator-=(const CNumber &n);
  CNumber &operator*=(const CNumber &n);
  CNumber &operator/=(const CNumber &n);
  CNumber &operator%=(const CNumber &n);
  
  // 操作符重载
  bool operator>(const CNumber &n);
  bool operator>=(const CNumber &n);
  bool operator<(const CNumber &n);
  bool operator<=(const CNumber &n);
  bool operator==(const CNumber &n);
  bool operator!=(const CNumber &n);

  // 加减乘除取模
  static void add(const CNumber &n1, const CNumber &n2, CNumber &Res);
  static void multiply(const CNumber &n1, const CNumber &n2, CNumber &Res);
  static void subtract(const CNumber &n1, const CNumber &n2, CNumber &Res);
  static void divide(const CNumber &n1, const CNumber &n2, CNumber &Res);
  static void monad(const CNumber &n1, const CNumber &n2, CNumber &Res);

  // 算术函数指针类型
  typedef void (*CALMATH)(const CNumber &, const CNumber &, CNumber &);

  // 根据算术函数返回结果
  static char *Result(const char *val1, const char *val2, CALMATH pfnCalc);
};
```

### ArrayList类的创建

在实现project的过程中，我们如果想要直接计算出某个式子的结果，这显然通过某个方法即可解决，然而我们想要通过输入赋值变量例如 “x = 3” 求解“x + 1”的问题，则会遇到困难。

该类主要有以下作用：

1.在程序运行时，创建出一个储存变量名的valueNameList，一个储存变量名所对应值的valueList，两者均为类变量，且通过下标联系变量名与对应的值

2.如在输入比如“x = 3”的赋值语句时，两个list分别添加“x”和“3”

3.每次程序开始时，创建下标为0和1分别对应的“PI = 3.141592653589”和“e = 2.718281828459”并设置为常量，当用户想要输入PI和e的赋值时将会被禁止，但是已有的变量可以进行二次赋值

相关实现代码如下：

```c++
template <typename string> class ArrayList {
private:
  string value[255];
  int size;

public:
  ArrayList() { size = 0; }

  void add(string t) {
    value[size] = t;
    size++;
  }
  
  void clear() {
    delete[] value;
    string value[255];
    size = 0;
  }

  string get(int index) { return value[index]; }

  void set(int index, string t) {
    // 将下标0和1设置为常量 无法设置也无法删除
    if (index == 0 || index == 1) {
      cout << "Sorry, const value is not permitted to change." << endl;
      return;
    }
    value[index] = t;
  }

  int getSize() { return size; }

  bool contains(string t) { return indexOf(t) != -1; }

  bool isEmpty() { return size == 0; }

  int indexOf(string t) {
    for (int i = 0; i < size; ++i) {
      if (value[i] == t) {
        return i;
      }
    }
    return -1;
  }
};
```

### 输入数据的预处理

#### emptySpace()

在用户输入的时候，很有可能输入空格，清空输入的某一行的多余空格有助于我们进行中缀表达式转为后缀表达式，这里我们直接新建一个string将其返回，避免开辟字符串删除过程中开辟过多空间

相关代码如下：

```c++
string emptySpace(const string &expression) {
  string newString;
  for (char i : expression) {
    if (i != ' ') {
      newString += i;
    }
  }
  return newString;
}
```

#### addVariable()

当用户在命令行输入“X = 3”的时候，先在varNameList中进行变量名的寻找，如果找到之后这个值并非常量PI或e，我们就对这个值的下标所对应的value进行修改；如果没有找到这个变量，我们就利用add方法进行添加

相关代码如下：

```c++
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
```

#### replaceVar()

在运算过程中，无论我们运算方式如何，变量是无法直接参与运算的，所以我们会提前将string表达式中的变量，如x，替换为x的真实数值，以便于后序的计算

相关代码如下：

```c++
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
```

#### containsCertainMathFunctions()

此方法主要用于判断输入的表达式是否包含Math函数，如果确实包含，返回首字母下标，反之则返回-1

```c++
bool containsCertainMathFunctions(const string &expression, string funName) {
  return expression.find(funName) != -1;
}
```

#### replaceAllMathFunctions()

封装了所有的Math方法

1.首先清除所有空格

2.设置while循环，使用containsCertainMathFunctions()方法判断是否存在数学函数，如果有则进行计算（Math方法的实现将于后序部分说明）

```c++
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
```

### 中缀表达式转为后缀表达式

#### priority()

此方法主要为下一个方法服务，具体实现为将加减优先级设置为1，将乘除设置为2，其余遇到的如左右括号优先级设置为0

```c++
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
```

#### middleToBack()

将中缀表达式转为后缀表达式，便于后序直接从栈中pop出数据计算，思路如下：

1.初始化一个个栈：运算符栈S1

2.从左往右开始扫描中缀表达式

I.遇到数字，直接输出

II.遇到运算符：

- 若为 '('  直接入栈
- 若为 ')'  将符号栈中的元素依次出栈并输出,直到 '(', '(' 只出栈，不输出
- 若为其他符号，将符号栈中的元素依次出栈并将其输出，直到遇到比当前符号优先级更低的符号或者 '('。将当前符号入栈。
- 扫描完后，将栈中剩余的符号依次输出。

```c++
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
    } else // 不是数字的情况分类讨论进行判断
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
  // 最后，如果不为空，就把所有的元素全部弹出
  while (!s.empty()) {
    string temp;
    temp += s.top();
    vs.push_back(temp);
    s.pop();
    temp = "";
  }
  return vs;
}
```

### 数据的计算与结果的得出

#### Math方法

基于此思路，我们对于该问题的求解思路为：

1. 寻找定位数学函数的位置，并将原表达式一分为三 
2. 调用 c++中内置cmath 头文件中的相应方法计算相应的数值
3. 再将前面+计算结果+后面拼接到一起（即相当于对方法和参数直接进行结果的值替换，本质上还是为了把表达式变为只有数的形式）

Math方法如下：

sqrt(), pow(), max(), min(), abs(), exp(), sin(), cos(), tan(), ceil(), log(), floor(), random()

由于实现思路一致，这里只放出sqrt()的代码：

```c++
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
```

#### result()

先前我们已经提到，我们通过CNumber对符号进行了重构，而且所有的变量和数学函数已经被我们替换完毕，所以我们只需要进行计算即可，下面要完成的是逆波兰表达式的计算，计算十分简单，我们构建一个栈储存最后的结果，同时对早已完成的后序表达式逐个输出，如果遇到数字就存进栈中，遇到符号便拿出两个数字进行计算，同时计算完毕后再次输入栈中，最后存在栈中的首个元素就是结果

```c++
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
      st.push((cn1 - cn2).toString()); 
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
        // 取模数为0返回INFINITE
        return "INFINITE";
      }
    }else if (bh[i] == "%") {
      CNumber cn2(st.top().c_str());
      st.pop();
      CNumber cn1(st.top().c_str());
      st.pop();
      st.push((cn1 % cn2).toString());
      if (cn2 == Zero) {
        // 除数为0返回INFINITE
        return "INFINITE";
      }
    }
  }
  return st.top();
}
```

#### answer()

封装部分预处理方法以及计算部分的方法

```c++
string answer(string expression) {
  if (expression == "nan") {
    return expression;
  }
  expression = replaceVar(expression);
  vector<string> bh = middleToBack(expression);
  string k = result(bh);
  return k;
}
```

### 程序的鲁棒性

#### isBracketMatching()

检查括号是否匹配，检查思路很简单，检查左右括号的数量是否匹配即可  ，主要是为了防止用户输入的过程中输入括号过多而出现漏输入某一层括号而不匹配的情况

```c++
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
```

#### checkUnknownVar()

对于所有替换过变量以及Math函数的表达式，理论上expression中只会存在数字和运算符，如果用户有不合法的输入，或者变量名并未被赋值，就会导致在运算前的表达式中出现没有被替换的变量或者元素，如果我们检查不合格，则不会步入计算，另用户重新输入以满足鲁棒性

```c++
bool checkUnknownVar(const string &expression) {
  for (char i : expression) {
    if (!(isdigit(i) || i == '.' || i == '+' || i == '-' || i == '*' ||
          i == '/' ||i == '%' || i == '(' || i == ')')) {
      return false;
    }
  }
  return true;
}
```

#### isAssigning()

由于我们在进行含变量计算时，形如“X = 2"的式子，我们需要检查式子中的变量到底是否符合规范：

1.对于等号做标记 进行前半部分和后半部分的截取

2.检查变量名开头是否符合规范，我们设定只能以字母开头

3.检查开头之外的部分，可以是下划线 字母 数字

4.如果变量检查出错 抛出异常

```c++
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
      cout << "Variable assign error." << endl
      throw exception();
    }
    return varNameValid;
  }
  return varNameValid;
}
```

##  Part2 - other Improvements

#### 多次输入的实现

在第一次project中，利用goto loop循环，成功完成了每次结果输出后都再次循环的操作， 然而后来经过思考发现其实有些冗余

在本次project中，利用while循环，设置两个变量，一个firstLine变量，一个wantToLeave变量，当读取的firstLine为表达式，赋值式，或者help，Welcome，C的时候，正常运行即可，当输入quit时，将wantToLeave设置为true，循环条件遇到时向用户询问是否需要退出，当firstLine为Y时离开

同时，main函数包含了刚刚提及的满足鲁棒性的方法，在出错时可以重新循环，这样代码更加清晰简洁

下面为main函数：

```c++
int main() {

  string firstLine;
  bool wantToLeave = false;

  varNameList.add("PI");
  valueList.add("3.14159 26535 89793 23846 26433 83279 50288 41971 69399 37");
  varNameList.add("e");
  valueList.add("2.71828 18284 59045 23536 02874 71352 66249 77572 47093 69");

  Welcome();

  while (getline(cin, firstLine)) {
    if (firstLine == "help") {
      Help();
      continue;
    }
    if (firstLine == "C") {
      C(varNameList);
      C(valueList);
      varNameList.add("PI");
      valueList.add("3.14159 26535 89793 23846 26433 83279 50288 41971 69399 37");
      varNameList.add("e");
      valueList.add("2.71828 18284 59045 23536 02874 71352 66249 77572 47093 69");
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
```

#### Welcome()

对于使用本计算器的用户，程序一旦运行，我们即使用内嵌函数Welcome()向用户说明基本情况

```c++
void Welcome() {
  cout << "Welcome to the simple calculator_V2" << endl;
  cout << "If you have any problem about how to use, please enter 'help' to remind the using way"<< endl;
  cout << "Please enter 'quit' to quit this calculator." << endl;
  cout << "You can enter the variable assgin or what you want to calculate now: ";
}
```

#### C()

类似于科学计算器，我们在运行程序的过程中希望不关闭程序（因为正常情况下计算器肯定只开一次），但是对已有的所有变量重新赋值计算，如果用户向命令行输入了‘C'，ArrayLits变量中除了PI和E的值都会被清空

```c++
static void C(ArrayList<string> list){
    list.clear();
}
```

#### Help()

同时，如果用户向命令行输入了‘help’，就向用户简单说明函数的基本功能与要求

```c++
void Help() {
  cout << "--------------------------------------------------------------------"<< endl;
  cout << "This is simple calculator_V2 simple help." << endl;
  cout << "Supported functions:" << endl;
  cout << "1. Basic operator: + - * / %" << endl;
  cout << "2. calculation with brackets: ( )" << endl;
  cout << "3. Definition variable by digit, alpha and '_' tips: [the variable must begin with alpha]."
       << endl;
  cout << "4. Math functions: sqrt, pow, max, min, abs, exp, sin, cos, tan, ceil, log, floor, random."
       << endl;
  cout << "5. Arbitrary Precision is permitted." << endl;
  cout << "6. const variable set: PI,e." << endl;
  cout << "7. clear input: input 'C' to make the variable assigned clear.";
  cout << "--------------------------------------------------------------------"<< endl;
}
```

## Part3 - Result and Verfication

用户名：nian@LN

#### 1.案例输入

![image-20221016004429763](C:\Users\nian5\AppData\Roaming\Typora\typora-user-images\image-20221016004429763.png)

#### 2.高精度加减乘除

![image-20221016004810548](C:\Users\nian5\AppData\Roaming\Typora\typora-user-images\image-20221016004810548.png)

#### 3.数学函数

**![image-20221016005721385](C:\Users\nian5\AppData\Roaming\Typora\typora-user-images\image-20221016005721385.png)**

#### 4.变量计算

![image-20221016005948700](C:\Users\nian5\AppData\Roaming\Typora\typora-user-images\image-20221016005948700.png)

![image-20221016010820079](C:\Users\nian5\AppData\Roaming\Typora\typora-user-images\image-20221016010820079.png)

#### 5.鲁棒性测试

![image-20221016010942406](C:\Users\nian5\AppData\Roaming\Typora\typora-user-images\image-20221016010942406.png)

![image-20221016011037860](C:\Users\nian5\AppData\Roaming\Typora\typora-user-images\image-20221016011037860.png)

#### 6.其他方法

![image-20221016232522528](C:\Users\nian5\AppData\Roaming\Typora\typora-user-images\image-20221016232522528.png)

## Part4 - Code

### 代码总览

本次代码使用**CMakeLists**进行管理，头文件和源文件目如下，一共六个文件（加上CMakelist.txt文件）。

![image-20221015194515552](C:\Users\nian5\AppData\Roaming\Typora\typora-user-images\image-20221015194515552.png)

### 通过GitHub获取所有代码

您可以点击这里快速获取上述提及的六个文件。

## Part5 - Summary

本次实现运用到了以下知识点：

1.class的使用，包括构造器、析构函数、运算符重载等写法

2.栈计算表达式的实现，优先级的使用、逆波兰表达式的运用

3.调用math函数实现string类型的math实现

4.string类型的使用，包括封装后的string类以及char数组

其他方面的收获：

1.在个人能力有限时，善于借鉴于前辈们的实现思路和经验

2.进一步提高了debug能力和运用git管理project

3.使用内联函数等特殊性质加快运算

4.养成每次计算完成后释放空间的意识

5.进一步提高程序鲁棒性，包括预处理和错误输入

由于时间有限，未能完成使用正则表达式再进一步提高程序的鲁棒性，并且程序本身鲁棒性，以及类如math函数的实现比较简单和暴力，未来学习更多知识后，将会进一步对程序进行改善。

本次project带给我的收获良多，远超第一次project，深刻感受到了自己知识点掌握太少，工具运用不熟练，希望能在下一次project中再接再厉，投入更多时间打磨。

感谢您的阅读！

#### 

