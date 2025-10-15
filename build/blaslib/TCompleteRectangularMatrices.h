#pragma once
#include "..//blaslib/TVector.h"  
#include <iostream>
#include <functional>

template <class T>
class TComplRectMatrix
{
protected:
  TVector<TVector<T>> matrix;  // Вектор векторов для хранения матрицы
  int rows;
  int cols;

public:
  TComplRectMatrix();
  TComplRectMatrix(int rows_, int cols_);
  TComplRectMatrix(int rows_, int cols_, const T& value);
  TComplRectMatrix(const TComplRectMatrix& obj);
  TComplRectMatrix(TComplRectMatrix&& obj);
  ~TComplRectMatrix();


  int GetRows() const;
  int GetCols() const;
  TVector<TVector<T>>& GetMatrix();
  const TVector<TVector<T>>& GetMatrix() const;

 //Итератор
  class Iterator
  {
  private:
    TComplRectMatrix<T>* matrix_ptr;
    int row;
    int col;

  public:
    Iterator(TComplRectMatrix<T>* mat, int r, int c);
    T& operator*();
    Iterator& operator++();
    Iterator operator++(int);
    bool operator==(const Iterator& other) const;
    bool operator!=(const Iterator& other) const;
  };

  Iterator begin();
  Iterator end();

  //Перегруженные операции
  TComplRectMatrix operator+(const TComplRectMatrix<T>& obj);
  TComplRectMatrix operator-(const TComplRectMatrix<T>& obj);
  TComplRectMatrix operator*(const TComplRectMatrix<T>& obj);
  TComplRectMatrix operator*(const T& scalar);

  TComplRectMatrix& operator=(const TComplRectMatrix<T>& obj);
  TComplRectMatrix& operator=(TComplRectMatrix<T>&& obj);
  bool operator==(const TComplRectMatrix<T>& obj);
  bool operator!=(const TComplRectMatrix<T>& obj);

  TVector<T>& operator[](int row);
  const TVector<T>& operator[](int row) const;

  template <class T>
  friend std::ostream& operator<<(std::ostream& os, TComplRectMatrix<T>& mat);
  template <class T>
  friend std::istream& operator>>(std::istream& is, TComplRectMatrix<T>& mat);

  bool IsEmpty() const;
  bool IsFull() const;
  void ApplyToAll(const std::function<void(T&)>& func);
  void ApplyToAll(const std::function<void(const T&)>& func) const;

 //доп таски
  int CountOccurrences(const T& value) const;
  T FirstNorm() const;           
  T SecondNorm() const;          
  T HelderNorm(T p) const;       
  T InfinityNorm() const;
};

template<class T>
inline TComplRectMatrix<T>::TComplRectMatrix()
  : rows(0), cols(0)
{
}

template<class T>
inline TComplRectMatrix<T>::TComplRectMatrix(int rows_, int cols_)
  : rows(rows_), cols(cols_)
{
  if (rows_ < 0 || cols_ < 0)
    throw std::invalid_argument("Rows and columns cannot be negative");

  matrix.SetLen(rows);
  for (int i = 0; i < rows; i++)
  {
    matrix[i].SetLen(cols);
  }
}

template<class T>
inline TComplRectMatrix<T>::TComplRectMatrix(int rows_, int cols_, const T& value)
  : rows(rows_), cols(cols_)
{
  if (rows_ < 0 || cols_ < 0)
    throw std::invalid_argument("Rows and columns cannot be negative");

  matrix.SetLen(rows);
  for (int i = 0; i < rows; i++)
  {
    matrix[i].SetLen(cols);
    for (int j = 0; j < cols; j++)
    {
      matrix[i][j] = value;
    }
  }
}

template<class T>
inline TComplRectMatrix<T>::TComplRectMatrix(const TComplRectMatrix& obj)
  : rows(obj.rows), cols(obj.cols)
{
  matrix.SetLen(rows);
  for (int i = 0; i < rows; i++)
  {
    matrix[i] = obj.matrix[i];  // Используем оператор присваивания TVector
  }
}

template<class T>
inline TComplRectMatrix<T>::TComplRectMatrix(TComplRectMatrix&& obj)
  : matrix(std::move(obj.matrix)), rows(obj.rows), cols(obj.cols)
{
  obj.rows = 0;
  obj.cols = 0;
}

template<class T>
inline TComplRectMatrix<T>::~TComplRectMatrix()
{
  rows = 0;
  cols = 0;
}

template<class T>
inline int TComplRectMatrix<T>::GetRows() const
{
  return rows;
}

template<class T>
inline int TComplRectMatrix<T>::GetCols() const
{
  return cols;
}

template<class T>
inline TVector<TVector<T>>& TComplRectMatrix<T>::GetMatrix()
{
  return matrix;
}

template<class T>
inline const TVector<TVector<T>>& TComplRectMatrix<T>::GetMatrix() const
{
  return matrix;
}

//Реализация итератора
template<class T>
inline TComplRectMatrix<T>::Iterator::Iterator(TComplRectMatrix<T>* mat, int r, int c)
  : matrix_ptr(mat), row(r), col(c)
{}

template<class T>
inline T& TComplRectMatrix<T>::Iterator::operator*()
{
  return (*matrix_ptr)[row][col];
}

template<class T>
inline typename TComplRectMatrix<T>::Iterator& TComplRectMatrix<T>::Iterator::operator++()
{
  col++;
  if (col >= matrix_ptr->cols)
  {
    col = 0;
    row++;
  }
  return *this;
}

template<class T>
inline typename TComplRectMatrix<T>::Iterator TComplRectMatrix<T>::Iterator::operator++(int)
{
  Iterator temp = *this;
  ++(*this);
  return temp;
}

template<class T>
inline bool TComplRectMatrix<T>::Iterator::operator==(const Iterator& other) const
{
  return matrix_ptr == other.matrix_ptr && row == other.row && col == other.col;
}

template<class T>
inline bool TComplRectMatrix<T>::Iterator::operator!=(const Iterator& other) const
{
  return !(*this == other);
}

template<class T>
inline typename TComplRectMatrix<T>::Iterator TComplRectMatrix<T>::begin()
{
  return Iterator(this, 0, 0);
}

template<class T>
inline typename TComplRectMatrix<T>::Iterator TComplRectMatrix<T>::end()
{
  return Iterator(this, rows, 0);
}

//Реализация перегруженных операций
template<class T>
inline TComplRectMatrix<T> TComplRectMatrix<T>::operator+(const TComplRectMatrix<T>& obj)
{
  if (rows != obj.rows || cols != obj.cols)
    throw std::invalid_argument("Matrix dimensions must match for addition");

  TComplRectMatrix<T> result(rows, cols);
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
      result.matrix[i][j] = matrix[i][j] + obj.matrix[i][j];
  }
  return result;
}

template<class T>
inline TComplRectMatrix<T> TComplRectMatrix<T>::operator-(const TComplRectMatrix<T>& obj)
{
  if (rows != obj.rows || cols != obj.cols)
    throw std::invalid_argument("Matrix dimensions must match for subtraction");

  TComplRectMatrix<T> result(rows, cols);
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
      result.matrix[i][j] = matrix[i][j] - obj.matrix[i][j];
  }
  return result;
}

template<class T>
inline TComplRectMatrix<T> TComplRectMatrix<T>::operator*(const TComplRectMatrix<T>& obj)
{
  if (cols != obj.rows)
    throw std::invalid_argument("Number of columns in first matrix must equal number of rows in second matrix");

  TComplRectMatrix<T> result(rows, obj.cols);
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < obj.cols; j++)
    {
      T sum = T(0);
      for (int k = 0; k < cols; k++)
      {
        sum += matrix[i][k] * obj.matrix[k][j];
      }
      result.matrix[i][j] = sum;
    }
  }
  return result;
}

template<class T>
inline TComplRectMatrix<T> TComplRectMatrix<T>::operator*(const T& scalar)
{
  TComplRectMatrix<T> result(rows, cols);
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
      result.matrix[i][j] = matrix[i][j] * scalar;
  }
  return result;
}

template<class T>
inline TComplRectMatrix<T>& TComplRectMatrix<T>::operator=(const TComplRectMatrix<T>& obj)
{
  if (this == &obj)
    return *this;

  rows = obj.rows;
  cols = obj.cols;
  matrix = obj.matrix;  // Используем оператор присваивания TVector

  return *this;
}

template<class T>
inline TComplRectMatrix<T>& TComplRectMatrix<T>::operator=(TComplRectMatrix<T>&& obj)
{
  if (this == &obj)
    return *this;

  rows = obj.rows;
  cols = obj.cols;
  matrix = std::move(obj.matrix);

  obj.rows = 0;
  obj.cols = 0;

  return *this;
}

template<class T>
inline bool TComplRectMatrix<T>::operator==(const TComplRectMatrix<T>& obj)
{
  if (rows != obj.rows || cols != obj.cols)
    return false;

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      if (matrix[i][j] != obj.matrix[i][j])
        return false;
    }
  }
  return true;
}

template<class T>
inline bool TComplRectMatrix<T>::operator!=(const TComplRectMatrix<T>& obj)
{
  return !(*this == obj);
}

template<class T>
inline TVector<T>& TComplRectMatrix<T>::operator[](int row)
{
  if (row < 0 || row >= rows)
    throw std::out_of_range("Row index out of range");
  return matrix[row];
}

template<class T>
inline const TVector<T>& TComplRectMatrix<T>::operator[](int row) const
{
  if (row < 0 || row >= rows)
    throw std::out_of_range("Row index out of range");
  return matrix[row];
}

template<class T>
inline std::ostream& operator<<(std::ostream& os, TComplRectMatrix<T>& mat)
{
  for (int i = 0; i < mat.rows; i++)
  {
    for (int j = 0; j < mat.cols; j++)
    {
      os << mat.matrix[i][j] << " ";
    }
    os << "\n";
  }
  return os;
}

template<class T>
inline std::istream& operator>>(std::istream& is, TComplRectMatrix<T>& mat)
{
  int rows, cols;
  is >> rows >> cols;

  TComplRectMatrix<T> temp(rows, cols);

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
      is >> mat.matrix[i][j];
  }
  mat = std::move(temp);
  return is;
}

template<class T>
inline bool TComplRectMatrix<T>::IsEmpty() const
{
  return rows == 0 || cols == 0;
}

template<class T>
inline bool TComplRectMatrix<T>::IsFull() const
{
  return !IsEmpty();
}

template<class T>
inline void TComplRectMatrix<T>::ApplyToAll(const std::function<void(T&)>& func)
{
  for (int i = 0; i < rows; i++)
    matrix[i].ApplyToAll(func);
}

template<class T>
inline void TComplRectMatrix<T>::ApplyToAll(const std::function<void(const T&)>& func) const
{
  for (int i = 0; i < rows; i++)
    matrix[i].ApplyToAll(func);
}

template<class T>
inline int TComplRectMatrix<T>::CountOccurrences(const T& value) const
{
  if (IsEmpty())
    return 0;

  int count = 0;
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      if (matrix[i][j] == value)
        count++;
    }
  }
  return count;
}

template<class T>
inline T TComplRectMatrix<T>::FirstNorm() const
{
  if (IsEmpty())
    return T(0);

  T max_col_sum = T(0);

  for (int j = 0; j < cols; j++)
  {
    T col_sum = T(0);
    for (int i = 0; i < rows; i++)
      col_sum += std::abs(matrix[i][j]);

    if (col_sum > max_col_sum)
      max_col_sum = col_sum;
  }

  return max_col_sum;
}

template<class T>
inline T TComplRectMatrix<T>::SecondNorm() const
{
  if (IsEmpty())
    return T(0);

  T sum = T(0);
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
      sum += matrix[i][j] * matrix[i][j];
  }

  return std::sqrt(sum);
}

template<class T>
inline T TComplRectMatrix<T>::HelderNorm(T p) const
{
  if (IsEmpty())
    return T(0);

  if (p < 1)
    throw std::invalid_argument("p must be >= 1 for Helder norm");

  T sum = T(0);
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
      sum += std::pow(std::abs(matrix[i][j]), p);
  }

  return std::pow(sum, T(1) / p);
}

template<class T>
inline T TComplRectMatrix<T>::InfinityNorm() const
{
  if (IsEmpty())
    return T(0);

  T max_row_sum = T(0);

  for (int i = 0; i < rows; i++)
  {
    T row_sum = T(0);
    for (int j = 0; j < cols; j++)
      row_sum += std::abs(matrix[i][j]);

    if (row_sum > max_row_sum)
      max_row_sum = row_sum;
  }

  return max_row_sum;
}





