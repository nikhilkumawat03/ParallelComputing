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

// To get the LU decompositions
void findLU(vector<vector<double>>& matrix) {
    
    int matrixSize = matrix.size();
	// declare permutation, lower and upper matrices
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
}
