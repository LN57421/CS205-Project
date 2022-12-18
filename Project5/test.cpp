 #include <iostream>

 #include "Matrix.h"

 using namespace std;

 int main() {
   Matrix<int> matrix1;
   int nums[20] = {1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
                   11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
   Matrix<int> matrix2 = Matrix<int>(5, 2, 2, nums);
   //
   cout << matrix2;
   matrix1 = matrix2;
   cout << matrix1;

   int *nums2 = new int[30];
   for (int i = 0; i < 30; i++) {
     nums2[i] = i + 1;
   }

   Matrix<int> mat1(5, 6, 1, nums2);
   cout << "mat1:\n" << endl;
   cout << mat1 << endl;
   // 友元函数
   Matrix<int> mat2 = 2 + mat1;
   cout << "mat2:\n " << endl;
   cout << mat2 << endl;
   Matrix<int> mat3 = mat1 + mat2;
   cout << "mat3:\n " << endl;
   cout << mat3 << endl;
   Matrix<int> mat4 = mat3 - mat2;
   cout << "mat4:\n" << endl;
   cout << mat4 << endl;
   if (mat1 == mat4) {
     cout << "mat1 is equals to mat4";
   }
   if (mat2 != mat3) {
     cout << "mat2 is not equals to mat3";
   }

   //
   Matrix<int> mat1(1, 3, 1, nums3);
   cout << "mat1: \n" << mat1 << endl;
   Matrix<int> mat2(3, 1, 1, nums3);
   cout << "mat2: \n" << mat2 << endl;
   Matrix<int> mat3 = mat2 * mat1;
   cout << "mat3:\n" << mat3 << endl;
   mat3 = mat1 * mat2;
   cout << "mat3:\n" << mat3 << endl;

   

   return 0;
 }
