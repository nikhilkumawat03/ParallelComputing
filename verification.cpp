#include "vector"
#include "fstream"
#include "iostream"
#include "utils.h"

using namespace std;

// utility function to read the matrix from file
vector<vector<double>> getMatrix(string fileName) {
  ifstream file;
  file.open(fileName, ios::in);

  if (!file.is_open()) {
    cerr << "Can't open " + fileName + " exiting..." << endl;
    exit(1);
  }

  return read_matrix(file);
}

int main() {
  // get all the 4 matrices needed for verification of  PA = LU
  auto originalMatrix = getMatrix(ORIGINAL_MATRIX_FILE_NAME);
  auto permutationMatrix = getMatrix(PERMUTATION_MATRIX_FILE_NAME);
  auto lowerMatrix = getMatrix(LOWER_MATRIX_FILE_NAME);
  auto upperMatrix = getMatrix(UPPER_MATRIX_FILE_NAME);

  // multiply P, A and L, U to get PA and LU
  auto PA = multiply(permutationMatrix, originalMatrix);
  auto LU = multiply(lowerMatrix, upperMatrix);

  // subtract LU from PA
  auto result = subtract(PA, LU);

  // save the result of verification
  save_matrix(result, VERIFICATION_MATRIX_FILE_NAME);

  return 0;
}