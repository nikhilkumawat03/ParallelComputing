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
