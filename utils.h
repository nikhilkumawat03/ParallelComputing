//
// Created by chandrakishorsingh on 12/8/21.
//

#include <vector>

using namespace std;

// save the matrix in the file
void save_matrix(vector <vector <double>>, string);

// prints the matrix entries
void print_matrix(vector<vector<double>>& matrix);

// reads the matrix entries in the file and returns a Matrix
vector<vector<double>> read_matrix(ifstream& file);

// returns a matrix with randomly generated entries
vector<vector<double>> generate_random_matrix(int n);
