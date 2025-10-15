#include <iostream>
#include "..//blaslib/TVector.h"
#include "..//blaslib/TCompleteRectangularMatrices.h"
#include "..//blaslib/TDecRecMatrix.h"
#include "..//blaslib/TSqTriangMatrix.h"
#include "..//blaslib/TSquareTapeMatrix.h"
#include <ctime>


#include <chrono>
#include <vector>


// Шаблонная функция для измерения времени выполнения операции
template<typename Func>
double measureTime(Func operation, int iterations = 1000) {
  auto start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < iterations; ++i) {
    operation();
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  return duration.count() / static_cast<double>(iterations);
}

int main() {
  std::cout << "=== СРАВНЕНИЕ ПРОИЗВОДИТЕЛЬНОСТИ КЛАССОВ МАТРИЦ ===\n" << std::endl;

  // Размеры матриц для тестирования
  const int SMALL_SIZE = 10;
  const int MEDIUM_SIZE = 50;
  const int LARGE_SIZE = 100;

  // Тестируем для разных размеров
  std::vector<int> sizes = { SMALL_SIZE, MEDIUM_SIZE, LARGE_SIZE };

  for (int size : sizes) {
    std::cout << "=== РАЗМЕР МАТРИЦ: " << size << "x" << size << " ===" << std::endl;

    // Создаем матрицы для тестирования
    TComplRectMatrix<double> rect1(size, size, 1.5);
    TComplRectMatrix<double> rect2(size, size, 2.5);

    TSquareTapeMatrix<double> tape1(size, 3);
    TSquareTapeMatrix<double> tape2(size, 3);

    TSqTriangMatrix<double> triang1(size, true, 1.5);
    TSqTriangMatrix<double> triang2(size, true, 2.5);

    TDecRecMatrix<double> sparse1(size);
    TDecRecMatrix<double> sparse2(size);

    // Заполняем разреженные матрицы некоторыми значениями
    for (int i = 0; i < size; i += 2) {
      for (int j = 0; j < size; j += 2) {
        sparse1(i, j) = 1.5;
        sparse2(i, j) = 2.5;
      }
    }

    // Тестируем операции сложения
    std::cout << "\n--- СЛОЖЕНИЕ ---" << std::endl;
    double time_rect_add = measureTime([&]() { auto result = rect1 + rect2; });
    double time_tape_add = measureTime([&]() { auto result = tape1 + tape2; });
    double time_triang_add = measureTime([&]() { auto result = triang1 + triang2; });
    double time_sparse_add = measureTime([&]() { auto result = sparse1 + sparse2; });

    std::cout << "Прямоугольные матрицы: " << time_rect_add << " мкс" << std::endl;
    std::cout << "Ленточные матрицы: " << time_tape_add << " мкс" << std::endl;
    std::cout << "Треугольные матрицы: " << time_triang_add << " мкс" << std::endl;
    std::cout << "Разреженные матрицы: " << time_sparse_add << " мкс" << std::endl;

    // Тестируем операции вычитания
    std::cout << "\n--- ВЫЧИТАНИЕ ---" << std::endl;
    double time_rect_sub = measureTime([&]() { auto result = rect1 - rect2; });
    double time_tape_sub = measureTime([&]() { auto result = tape1 - tape2; });
    double time_triang_sub = measureTime([&]() { auto result = triang1 - triang2; });
    double time_sparse_sub = measureTime([&]() { auto result = sparse1 - sparse2; });

    std::cout << "Прямоугольные матрицы: " << time_rect_sub << " мкс" << std::endl;
    std::cout << "Ленточные матрицы: " << time_tape_sub << " мкс" << std::endl;
    std::cout << "Треугольные матрицы: " << time_triang_sub << " мкс" << std::endl;
    std::cout << "Разреженные матрицы: " << time_sparse_sub << " мкс" << std::endl;

    // Тестируем операции умножения на скаляр
    std::cout << "\n--- УМНОЖЕНИЕ НА СКАЛЯР ---" << std::endl;
    double time_rect_scalar = measureTime([&]() { auto result = rect1 * 2.0; });
    double time_tape_scalar = measureTime([&]() { auto result = tape1 * 2.0; });
    double time_triang_scalar = measureTime([&]() { auto result = triang1 * 2.0; });
    double time_sparse_scalar = measureTime([&]() { auto result = sparse1 * 2.0; });

    std::cout << "Прямоугольные матрицы: " << time_rect_scalar << " мкс" << std::endl;
    std::cout << "Ленточные матрицы: " << time_tape_scalar << " мкс" << std::endl;
    std::cout << "Треугольные матрицы: " << time_triang_scalar << " мкс" << std::endl;
    std::cout << "Разреженные матрицы: " << time_sparse_scalar << " мкс" << std::endl;

    // Тестируем операции умножения матриц (только для небольших размеров)
    if (size <= MEDIUM_SIZE) {
      std::cout << "\n--- УМНОЖЕНИЕ МАТРИЦ ---" << std::endl;
      double time_rect_mult = measureTime([&]() { auto result = rect1 * rect2; }, 10);
      double time_tape_mult = measureTime([&]() { auto result = tape1 * tape2; }, 10);
      double time_triang_mult = measureTime([&]() { auto result = triang1 * triang2; }, 10);
      double time_sparse_mult = measureTime([&]() { auto result = sparse1 * sparse2; }, 10);

      std::cout << "Прямоугольные матрицы: " << time_rect_mult << " мкс" << std::endl;
      std::cout << "Ленточные матрицы: " << time_tape_mult << " мкс" << std::endl;
      std::cout << "Треугольные матрицы: " << time_triang_mult << " мкс" << std::endl;
      std::cout << "Разреженные матрицы: " << time_sparse_mult << " мкс" << std::endl;
    }

    // Тестируем доступ к элементам
    /*std::cout << "\n--- ДОСТУП К ЭЛЕМЕНТАМ ---" << std::endl;
    double time_rect_access = measureTime([&]() {
      double sum = 0;
      for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
          sum += rect1(i, j);
        }
      }
      });*/

    double time_tape_access = measureTime([&]() {
      double sum = 0;
      for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
          sum += tape1(i, j);
        }
      }
      });

    double time_triang_access = measureTime([&]() {
      double sum = 0;
      for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
          sum += triang1(i, j);
        }
      }
      });

    double time_sparse_access = measureTime([&]() {
      double sum = 0;
      for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
          sum += sparse1(i, j);
        }
      }
      });

    //std::cout << "Прямоугольные матрицы: " << time_rect_access << " мкс" << std::endl;
    std::cout << "Ленточные матрицы: " << time_tape_access << " мкс" << std::endl;
    std::cout << "Треугольные матрицы: " << time_triang_access << " мкс" << std::endl;
    std::cout << "Разреженные матрицы: " << time_sparse_access << " мкс" << std::endl;

    std::cout << "\n" << std::string(50, '=') << std::endl;
  }
  return 0;
}