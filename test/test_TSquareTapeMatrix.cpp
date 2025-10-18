#include <gtest.h>
#include <cmath>
#include "..//matrixlib/TSquareTapeMatrix.h"

TEST(TSquareTapeMatrixTest, DefaultConstructor)
{
  TSquareTapeMatrix<int> mat;
  EXPECT_TRUE(mat.IsEmpty());
  EXPECT_EQ(mat.GetSize(), 0);
  EXPECT_EQ(mat.GetTapeWidth(), 0);
}

TEST(TSquareTapeMatrixTest, ParameterizedConstructor)
{
  TSquareTapeMatrix<double> mat(5, 3);
  EXPECT_FALSE(mat.IsEmpty());
  EXPECT_EQ(mat.GetSize(), 5);
  EXPECT_EQ(mat.GetTapeWidth(), 3);
  EXPECT_TRUE(mat.IsFull());
}

TEST(TSquareTapeMatrixTest, ParameterizedConstructorWithValue)
{
  TSquareTapeMatrix<int> mat(4, 2, 7);
  EXPECT_EQ(mat.GetSize(), 4);
  EXPECT_EQ(mat.GetTapeWidth(), 2);

  // Проверяем, что элементы внутри ленты инициализированы значением 7
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      if (mat.IsInBand(i, j))
      {
        EXPECT_EQ(mat(i, j), 7);
      }
    }
  }
}


TEST(TSquareTapeMatrixTest, EqualityOperator)
{
  TSquareTapeMatrix<int> mat1(3, 2);
  mat1(0, 0) = 1; mat1(0, 1) = 2;
  mat1(1, 0) = 3; mat1(1, 1) = 4;
  mat1(2, 1) = 5; mat1(2, 2) = 6;

  TSquareTapeMatrix<int> mat2(3, 2);
  mat2(0, 0) = 1; mat2(0, 1) = 2;
  mat2(1, 0) = 3; mat2(1, 1) = 4;
  mat2(2, 1) = 5; mat2(2, 2) = 6;

  TSquareTapeMatrix<int> mat3(3, 2);
  mat3(0, 0) = 7; mat3(0, 1) = 8;
  mat3(1, 0) = 9; mat3(1, 1) = 10;
  mat3(2, 1) = 11; mat3(2, 2) = 12;

  EXPECT_TRUE(mat1 == mat2);
  EXPECT_FALSE(mat1 == mat3);
}

TEST(TSquareTapeMatrixTest, InequalityOperator)
{
  TSquareTapeMatrix<int> mat1(2, 2);
  mat1(0, 0) = 1; mat1(0, 1) = 2;
  mat1(1, 0) = 3; mat1(1, 1) = 4;

  TSquareTapeMatrix<int> mat2(2, 2);
  mat2(0, 0) = 5; mat2(0, 1) = 6;
  mat2(1, 0) = 7; mat2(1, 1) = 8;

  EXPECT_TRUE(mat1 != mat2);
  EXPECT_FALSE(mat1 != mat1);
}

TEST(TSquareTapeMatrixTest, IsInBandMethod)
{
  TSquareTapeMatrix<int> mat(5, 3);

  // Элементы внутри ленты
  EXPECT_TRUE(mat.IsInBand(0, 0));  // Главная диагональ
  EXPECT_TRUE(mat.IsInBand(0, 1));  // Первая наддиагональ
  EXPECT_TRUE(mat.IsInBand(0, 2));  // Вторая наддиагональ
  EXPECT_TRUE(mat.IsInBand(1, 0));  // Первая поддиагональ
  EXPECT_TRUE(mat.IsInBand(2, 0));  // Вторая поддиагональ

  // Элементы вне ленты
  EXPECT_FALSE(mat.IsInBand(0, 3)); // Вне ленты
  EXPECT_FALSE(mat.IsInBand(0, 4)); // Вне ленты
  EXPECT_FALSE(mat.IsInBand(3, 0)); // Вне ленты
  EXPECT_FALSE(mat.IsInBand(4, 0)); // Вне ленты

  // Граничные случаи
  EXPECT_FALSE(mat.IsInBand(-1, 0)); // Отрицательные индексы
  EXPECT_FALSE(mat.IsInBand(0, -1)); // Отрицательные индексы
  EXPECT_FALSE(mat.IsInBand(5, 0));  // За пределами размера
  EXPECT_FALSE(mat.IsInBand(0, 5));  // За пределами размера
}

TEST(TSquareTapeMatrixTest, ConstOperatorAccess)
{
  TSquareTapeMatrix<int> temp(3, 2, 5);
  const TSquareTapeMatrix<int> mat = temp;

  // Проверка доступа к элементам внутри ленты
  EXPECT_EQ(mat(0, 0), 5);
  EXPECT_EQ(mat(0, 1), 5);
  EXPECT_EQ(mat(1, 1), 5);

  // Проверка доступа к элементам вне ленты (должны возвращать 0)
  EXPECT_EQ(mat(0, 2), 0);
  EXPECT_EQ(mat(2, 0), 0);
}

TEST(TSquareTapeMatrixTest, AdditionOperator)
{
  TSquareTapeMatrix<int> mat1(3, 2);
  mat1(0, 0) = 1; mat1(0, 1) = 2;
  mat1(1, 0) = 3; mat1(1, 1) = 4;
  mat1(2, 1) = 5; mat1(2, 2) = 6;

  TSquareTapeMatrix<int> mat2(3, 2);
  mat2(0, 0) = 6; mat2(0, 1) = 5;
  mat2(1, 0) = 4; mat2(1, 1) = 3;
  mat2(2, 1) = 2; mat2(2, 2) = 1;

  TSquareTapeMatrix<int> result = mat1 + mat2;

  EXPECT_EQ(result.GetSize(), 3);
  EXPECT_EQ(result.GetTapeWidth(), 2);
  EXPECT_EQ(result(0, 0), 7);
  EXPECT_EQ(result(0, 1), 7);
  EXPECT_EQ(result(1, 1), 7);
  EXPECT_EQ(result(2, 2), 7);
}

TEST(TSquareTapeMatrixTest, InfinityNorm)
{
  TSquareTapeMatrix<double> mat(3, 2);
  mat(0, 0) = 1; mat(0, 1) = -2;
  mat(1, 0) = -3; mat(1, 1) = 4;
  mat(2, 1) = 6; mat(2, 2) = -7;

  double result = mat.InfinityNorm();
  EXPECT_NEAR(result, 13.0, 1e-10);
}


TEST(TSquareTapeMatrixTest, InvalidArguments)
{
  EXPECT_THROW(TSquareTapeMatrix<int>(-1, 2), std::invalid_argument);
  EXPECT_THROW(TSquareTapeMatrix<int>(3, -1), std::invalid_argument);
  EXPECT_THROW(TSquareTapeMatrix<int>(3, 5), std::invalid_argument); // tape_width > size
}

TEST(TSquareTapeMatrixTest, OutOfRangeAccess)
{
  TSquareTapeMatrix<int> mat(3, 2);

  EXPECT_THROW(mat(-1, 0), std::out_of_range);
  EXPECT_THROW(mat(0, -1), std::out_of_range);
  EXPECT_THROW(mat(3, 0), std::out_of_range);
  EXPECT_THROW(mat(0, 3), std::out_of_range);
  EXPECT_THROW(mat(0, 2), std::out_of_range); // Вне ленты
}

TEST(TSquareTapeMatrixTest, GetTapeIndexCorrectness)
{
  TSquareTapeMatrix<int> mat(3, 2);

  // Проверяем правильность индексов
  EXPECT_NE(mat.GetTapeIndex(0, 0), -1);
  EXPECT_NE(mat.GetTapeIndex(0, 1), -1);
  EXPECT_NE(mat.GetTapeIndex(1, 0), -1);
  EXPECT_NE(mat.GetTapeIndex(1, 1), -1);
  EXPECT_NE(mat.GetTapeIndex(1, 2), -1);
  EXPECT_NE(mat.GetTapeIndex(2, 1), -1);
  EXPECT_NE(mat.GetTapeIndex(2, 2), -1);

  // Проверяем, что индексы вне ленты возвращают -1
  EXPECT_EQ(mat.GetTapeIndex(0, 2), -1);
  EXPECT_EQ(mat.GetTapeIndex(2, 0), -1);
}

TEST(TSquareTapeMatrixTest, EmptyMatrixOperations)
{
  TSquareTapeMatrix<int> empty_mat;
  TSquareTapeMatrix<int> mat(2, 2);

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

