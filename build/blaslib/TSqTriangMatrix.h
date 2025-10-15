#pragma once

#include <iostream>
#include "..//blaslib/TVector.h"
#include <functional>
#include <cmath>
#include <algorithm>

template <class T>
class TSqTriangMatrix
{
protected:
  TVector<T> data;          // Вектор для хранения треугольной матрицы
  int size;                 // Размер матрицы (size x size)
  bool is_upper;            // true - верхняя треугольная, false - нижняя треугольная

public:
  // А) Конструкторы и деструктор
  TSqTriangMatrix();
  TSqTriangMatrix(int size_, bool is_upper_ = true);
  TSqTriangMatrix(int size_, bool is_upper_, const T& value);
  TSqTriangMatrix(const TSqTriangMatrix& obj);
  TSqTriangMatrix(TSqTriangMatrix&& obj);
  ~TSqTriangMatrix();

  // Доступ к защищенным полям
  int GetSize() const;
  bool IsUpper() const;
  const TVector<T>& GetData() const;

  // Б) Итератор
  class Iterator
  {
  private:
    TSqTriangMatrix<T>* matrix_ptr;
    int row;
    int col;

  public:
    Iterator(TSqTriangMatrix<T>* mat, int r, int c);
    T& operator*();
    Iterator& operator++();
    Iterator operator++(int);
    bool operator==(const Iterator& other) const;
    bool operator!=(const Iterator& other) const;
  };

  Iterator begin();
  Iterator end();

  // В) Перегруженные операции
  TSqTriangMatrix operator+(const TSqTriangMatrix<T>& obj);
  TSqTriangMatrix operator-(const TSqTriangMatrix<T>& obj);
  TSqTriangMatrix operator*(const TSqTriangMatrix<T>& obj);
  TSqTriangMatrix operator*(const T& scalar);

  TSqTriangMatrix& operator=(const TSqTriangMatrix<T>& obj);
  TSqTriangMatrix& operator=(TSqTriangMatrix<T>&& obj);
  bool operator==(const TSqTriangMatrix<T>& obj);
  bool operator!=(const TSqTriangMatrix<T>& obj);

  T& operator()(int row, int col);
  const T& operator()(int row, int col) const;

  // Г) Потоковый ввод/вывод
  template <class U>
  friend std::ostream& operator<<(std::ostream& os, TSqTriangMatrix<U>& mat);
  template <class U>
  friend std::istream& operator>>(std::istream& is, TSqTriangMatrix<U>& mat);

  // Е) Основные методы
  bool IsEmpty() const;
  bool IsFull() const;
  void ApplyToAll(const std::function<void(T&)>& func);
  void ApplyToAll(const std::function<void(const T&)>& func) const;

  // Дополнительные методы
  int CountOccurrences(const T& value) const;
  T FirstNorm() const;           // Первая норма (L1-норма)
  T SecondNorm() const;          // Вторая норма (Фробениуса)
  T HelderNorm(T p) const;       // Гёльдерова норма (Lp-норма)
  T InfinityNorm() const;        // Бесконечная норма (L∞-норма)

private:
  // Вспомогательные методы
  void InitializeData();
  int GetDataIndex(int i, int j) const;
  bool IsInTriangle(int i, int j) const;
  int GetDataSize() const;
};

// Реализация методов класса TSqTriangMatrix

// А) Конструкторы и деструктор
template<class T>
inline TSqTriangMatrix<T>::TSqTriangMatrix()
  : size(0), is_upper(true)
{}

template<class T>
inline TSqTriangMatrix<T>::TSqTriangMatrix(int size_, bool is_upper_)
  : size(size_), is_upper(is_upper_)
{
  if (size_ < 0)
    throw std::invalid_argument("Size cannot be negative");

  InitializeData();
}

template<class T>
inline TSqTriangMatrix<T>::TSqTriangMatrix(int size_, bool is_upper_, const T& value)
  : size(size_), is_upper(is_upper_)
{
  if (size_ < 0)
    throw std::invalid_argument("Size cannot be negative");

  InitializeData();

  // Заполняем значением
  for (int i = 0; i < data.GetLen(); i++)
  {
    data[i] = value;
  }
}

template<class T>
inline TSqTriangMatrix<T>::TSqTriangMatrix(const TSqTriangMatrix& obj)
  : data(obj.data), size(obj.size), is_upper(obj.is_upper)
{}

template<class T>
inline TSqTriangMatrix<T>::TSqTriangMatrix(TSqTriangMatrix&& obj)
  : data(std::move(obj.data)), size(obj.size), is_upper(obj.is_upper)
{
  obj.size = 0;
  obj.is_upper = true;
}

template<class T>
inline TSqTriangMatrix<T>::~TSqTriangMatrix()
{
  size = 0;
  is_upper = true;
}

// Доступ к защищенным полям
template<class T>
inline int TSqTriangMatrix<T>::GetSize() const
{
  return size;
}

template<class T>
inline bool TSqTriangMatrix<T>::IsUpper() const
{
  return is_upper;
}

template<class T>
inline const TVector<T>& TSqTriangMatrix<T>::GetData() const
{
  return data;
}

// Вспомогательные методы
template<class T>
inline void TSqTriangMatrix<T>::InitializeData()
{
  if (size == 0)
    return;

  int data_size = size * (size + 1) / 2;
  data.SetLen(data_size);
}

template<class T>
inline int TSqTriangMatrix<T>::GetDataIndex(int i, int j) const
{
  if (!IsInTriangle(i, j))
    return -1;

  if (is_upper)
  {
    // Для верхней треугольной матрицы: row <= col
    return i * size - (i * (i - 1)) / 2 + (j - i);
  }
  else
  {
    // Для нижней треугольной матрицы: row >= col
    return i * (i + 1) / 2 + j;
  }
}

template<class T>
inline bool TSqTriangMatrix<T>::IsInTriangle(int i, int j) const
{
  if (i < 0 || i >= size || j < 0 || j >= size)
    return false;

  if (is_upper)
  {
    return i <= j;  // Верхняя треугольная: элементы на и выше главной диагонали
  }
  else
  {
    return i >= j;  // Нижняя треугольная: элементы на и ниже главной диагонали
  }
}

template<class T>
inline int TSqTriangMatrix<T>::GetDataSize() const
{
  return size * (size + 1) / 2;
}

// Б) Реализация итератора
template<class T>
inline TSqTriangMatrix<T>::Iterator::Iterator(TSqTriangMatrix<T>* mat, int r, int c)
  : matrix_ptr(mat), row(r), col(c)
{}

template<class T>
inline T& TSqTriangMatrix<T>::Iterator::operator*()
{
  return (*matrix_ptr)(row, col);
}

template<class T>
inline typename TSqTriangMatrix<T>::Iterator& TSqTriangMatrix<T>::Iterator::operator++()
{
  col++;
  if (col >= matrix_ptr->size)
  {
    col = 0;
    row++;
  }
  return *this;
}

template<class T>
inline typename TSqTriangMatrix<T>::Iterator TSqTriangMatrix<T>::Iterator::operator++(int)
{
  Iterator temp = *this;
  ++(*this);
  return temp;
}

template<class T>
inline bool TSqTriangMatrix<T>::Iterator::operator==(const Iterator& other) const
{
  return matrix_ptr == other.matrix_ptr && row == other.row && col == other.col;
}

template<class T>
inline bool TSqTriangMatrix<T>::Iterator::operator!=(const Iterator& other) const
{
  return !(*this == other);
}

template<class T>
inline typename TSqTriangMatrix<T>::Iterator TSqTriangMatrix<T>::begin()
{
  return Iterator(this, 0, 0);
}

template<class T>
inline typename TSqTriangMatrix<T>::Iterator TSqTriangMatrix<T>::end()
{
  return Iterator(this, size, 0);
}

// В) Перегруженные операции
template<class T>
inline TSqTriangMatrix<T> TSqTriangMatrix<T>::operator+(const TSqTriangMatrix<T>& obj)
{
  if (size != obj.size || is_upper != obj.is_upper)
    throw std::invalid_argument("Matrix dimensions and type must match for addition");

  TSqTriangMatrix<T> result(size, is_upper);
  for (int i = 0; i < data.GetLen(); i++)
  {
    result.data[i] = data[i] + obj.data[i];
  }
  return result;
}

template<class T>
inline TSqTriangMatrix<T> TSqTriangMatrix<T>::operator-(const TSqTriangMatrix<T>& obj)
{
  if (size != obj.size || is_upper != obj.is_upper)
    throw std::invalid_argument("Matrix dimensions and type must match for subtraction");

  TSqTriangMatrix<T> result(size, is_upper);
  for (int i = 0; i < data.GetLen(); i++)
  {
    result.data[i] = data[i] - obj.data[i];
  }
  return result;
}

template<class T>
inline TSqTriangMatrix<T> TSqTriangMatrix<T>::operator*(const TSqTriangMatrix<T>& obj)
{
  if (size != obj.size)
    throw std::invalid_argument("Matrix sizes must match for multiplication");

  // Результат умножения треугольных матриц может не быть треугольной матрицей
  // Поэтому создаем полную матрицу, но храним только треугольную часть
  TSqTriangMatrix<T> result(size, is_upper);

  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size; j++)
    {
      if (result.IsInTriangle(i, j))
      {
        T sum = T(0);
        for (int k = 0; k < size; k++)
        {
          if (IsInTriangle(i, k) && obj.IsInTriangle(k, j))
          {
            sum += (*this)(i, k) * obj(k, j);
          }
        }
        result(i, j) = sum;
      }
    }
  }
  return result;
}

template<class T>
inline TSqTriangMatrix<T> TSqTriangMatrix<T>::operator*(const T& scalar)
{
  TSqTriangMatrix<T> result(size, is_upper);
  for (int i = 0; i < data.GetLen(); i++)
  {
    result.data[i] = data[i] * scalar;
  }
  return result;
}

template<class T>
inline TSqTriangMatrix<T>& TSqTriangMatrix<T>::operator=(const TSqTriangMatrix<T>& obj)
{
  if (this == &obj)
    return *this;

  size = obj.size;
  is_upper = obj.is_upper;
  data = obj.data;

  return *this;
}

template<class T>
inline TSqTriangMatrix<T>& TSqTriangMatrix<T>::operator=(TSqTriangMatrix<T>&& obj)
{
  if (this == &obj)
    return *this;

  size = obj.size;
  is_upper = obj.is_upper;
  data = std::move(obj.data);

  obj.size = 0;
  obj.is_upper = true;

  return *this;
}

template<class T>
inline bool TSqTriangMatrix<T>::operator==(const TSqTriangMatrix<T>& obj)
{
  if (size != obj.size || is_upper != obj.is_upper)
    return false;

  return data == obj.data;
}

template<class T>
inline bool TSqTriangMatrix<T>::operator!=(const TSqTriangMatrix<T>& obj)
{
  return !(*this == obj);
}

template<class T>
inline T& TSqTriangMatrix<T>::operator()(int row, int col)
{
  if (row < 0 || row >= size || col < 0 || col >= size)
    throw std::out_of_range("Matrix indices out of range");

  int index = GetDataIndex(row, col);
  if (index == -1)
    throw std::out_of_range("Element outside matrix triangle");

  return data[index];
}

template<class T>
inline const T& TSqTriangMatrix<T>::operator()(int row, int col) const
{
  if (row < 0 || row >= size || col < 0 || col >= size)
    throw std::out_of_range("Matrix indices out of range");

  int index = GetDataIndex(row, col);
  if (index == -1)
    return T(0);  // Для элементов вне треугольника возвращаем 0

  return data[index];
}

// Г) Потоковый ввод/вывод
template<class U>
inline std::ostream& operator<<(std::ostream& os, TSqTriangMatrix<U>& mat)
{
  for (int i = 0; i < mat.size; i++)
  {
    for (int j = 0; j < mat.size; j++)
    {
      os << mat(i, j) << " ";
    }
    os << "\n";
  }
  return os;
}

template<class U>
inline std::istream& operator>>(std::istream& is, TSqTriangMatrix<U>& mat)
{
  int size;
  bool is_upper;
  is >> size >> is_upper;

  // Создаем временную матрицу
  TSqTriangMatrix<U> temp(size, is_upper);

  // Читаем только элементы внутри треугольника
  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size; j++)
    {
      if (temp.IsInTriangle(i, j))
      {
        is >> temp(i, j);
      }
    }
  }

  // Присваиваем временную матрицу исходной
  mat = std::move(temp);
  return is;
}

// Е) Основные методы
template<class T>
inline bool TSqTriangMatrix<T>::IsEmpty() const
{
  return size == 0 || data.GetLen() == 0;
}

template<class T>
inline bool TSqTriangMatrix<T>::IsFull() const
{
  return !IsEmpty();
}

template<class T>
inline void TSqTriangMatrix<T>::ApplyToAll(const std::function<void(T&)>& func)
{
  if (!IsEmpty())
  {
    data.ApplyToAll(func);
  }
}

template<class T>
inline void TSqTriangMatrix<T>::ApplyToAll(const std::function<void(const T&)>& func) const
{
  if (!IsEmpty())
  {
    data.ApplyToAll(func);
  }
}

// Дополнительные методы
template<class T>
inline int TSqTriangMatrix<T>::CountOccurrences(const T& value) const
{
  if (IsEmpty())
    return 0;

  int count = 0;
  for (int i = 0; i < data.GetLen(); i++)
  {
    if (data[i] == value)
      count++;
  }
  return count;
}

template<class T>
inline T TSqTriangMatrix<T>::FirstNorm() const
{
  if (IsEmpty())
    return T(0);

  T max_col_sum = T(0);

  for (int j = 0; j < size; j++)
  {
    T col_sum = T(0);
    for (int i = 0; i < size; i++)
    {
      col_sum += std::abs((*this)(i, j));
    }

    if (col_sum > max_col_sum)
      max_col_sum = col_sum;
  }

  return max_col_sum;
}

template<class T>
inline T TSqTriangMatrix<T>::SecondNorm() const
{
  if (IsEmpty())
    return T(0);

  T sum = T(0);
  for (int i = 0; i < data.GetLen(); i++)
  {
    sum += data[i] * data[i];
  }

  return std::sqrt(sum);
}

template<class T>
inline T TSqTriangMatrix<T>::HelderNorm(T p) const
{
  if (IsEmpty())
    return T(0);

  if (p < 1)
    throw std::invalid_argument("p must be >= 1 for Helder norm");

  T sum = T(0);
  for (int i = 0; i < data.GetLen(); i++)
  {
    sum += std::pow(std::abs(data[i]), p);
  }

  return std::pow(sum, T(1) / p);
}

template<class T>
inline T TSqTriangMatrix<T>::InfinityNorm() const
{
  if (IsEmpty())
    return T(0);

  T max_row_sum = T(0);

  for (int i = 0; i < size; i++)
  {
    T row_sum = T(0);
    for (int j = 0; j < size; j++)
    {
      row_sum += std::abs((*this)(i, j));
    }

    if (row_sum > max_row_sum)
      max_row_sum = row_sum;
  }

  return max_row_sum;
}