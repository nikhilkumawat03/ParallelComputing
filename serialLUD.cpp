// Author: Nikhil Kumawat, chandrakishorsingh
// Date: 06/12/2021
// Program: LU Decomposition using Gaussian Elimination


#include <iostream>
#include <bits/stdc++.h>
#include "utils.h"

using namespace std;

pair<vector<vector<double>>, vector<vector<double>>> findLU(vector<vector<double>>& matrix);

int main(int argc, char** argv) {
    // declare matrix and size
    vector<vector<double>> matrix;
    int matrixSize;

    // get the matrix and size
    if (argc == 3) {
        // expected arguments are `-n` and `size`
        // generates a matrix of order`size` with random values
        matrixSize = atoi(argv[2]);
        matrix = generate_random_matrix(matrixSize);
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
        matrixSize = matrix.size();
    } else {
        // exit if program is executed in none of the above format
        cout << "Either provide size of the matrix or the entire matrix from an input file." << endl;
        exit(1);
    }

    // print the matrix and upper and lower triangular matrices
    save_matrix(matrix, ORIGINAL_MATRIX_FILE_NAME);
	
    auto result = findLU(matrix);
    save_matrix(result.first, LOWER_MATRIX_FILE_NAME);
    save_matrix(result.second, UPPER_MATRIX_FILE_NAME);
}

pair<vector<vector<double>>, vector<vector<double>>> findLU
(vector<vector<double>>& matrix) {
    // declare permutation, lower and upper matrices
    int matrixSize = matrix.size();
    vector<vector<double>> permutationMatrix(matrixSize, vector <double>(matrixSize));
    vector<vector<double>> upperMatrix(matrixSize, vector<double>(matrixSize));
    vector<vector<double>> lowerMatrix(matrixSize, vector<double>(matrixSize));

    // initialize lower triangular matrix, and permutation matrix
    for (int i = 0; i < matrixSize; i++){
        lowerMatrix[i][i] = 1;
      	permutationMatrix[i][i] = 1;
     }

    for(int col = 0; col < matrixSize; col++) {
        double maxEle = 0;
        int swapRowNo = col;
        for(int i = col; i < matrixSize; i++) {
            if(maxEle < abs(matrix[i][col])) {
                maxEle = abs(matrix[i][col]);
                swapRowNo = i;
            }
        }
        if(maxEle == 0) {
            cout << "Given matrix is singular. LU decomposition can't be calculated.\n";
            exit(1);
        }
        
        swap(permutationMatrix[col], permutationMatrix[swapRowNo]);
		swap(matrix[col], matrix[swapRowNo]);
        
        for(int i = 0; i < col; i++) {
            swap(lowerMatrix[col][i], lowerMatrix[swapRowNo][i]);
        }
        
        upperMatrix[col][col] = matrix[col][col];
        
        for(int i = col+1; i < matrixSize; i++) {
            lowerMatrix[i][col] = matrix[i][col] / upperMatrix[col][col];
            upperMatrix[col][i] = matrix[col][i];
        }
        for (int i = col + 1; i < matrixSize; ++i){
            for(int j = col+1; j < matrixSize; j++) {
                matrix[i][j] -= lowerMatrix[i][col] * upperMatrix[col][j];
            }
        }
    }
	save_matrix(permutationMatrix, PERMUTATION_MATRIX_FILE_NAME);
    // return the upper and lower triangular matrices
    return { lowerMatrix, upperMatrix };
}
