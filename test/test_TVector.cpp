#include "../blaslib/TVector.h"

#include <gtest.h>
#include <cmath>

TEST(TVectorTest, DefaultConstructor)
{
  TVector<int> v;
  EXPECT_EQ(v.GetLen(), 0);
}


TEST(TVectorTest, ParameterizedConstructor) 
{
  TVector<int> v(5);
  EXPECT_EQ(v.GetLen(), 5);
  for (int i = 0; i < 5; i++) 
  {
    EXPECT_EQ(v[i], 0);  
  }
}

TEST(TVectorTest, CopyConstructor) 
{
  TVector<int> v1(3);
  v1[0] = 1; v1[1] = 2; v1[2] = 3;

  TVector<int> v2(v1);  
  EXPECT_EQ(v2.GetLen(), 3);
  EXPECT_EQ(v2[0], 1);
  EXPECT_EQ(v2[1], 2);
  EXPECT_EQ(v2[2], 3);
}

TEST(TVectorTest, MoveConstructor) 
{
  TVector<int> v1(3);
  v1[0] = 10; v1[1] = 20; v1[2] = 30;

  TVector<int> v2(std::move(v1));  
  EXPECT_EQ(v2.GetLen(), 3);
  EXPECT_EQ(v2[0], 10);
  EXPECT_EQ(v2[1], 20);
  EXPECT_EQ(v2[2], 30);
  EXPECT_EQ(v1.GetLen(), 0);  
}

// Тесты для операторов
TEST(TVectorTest, OperatorPlus) 
{
  TVector<int> v1(3), v2(3);
  v1[0] = 1; v1[1] = 2; v1[2] = 3;
  v2[0] = 4; v2[1] = 5; v2[2] = 6;

  TVector<int> v3 = v1 + v2;
  EXPECT_EQ(v3[0], 5);
  EXPECT_EQ(v3[1], 7);
  EXPECT_EQ(v3[2], 9);
}

TEST(TVectorTest, OperatorMinus)
{
  TVector<int> v1(3), v2(3);
  v1[0] = 5; v1[1] = 5; v1[2] = 5;
  v2[0] = 1; v2[1] = 2; v2[2] = 3;

  TVector<int> v3 = v1 - v2;
  EXPECT_EQ(v3[0], 4);
  EXPECT_EQ(v3[1], 3);
  EXPECT_EQ(v3[2], 2);
}

TEST(TVectorTest, OperatorMultiplyVectors) 
{
  TVector<int> v1(3), v2(3);
  v1[0] = 1; v1[1] = 2; v1[2] = 3;
  v2[0] = 4; v2[1] = 5; v2[2] = 6;

  int res = v1 * v2;
  EXPECT_EQ(res, 32);  // 1*4 + 2*5 + 3*6 = 4 + 10 + 18 = 32
}

TEST(TVectorTest, OperatorMultiplyByScalar)
{
  TVector<int> v1(3);
  v1[0] = 1; v1[1] = 2; v1[2] = 3;

  TVector<int> v2 = v1 * 2;
  EXPECT_EQ(v2[0], 2);
  EXPECT_EQ(v2[1], 4);
  EXPECT_EQ(v2[2], 6);
}

TEST(TVectorTest, OperatorDivideByScalar) 
{
  TVector<double> v1(3);
  v1[0] = 4.0; v1[1] = 6.0; v1[2] = 8.0;

  TVector<double> v2 = v1 / 2.0;
  EXPECT_DOUBLE_EQ(v2[0], 2.0);
  EXPECT_DOUBLE_EQ(v2[1], 3.0);
  EXPECT_DOUBLE_EQ(v2[2], 4.0);
}

TEST(TVectorTest, OperatorAssignment) 
{
  TVector<int> v1(3);
  v1[0] = 10; v1[1] = 20; v1[2] = 30;

  TVector<int> v2;
  v2 = v1;
  EXPECT_EQ(v2[0], 10);
  EXPECT_EQ(v2[1], 20);
  EXPECT_EQ(v2[2], 30);
}

TEST(TVectorTest, OperatorMoveAssignment)
{
  TVector<int> v1(3);
  v1[0] = 100; v1[1] = 200; v1[2] = 300;

  TVector<int> v2;
  v2 = std::move(v1);
  EXPECT_EQ(v2[0], 100);
  EXPECT_EQ(v2[1], 200);
  EXPECT_EQ(v2[2], 300);
  EXPECT_EQ(v1.GetLen(), 0);  // Проверяем, что v1 "опустошён"
}

TEST(TVectorTest, OperatorEquality)
{
  TVector<int> v1(3), v2(3), v3(3);
  v1[0] = 1; v1[1] = 2; v1[2] = 3;
  v2[0] = 1; v2[1] = 2; v2[2] = 3;
  v3[0] = 4; v3[1] = 5; v3[2] = 6;

  EXPECT_TRUE(v1 == v2);
  EXPECT_FALSE(v1 == v3);
}

// Тесты для норм вектора
TEST(TVectorTest, FirstNorm) 
{
  TVector<double> v(3);
  v[0] = 1.0; v[1] = -2.0; v[2] = 3.0;

  double norm = v.FirstNorm(0.0);
  EXPECT_DOUBLE_EQ(norm, 6.0);  // |1| + |-2| + |3| = 6
}

TEST(TVectorTest, SecondNorm) 
{
  TVector<double> v(2);
  v[0] = 3.0; v[1] = 4.0;

  double norm = v.SecondNorm(0.0);
  EXPECT_DOUBLE_EQ(norm, 5.0);  // sqrt(3^2 + 4^2) = 5
}

TEST(TVectorTest, InfinityNorm)
{
  TVector<double> v(4);
  v[0] = 1.0; v[1] = -5.0; v[2] = 3.0; v[3] = 2.0;

  double norm = v.InfinityNorm(0.0);
  EXPECT_DOUBLE_EQ(norm, 5.0);  // max(|1|, |-5|, |3|, |2|) = 5
}

TEST(TVectorTest, HelderNorm) 
{
  TVector<double> v(3);
  v[0] = 1.0; v[1] = 2.0; v[2] = 3.0;

  double norm = v.HelderNorm(2.0);  // p=2 (евклидова норма)
  EXPECT_DOUBLE_EQ(norm, std::sqrt(14.0));  // sqrt(1^2 + 2^2 + 3^2) = sqrt(14)
}

// Тесты для push_back
TEST(TVectorTest, PushBack_ToEmptyVector) 
{
  TVector<int> vec;
  vec.push_back(42);

  EXPECT_EQ(vec.GetLen(), 1);
  EXPECT_EQ(vec[0], 42);
}

TEST(TVectorTest, PushBack_MultipleElements) 
{
  TVector<int> vec;

  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);

  EXPECT_EQ(vec.GetLen(), 3);
  EXPECT_EQ(vec[0], 1);
  EXPECT_EQ(vec[1], 2);
  EXPECT_EQ(vec[2], 3);
}

TEST(TVectorTest, PushBack_ToExistingVector) 
{
  TVector<int> vec(2);
  vec[0] = 10;
  vec[1] = 20;

  vec.push_back(30);

  EXPECT_EQ(vec.GetLen(), 3);
  EXPECT_EQ(vec[0], 10);
  EXPECT_EQ(vec[1], 20);
  EXPECT_EQ(vec[2], 30);
}

TEST(TVectorTest, PushBack_WithStrings) 
{
  TVector<std::string> vec;

  vec.push_back("hello");
  vec.push_back("world");

  EXPECT_EQ(vec.GetLen(), 2);
  EXPECT_EQ(vec[0], "hello");
  EXPECT_EQ(vec[1], "world");
}

TEST(TVectorTest, PushBack_WithDoubles) 
{
  TVector<double> vec;

  vec.push_back(1.1);
  vec.push_back(2.2);
  vec.push_back(3.3);

  EXPECT_EQ(vec.GetLen(), 3);
  EXPECT_DOUBLE_EQ(vec[0], 1.1);
  EXPECT_DOUBLE_EQ(vec[1], 2.2);
  EXPECT_DOUBLE_EQ(vec[2], 3.3);
}

// Тесты для push_front
TEST(TVectorTest, PushFront_ToEmptyVector) 
{
  TVector<int> vec;
  vec.push_front(42);

  EXPECT_EQ(vec.GetLen(), 1);
  EXPECT_EQ(vec[0], 42);
}

TEST(TVectorTest, PushFront_MultipleElements) 
{
  TVector<int> vec;

  vec.push_front(1);
  vec.push_front(2);
  vec.push_front(3);

  EXPECT_EQ(vec.GetLen(), 3);
  EXPECT_EQ(vec[0], 3);  // Последний добавленный элемент должен быть первым
  EXPECT_EQ(vec[1], 2);
  EXPECT_EQ(vec[2], 1);
}

TEST(TVectorTest, PushFront_ToExistingVector) 
{
  TVector<int> vec(2);
  vec[0] = 10;
  vec[1] = 20;

  vec.push_front(5);

  EXPECT_EQ(vec.GetLen(), 3);
  EXPECT_EQ(vec[0], 5);
  EXPECT_EQ(vec[1], 10);
  EXPECT_EQ(vec[2], 20);
}

TEST(TVectorTest, PushFront_WithStrings) 
{
  TVector<std::string> vec;

  vec.push_front("world");
  vec.push_front("hello");

  EXPECT_EQ(vec.GetLen(), 2);
  EXPECT_EQ(vec[0], "hello");
  EXPECT_EQ(vec[1], "world");
}

// Комбинированные тесты
TEST(TVectorTest, PushBackAndPushFront_Combined) 
{
  TVector<int> vec;

  vec.push_back(2);
  vec.push_front(1);
  vec.push_back(3);
  vec.push_front(0);

  EXPECT_EQ(vec.GetLen(), 4);
  EXPECT_EQ(vec[0], 0);
  EXPECT_EQ(vec[1], 1);
  EXPECT_EQ(vec[2], 2);
  EXPECT_EQ(vec[3], 3);
}

TEST(TVectorTest, PushMethods_AfterAssignment)
{
  TVector<int> vec1;
  vec1.push_back(1);
  vec1.push_back(2);

  TVector<int> vec2 = vec1;  // Используем конструктор копирования

  vec2.push_back(3);
  vec2.push_front(0);

  EXPECT_EQ(vec1.GetLen(), 2);  // Исходный вектор не должен измениться
  EXPECT_EQ(vec2.GetLen(), 4);  // Новый вектор должен иметь новые элементы

  EXPECT_EQ(vec2[0], 0);
  EXPECT_EQ(vec2[1], 1);
  EXPECT_EQ(vec2[2], 2);
  EXPECT_EQ(vec2[3], 3);
}

TEST(TVectorTest, PushMethods_WithMoveSemantics) 
{
  TVector<int> vec1;
  vec1.push_back(1);
  vec1.push_back(2);

  TVector<int> vec2 = std::move(vec1);  // Используем move конструктор

  vec2.push_back(3);
  vec2.push_front(0);

  EXPECT_EQ(vec1.GetLen(), 0);  // Исходный вектор должен быть пустым
  EXPECT_EQ(vec2.GetLen(), 4);  // Новый вектор должен иметь все элементы

  EXPECT_EQ(vec2[0], 0);
  EXPECT_EQ(vec2[1], 1);
  EXPECT_EQ(vec2[2], 2);
  EXPECT_EQ(vec2[3], 3);
}

TEST(TVectorTest, IsEmpty_InitialState) 
{
  TVector<int> vec;
  EXPECT_TRUE(vec.IsEmpty());
}

TEST(TVectorTest, IsEmpty_AfterAddingElements) 
{
  TVector<int> vec;
  vec.push_back(1);
  EXPECT_FALSE(vec.IsEmpty());
}

TEST(TVectorTest, IsEmpty_AfterClear) 
{
  TVector<int> vec;
  vec.push_back(1);
  vec.push_back(2);
  vec.SetLen(0); // Очищаем вектор
  EXPECT_TRUE(vec.IsEmpty());
}

TEST(TVectorTest, IsFull_AlwaysFalseForDynamicVector) 
{
  TVector<int> vec;
  EXPECT_FALSE(vec.IsFull());

  vec.push_back(1);
  vec.push_back(2);
  EXPECT_FALSE(vec.IsFull());
}

TEST(TVectorTest, ApplyToAll_ModifyElements) 
{
  TVector<int> vec;
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);

  vec.ApplyToAll([](int& x) { x += 10; });

  EXPECT_EQ(vec[0], 11);
  EXPECT_EQ(vec[1], 12);
  EXPECT_EQ(vec[2], 13);
}

TEST(TVectorTest, ApplyToAll_ConstVersion) 
{
  TVector<int> vec;
  vec.push_back(1);
  vec.push_back(2);

  int sum = 0;
  const TVector<int>& constVec = vec;
  constVec.ApplyToAll([&sum](const int& x) {sum += x; });

  EXPECT_EQ(sum, 3);
}

TEST(TVectorTest, ApplyToAll_WithDifferentTypes) {
  TVector<double> vec;
  vec.push_back(1.1);
  vec.push_back(2.2);

  vec.ApplyToAll([](double& x) { x = std::sqrt(x);});

  EXPECT_DOUBLE_EQ(vec[0], std::sqrt(1.1));
  EXPECT_DOUBLE_EQ(vec[1], std::sqrt(2.2));
}

TEST(TVectorTest, ApplyToAll_EmptyVector) {
  TVector<int> vec;

  EXPECT_NO_THROW({
      vec.ApplyToAll([](int& x) { x = 0;});
    });
}

TEST(TVectorTest, ApplyToAll_ComplexOperation) {
  TVector<std::string> vec;
  vec.push_back("lol");
  vec.push_back("kek");

  vec.ApplyToAll([](std::string& str) {
    str = "prefix_" + str + "_suffix";
    });

  EXPECT_EQ(vec[0], "prefix_lol_suffix");
  EXPECT_EQ(vec[1], "prefix_kek_suffix");
}

// Тесты конструкторов
//TEST(TMatrixTest, DefaultConstructor)
//{
//  TMatrix<int> m;
//  EXPECT_EQ(m.GetRows(), 0);
//  EXPECT_EQ(m.GetCols(), 0);
//}
//
//TEST(TMatrixTest, ParameterizedConstructor)
//{
//  TMatrix<double> m(3, 4);
//  EXPECT_EQ(m.GetRows(), 3);
//  EXPECT_EQ(m.GetCols(), 4);
//}
//
//TEST(TMatrixTest, CopyConstructor)
//{
//  TMatrix<int> m1(2, 2);
//  m1.RandomFillSimple(1, 10);
//  TMatrix<int> m2(m1);
//  EXPECT_TRUE(m1 == m2);
//}
//
// //Тесты операций
//TEST(TMatrixTest, MatrixAddition) 
//{
//  TMatrix<int> m1(2, 2);
//  TMatrix<int> m2(2, 2);
//  m1(0, 0) = 1; m1(0, 1) = 2; m1(1, 0) = 3; m1(1, 1) = 4;
//  m2(0, 0) = 5; m2(0, 1) = 6; m2(1, 0) = 7; m2(1, 1) = 8;
//
//  auto res = m1 + m2;
//  EXPECT_EQ(res(0, 0), 6);
//  EXPECT_EQ(res(0, 1), 8);
//  EXPECT_EQ(res(1, 0), 10);
//  EXPECT_EQ(res(1, 1), 12);
//}
//
//TEST(TMatrixTest, MatrixMultiplication)
//{
//  TMatrix<int> m1(2, 3);
//  TMatrix<int> m2(3, 2);
//  m1(0, 0) = 1; m1(0, 1) = 2; m1(0, 2) = 3;
//  m1(1, 0) = 4; m1(1, 1) = 5; m1(1, 2) = 6;
//
//  m2(0, 0) = 7; m2(0, 1) = 8;
//  m2(1, 0) = 9; m2(1, 1) = 10;
//  m2(2, 0) = 11; m2(2, 1) = 12;
//
//  auto res = m1 * m2;
//  EXPECT_EQ(res(0, 0), 58);
//  EXPECT_EQ(res(0, 1), 64);
//  EXPECT_EQ(res(1, 0), 139);
//  EXPECT_EQ(res(1, 1), 154);
//}
//
//TEST(TMatrixTest, ScalarMultiplication)
//{
//  TMatrix<double> m(2, 2);
//  m(0, 0) = 1.5; m(0, 1) = 2.5;
//  m(1, 0) = 3.5; m(1, 1) = 4.5;
//
//  auto res = m * 2.0;
//  EXPECT_DOUBLE_EQ(res(0, 0), 3.0);
//  EXPECT_DOUBLE_EQ(res(0, 1), 5.0);
//  EXPECT_DOUBLE_EQ(res(1, 0), 7.0);
//  EXPECT_DOUBLE_EQ(res(1, 1), 9.0);
//}
//
//// Тесты норм
//TEST(TMatrixTest, FirstNorm) 
//{
//  TMatrix<double> m(2, 2);
//  m(0, 0) = 1.0; m(0, 1) = -2.0;
//  m(1, 0) = -3.0; m(1, 1) = 4.0;
//
//  EXPECT_DOUBLE_EQ(m.FirstNorm(), 10.0); // Убрали аргумент 0.0
//}
//
//TEST(TMatrixTest, InfinityNorm)
//{
//  TMatrix<double> m(2, 3);
//  m(0, 0) = 1.0; m(0, 1) = -5.0; m(0, 2) = 3.0;
//  m(1, 0) = -2.0; m(1, 1) = 4.0; m(1, 2) = -6.0;
//
//  EXPECT_DOUBLE_EQ(m.InfinityNorm(), 12.0);
//}
//
//// Тест заполнения случайными числами
//TEST(TMatrixTest, RandomFill) 
//{
//  TMatrix<int> m(10, 10);
//  m.RandomFillSimple(0, 100);
//
//  bool all_in_range = true;
//  for (size_t i = 0; i < m.GetRows(); ++i) 
//  {
//    for (size_t j = 0; j < m.GetCols(); ++j) 
//    {
//      if (m(i, j) < 0 || m(i, j) > 100) 
//      {
//        all_in_range = false;
//        break;
//      }
//    }
//  }
//  EXPECT_TRUE(all_in_range);
//}
//
//TEST(TMatrixTest, GaussSolve3x3System) 
//{
//  TMatrix<double> A(3, 3);
//  A(0, 0) = 2.0; A(0, 1) = 1.0; A(0, 2) = -1.0;
//  A(1, 0) = -3.0; A(1, 1) = -1.0; A(1, 2) = 2.0;
//  A(2, 0) = -2.0; A(2, 1) = 1.0; A(2, 2) = 2.0;
//
//  TVector<double> b(3);
//  b[0] = 8.0; b[1] = -11.0; b[2] = -3.0;
//
//  TVector<double> x = A.SolveGauss(b);
//
//  // Проверка решения
//  EXPECT_NEAR(x[0], 2.0, 1e-10);
//  EXPECT_NEAR(x[1], 3.0, 1e-10);
//  EXPECT_NEAR(x[2], -1.0, 1e-10);
//}
