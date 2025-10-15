#pragma once

#include <iostream>
#include "..//blaslib/TVector.h"
#include <functional>
#include <cmath>
#include <algorithm>

template <class T>
class TSquareTapeMatrix
{
protected:
  TVector<T> tape;          // Лента матрицы
  int size;                 // Размер матрицы (size x size)
  int tape_width;           // Ширина ленты (количество диагоналей)

public:
  TSquareTapeMatrix();
  TSquareTapeMatrix(int size_, int tape_width_);
  TSquareTapeMatrix(int size_, int tape_width_, const T& value);
  TSquareTapeMatrix(const TSquareTapeMatrix& obj);
  TSquareTapeMatrix(TSquareTapeMatrix&& obj);
  ~TSquareTapeMatrix();

  int GetSize() const;
  int GetTapeWidth() const;
  const TVector<T>& GetTape() const;

  //Итератор
  class Iterator
  {
  private:
    TSquareTapeMatrix<T>* matrix_ptr;
    int row;
    int col;

  public:
    Iterator(TSquareTapeMatrix<T>* mat, int r, int c);
    T& operator*();
    Iterator& operator++();
    Iterator operator++(int);
    bool operator==(const Iterator& other) const;
    bool operator!=(const Iterator& other) const;
  };

  Iterator begin();
  Iterator end();

  //Перегруженные операции
  TSquareTapeMatrix operator+(const TSquareTapeMatrix<T>& obj);
  TSquareTapeMatrix operator-(const TSquareTapeMatrix<T>& obj);
  TSquareTapeMatrix operator*(const TSquareTapeMatrix<T>& obj);
  TSquareTapeMatrix operator*(const T& scalar);

  TSquareTapeMatrix& operator=(const TSquareTapeMatrix<T>& obj);
  TSquareTapeMatrix& operator=(TSquareTapeMatrix<T>&& obj);
  bool operator==(const TSquareTapeMatrix<T>& obj);
  bool operator!=(const TSquareTapeMatrix<T>& obj);

  T& operator()(int row, int col);
  const T& operator()(int row, int col) const;

  template <class T>
  friend std::ostream& operator<<(std::ostream& os, TSquareTapeMatrix<T>& mat);
  template <class T>
  friend std::istream& operator>>(std::istream& is, TSquareTapeMatrix<T>& mat);

  bool IsEmpty() const;
  bool IsFull() const;
  void ApplyToAll(const std::function<void(T&)>& func);
  void ApplyToAll(const std::function<void(const T&)>& func) const;

  // Доп таски
  int CountOccurrences(const T& value) const;
  T FirstNorm() const;
  T SecondNorm() const;
  T HelderNorm(T p) const;
  T InfinityNorm() const;

  int GetTapeIndex(int i, int j) const;
  bool IsInBand(int i, int j) const;
};

template<class T>
inline TSquareTapeMatrix<T>::TSquareTapeMatrix()
  : size(0), tape_width(0)
{}

template<class T>
inline TSquareTapeMatrix<T>::TSquareTapeMatrix(int size_, int tape_width_)
  : size(size_), tape_width(tape_width_)
{
  if (size_ < 0 || tape_width_ < 0)
    throw std::invalid_argument("Size and tape width cannot be negative");
  if (tape_width_ > size_)
    throw std::invalid_argument("Tape width cannot be greater than matrix size");

  // Вычисляем размер ленты и инициализируем
  int tape_size = size * tape_width - (tape_width * (tape_width - 1)) / 2;
  if (tape_size > 0) {
    tape.SetLen(tape_size);
  }
}

template<class T>
inline TSquareTapeMatrix<T>::TSquareTapeMatrix(int size_, int tape_width_, const T& value)
  : size(size_), tape_width(tape_width_)
{
  if (size_ < 0 || tape_width_ < 0)
    throw std::invalid_argument("Size and tape width cannot be negative");
  if (tape_width_ > size_)
    throw std::invalid_argument("Tape width cannot be greater than matrix size");

  // Вычисляем размер ленты и инициализируем
  int tape_size = size * tape_width - (tape_width * (tape_width - 1)) / 2;
  if (tape_size > 0) {
    tape.SetLen(tape_size);
    // Заполняем значением
    for (int i = 0; i < tape_size; i++) {
      tape[i] = value;
    }
  }
}

template<class T>
inline TSquareTapeMatrix<T>::TSquareTapeMatrix(const TSquareTapeMatrix& obj)
  : tape(obj.tape), size(obj.size), tape_width(obj.tape_width)
{}

template<class T>
inline TSquareTapeMatrix<T>::TSquareTapeMatrix(TSquareTapeMatrix&& obj)
  : tape(std::move(obj.tape)), size(obj.size), tape_width(obj.tape_width)
{
  obj.size = 0;
  obj.tape_width = 0;
}

template<class T>
inline TSquareTapeMatrix<T>::~TSquareTapeMatrix()
{
  size = 0;
  tape_width = 0;
}

// Доступ к защищенным полям
template<class T>
inline int TSquareTapeMatrix<T>::GetSize() const
{
  return size;
}

template<class T>
inline int TSquareTapeMatrix<T>::GetTapeWidth() const
{
  return tape_width;
}

template<class T>
inline const TVector<T>& TSquareTapeMatrix<T>::GetTape() const
{
  return tape;
}

// Вспомогательные методы для ленточных матриц
template<class T>
inline int TSquareTapeMatrix<T>::GetTapeIndex(int i, int j) const
{
  if (i < 0 || i >= size || j < 0 || j >= size)
    return -1;

  // Проверяем, находится ли элемент в ленте
  if (std::abs(i - j) >= tape_width)
    return -1;

  // Вычисляем индекс в ленте (хранение по строкам)
  int row_start = i * tape_width - std::min(i, tape_width - 1);
  int col_offset = j - i + (tape_width - 1);

  return row_start + col_offset;
}

template<class T>
inline bool TSquareTapeMatrix<T>::IsInBand(int i, int j) const
{
  return (i >= 0 && i < size && j >= 0 && j < size &&
    std::abs(i - j) < tape_width);
}

// Б) Реализация итератора
template<class T>
inline TSquareTapeMatrix<T>::Iterator::Iterator(TSquareTapeMatrix<T>* mat, int r, int c)
  : matrix_ptr(mat), row(r), col(c)
{}

template<class T>
inline T& TSquareTapeMatrix<T>::Iterator::operator*()
{
  return (*matrix_ptr)(row, col);
}

template<class T>
inline typename TSquareTapeMatrix<T>::Iterator& TSquareTapeMatrix<T>::Iterator::operator++()
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
inline typename TSquareTapeMatrix<T>::Iterator TSquareTapeMatrix<T>::Iterator::operator++(int)
{
  Iterator temp = *this;
  ++(*this);
  return temp;
}

template<class T>
inline bool TSquareTapeMatrix<T>::Iterator::operator==(const Iterator& other) const
{
  return matrix_ptr == other.matrix_ptr && row == other.row && col == other.col;
}

template<class T>
inline bool TSquareTapeMatrix<T>::Iterator::operator!=(const Iterator& other) const
{
  return !(*this == other);
}

template<class T>
inline typename TSquareTapeMatrix<T>::Iterator TSquareTapeMatrix<T>::begin()
{
  return Iterator(this, 0, 0);
}

template<class T>
inline typename TSquareTapeMatrix<T>::Iterator TSquareTapeMatrix<T>::end()
{
  return Iterator(this, size, 0);
}

// В) Перегруженные операции
template<class T>
inline TSquareTapeMatrix<T> TSquareTapeMatrix<T>::operator+(const TSquareTapeMatrix<T>& obj)
{
  if (size != obj.size || tape_width != obj.tape_width)
    throw std::invalid_argument("Matrix dimensions and tape width must match for addition");

  TSquareTapeMatrix<T> result(size, tape_width);
  for (int i = 0; i < tape.GetLen(); i++)
  {
    result.tape[i] = tape[i] + obj.tape[i];
  }
  return result;
}

template<class T>
inline TSquareTapeMatrix<T> TSquareTapeMatrix<T>::operator-(const TSquareTapeMatrix<T>& obj)
{
  if (size != obj.size || tape_width != obj.tape_width)
    throw std::invalid_argument("Matrix dimensions and tape width must match for subtraction");

  TSquareTapeMatrix<T> result(size, tape_width);
  for (int i = 0; i < tape.GetLen(); i++)
  {
    result.tape[i] = tape[i] - obj.tape[i];
  }
  return result;
}

template<class T>
inline TSquareTapeMatrix<T> TSquareTapeMatrix<T>::operator*(const TSquareTapeMatrix<T>& obj)
{
  if (size != obj.size)
    throw std::invalid_argument("Matrix sizes must match for multiplication");

  // Ширина ленты результата
  int result_tape_width = std::min(tape_width + obj.tape_width - 1, size);
  TSquareTapeMatrix<T> result(size, result_tape_width);

  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size; j++)
    {
      if (result.IsInBand(i, j))
      {
        T sum = T(0);
        for (int k = 0; k < size; k++)
        {
          if (IsInBand(i, k) && obj.IsInBand(k, j))
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
inline TSquareTapeMatrix<T> TSquareTapeMatrix<T>::operator*(const T& scalar)
{
  TSquareTapeMatrix<T> result(size, tape_width);
  for (int i = 0; i < tape.GetLen(); i++)
  {
    result.tape[i] = tape[i] * scalar;
  }
  return result;
}

template<class T>
inline TSquareTapeMatrix<T>& TSquareTapeMatrix<T>::operator=(const TSquareTapeMatrix<T>& obj)
{
  if (this == &obj)
    return *this;

  size = obj.size;
  tape_width = obj.tape_width;
  tape = obj.tape;

  return *this;
}

template<class T>
inline TSquareTapeMatrix<T>& TSquareTapeMatrix<T>::operator=(TSquareTapeMatrix<T>&& obj)
{
  if (this == &obj)
    return *this;

  size = obj.size;
  tape_width = obj.tape_width;
  tape = std::move(obj.tape);

  obj.size = 0;
  obj.tape_width = 0;

  return *this;
}

template<class T>
inline bool TSquareTapeMatrix<T>::operator==(const TSquareTapeMatrix<T>& obj)
{
  if (size != obj.size || tape_width != obj.tape_width)
    return false;

  return tape == obj.tape;
}

template<class T>
inline bool TSquareTapeMatrix<T>::operator!=(const TSquareTapeMatrix<T>& obj)
{
  return !(*this == obj);
}

template<class T>
inline T& TSquareTapeMatrix<T>::operator()(int row, int col)
{
  if (row < 0 || row >= size || col < 0 || col >= size)
    throw std::out_of_range("Matrix indices out of range");

  int index = GetTapeIndex(row, col);
  if (index == -1 || index >= tape.GetLen())
    throw std::out_of_range("Element outside matrix band");

  return tape[index];
}

template<class T>
inline const T& TSquareTapeMatrix<T>::operator()(int row, int col) const
{
  if (row < 0 || row >= size || col < 0 || col >= size)
    throw std::out_of_range("Matrix indices out of range");

  int index = GetTapeIndex(row, col);
  if (index == -1 || index >= tape.GetLen())
    return T(0);  // Для элементов вне ленты возвращаем 0

  return tape[index];
}

// Г) Потоковый ввод/вывод
template<class T>
inline std::ostream& operator<<(std::ostream& os, TSquareTapeMatrix<T>& mat)
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

template<class T>
inline std::istream& operator>>(std::istream& is, TSquareTapeMatrix<T>& mat)
{
  int size, tape_width;
  is >> size >> tape_width;

  // Создаем временную матрицу
  TSquareTapeMatrix<T> temp(size, tape_width);

  // Читаем только элементы внутри ленты
  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size; j++)
    {
      if (temp.IsInBand(i, j))
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
inline bool TSquareTapeMatrix<T>::IsEmpty() const
{
  return size == 0 || tape_width == 0 || tape.GetLen() == 0;
}

template<class T>
inline bool TSquareTapeMatrix<T>::IsFull() const
{
  return !IsEmpty();
}

template<class T>
inline void TSquareTapeMatrix<T>::ApplyToAll(const std::function<void(T&)>& func)
{
  if (!IsEmpty())
  {
    tape.ApplyToAll(func);
  }
}

template<class T>
inline void TSquareTapeMatrix<T>::ApplyToAll(const std::function<void(const T&)>& func) const
{
  if (!IsEmpty())
  {
    tape.ApplyToAll(func);
  }
}

// Дополнительные методы
template<class T>
inline int TSquareTapeMatrix<T>::CountOccurrences(const T& value) const
{
  if (IsEmpty())
    return 0;

  int count = 0;
  for (int i = 0; i < tape.GetLen(); i++)
  {
    if (tape[i] == value)
      count++;
  }
  return count;
}

template<class T>
inline T TSquareTapeMatrix<T>::FirstNorm() const
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
inline T TSquareTapeMatrix<T>::SecondNorm() const
{
  if (IsEmpty())
    return T(0);

  T sum = T(0);
  for (int i = 0; i < tape.GetLen(); i++)
  {
    sum += tape[i] * tape[i];
  }

  return std::sqrt(sum);
}

template<class T>
inline T TSquareTapeMatrix<T>::HelderNorm(T p) const
{
  if (IsEmpty())
    return T(0);

  if (p < 1)
    throw std::invalid_argument("p must be >= 1 for Helder norm");

  T sum = T(0);
  for (int i = 0; i < tape.GetLen(); i++)
  {
    sum += std::pow(std::abs(tape[i]), p);
  }

  return std::pow(sum, T(1) / p);
}

template<class T>
inline T TSquareTapeMatrix<T>::InfinityNorm() const
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
