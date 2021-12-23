//
// Created by chandrakishorsingh on 12/8/21.
//

#include "iostream"
#include <cstdlib>
#include "fstream"
#include "utils.h"

using namespace std;

//save the given matrix in the file
void save_matrix(vector <vector <double>> matrix, string fileName){
	ofstream file;
	file.open(fileName);
	if (!file){
		cerr << "Error: file could not be opened" << endl;
		exit(1);
	}

        int n = matrix.size();
        file << n << endl;

	for (auto &row : matrix){
		for (auto element: row){
			file << element << " ";
		}
		file << "\n";
	}
}

// prints the given matrix to standard output
void print_matrix(vector<vector<double>>& matrix) {
    for (auto& row: matrix) {
        for (auto element: row)
            cout << element << "\t  ";
        cout << endl;
    }

    cout << "\n\n" << flush;
}

// reads the matrix from the file(given as ifstream object) and returns it
vector<vector<double>> read_matrix(ifstream& file) {
    int n;
    file >> n;

    vector<vector<double>> matrix(n, vector<double>(n));
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        file >> matrix[i][j];
      }
    }

    return matrix;
}

// returns a matrix with randomly generated entries
vector<vector<double>> generate_random_matrix(int n) {
    vector<vector<double>> matrix(n, vector<double>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            matrix[i][j] = rand() % 100;
    return matrix;
}

// multiplies two matrices and gives its result
vector<vector<double>> multiply(vector<vector<double>>& mat1, vector<vector<double>>& mat2) {
  int rows = mat1.size();
  int cols = mat2[0].size();

  vector<vector<double>> result(rows, vector<double>(cols));
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      for (int k = 0; k < mat1[0].size(); k++)
        result[i][j] += mat1[i][k] * mat2[k][j];

  return result;
}

// subtracts two matrices and gives its result
vector<vector<double>> subtract(vector<vector<double>>& mat1, vector<vector<double>>& mat2) {
  if (mat1.size() != mat2.size() || mat1[0].size() != mat2[0].size()) {
    cerr << "Can't subtract matrices of different sizes" << endl;
    exit(1);
  }

  int rows = mat1.size();
  int cols = mat1[0].size();

  vector<vector<double>> result(rows, vector<double>(cols));
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      result[i][j] = mat1[i][j] - mat2[i][j];

  return result;
}