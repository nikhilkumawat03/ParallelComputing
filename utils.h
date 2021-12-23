//
// Created by chandrakishorsingh on 12/8/21.
//

#include <vector>

using namespace std;

// constants used in multiple files
const string ORIGINAL_MATRIX_FILE_NAME = "originalMatrix";
const string PERMUTATION_MATRIX_FILE_NAME = "permutationMatrix";
const string LOWER_MATRIX_FILE_NAME = "lowerMatrix";
const string UPPER_MATRIX_FILE_NAME = "upperMatrix";
const string VERIFICATION_MATRIX_FILE_NAME = "verificationMatrix";

// save the matrix in the file
void save_matrix(vector <vector <double>>, string);

// prints the matrix entries
void print_matrix(vector<vector<double>>& matrix);

// reads the matrix entries in the file and returns a Matrix
vector<vector<double>> read_matrix(ifstream& file);

// returns a matrix with randomly generated entries
vector<vector<double>> generate_random_matrix(int n);

// multiplies two matrices and gives its result
vector<vector<double>> multiply(vector<vector<double>>& mat1, vector<vector<double>>& mat2);

// subtracts two matrices and gives its result
vector<vector<double>> subtract(vector<vector<double>>& mat1, vector<vector<double>>& mat2);
