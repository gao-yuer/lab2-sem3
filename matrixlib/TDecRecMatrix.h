#pragma once

#include <iostream>
#include "..//matrixlib/TVector.h"
#include <functional>
#include <cmath>
#include <algorithm>

template <class T>
class TDecRecMatrix
{
protected:
  struct MatrixElement {
    int row;
    int col;
    T value;

    MatrixElement() : row(0), col(0), value(T()) {}
    MatrixElement(int r, int c, const T& v) : row(r), col(c), value(v) {}
    bool operator==(const MatrixElement& other) const {
      return row == other.row && col == other.col && value == other.value;
    }
    bool operator!=(const MatrixElement& other) const {
      return !(*this == other);
    }
  };

  TVector<MatrixElement> data;  // Вектор для хранения ненулевых элементов
  int size;                     // Размер матрицы (size x size)
  T zero_value;                 // Значение, считающееся нулевым (по умолчанию T(0))

public:
  // А) Конструкторы и деструктор
  TDecRecMatrix();
  TDecRecMatrix(int size_);
  TDecRecMatrix(int size_, const T& zero_value_);
  TDecRecMatrix(const TDecRecMatrix& obj);
  TDecRecMatrix(TDecRecMatrix&& obj);
  ~TDecRecMatrix();

  // Доступ к защищенным полям
  int GetSize() const;
  T GetZeroValue() const;
  const TVector<MatrixElement>& GetData() const;
  int GetNonZeroCount() const;

  // Б) Итератор
  class Iterator
  {
  private:
    TDecRecMatrix<T>* matrix_ptr;
    int row;
    int col;
    int data_index;

  public:
    Iterator(TDecRecMatrix<T>* mat, int r, int c, int idx = 0);
    T& operator*();
    Iterator& operator++();
    Iterator operator++(int);
    bool operator==(const Iterator& other) const;
    bool operator!=(const Iterator& other) const;
  };

  Iterator begin();
  Iterator end();

  // В) Перегруженные операции
  TDecRecMatrix operator+(const TDecRecMatrix<T>& obj);
  TDecRecMatrix operator-(const TDecRecMatrix<T>& obj);
  TDecRecMatrix operator*(const TDecRecMatrix<T>& obj);
  TDecRecMatrix operator*(const T& scalar);

  TDecRecMatrix& operator=(const TDecRecMatrix<T>& obj);
  TDecRecMatrix& operator=(TDecRecMatrix<T>&& obj);
  bool operator==(const TDecRecMatrix<T>& obj);
  bool operator!=(const TDecRecMatrix<T>& obj);

  T& operator()(int row, int col);
  const T& operator()(int row, int col) const;

  // Г) Потоковый ввод/вывод
  template <class U>
  friend std::ostream& operator<<(std::ostream& os, TDecRecMatrix<U>& mat);
  template <class U>
  friend std::istream& operator>>(std::istream& is, TDecRecMatrix<U>& mat);

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
  T InfinityNorm() const;        // Бесконечная норма (L?-норма)

  // Вспомогательные методы для разреженных матриц
  void SetElement(int row, int col, const T& value);
  void RemoveElement(int row, int col);
  int FindElementIndex(int row, int col) const;
  void SortData();  // Сортировка элементов по строкам и столбцам

private:
  bool IsZero(const T& value) const;
};

// Реализация методов класса TDecRecMatrix

// А) Конструкторы и деструктор
template<class T>
inline TDecRecMatrix<T>::TDecRecMatrix()
  : size(0), zero_value(T(0))
{}

template<class T>
inline TDecRecMatrix<T>::TDecRecMatrix(int size_)
  : size(size_), zero_value(T(0))
{
  if (size_ < 0)
    throw std::invalid_argument("Size cannot be negative");
}

template<class T>
inline TDecRecMatrix<T>::TDecRecMatrix(int size_, const T& zero_value_)
  : size(size_), zero_value(zero_value_)
{
  if (size_ < 0)
    throw std::invalid_argument("Size cannot be negative");
}

template<class T>
inline TDecRecMatrix<T>::TDecRecMatrix(const TDecRecMatrix& obj)
  : data(obj.data), size(obj.size), zero_value(obj.zero_value)
{}

template<class T>
inline TDecRecMatrix<T>::TDecRecMatrix(TDecRecMatrix&& obj)
  : data(std::move(obj.data)), size(obj.size), zero_value(obj.zero_value)
{
  obj.size = 0;
  obj.zero_value = T(0);
}

template<class T>
inline TDecRecMatrix<T>::~TDecRecMatrix()
{
  size = 0;
  zero_value = T(0);
}

// Доступ к защищенным полям
template<class T>
inline int TDecRecMatrix<T>::GetSize() const
{
  return size;
}

template<class T>
inline T TDecRecMatrix<T>::GetZeroValue() const
{
  return zero_value;
}

template<class T>
inline const TVector<typename TDecRecMatrix<T>::MatrixElement>& TDecRecMatrix<T>::GetData() const
{
  return data;
}

template<class T>
inline int TDecRecMatrix<T>::GetNonZeroCount() const
{
  return data.GetLen();
}

// Вспомогательные методы
template<class T>
inline bool TDecRecMatrix<T>::IsZero(const T& value) const
{
  return value == zero_value;
}

template<class T>
inline int TDecRecMatrix<T>::FindElementIndex(int row, int col) const
{
  for (int i = 0; i < data.GetLen(); i++)
  {
    if (data[i].row == row && data[i].col == col)
      return i;
  }
  return -1;
}

template<class T>
inline void TDecRecMatrix<T>::SetElement(int row, int col, const T& value)
{
  if (row < 0 || row >= size || col < 0 || col >= size)
    throw std::out_of_range("Matrix indices out of range");

  int index = FindElementIndex(row, col);

  if (IsZero(value))
  {
    // Если устанавливаем нулевое значение, удаляем элемент
    if (index != -1)
    {
      RemoveElement(row, col);
    }
  }
  else
  {
    // Если устанавливаем ненулевое значение
    if (index != -1)
    {
      // Обновляем существующий элемент
      data[index].value = value;
    }
    else
    {
      // Добавляем новый элемент
      data.push_back(MatrixElement(row, col, value));
    }
  }
}

template<class T>
inline void TDecRecMatrix<T>::RemoveElement(int row, int col)
{
  int index = FindElementIndex(row, col);
  if (index != -1)
  {
    // Создаем новый вектор без удаляемого элемента
    TVector<MatrixElement> new_data;
    for (int i = 0; i < data.GetLen(); i++)
    {
      if (i != index)
      {
        new_data.push_back(data[i]);
      }
    }
    data = std::move(new_data);
  }
}

template<class T>
inline void TDecRecMatrix<T>::SortData()
{
  // Сортировка по строкам, затем по столбцам
  for (int i = 0; i < data.GetLen() - 1; i++)
  {
    for (int j = 0; j < data.GetLen() - i - 1; j++)
    {
      if (data[j].row > data[j + 1].row ||
        (data[j].row == data[j + 1].row && data[j].col > data[j + 1].col))
      {
        std::swap(data[j], data[j + 1]);
      }
    }
  }
}

// Б) Реализация итератора
template<class T>
inline TDecRecMatrix<T>::Iterator::Iterator(TDecRecMatrix<T>* mat, int r, int c, int idx)
  : matrix_ptr(mat), row(r), col(c), data_index(idx)
{}

template<class T>
inline T& TDecRecMatrix<T>::Iterator::operator*()
{
  return (*matrix_ptr)(row, col);
}

template<class T>
inline typename TDecRecMatrix<T>::Iterator& TDecRecMatrix<T>::Iterator::operator++()
{
  col++;
  if (col >= matrix_ptr->size)
  {
    col = 0;
    row++;
  }

  // Находим следующий ненулевой элемент
  while (row < matrix_ptr->size && matrix_ptr->FindElementIndex(row, col) == -1)
  {
    col++;
    if (col >= matrix_ptr->size)
    {
      col = 0;
      row++;
    }
  }

  return *this;
}

template<class T>
inline typename TDecRecMatrix<T>::Iterator TDecRecMatrix<T>::Iterator::operator++(int)
{
  Iterator temp = *this;
  ++(*this);
  return temp;
}

template<class T>
inline bool TDecRecMatrix<T>::Iterator::operator==(const Iterator& other) const
{
  return matrix_ptr == other.matrix_ptr && row == other.row && col == other.col;
}

template<class T>
inline bool TDecRecMatrix<T>::Iterator::operator!=(const Iterator& other) const
{
  return !(*this == other);
}

template<class T>
inline typename TDecRecMatrix<T>::Iterator TDecRecMatrix<T>::begin()
{
  // Находим первый ненулевой элемент
  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size; j++)
    {
      if (FindElementIndex(i, j) != -1)
      {
        return Iterator(this, i, j);
      }
    }
  }
  return Iterator(this, size, 0); // Если нет ненулевых элементов
}

template<class T>
inline typename TDecRecMatrix<T>::Iterator TDecRecMatrix<T>::end()
{
  return Iterator(this, size, 0);
}

// В) Перегруженные операции
template<class T>
inline TDecRecMatrix<T> TDecRecMatrix<T>::operator+(const TDecRecMatrix<T>& obj)
{
  if (size != obj.size)
    throw std::invalid_argument("Matrix sizes must match for addition");

  TDecRecMatrix<T> result(size, zero_value);

  // Копируем все элементы из текущей матрицы
  for (int i = 0; i < data.GetLen(); i++)
  {
    result.SetElement(data[i].row, data[i].col, data[i].value);
  }

  // Добавляем элементы из второй матрицы
  for (int i = 0; i < obj.data.GetLen(); i++)
  {
    const MatrixElement& elem = obj.data[i];
    T current_value = result(elem.row, elem.col);
    T new_value = current_value + elem.value;
    result.SetElement(elem.row, elem.col, new_value);
  }

  return result;
}

template<class T>
inline TDecRecMatrix<T> TDecRecMatrix<T>::operator-(const TDecRecMatrix<T>& obj)
{
  if (size != obj.size)
    throw std::invalid_argument("Matrix sizes must match for subtraction");

  TDecRecMatrix<T> result(size, zero_value);

  // Копируем все элементы из текущей матрицы
  for (int i = 0; i < data.GetLen(); i++)
  {
    result.SetElement(data[i].row, data[i].col, data[i].value);
  }

  // Вычитаем элементы из второй матрицы
  for (int i = 0; i < obj.data.GetLen(); i++)
  {
    const MatrixElement& elem = obj.data[i];
    T current_value = result(elem.row, elem.col);
    T new_value = current_value - elem.value;
    result.SetElement(elem.row, elem.col, new_value);
  }

  return result;
}

template<class T>
inline TDecRecMatrix<T> TDecRecMatrix<T>::operator*(const TDecRecMatrix<T>& obj)
{
  if (size != obj.size)
    throw std::invalid_argument("Matrix sizes must match for multiplication");

  TDecRecMatrix<T> result(size, zero_value);

  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size; j++)
    {
      T sum = zero_value;
      for (int k = 0; k < size; k++)
      {
        T a = (*this)(i, k);
        T b = obj(k, j);
        if (!IsZero(a) && !IsZero(b))
        {
          sum = sum + a * b;
        }
      }
      if (!IsZero(sum))
      {
        result.SetElement(i, j, sum);
      }
    }
  }

  return result;
}

template<class T>
inline TDecRecMatrix<T> TDecRecMatrix<T>::operator*(const T& scalar)
{
  TDecRecMatrix<T> result(size, zero_value);

  if (IsZero(scalar))
  {
    // Умножение на ноль - возвращаем нулевую матрицу
    return result;
  }

  for (int i = 0; i < data.GetLen(); i++)
  {
    T new_value = data[i].value * scalar;
    if (!IsZero(new_value))
    {
      result.SetElement(data[i].row, data[i].col, new_value);
    }
  }

  return result;
}

template<class T>
inline TDecRecMatrix<T>& TDecRecMatrix<T>::operator=(const TDecRecMatrix<T>& obj)
{
  if (this == &obj)
    return *this;

  size = obj.size;
  zero_value = obj.zero_value;
  data = obj.data;

  return *this;
}

template<class T>
inline TDecRecMatrix<T>& TDecRecMatrix<T>::operator=(TDecRecMatrix<T>&& obj)
{
  if (this == &obj)
    return *this;

  size = obj.size;
  zero_value = obj.zero_value;
  data = std::move(obj.data);

  obj.size = 0;
  obj.zero_value = T(0);

  return *this;
}

template<class T>
inline bool TDecRecMatrix<T>::operator==(const TDecRecMatrix<T>& obj)
{
  if (size != obj.size || zero_value != obj.zero_value)
    return false;

  // Сравниваем ненулевые элементы
  if (data.GetLen() != obj.data.GetLen())
    return false;

  for (int i = 0; i < data.GetLen(); i++)
  {
    if (data[i] != obj.data[i])
      return false;
  }

  return true;
}

template<class T>
inline bool TDecRecMatrix<T>::operator!=(const TDecRecMatrix<T>& obj)
{
  return !(*this == obj);
}

template<class T>
inline T& TDecRecMatrix<T>::operator()(int row, int col)
{
  if (row < 0 || row >= size || col < 0 || col >= size)
    throw std::out_of_range("Matrix indices out of range");

  int index = FindElementIndex(row, col);
  if (index == -1)
  {
    // Создаем новый нулевой элемент
    data.push_back(MatrixElement(row, col, zero_value));
    return data[data.GetLen() - 1].value;
  }

  return data[index].value;
}

template<class T>
inline const T& TDecRecMatrix<T>::operator()(int row, int col) const
{
  if (row < 0 || row >= size || col < 0 || col >= size)
    throw std::out_of_range("Matrix indices out of range");

  int index = FindElementIndex(row, col);
  if (index == -1)
    return zero_value;

  return data[index].value;
}

// Г) Потоковый ввод/вывод
template<class U>
inline std::ostream& operator<<(std::ostream& os, TDecRecMatrix<U>& mat)
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
inline std::istream& operator>>(std::istream& is, TDecRecMatrix<U>& mat)
{
  int size;
  is >> size;

  // Создаем временную матрицу
  TDecRecMatrix<U> temp(size);

  // Читаем элементы
  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size; j++)
    {
      U value;
      is >> value;
      if (value != temp.zero_value)
      {
        temp.SetElement(i, j, value);
      }
    }
  }

  // Присваиваем временную матрицу исходной
  mat = std::move(temp);
  return is;
}

// Е) Основные методы
template<class T>
inline bool TDecRecMatrix<T>::IsEmpty() const
{
  return size == 0 || data.GetLen() == 0;
}

template<class T>
inline bool TDecRecMatrix<T>::IsFull() const
{
  return !IsEmpty();
}

template<class T>
inline void TDecRecMatrix<T>::ApplyToAll(const std::function<void(T&)>& func)
{
  for (int i = 0; i < data.GetLen(); i++)
  {
    func(data[i].value);
    // Если элемент стал нулевым, удаляем его
    if (IsZero(data[i].value))
    {
      RemoveElement(data[i].row, data[i].col);
      i--; // Уменьшаем счетчик, так как размер изменился
    }
  }
}

template<class T>
inline void TDecRecMatrix<T>::ApplyToAll(const std::function<void(const T&)>& func) const
{
  for (int i = 0; i < data.GetLen(); i++)
  {
    func(data[i].value);
  }
}

// Дополнительные методы
template<class T>
inline int TDecRecMatrix<T>::CountOccurrences(const T& value) const
{
  if (IsEmpty())
    return 0;

  int count = 0;
  for (int i = 0; i < data.GetLen(); i++)
  {
    if (data[i].value == value)
      count++;
  }
  return count;
}

template<class T>
inline T TDecRecMatrix<T>::FirstNorm() const
{
  if (IsEmpty())
    return zero_value;

  T max_col_sum = zero_value;

  for (int j = 0; j < size; j++)
  {
    T col_sum = zero_value;
    for (int i = 0; i < size; i++)
    {
      col_sum = col_sum + std::abs((*this)(i, j));
    }

    if (col_sum > max_col_sum)
      max_col_sum = col_sum;
  }

  return max_col_sum;
}

template<class T>
inline T TDecRecMatrix<T>::SecondNorm() const
{
  if (IsEmpty())
    return zero_value;

  T sum = zero_value;
  for (int i = 0; i < data.GetLen(); i++)
  {
    sum = sum + data[i].value * data[i].value;
  }

  return std::sqrt(sum);
}

template<class T>
inline T TDecRecMatrix<T>::HelderNorm(T p) const
{
  if (IsEmpty())
    return zero_value;

  if (p < 1)
    throw std::invalid_argument("p must be >= 1 for Helder norm");

  T sum = zero_value;
  for (int i = 0; i < data.GetLen(); i++)
  {
    sum = sum + std::pow(std::abs(data[i].value), p);
  }

  return std::pow(sum, T(1) / p);
}

template<class T>
inline T TDecRecMatrix<T>::InfinityNorm() const
{
  if (IsEmpty())
    return zero_value;

  T max_row_sum = zero_value;

  for (int i = 0; i < size; i++)
  {
    T row_sum = zero_value;
    for (int j = 0; j < size; j++)
    {
      row_sum = row_sum + std::abs((*this)(i, j));
    }

    if (row_sum > max_row_sum)
      max_row_sum = row_sum;
  }

  return max_row_sum;
}