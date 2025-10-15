#include "..//blaslib/TCompleteRectangularMatrices.h"

#include <gtest.h>
#include <cmath>

// Тесты конструкторов
TEST(TCompleteRectangularMatricesTest, DefaultConstructor)
{
  TComplRectMatrix<int> mat;
  EXPECT_TRUE(mat.IsEmpty());
  EXPECT_EQ(mat.GetRows(), 0);
  EXPECT_EQ(mat.GetCols(), 0);
}

TEST(TComplRectMatrixTest, ParameterizedConstructor)
{
  TComplRectMatrix<double> mat(3, 4);
  EXPECT_FALSE(mat.IsEmpty());
  EXPECT_EQ(mat.GetRows(), 3);
  EXPECT_EQ(mat.GetCols(), 4);
  EXPECT_TRUE(mat.IsFull());
}

TEST(TComplRectMatrixTest, ParameterizedConstructorWithValue)
{
  TComplRectMatrix<int> mat(2, 3, 5);
  EXPECT_EQ(mat.GetRows(), 2);
  EXPECT_EQ(mat.GetCols(), 3);

  for (int i = 0; i < 2; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      EXPECT_EQ(mat[i][j], 5);
    }
  }
}

TEST(TComplRectMatrixTest, CopyConstructor)
{
  TComplRectMatrix<int> original(2, 3);
  original[0][0] = 1; original[0][1] = 2; original[0][2] = 3;
  original[1][0] = 4; original[1][1] = 5; original[1][2] = 6;

  TComplRectMatrix<int> copy(original);

  EXPECT_EQ(copy.GetRows(), 2);
  EXPECT_EQ(copy.GetCols(), 3);
  EXPECT_EQ(copy[0][0], 1);
  EXPECT_EQ(copy[1][2], 6);
}

TEST(TComplRectMatrixTest, MoveConstructor)
{
  TComplRectMatrix<int> original(3, 4);  // Прямоугольная матрица 3x4
  original[0][0] = 1; original[0][1] = 2; original[0][2] = 3; original[0][3] = 4;
  original[1][0] = 5; original[1][1] = 6; original[1][2] = 7; original[1][3] = 8;
  original[2][0] = 9; original[2][1] = 10; original[2][2] = 11; original[2][3] = 12;

  TComplRectMatrix<int> moved(std::move(original));

  EXPECT_EQ(moved.GetRows(), 3);
  EXPECT_EQ(moved.GetCols(), 4);
  EXPECT_EQ(moved[0][0], 1);
  EXPECT_EQ(moved[2][3], 12);  // Проверяем элемент в последней строке и столбце
  EXPECT_EQ(original.GetRows(), 0);
  EXPECT_EQ(original.GetCols(), 0);
}

TEST(TComplRectMatrixTest, AssignmentOperator)
{
  TComplRectMatrix<int> mat1(2, 5);  // Прямоугольная матрица 2x5
  mat1[0][0] = 1; mat1[0][1] = 2; mat1[0][2] = 3; mat1[0][3] = 4; mat1[0][4] = 5;
  mat1[1][0] = 6; mat1[1][1] = 7; mat1[1][2] = 8; mat1[1][3] = 9; mat1[1][4] = 10;

  TComplRectMatrix<int> mat2;
  mat2 = mat1;

  EXPECT_EQ(mat2.GetRows(), 2);
  EXPECT_EQ(mat2.GetCols(), 5);
  EXPECT_EQ(mat2[0][0], 1);
  EXPECT_EQ(mat2[1][4], 10);  // Проверяем элемент в последнем столбце
}

TEST(TComplRectMatrixTest, MoveAssignmentOperator)
{
  TComplRectMatrix<int> mat1(4, 3);  // Прямоугольная матрица 4x3
  mat1[0][0] = 1; mat1[0][1] = 2; mat1[0][2] = 3;
  mat1[1][0] = 4; mat1[1][1] = 5; mat1[1][2] = 6;
  mat1[2][0] = 7; mat1[2][1] = 8; mat1[2][2] = 9;
  mat1[3][0] = 10; mat1[3][1] = 11; mat1[3][2] = 12;

  TComplRectMatrix<int> mat2;
  mat2 = std::move(mat1);

  EXPECT_EQ(mat2.GetRows(), 4);
  EXPECT_EQ(mat2.GetCols(), 3);
  EXPECT_EQ(mat2[0][0], 1);
  EXPECT_EQ(mat2[3][2], 12);  // Проверяем элемент в последней строке и столбце
  EXPECT_EQ(mat1.GetRows(), 0);
  EXPECT_EQ(mat1.GetCols(), 0);
}

TEST(TComplRectMatrixTest, EqualityOperator)
{
  TComplRectMatrix<int> mat1(3, 2);  // Прямоугольная матрица 3x2
  mat1[0][0] = 1; mat1[0][1] = 2;
  mat1[1][0] = 3; mat1[1][1] = 4;
  mat1[2][0] = 5; mat1[2][1] = 6;

  TComplRectMatrix<int> mat2(3, 2);
  mat2[0][0] = 1; mat2[0][1] = 2;
  mat2[1][0] = 3; mat2[1][1] = 4;
  mat2[2][0] = 5; mat2[2][1] = 6;

  TComplRectMatrix<int> mat3(3, 2);
  mat3[0][0] = 7; mat3[0][1] = 8;
  mat3[1][0] = 9; mat3[1][1] = 10;
  mat3[2][0] = 11; mat3[2][1] = 12;

  EXPECT_TRUE(mat1 == mat2);
  EXPECT_FALSE(mat1 == mat3);
}

TEST(TComplRectMatrixTest, InequalityOperator)
{
  TComplRectMatrix<int> mat1(2, 4);  // Прямоугольная матрица 2x4
  mat1[0][0] = 1; mat1[0][1] = 2; mat1[0][2] = 3; mat1[0][3] = 4;
  mat1[1][0] = 5; mat1[1][1] = 6; mat1[1][2] = 7; mat1[1][3] = 8;

  TComplRectMatrix<int> mat2(2, 4);
  mat2[0][0] = 9; mat2[0][1] = 10; mat2[0][2] = 11; mat2[0][3] = 12;
  mat2[1][0] = 13; mat2[1][1] = 14; mat2[1][2] = 15; mat2[1][3] = 16;

  EXPECT_TRUE(mat1 != mat2);
  EXPECT_FALSE(mat1 != mat1);
}

TEST(TComplRectMatrixTest, IndexOperator)
{
  TComplRectMatrix<int> mat(2, 3);
  mat[0][0] = 1; mat[0][1] = 2; mat[0][2] = 3;
  mat[1][0] = 4; mat[1][1] = 5; mat[1][2] = 6;

  EXPECT_EQ(mat[0][0], 1);
  EXPECT_EQ(mat[0][2], 3);
  EXPECT_EQ(mat[1][1], 5);
}

TEST(TComplRectMatrixTest, ConstIndexOperator)
{
  const TComplRectMatrix<int> mat(2, 2, 5);

  EXPECT_EQ(mat[0][0], 5);
  EXPECT_EQ(mat[1][1], 5);
}

TEST(TComplRectMatrixTest, AdditionOperator)
{
  TComplRectMatrix<int> mat1(2, 3);
  mat1[0][0] = 1; mat1[0][1] = 2; mat1[0][2] = 3;
  mat1[1][0] = 4; mat1[1][1] = 5; mat1[1][2] = 6;

  TComplRectMatrix<int> mat2(2, 3);
  mat2[0][0] = 6; mat2[0][1] = 5; mat2[0][2] = 4;
  mat2[1][0] = 3; mat2[1][1] = 2; mat2[1][2] = 1;

  TComplRectMatrix<int> result = mat1 + mat2;

  EXPECT_EQ(result.GetRows(), 2);
  EXPECT_EQ(result.GetCols(), 3);
  EXPECT_EQ(result[0][0], 7);
  EXPECT_EQ(result[0][1], 7);
  EXPECT_EQ(result[1][2], 7);
}

TEST(TComplRectMatrixTest, SubtractionOperator)
{
  TComplRectMatrix<int> mat1(2, 3);
  mat1[0][0] = 1; mat1[0][1] = 2; mat1[0][2] = 3;
  mat1[1][0] = 4; mat1[1][1] = 5; mat1[1][2] = 6;

  TComplRectMatrix<int> mat2(2, 3);
  mat2[0][0] = 6; mat2[0][1] = 5; mat2[0][2] = 4;
  mat2[1][0] = 3; mat2[1][1] = 2; mat2[1][2] = 1;

  TComplRectMatrix<int> result = mat1 - mat2;

  EXPECT_EQ(result.GetRows(), 2);
  EXPECT_EQ(result.GetCols(), 3);
  EXPECT_EQ(result[0][0], -5);
  EXPECT_EQ(result[0][1], -3);
  EXPECT_EQ(result[1][2], 5);
}

TEST(TComplRectMatrixTest, ScalarMultiplication)
{
  TComplRectMatrix<int> mat(2, 3);
  mat[0][0] = 1; mat[0][1] = 2; mat[0][2] = 3;
  mat[1][0] = 4; mat[1][1] = 5; mat[1][2] = 6;

  TComplRectMatrix<int> result = mat * 2;

  EXPECT_EQ(result.GetRows(), 2);
  EXPECT_EQ(result.GetCols(), 3);
  EXPECT_EQ(result[0][0], 2);
  EXPECT_EQ(result[0][2], 6);
  EXPECT_EQ(result[1][1], 10);
}

TEST(TComplRectMatrixTest, MatrixMultiplication)
{
  TComplRectMatrix<int> mat1(2, 3);
  mat1[0][0] = 1; mat1[0][1] = 2; mat1[0][2] = 3;
  mat1[1][0] = 4; mat1[1][1] = 5; mat1[1][2] = 6;

  TComplRectMatrix<int> mat2(3, 2);
  mat2[0][0] = 7; mat2[0][1] = 8;
  mat2[1][0] = 9; mat2[1][1] = 10;
  mat2[2][0] = 11; mat2[2][1] = 12;

  TComplRectMatrix<int> result = mat1 * mat2;

  EXPECT_EQ(result.GetRows(), 2);
  EXPECT_EQ(result.GetCols(), 2);
  EXPECT_EQ(result[0][0], 58);   // 1*7 + 2*9 + 3*11
  EXPECT_EQ(result[0][1], 64);   // 1*8 + 2*10 + 3*12
  EXPECT_EQ(result[1][0], 139);  // 4*7 + 5*9 + 6*11
  EXPECT_EQ(result[1][1], 154);  // 4*8 + 5*10 + 6*12
}

TEST(TComplRectMatrixTest, CountOccurrences)
{
  TComplRectMatrix<int> mat(2, 3);
  mat[0][0] = 1; mat[0][1] = 2; mat[0][2] = 1;
  mat[1][0] = 3; mat[1][1] = 1; mat[1][2] = 4;

  EXPECT_EQ(mat.CountOccurrences(1), 3);
  EXPECT_EQ(mat.CountOccurrences(2), 1);
  EXPECT_EQ(mat.CountOccurrences(5), 0);
}

TEST(TComplRectMatrixTest, FirstNorm)
{
  TComplRectMatrix<double> mat(2, 3);
  mat[0][0] = 1; mat[0][1] = -2; mat[0][2] = 3;
  mat[1][0] = -4; mat[1][1] = 5; mat[1][2] = -6;

  // Суммы по столбцам: |1|+|-4|=5, |-2|+|5|=7, |3|+|-6|=9
  double result = mat.FirstNorm();
  EXPECT_DOUBLE_EQ(result, 9.0);
}

TEST(TComplRectMatrixTest, SecondNorm)
{
  TComplRectMatrix<double> mat(3, 2);  // Прямоугольная матрица 3x2
  mat[0][0] = 3; mat[0][1] = 4;
  mat[1][0] = 1; mat[1][1] = 2;
  mat[2][0] = 0; mat[2][1] = 1;

  // sqrt(3² + 4² + 1² + 2² + 0² + 1²) = sqrt(9 + 16 + 1 + 4 + 0 + 1) = sqrt(31)
  double result = mat.SecondNorm();
  EXPECT_DOUBLE_EQ(result, std::sqrt(31.0));
}

TEST(TComplRectMatrixTest, InfinityNorm)
{
  TComplRectMatrix<double> mat(2, 3);
  mat[0][0] = 1; mat[0][1] = -2; mat[0][2] = 3;
  mat[1][0] = -4; mat[1][1] = 5; mat[1][2] = -6;

  // Суммы по строкам: |1|+|-2|+|3|=6, |-4|+|5|+|-6|=15
  double result = mat.InfinityNorm();
  EXPECT_DOUBLE_EQ(result, 15.0);
}

TEST(TComplRectMatrixTest, HelderNorm)
{
  TComplRectMatrix<double> mat(2, 3);  // Прямоугольная матрица 2x3
  mat[0][0] = 2; mat[0][1] = 0; mat[0][2] = 1;
  mat[1][0] = 0; mat[1][1] = 2; mat[1][2] = 1;

  // Для p=2: (|2|² + |0|² + |1|² + |0|² + |2|² + |1|²)^(1/2) = sqrt(4 + 0 + 1 + 0 + 4 + 1) = sqrt(10)
  double result = mat.HelderNorm(2.0);
  EXPECT_DOUBLE_EQ(result, std::sqrt(10.0));
}
TEST(TComplRectMatrixTest, ApplyToAll)
{
  TComplRectMatrix<int> mat(3, 2);  // Прямоугольная матрица 3x2
  mat[0][0] = 1; mat[0][1] = 2;
  mat[1][0] = 3; mat[1][1] = 4;
  mat[2][0] = 5; mat[2][1] = 6;

  mat.ApplyToAll([](int& x) { x *= 2; });

  EXPECT_EQ(mat[0][0], 2);
  EXPECT_EQ(mat[0][1], 4);
  EXPECT_EQ(mat[1][0], 6);
  EXPECT_EQ(mat[1][1], 8);
  EXPECT_EQ(mat[2][0], 10);
  EXPECT_EQ(mat[2][1], 12);
}

TEST(TComplRectMatrixTest, ConstApplyToAll)
{
  TComplRectMatrix<int> mat(2, 3);  // Прямоугольная матрица 2x3
  mat[0][0] = 1; mat[0][1] = 2; mat[0][2] = 3;
  mat[1][0] = 4; mat[1][1] = 5; mat[1][2] = 6;

  int sum = 0;
  const TComplRectMatrix<int>& constMat = mat;
  constMat.ApplyToAll([&sum](const int& x) { sum += x; });

  EXPECT_EQ(sum, 21);  // 1+2+3+4+5+6 = 21
}

TEST(TComplRectMatrixTest, Iterator)
{
  TComplRectMatrix<int> mat(2, 4);  // Прямоугольная матрица 2x4
  mat[0][0] = 1; mat[0][1] = 2; mat[0][2] = 3; mat[0][3] = 4;
  mat[1][0] = 5; mat[1][1] = 6; mat[1][2] = 7; mat[1][3] = 8;

  int sum = 0;
  for (auto it = mat.begin(); it != mat.end(); ++it)
  {
    sum += *it;
  }

  EXPECT_EQ(sum, 36);  // 1+2+3+4+5+6+7+8 = 36
}
