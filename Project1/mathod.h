#include <string>
#include <vector>
using namespace std;

// 判断
int judgeType(const string &);
bool isInt(const char *str);
bool isFloatE(const char *str);
bool isFloatPoint(const char *str);
bool isNumber(const string &);
// 乘法
string multiply1(string num1, string num2);
vector<int> mul(vector<int> A, vector<int> B);
// 转换
double stringToDouble(string s);
string doubleToString(double d);
