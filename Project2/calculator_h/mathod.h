#include <string>
#include <vector>
using namespace std;

// 提供帮助列表
static inline void Help();
// 初始语句设定
static inline void Welcome();
// 清空变量 返回初始状态
static void C(ArrayList<string> list);

// 清除空格
static string emptySpace(const string &expression);
// 检查括号是否成对
static bool isBracketMatching(const string &expression);
// 得到符号优先级
static int priority(char op);
// 中缀表达式转后缀表达式
static vector<string> middleToBack(string expression);
// 检查变量是否进行赋值
static bool checkUnknownVar(const string &expression);
// 检查表达式是否为赋值语句
static bool isAssigning(const string &expression);
// 添加变量到List当中
static void addVariable(const string &expression);
// 在计算时将变量替换为实际值
static string replaceVar(string expression);

// 检查是否包含Math函数
static bool containsCertainMathFunctions(const string &expression,
                                                string funName);
// 封装方法 将Math函数的结果替换
static string replaceAllMathFunctions(string expression);
// Math方法
string sqrt(string expression);
string pow(string expression);
string max(string expression);
string min(string expression);
string abs(string expression);
string exp(string expression);
string sin(string expression);
string cos(string expression);
string tan(string expression);
string ceil(string expression);
string log(string expression);
string floor(string expression);
string random(string expression);

// 用后缀表达式计算得出结果
static string result(vector<string> bh);
// 封装方法 实现转后缀以及用后缀表达式计算得出结果
static string answer(string expression);
