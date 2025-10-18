#include <gtest.h>
#include <cmath>
#include <sstream>
#include "..//matrixlib/TSqTriangMatrix.h"


TEST(TSqTriangMatrixTest, DefaultConstructor)
{
  TSqTriangMatrix<int> mat;
  EXPECT_TRUE(mat.IsEmpty());
  EXPECT_EQ(mat.GetSize(), 0);
  EXPECT_TRUE(mat.IsUpper());
}

TEST(TSqTriangMatrixTest, ParameterizedConstructor)
{
  TSqTriangMatrix<double> mat(5, true);
  EXPECT_FALSE(mat.IsEmpty());
  EXPECT_EQ(mat.GetSize(), 5);
  EXPECT_TRUE(mat.IsUpper());
  EXPECT_TRUE(mat.IsFull());

  TSqTriangMatrix<double> mat2(4, false);
  EXPECT_EQ(mat2.GetSize(), 4);
  EXPECT_FALSE(mat2.IsUpper());
}


TEST(TSqTriangMatrixTest, CopyConstructor)
{
  TSqTriangMatrix<int> original(3, true);
  original(0, 0) = 1; original(0, 1) = 2; original(0, 2) = 3;
  original(1, 1) = 4; original(1, 2) = 5;
  original(2, 2) = 6;

  TSqTriangMatrix<int> copy(original);

  EXPECT_EQ(copy.GetSize(), 3);
  EXPECT_TRUE(copy.IsUpper());
  EXPECT_EQ(copy(0, 0), 1);
  EXPECT_EQ(copy(0, 2), 3);
  EXPECT_EQ(copy(1, 2), 5);
  EXPECT_EQ(copy(2, 2), 6);
}

TEST(TSqTriangMatrixTest, MoveConstructor)
{
  TSqTriangMatrix<int> original(3, false);
  original(0, 0) = 1;
  original(1, 0) = 2; original(1, 1) = 3;
  original(2, 0) = 4; original(2, 1) = 5; original(2, 2) = 6;

  TSqTriangMatrix<int> moved(std::move(original));

  EXPECT_EQ(moved.GetSize(), 3);
  EXPECT_FALSE(moved.IsUpper());
  EXPECT_EQ(moved(0, 0), 1);
  EXPECT_EQ(moved(2, 2), 6);
  EXPECT_EQ(original.GetSize(), 0);
}

TEST(TSqTriangMatrixTest, AssignmentOperator)
{
  TSqTriangMatrix<int> mat1(3, true);
  mat1(0, 0) = 1; mat1(0, 1) = 2; mat1(0, 2) = 3;
  mat1(1, 1) = 4; mat1(1, 2) = 5;
  mat1(2, 2) = 6;

  TSqTriangMatrix<int> mat2;
  mat2 = mat1;

  EXPECT_EQ(mat2.GetSize(), 3);
  EXPECT_TRUE(mat2.IsUpper());
  EXPECT_EQ(mat2(0, 0), 1);
  EXPECT_EQ(mat2(1, 1), 4);
  EXPECT_EQ(mat2(2, 2), 6);
}

TEST(TSqTriangMatrixTest, MoveAssignmentOperator)
{
  TSqTriangMatrix<int> mat1(4, false);
  mat1(0, 0) = 1;
  mat1(1, 0) = 2; mat1(1, 1) = 3;
  mat1(2, 0) = 4; mat1(2, 1) = 5; mat1(2, 2) = 6;
  mat1(3, 0) = 7; mat1(3, 1) = 8; mat1(3, 2) = 9; mat1(3, 3) = 10;

  TSqTriangMatrix<int> mat2;
  mat2 = std::move(mat1);

  EXPECT_EQ(mat2.GetSize(), 4);
  EXPECT_FALSE(mat2.IsUpper());
  EXPECT_EQ(mat2(0, 0), 1);
  EXPECT_EQ(mat2(3, 3), 10);
  EXPECT_EQ(mat1.GetSize(), 0);
}

TEST(TSqTriangMatrixTest, EqualityOperator)
{
  TSqTriangMatrix<int> mat1(3, true);
  mat1(0, 0) = 1; mat1(0, 1) = 2; mat1(0, 2) = 3;
  mat1(1, 1) = 4; mat1(1, 2) = 5;
  mat1(2, 2) = 6;

  TSqTriangMatrix<int> mat2(3, true);
  mat2(0, 0) = 1; mat2(0, 1) = 2; mat2(0, 2) = 3;
  mat2(1, 1) = 4; mat2(1, 2) = 5;
  mat2(2, 2) = 6;

  TSqTriangMatrix<int> mat3(3, true);
  mat3(0, 0) = 7; mat3(0, 1) = 8; mat3(0, 2) = 9;
  mat3(1, 1) = 10; mat3(1, 2) = 11;
  mat3(2, 2) = 12;

  EXPECT_TRUE(mat1 == mat2);
  EXPECT_FALSE(mat1 == mat3);
}

TEST(TSqTriangMatrixTest, InequalityOperator)
{
  TSqTriangMatrix<int> mat1(2, true);
  mat1(0, 0) = 1; mat1(0, 1) = 2;
  mat1(1, 1) = 3;

  TSqTriangMatrix<int> mat2(2, true);
  mat2(0, 0) = 4; mat2(0, 1) = 5;
  mat2(1, 1) = 6;

  EXPECT_TRUE(mat1 != mat2);
  EXPECT_FALSE(mat1 != mat1);
}


TEST(TSqTriangMatrixTest, OperatorAccess)
{
  // Верхняя треугольная матрица
  TSqTriangMatrix<int> upper_mat(3, true);
  upper_mat(0, 0) = 1; upper_mat(0, 1) = 2; upper_mat(0, 2) = 3;
  upper_mat(1, 1) = 4; upper_mat(1, 2) = 5;
  upper_mat(2, 2) = 6;

  // Проверка значений
  EXPECT_EQ(upper_mat(0, 0), 1);
  EXPECT_EQ(upper_mat(0, 1), 2);
  EXPECT_EQ(upper_mat(0, 2), 3);
  EXPECT_EQ(upper_mat(1, 1), 4);
  EXPECT_EQ(upper_mat(1, 2), 5);
  EXPECT_EQ(upper_mat(2, 2), 6);

  // Нижняя треугольная матрица
  TSqTriangMatrix<int> lower_mat(3, false);
  lower_mat(0, 0) = 1;
  lower_mat(1, 0) = 2; lower_mat(1, 1) = 3;
  lower_mat(2, 0) = 4; lower_mat(2, 1) = 5; lower_mat(2, 2) = 6;

  // Проверка значений
  EXPECT_EQ(lower_mat(0, 0), 1);
  EXPECT_EQ(lower_mat(1, 0), 2);
  EXPECT_EQ(lower_mat(1, 1), 3);
  EXPECT_EQ(lower_mat(2, 0), 4);
  EXPECT_EQ(lower_mat(2, 1), 5);
  EXPECT_EQ(lower_mat(2, 2), 6);
}

TEST(TSqTriangMatrixTest, ConstOperatorAccess)
{
  TSqTriangMatrix<int> temp(3, true, 5);
  const TSqTriangMatrix<int> mat = temp;

  // Проверка доступа к элементам внутри треугольника
  EXPECT_EQ(mat(0, 0), 5);
  EXPECT_EQ(mat(0, 1), 5);
  EXPECT_EQ(mat(1, 1), 5);

  // Проверка доступа к элементам вне треугольника (должны возвращать 0)
  EXPECT_EQ(mat(1, 0), 0);
  EXPECT_EQ(mat(2, 0), 0);
}

TEST(TSqTriangMatrixTest, AdditionOperator)
{
  TSqTriangMatrix<int> mat1(3, true);
  mat1(0, 0) = 1; mat1(0, 1) = 2; mat1(0, 2) = 3;
  mat1(1, 1) = 4; mat1(1, 2) = 5;
  mat1(2, 2) = 6;

  TSqTriangMatrix<int> mat2(3, true);
  mat2(0, 0) = 6; mat2(0, 1) = 5; mat2(0, 2) = 4;
  mat2(1, 1) = 3; mat2(1, 2) = 2;
  mat2(2, 2) = 1;

  TSqTriangMatrix<int> result = mat1 + mat2;

  EXPECT_EQ(result.GetSize(), 3);
  EXPECT_TRUE(result.IsUpper());
  EXPECT_EQ(result(0, 0), 7);
  EXPECT_EQ(result(0, 1), 7);
  EXPECT_EQ(result(0, 2), 7);
  EXPECT_EQ(result(1, 1), 7);
  EXPECT_EQ(result(1, 2), 7);
  EXPECT_EQ(result(2, 2), 7);
}

TEST(TSqTriangMatrixTest, SubtractionOperator)
{
  TSqTriangMatrix<int> mat1(3, true);
  mat1(0, 0) = 5; mat1(0, 1) = 4; mat1(0, 2) = 3;
  mat1(1, 1) = 2; mat1(1, 2) = 1;
  mat1(2, 2) = 0;

  TSqTriangMatrix<int> mat2(3, true);
  mat2(0, 0) = 1; mat2(0, 1) = 1; mat2(0, 2) = 1;
  mat2(1, 1) = 1; mat2(1, 2) = 1;
  mat2(2, 2) = 1;

  TSqTriangMatrix<int> result = mat1 - mat2;

  EXPECT_EQ(result.GetSize(), 3);
  EXPECT_TRUE(result.IsUpper());
  EXPECT_EQ(result(0, 0), 4);
  EXPECT_EQ(result(0, 1), 3);
  EXPECT_EQ(result(0, 2), 2);
  EXPECT_EQ(result(1, 1), 1);
  EXPECT_EQ(result(1, 2), 0);
  EXPECT_EQ(result(2, 2), -1);
}

TEST(TSqTriangMatrixTest, ScalarMultiplication)
{
  TSqTriangMatrix<int> mat(3, true);
  mat(0, 0) = 1; mat(0, 1) = 2; mat(0, 2) = 3;
  mat(1, 1) = 4; mat(1, 2) = 5;
  mat(2, 2) = 6;

  TSqTriangMatrix<int> result = mat * 3;

  EXPECT_EQ(result.GetSize(), 3);
  EXPECT_TRUE(result.IsUpper());
  EXPECT_EQ(result(0, 0), 3);
  EXPECT_EQ(result(0, 1), 6);
  EXPECT_EQ(result(0, 2), 9);
  EXPECT_EQ(result(1, 1), 12);
  EXPECT_EQ(result(1, 2), 15);
  EXPECT_EQ(result(2, 2), 18);
}

TEST(TSqTriangMatrixTest, MatrixMultiplication)
{
  TSqTriangMatrix<int> mat1(3, true);
  mat1(0, 0) = 1; mat1(0, 1) = 2; mat1(0, 2) = 3;
  mat1(1, 1) = 4; mat1(1, 2) = 5;
  mat1(2, 2) = 6;

  TSqTriangMatrix<int> mat2(3, true);
  mat2(0, 0) = 2; mat2(0, 1) = 1; mat2(0, 2) = 0;
  mat2(1, 1) = 3; mat2(1, 2) = 2;
  mat2(2, 2) = 4;

  TSqTriangMatrix<int> result = mat1 * mat2;

  EXPECT_EQ(result.GetSize(), 3);
  EXPECT_TRUE(result.IsUpper());

  // Проверка некоторых элементов
  // (0,0): 1*2 + 2*0 + 3*0 = 2
  // (0,1): 1*1 + 2*3 + 3*0 = 1 + 6 = 7
  // (1,1): 0*0 + 4*3 + 5*0 = 12
  EXPECT_EQ(result(0, 0), 2);
  EXPECT_EQ(result(0, 1), 7);
  EXPECT_EQ(result(1, 1), 12);
}

TEST(TSqTriangMatrixTest, FirstNorm)
{
  TSqTriangMatrix<double> mat(3, true);
  mat(0, 0) = 1; mat(0, 1) = -2; mat(0, 2) = 3;
  mat(1, 1) = -4; mat(1, 2) = 5;
  mat(2, 2) = -6;

  // Суммы по столбцам: |1|=1, |-2|+|-4|=6, |3|+|5|+|-6|=14
  double result = mat.FirstNorm();
  EXPECT_NEAR(result, 14.0, 1e-10);
}

TEST(TSqTriangMatrixTest, InfinityNorm)
{
  TSqTriangMatrix<double> mat(3, true);
  mat(0, 0) = 1; mat(0, 1) = -2; mat(0, 2) = 3;
  mat(1, 1) = -4; mat(1, 2) = 5;
  mat(2, 2) = -6;

  // Суммы по строкам: |1|+|-2|+|3|=6, |-4|+|5|=9, |-6|=6
  double result = mat.InfinityNorm();
  EXPECT_NEAR(result, 9.0, 1e-10);
}

TEST(TSqTriangMatrixTest, HelderNorm)
{
  TSqTriangMatrix<double> mat(2, true);
  mat(0, 0) = 2; mat(0, 1) = 1;
  mat(1, 1) = 2;

  // Для p=2: (|2|? + |1|? + |2|?)^(1/2) = sqrt(4+1+4) = sqrt(9) = 3
  double result = mat.HelderNorm(2.0);
  EXPECT_NEAR(result, 3.0, 1e-10);
}

TEST(TSqTriangMatrixTest, ApplyToAll)
{
  TSqTriangMatrix<int> mat(3, true);
  mat(0, 0) = 1; mat(0, 1) = 2; mat(0, 2) = 3;
  mat(1, 1) = 4; mat(1, 2) = 5;
  mat(2, 2) = 6;

  mat.ApplyToAll([](int& x) { x += 10; });

  EXPECT_EQ(mat(0, 0), 11);
  EXPECT_EQ(mat(0, 1), 12);
  EXPECT_EQ(mat(0, 2), 13);
  EXPECT_EQ(mat(1, 1), 14);
  EXPECT_EQ(mat(1, 2), 15);
  EXPECT_EQ(mat(2, 2), 16);
}

TEST(TSqTriangMatrixTest, ConstApplyToAll)
{
  TSqTriangMatrix<int> mat(2, true);
  mat(0, 0) = 1; mat(0, 1) = 2;
  mat(1, 1) = 3;

  int sum = 0;
  const TSqTriangMatrix<int>& constMat = mat;
  constMat.ApplyToAll([&sum](const int& x) { sum += x; });

  EXPECT_EQ(sum, 6);  // 1+2+3
}

TEST(TSqTriangMatrixTest, InvalidArguments)
{
  EXPECT_THROW(TSqTriangMatrix<int>(-1, true), std::invalid_argument);
  EXPECT_THROW(TSqTriangMatrix<int>(-1, false), std::invalid_argument);
}

TEST(TSqTriangMatrixTest, OutOfRangeAccess)
{
  TSqTriangMatrix<int> mat(3, true);

  EXPECT_THROW(mat(-1, 0), std::out_of_range);
  EXPECT_THROW(mat(0, -1), std::out_of_range);
  EXPECT_THROW(mat(3, 0), std::out_of_range);
  EXPECT_THROW(mat(0, 3), std::out_of_range);
  EXPECT_THROW(mat(1, 0), std::out_of_range); // Вне треугольника
}

TEST(TSqTriangMatrixTest, EmptyMatrixOperations)
{
  TSqTriangMatrix<int> empty_mat;
  TSqTriangMatrix<int> mat(2, true);

  // Проверяем операции с пустой матрицей
  EXPECT_TRUE(empty_mat.IsEmpty());
  EXPECT_EQ(empty_mat.CountOccurrences(1), 0);
  EXPECT_EQ(empty_mat.FirstNorm(), 0);
  EXPECT_EQ(empty_mat.SecondNorm(), 0);
  EXPECT_EQ(empty_mat.InfinityNorm(), 0);

  // ApplyToAll на пустой матрице не должен вызывать ошибок
  EXPECT_NO_THROW({
      empty_mat.ApplyToAll([](int& x) { x = 5; });
    });
}

TEST(TSqTriangMatrixTest, DataSizeCalculation)
{
  // Проверяем правильность расчета размера данных
  TSqTriangMatrix<int> mat1(4, true);
  EXPECT_EQ(mat1.GetData().GetLen(), 10); // 4*(4+1)/2 = 10

  TSqTriangMatrix<int> mat2(5, false);
  EXPECT_EQ(mat2.GetData().GetLen(), 15); // 5*(5+1)/2 = 15

  TSqTriangMatrix<int> mat3(1, true);
  EXPECT_EQ(mat3.GetData().GetLen(), 1); // 1*(1+1)/2 = 1
}

