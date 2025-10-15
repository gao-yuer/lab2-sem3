#pragma once
#include <iostream>
#include <cstdlib>  // Для rand() и srand()
#include <ctime> 
#include <functional>

template <class T>
class TVector
{
protected:
  T* vector;
  int len;
public:
  TVector();
  TVector(int len_);
  TVector(const TVector& obj);
  TVector(TVector&& obj);
  ~TVector();

  int GetLen();
  int GetLen() const;
  void GetVector(T** vector_);

  void SetLen(int len_);
  void SetVector(T* vector_, int len_);

  TVector operator+(const TVector<T>& obj);
  TVector operator-(const TVector<T>& obj);
  T operator*(const TVector<T>& obj);
  TVector operator*(const T mul);
  TVector operator/(const T div);
    
  TVector& operator=(const TVector<T>& obj);
  TVector& operator=(TVector<T>&& obj);
  bool operator==(const TVector<T>& obj);
  bool operator!=(const TVector<T>& obj);

  T& operator[](int index);
  const T& operator[](int index) const;

  void push_back(const T& value);
  void push_front(const T& value);

  bool IsEmpty() const;
  bool IsFull() const; 
  void ApplyToAll(const std::function<void(T&)>& func);
  void ApplyToAll(const std::function<void(const T&)>& func) const;

  void RandomFillSimple(T min = T(0), T max = T(1));

  template <class O>
  friend std::ostream& operator<<(std::ostream& o, TVector<O>& v);
  template <class I>
  friend std::istream& operator>>(std::istream& i, TVector<I>& v);

  T Normalization(T obj);
  T FirstNorm(T obj);
  T SecondNorm(T obj);
  T InfinityNorm(T obj);
  T HelderNorm(T obj);
};

template<class T>
inline TVector<T>::TVector()
{
  vector = nullptr;
  len = 0;
}

template<class T>
inline TVector<T>::TVector(int len_)
{
  if (len_ < 0)
    throw(0);
  else if (len_ == 0)
  {
    len = 0;
    vector = nullptr;
  }
  else
  {
    len = len_;
    vector = new T[len]();
  }
}

template<class T>
inline TVector<T>::TVector(const TVector& obj) :
  TVector<T>::TVector(obj.len)
{
  for (int i = 0; i < len; i++)
    vector[i] = obj.vector[i];
}

template<class T>
inline TVector<T>::TVector(TVector&& obj)
{
  vector = obj.vector;
  len = obj.len;

  obj.len = 0;
  obj.vector = nullptr;
}

template<class T>
inline TVector<T>::~TVector()
{
  if (vector != nullptr)
    delete[] vector;
  vector = nullptr;
  len = 0;
}

template<class T>
inline int TVector<T>::GetLen()
{
  return len;
}

template<class T>
inline int TVector<T>::GetLen() const
{
  return len;
}

template<class T>
inline void TVector<T>::GetVector(T** vector_)
{
  *vector_ = vector;
}

template<class T>
inline void TVector<T>::SetLen(int len_)
{
  if (len == len_)
    return;
  if (len_ < 0)
    throw - 1;
  else if (len_ == 0)
  {
    if (vector != nullptr)
      delete[] vector;
    vector = nullptr;
    len = 0;
    return;
  }

  T* newVector = new T[len_];//10
  for (int i = 0; i < std::min(len, len_); i++)
    newVector[i] = vector[i];

  if (vector != nullptr)
    delete[] vector;

  vector = newVector;
  len = len_;
}

template<class T>
inline void TVector<T>::SetVector(T* vector_, int len_)
{
  if (len_ < 0)
    throw - 1;
  if (vector != nullptr)
    delete[] vector;
  vector = vector_;
  len = len_;

}



template<class T>
inline TVector<T> TVector<T>::operator+
(const TVector<T>& obj)
{
  if (len != obj.len)
    throw - 1;

  TVector<T> res = len;
  for (int i = 0; i < len; i++)
    //res[i] = (*this)[i] + obj[i];
    res.vector[i] = vector[i] + obj.vector[i];

  return res;
}

template<class T>
inline TVector<T> TVector<T>::operator-(const TVector<T>& obj)
{
  if (len != obj.len)
    throw - 1;

  TVector<T> res = len;
  for (int i = 0; i < len; i++)
    //res[i] = (*this)[i] - obj[i];
    res.vector[i] = vector[i] - obj.vector[i];

  return res;
}

template<class T>
inline T TVector<T>::operator*(const TVector<T>& obj)
{
  if (len != obj.len)
    throw - 1;

  T res = 0;
  for (int i = 0; i < len; i++)
    res += vector[i] * obj.vector[i];
    //res += (*this)[i] * obj[i];
    //res.vector[i] = vector[i] * obj.vector[i];

  return res;
}

template<class T>
inline TVector<T> TVector<T>::operator*(const T mul)
{
  TVector<T> res = len;
  for (int i = 0; i < len; i++)
    res.vector[i] = vector[i] * mul;

  return res;
}

template<class T>
inline TVector<T> TVector<T>::operator/(const T div)
{
  TVector<T> res = len;
  for (int i = 0; i < len; i++)
    res.vector[i] = vector[i] / div;

  return res;
}

template<class T>
inline TVector<T>& TVector<T>::operator=
(const TVector<T>& obj)
{
  if (this == &obj)
    return *this;

  if (len != obj.len)
  {
    delete[] vector;
    vector = new T[obj.len];
  }
  len = obj.len;
  for (int i = 0; i < len; i++)
    vector[i] = obj.vector[i];

  return *this;
}

template<class T>
inline TVector<T>& TVector<T>::operator=(TVector<T>&& obj)
{
  if (this == &obj)
    return *this;

  if (vector != nullptr)
    delete[] vector;

  vector = obj.vector;
  len = obj.len;
  obj.vector = nullptr;
  obj.len = 0;
  return *this;
}

template<class T>
inline bool TVector<T>::operator==(const TVector<T>& obj)
{
  if (len != obj.len)
    return false;

  for (int i = 0; i < len; i++)
  {
    if (vector[i] != obj.vector[i])
      return false;
  }
  return true;
}

template<class T>
inline bool TVector<T>::operator!=(const TVector<T>& obj)
{
  return !(*this == obj);
}

template<class T>
inline T& TVector<T>::operator[](int index)
{
  if (vector == nullptr)
    throw - 1;
  if (index < 0 || index > len)
    throw - 1;

  return vector[index];
}

template<class T>
inline const T& TVector<T>::operator[](int index) const 
{
  if (index >= len) throw std::out_of_range("Index out of range");
  return vector[index];
}

template<class T>
inline void TVector<T>::push_back(const T& value)
{
  T* newVector = new T[len + 1];

  // Копируем существующие элементы
  for (int i = 0; i < len; i++)
    newVector[i] = vector[i];

  // Добавляем новый элемент в конец
  newVector[len] = value;

  // Удаляем старый массив и обновляем указатель
  if (vector != nullptr)
    delete[] vector;
  vector = newVector;
  len++;
}

template<class T>
inline void TVector<T>::push_front(const T& value)
{
  T* newVector = new T[len + 1];

  // Добавляем новый элемент в начало
  newVector[0] = value;

  // Копируем существующие элементы после нового элемента
  for (int i = 0; i < len; i++)
    newVector[i + 1] = vector[i];

  // Удаляем старый массив и обновляем указатель
  if (vector != nullptr)
    delete[] vector;
  vector = newVector;
  len++;
}

template<class T>
inline bool TVector<T>::IsEmpty() const
{
  return len == 0 || vector == nullptr;
}

template<class T>
inline bool TVector<T>::IsFull() const
{
  return false;
}

template<class T>
inline void TVector<T>::ApplyToAll(const std::function<void(T&)>& func)
{
  for (int i = 0; i < len; i++)
  {
    func(vector[i]);
  }
}

template<class T>
inline void TVector<T>::ApplyToAll(const std::function<void(const T&)>& func) const
{
  for (int i = 0; i < len; i++)
  {
    func(vector[i]);
  }
}

template<class T>
inline void TVector<T>::RandomFillSimple(T min, T max)
{
  if (len == 0) return;  // Если вектор пустой, ничего не делаем

  // Инициализация генератора случайных чисел (выполняется один раз)
  static bool initialized = []() 
    {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    return true;
    }();

  // Заполнение вектора случайными числами
  for (int i = 0; i < len; ++i) 
  {
    if constexpr (std::is_integral_v<T>) 
    {  // Для целочисленных типов
      vector[i] = min + std::rand() % (max - min + 1);
    }
    else {  // Для вещественных типов
      vector[i] = min + static_cast<T>(std::rand()) / RAND_MAX * (max - min);
    }
  }
}



template<class T>
inline T TVector<T>::Normalization(T obj)
{
  return SecondNorm(obj);
}

template<class T>
inline T TVector<T>::FirstNorm(T obj)
{
  T sum = 0;
  for (int i = 0; i < len; i++)
    sum += std::abs(vector[i]);
  return sum;
}

template<class T>
inline T TVector<T>::SecondNorm(T obj)
{
  T sum = 0;
  for (int i = 0; i < len; i++)
    sum += vector[i] * vector[i];
  return std::sqrt(sum);
}

template<class T>
inline T TVector<T>::InfinityNorm(T obj)
{
  T max = std::abs(vector[0]);
  for (int i = 1; i < len; i++)
    if (std::abs(vector[i]) > max)
      max = std::abs(vector[i]);
  return max;
}

template<class T>
inline T TVector<T>::HelderNorm(T obj)
{
  T sum = 0;
  for (int i = 0; i < len; i++)
    sum += std::pow(std::abs(vector[i]), obj);
  return std::pow(sum, 1.0 / obj);
}

template<class O>
inline std::ostream& operator<<(std::ostream& o, TVector<O>& v)
{
  for (int i = 0; i < v.GetLen(); i++)
    o << v[i] << " ";
  o << "\n";
  return o;
}

template<class I>
inline std::istream& operator>>(std::istream& i, TVector<I>& v)
{
  int len;
  i >> len;
  v.SetLen(len);

  for (int j = 0; j < len; j++)
    i >> v[j];
}
