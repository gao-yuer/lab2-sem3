#include <iostream>
#include "..//matrixlib/TVector.h"
#include "..//matrixlib/TCompleteRectangularMatrices.h"
#include "..//matrixlib/TDecRecMatrix.h"
#include "..//matrixlib/TSqTriangMatrix.h"
#include "..//matrixlib/TSquareTapeMatrix.h"



int main() {
  try {
    TSqTriangMatrix<double> triang(3, true);
    triang(0, 0) = 1.0; triang(0, 1) = 2.0; triang(0, 2) = 3.0;
    triang(1, 1) = 4.0; triang(1, 2) = 5.0;
    triang(2, 2) = 6.0;

    TSquareTapeMatrix<int> tape(4, 2);
    tape(0, 0) = 1; tape(0, 1) = 2;
    tape(1, 0) = 3; tape(1, 1) = 4; tape(1, 2) = 5;
    tape(2, 1) = 6; tape(2, 2) = 7; tape(2, 3) = 8;
    tape(3, 2) = 9; tape(3, 3) = 10;

    TComplRectMatrix<float> rect(2, 3);
    rect[0][0] = 1.1f; rect[0][1] = 2.2f; rect[0][2] = 3.3f;
    rect[1][0] = 4.4f; rect[1][1] = 5.5f; rect[1][2] = 6.6f;

    TDecRecMatrix<int> sparse(3);
    sparse.SetElement(0, 0, 10);
    sparse.SetElement(0, 2, 20);
    sparse.SetElement(1, 1, 30);
    sparse.SetElement(2, 0, 40);
    sparse.SetElement(2, 2, 50);

    std::cout << "1. TRIANGULAR MATRIX:\n" << triang;
    std::cout << "2. TAPE MATRIX:\n" << tape;
    std::cout << "3. RECTANGULAR MATRIX:\n" << rect;
    std::cout << "4. SPARSE MATRIX:\n" << sparse;

    auto scaledRect = rect * 2.0f;
    std::cout << "Rectangular matrix × 2:\n" << scaledRect;

    std::cout << "The number of nonzero elements in a sparse matrix: "
      << sparse.GetNonZeroCount() << "\n";

    std::cout << "Second norm trangl matrix: " << triang.SecondNorm() << "\n";

  }
  catch (const std::exception& err) {
    std::cerr << "Error: " << err.what() << std::endl;
    return 1;
  }

  return 0;
}