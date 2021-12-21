//
// Created by chandrakishorsingh on 12/21/21.
//

#include <iostream>
#include <bits/stdc++.h>
#include <omp.h>
#include "utils.h"

using namespace std;

pair<vector<vector<double>>, vector<vector<double>>> findLU(vector<vector<double>>& matrix);

int main(int argc, char** argv) {
  // declare matrix and size
  vector<vector<double>> matrix;
  int size;

  // get the matrix and size
  if (argc == 3) {
    // expected arguments are `-n` and `size`
    // generates a matrix of order`size` with random values
    size = atoi(argv[2]);
    matrix = generate_random_matrix(size);
  } else if (argc == 2) {
    // expected argument is `fileName`
    // first line contains the size of matrix and further lines contains the entries of matrix
    ifstream file;
    file.open(argv[1], ios::in);
    if (!file.is_open()) {
      cout << "Can't open the file. Exiting..." << endl;
      exit(1);
    }

    matrix = read_matrix(file);
    size = matrix.size();
  } else {
    // exit if program is executed in none of the above format
    cout << "Either provide size of the matrix or the entire matrix from an input file." << endl;
    exit(1);
  }

  // print the matrix and upper and lower triangular matrices
  cout << "Matrix A is as follows" << endl;
  print_matrix(matrix);

  auto result = findLU(matrix);
  cout << "Lower triangular matrix is as follows" << endl;
  print_matrix(result.first);

  cout << "Upper triangular matrix is as follows" << endl;
  print_matrix(result.second);
}

pair<vector<vector<double>>, vector<vector<double>>> findLU
    (vector<vector<double>>& matrix) {
  // declare permutation, lower and upper matrices
  int size = matrix.size();
  vector<int> permutation(size);
  vector<vector<double>> upper(size, vector<double>(size));
  vector<vector<double>> lower(size, vector<double>(size));

  // initialize lower triangular matrix
  for (int i = 0; i < size; i++)
    lower[i][i] = 1;

  // initialize permutation matrix
  for (int i = 0; i < size; i++)
    permutation[i] = i;

  for(int k = 0; k < size; k++) {
    double maxEle = 0;
    int swapRowNo = k;
    for(int i = k; i < size; i++) {
      if(maxEle < abs(matrix[i][k])) {
        maxEle = abs(matrix[i][k]);
        swapRowNo = i;
      }
    }
    if(maxEle == 0) {
      cout << "Given matrix is singular. LU decomposition can't be calculated.\n";
      exit(1);
    }

    swap(permutation[k], permutation[swapRowNo]);
    matrix[k].swap(matrix[swapRowNo]);
    for(int i = 0; i < k; i++) {
      swap(lower[k][i], lower[swapRowNo][i]);
    }
    upper[k][k] = matrix[k][k];

    int chunk = ((size - k) / omp_get_max_threads()) + 1;
    #pragma omp parallel shared(size, matrix, lower, upper, permutation, k, chunk)
    {
      #pragma omp for schedule(static, chunk)
      for(int i = k+1; i < size; i++) {
        lower[i][k] = matrix[i][k] / upper[k][k];
        upper[k][i] = matrix[k][i];
      }

      #pragma omp for schedule(static, chunk)
      for (int i = k + 1; i < size; ++i){
        for(int j=k+1; j<size; j++) {
          matrix[i][j] -= lower[i][k] * upper[k][j];
        }
      }
    }
    print_matrix(matrix);
    cout << "------------------------------" << endl;
  }

  // return the upper and lower triangular matrices
  return { lower, upper };
}
