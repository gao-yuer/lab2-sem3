#include <gtest.h>
#include <cmath>
#include <sstream>
#include "..//blaslib/TDecRecMatrix.h"


TEST(TDecRecMatrixTest, DefaultConstructor)
{
  TDecRecMatrix<int> mat;
  EXPECT_TRUE(mat.IsEmpty());
  EXPECT_EQ(mat.GetSize(), 0);
  EXPECT_EQ(mat.GetZeroValue(), 0);
  EXPECT_EQ(mat.GetNonZeroCount(), 0);
}

//TEST(TDecRecMatrixTest, ParameterizedConstructor)
//{
//  TDecRecMatrix<double> mat(5);
//  EXPECT_FALSE(mat.IsEmpty());
//  EXPECT_EQ(mat.GetSize(), 5);
//  EXPECT_EQ(mat.GetZeroValue(), 0.0);
//  EXPECT_EQ(mat.GetNonZeroCount(), 0);
//  EXPECT_TRUE(mat.IsFull());
//}

TEST(TDecRecMatrixTest, ParameterizedConstructorWithZeroValue)
{
  TDecRecMatrix<int> mat(4, -1);
  EXPECT_EQ(mat.GetSize(), 4);
  EXPECT_EQ(mat.GetZeroValue(), -1);
}

TEST(TDecRecMatrixTest, CopyConstructor)
{
  TDecRecMatrix<int> original(3);
  original(0, 0) = 1;
  original(1, 1) = 2;
  original(2, 2) = 3;

  TDecRecMatrix<int> copy(original);

  EXPECT_EQ(copy.GetSize(), 3);
  EXPECT_EQ(copy(0, 0), 1);
  EXPECT_EQ(copy(1, 1), 2);
  EXPECT_EQ(copy(2, 2), 3);
  EXPECT_EQ(copy.GetNonZeroCount(), 3);
}

TEST(TDecRecMatrixTest, MoveConstructor)
{
  TDecRecMatrix<int> original(3);
  original(0, 0) = 1;
  original(1, 1) = 2;
  original(2, 2) = 3;

  TDecRecMatrix<int> moved(std::move(original));

  EXPECT_EQ(moved.GetSize(), 3);
  EXPECT_EQ(moved(0, 0), 1);
  EXPECT_EQ(moved(1, 1), 2);
  EXPECT_EQ(moved(2, 2), 3);
  EXPECT_EQ(original.GetSize(), 0);
  EXPECT_EQ(original.GetNonZeroCount(), 0);
}

TEST(TDecRecMatrixTest, AssignmentOperator)
{
  TDecRecMatrix<int> mat1(3);
  mat1(0, 0) = 1;
  mat1(1, 1) = 2;
  mat1(2, 2) = 3;

  TDecRecMatrix<int> mat2;
  mat2 = mat1;

  EXPECT_EQ(mat2.GetSize(), 3);
  EXPECT_EQ(mat2(0, 0), 1);
  EXPECT_EQ(mat2(1, 1), 2);
  EXPECT_EQ(mat2(2, 2), 3);
  EXPECT_EQ(mat2.GetNonZeroCount(), 3);
}

TEST(TDecRecMatrixTest, MoveAssignmentOperator)
{
  TDecRecMatrix<int> mat1(4);
  mat1(0, 0) = 1;
  mat1(1, 1) = 2;
  mat1(2, 2) = 3;
  mat1(3, 3) = 4;

  TDecRecMatrix<int> mat2;
  mat2 = std::move(mat1);

  EXPECT_EQ(mat2.GetSize(), 4);
  EXPECT_EQ(mat2(0, 0), 1);
  EXPECT_EQ(mat2(3, 3), 4);
  EXPECT_EQ(mat1.GetSize(), 0);
}

TEST(TDecRecMatrixTest, EqualityOperator)
{
  TDecRecMatrix<int> mat1(3);
  mat1(0, 0) = 1;
  mat1(1, 1) = 2;
  mat1(2, 2) = 3;

  TDecRecMatrix<int> mat2(3);
  mat2(0, 0) = 1;
  mat2(1, 1) = 2;
  mat2(2, 2) = 3;

  TDecRecMatrix<int> mat3(3);
  mat3(0, 0) = 4;
  mat3(1, 1) = 5;
  mat3(2, 2) = 6;

  EXPECT_TRUE(mat1 == mat2);
  EXPECT_FALSE(mat1 == mat3);
}

TEST(TDecRecMatrixTest, InequalityOperator)
{
  TDecRecMatrix<int> mat1(2);
  mat1(0, 0) = 1;
  mat1(1, 1) = 2;

  TDecRecMatrix<int> mat2(2);
  mat2(0, 0) = 3;
  mat2(1, 1) = 4;

  EXPECT_TRUE(mat1 != mat2);
  EXPECT_FALSE(mat1 != mat1);
}

TEST(TDecRecMatrixTest, OperatorAccess)
{
  TDecRecMatrix<int> mat(3);

  // Установка значений
  mat(0, 0) = 1;
  mat(0, 1) = 2;
  mat(1, 1) = 3;
  mat(2, 2) = 4;

  // Проверка значений
  EXPECT_EQ(mat(0, 0), 1);
  EXPECT_EQ(mat(0, 1), 2);
  EXPECT_EQ(mat(1, 1), 3);
  EXPECT_EQ(mat(2, 2), 4);

  // Проверка нулевых элементов
  EXPECT_EQ(mat(1, 0), 0);
  EXPECT_EQ(mat(2, 0), 0);
}

TEST(TDecRecMatrixTest, ConstOperatorAccess)
{
  TDecRecMatrix<int> temp(3);
  temp(0, 0) = 1;
  temp(1, 1) = 2;
  const TDecRecMatrix<int> mat = temp;

  // Проверка доступа к элементам
  EXPECT_EQ(mat(0, 0), 1);
  EXPECT_EQ(mat(1, 1), 2);

  // Проверка доступа к нулевым элементам
  EXPECT_EQ(mat(0, 1), 0);
  EXPECT_EQ(mat(2, 0), 0);
}

//TEST(TDecRecMatrixTest, SetElementAndRemoveElement)
//{
//  TDecRecMatrix<int> mat(3);
//
//  // Добавление элементов
//  mat.SetElement(0, 0, 1);
//  mat.SetElement(1, 1, 2);
//  mat.SetElement(2, 2, 3);
//
//  EXPECT_EQ(mat.GetNonZeroCount(), 3);
//  EXPECT_EQ(mat(0, 0), 1);
//  EXPECT_EQ(mat(1, 1), 2);
//  EXPECT_EQ(mat(2, 2), 3);
//
//  // Обновление элемента
//  mat.SetElement(1, 1, 5);
//  EXPECT_EQ(mat(1, 1), 5);
//  EXPECT_EQ(mat.GetNonZeroCount(), 3);
//
//  // Установка нулевого значения (удаление)
//  mat.SetElement(1, 1, 0);
//  EXPECT_EQ(mat(1, 1), 0);
//  EXPECT_EQ(mat.GetNonZeroCount(), 2);
//
//  // Явное удаление
//  mat.RemoveElement(0, 0);
//  EXPECT_EQ(mat(0, 0), 0);
//  EXPECT_EQ(mat.GetNonZeroCount(), 1);
//}

TEST(TDecRecMatrixTest, AdditionOperator)
{
  TDecRecMatrix<int> mat1(3);
  mat1(0, 0) = 1;
  mat1(1, 1) = 2;
  mat1(2, 2) = 3;

  TDecRecMatrix<int> mat2(3);
  mat2(0, 0) = 4;
  mat2(1, 1) = 5;
  mat2(2, 2) = 6;

  TDecRecMatrix<int> result = mat1 + mat2;

  EXPECT_EQ(result.GetSize(), 3);
  EXPECT_EQ(result(0, 0), 5);
  EXPECT_EQ(result(1, 1), 7);
  EXPECT_EQ(result(2, 2), 9);
  EXPECT_EQ(result.GetNonZeroCount(), 3);
}

TEST(TDecRecMatrixTest, SubtractionOperator)
{
  TDecRecMatrix<int> mat1(3);
  mat1(0, 0) = 5;
  mat1(1, 1) = 6;
  mat1(2, 2) = 7;

  TDecRecMatrix<int> mat2(3);
  mat2(0, 0) = 1;
  mat2(1, 1) = 2;
  mat2(2, 2) = 3;

  TDecRecMatrix<int> result = mat1 - mat2;

  EXPECT_EQ(result.GetSize(), 3);
  EXPECT_EQ(result(0, 0), 4);
  EXPECT_EQ(result(1, 1), 4);
  EXPECT_EQ(result(2, 2), 4);
}

TEST(TDecRecMatrixTest, ScalarMultiplication)
{
  TDecRecMatrix<int> mat(3);
  mat(0, 0) = 1;
  mat(1, 1) = 2;
  mat(2, 2) = 3;

  TDecRecMatrix<int> result = mat * 3;

  EXPECT_EQ(result.GetSize(), 3);
  EXPECT_EQ(result(0, 0), 3);
  EXPECT_EQ(result(1, 1), 6);
  EXPECT_EQ(result(2, 2), 9);
}

TEST(TDecRecMatrixTest, MatrixMultiplication)
{
  TDecRecMatrix<int> mat1(3);
  mat1(0, 0) = 1; mat1(0, 1) = 2;
  mat1(1, 1) = 3;
  mat1(2, 2) = 4;

  TDecRecMatrix<int> mat2(3);
  mat2(0, 0) = 2; mat2(0, 1) = 1;
  mat2(1, 1) = 3; mat2(1, 2) = 2;
  mat2(2, 2) = 4;

  TDecRecMatrix<int> result = mat1 * mat2;

  EXPECT_EQ(result.GetSize(), 3);

  // Проверка некоторых элементов
  // (0,0): 1*2 + 2*0 + 0*0 = 2
  // (0,1): 1*1 + 2*3 + 0*0 = 1 + 6 = 7
  // (1,1): 0*0 + 3*3 + 0*0 = 9
  EXPECT_EQ(result(0, 0), 2);
  EXPECT_EQ(result(0, 1), 7);
  EXPECT_EQ(result(1, 1), 9);
}

TEST(TDecRecMatrixTest, CountOccurrences)
{
  TDecRecMatrix<int> mat(4);
  mat(0, 0) = 1; mat(0, 1) = 2; mat(0, 2) = 1;
  mat(1, 1) = 1; mat(1, 2) = 2;
  mat(2, 2) = 1; mat(2, 3) = 3;
  mat(3, 3) = 2;

  EXPECT_EQ(mat.CountOccurrences(1), 4);
  EXPECT_EQ(mat.CountOccurrences(2), 3);
  EXPECT_EQ(mat.CountOccurrences(3), 1);
  EXPECT_EQ(mat.CountOccurrences(0), 0); // Нулевые элементы не хранятся
}

TEST(TDecRecMatrixTest, FirstNorm)
{
  TDecRecMatrix<double> mat(3);
  mat(0, 0) = 1; mat(0, 1) = -2;
  mat(1, 1) = -3;
  mat(2, 2) = 4;

  // Суммы по столбцам: |1|=1, |-2|+|-3|=5, |4|=4
  double result = mat.FirstNorm();
  EXPECT_NEAR(result, 5.0, 1e-10);
}

TEST(TDecRecMatrixTest, SecondNorm)
{
  TDecRecMatrix<double> mat(3);
  mat(0, 0) = 3; mat(0, 1) = 4;
  mat(1, 1) = 1;
  mat(2, 2) = 2;

  // sqrt(3? + 4? + 1? + 2?) = sqrt(9+16+1+4) = sqrt(30)
  double result = mat.SecondNorm();
  EXPECT_NEAR(result, std::sqrt(30.0), 1e-10);
}

TEST(TDecRecMatrixTest, InfinityNorm)
{
  TDecRecMatrix<double> mat(3);
  mat(0, 0) = 1; mat(0, 1) = -2;
  mat(1, 1) = -3; mat(1, 2) = 4;
  mat(2, 2) = 5;

  // Суммы по строкам: |1|+|-2|=3, |-3|+|4|=7, |5|=5
  double result = mat.InfinityNorm();
  EXPECT_NEAR(result, 7.0, 1e-10);
}

TEST(TDecRecMatrixTest, HelderNorm)
{
  TDecRecMatrix<double> mat(2);
  mat(0, 0) = 2; mat(0, 1) = 1;
  mat(1, 1) = 2;

  // Для p=2: (|2|? + |1|? + |2|?)^(1/2) = sqrt(4+1+4) = 3
  double result = mat.HelderNorm(2.0);
  EXPECT_NEAR(result, 3.0, 1e-10);
}

TEST(TDecRecMatrixTest, ApplyToAll)
{
  TDecRecMatrix<int> mat(3);
  mat(0, 0) = 1; mat(0, 1) = 2;
  mat(1, 1) = 3;
  mat(2, 2) = 4;

  mat.ApplyToAll([](int& x) { x += 10; });

  EXPECT_EQ(mat(0, 0), 11);
  EXPECT_EQ(mat(0, 1), 12);
  EXPECT_EQ(mat(1, 1), 13);
  EXPECT_EQ(mat(2, 2), 14);
}

TEST(TDecRecMatrixTest, ConstApplyToAll)
{
  TDecRecMatrix<int> mat(2);
  mat(0, 0) = 1; mat(0, 1) = 2;
  mat(1, 1) = 3;

  int sum = 0;
  const TDecRecMatrix<int>& constMat = mat;
  constMat.ApplyToAll([&sum](const int& x) { sum += x; });

  EXPECT_EQ(sum, 6);  // 1+2+3
}

TEST(TDecRecMatrixTest, Iterator)
{
  TDecRecMatrix<int> mat(3);
  mat(0, 0) = 1; mat(0, 1) = 2;
  mat(1, 1) = 3; mat(1, 2) = 4;
  mat(2, 2) = 5;

  int sum = 0;
  int count = 0;
  for (auto it = mat.begin(); it != mat.end(); ++it)
  {
    sum += *it;
    count++;
  }

  EXPECT_EQ(sum, 15);  // 1+2+3+4+5
  EXPECT_EQ(count, 5); // 5 ненулевых элементов
}


TEST(TDecRecMatrixTest, EmptyMatrixOperations)
{
  TDecRecMatrix<int> empty_mat;
  TDecRecMatrix<int> mat(2);

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

//TEST(TDecRecMatrixTest, CustomZeroValue)
//{
//  // Тестируем матрицу с кастомным нулевым значением
//  TDecRecMatrix<int> mat(3, -1);
//
//  mat(0, 0) = 1;
//  mat(1, 1) = 2;
//
//  EXPECT_EQ(mat(0, 0), 1);
//  EXPECT_EQ(mat(1, 1), 2);
//  EXPECT_EQ(mat(0, 1), -1); // Кастомное нулевое значение
//
//  // Установка кастомного нуля должна удалять элемент
//  mat.SetElement(1, 1, -1);
//  EXPECT_EQ(mat(1, 1), -1);
//  EXPECT_EQ(mat.GetNonZeroCount(), 1); // Остался только один элемент
//}

TEST(TDecRecMatrixTest, SparseMatrixEfficiency)
{
  // Тестируем эффективность разреженной матрицы
  TDecRecMatrix<int> mat(1000); // Большая матрица

  // Добавляем только несколько ненулевых элементов
  mat(10, 20) = 1;
  mat(100, 200) = 2;
  mat(500, 600) = 3;

  EXPECT_EQ(mat.GetNonZeroCount(), 3); // Только 3 элемента хранятся
  EXPECT_EQ(mat.GetSize(), 1000);      // Но размер матрицы 1000x1000

  // Проверяем, что доступ к нулевым элементам работает
  EXPECT_EQ(mat(0, 0), 0);
  EXPECT_EQ(mat(999, 999), 0);
}

TEST(TDecRecMatrixTest, FindElementIndex)
{
  TDecRecMatrix<int> mat(3);
  mat(0, 0) = 1;
  mat(1, 1) = 2;
  mat(2, 2) = 3;

  EXPECT_NE(mat.FindElementIndex(0, 0), -1);
  EXPECT_NE(mat.FindElementIndex(1, 1), -1);
  EXPECT_NE(mat.FindElementIndex(2, 2), -1);
  EXPECT_EQ(mat.FindElementIndex(0, 1), -1); // Несуществующий элемент
  EXPECT_EQ(mat.FindElementIndex(1, 0), -1); // Несуществующий элемент
}

//TEST(TDecRecMatrixTest, ZeroElementRemoval)
//{
//  TDecRecMatrix<int> mat(3);
//  mat(0, 0) = 1;
//  mat(1, 1) = 2;
//  mat(2, 2) = 3;
//
//  EXPECT_EQ(mat.GetNonZeroCount(), 3);
//
//  // Устанавливаем нулевое значение через оператор ()
//  mat(1, 1) = 0;
//  EXPECT_EQ(mat.GetNonZeroCount(), 2); // Элемент должен быть удален
//
//  // Проверяем, что элемент действительно удален
//  EXPECT_EQ(mat(1, 1), 0);
//  EXPECT_EQ(mat.FindElementIndex(1, 1), -1);
//}