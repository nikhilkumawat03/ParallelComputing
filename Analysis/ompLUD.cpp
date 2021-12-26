// Author: Nikhil Kumawat, Chandrakishorsingh
// Date: 06/12/2021
// Program: LU Decomposition using Gaussian Elimination


#include <iostream>
#include <bits/stdc++.h>
#include "utils.h"
#include <omp.h>
#include <time.h>
#include <fstream>
#include <sys/time.h>

using namespace std;
struct timeval tstart, tend;
double pexectime, exectime;
int threads = 4;
fstream outputFile;

void findLUOpenMP(vector<vector<double>>& matrix);

int main(int argc, char** argv) {
  // declare matrix and size
  	vector<vector<double>> matrix;
    outputFile.open("openMPStats", ios::out);
    for (int matrixSize = 50; matrixSize <= 500; matrixSize += 50){
    	matrix = generate_random_matrix(matrixSize);
    	
    	gettimeofday( &tstart, NULL );
		findLU(matrix);		//Serial Computation
		gettimeofday( &tend, NULL );
		
		exectime = (tend.tv_sec - tstart.tv_sec) * 1000.0; // sec to ms
  		exectime += (tend.tv_usec - tstart.tv_usec) / 1000.0; // us to ms
		
    	findLUOpenMP(matrix);
    	matrix.clear();
    }
  	
}

void findLUOpenMP (vector<vector<double>>& matrix) {
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
	for (int noOfThreads = 1; noOfThreads <= threads; ++ noOfThreads){
		
		gettimeofday( &tstart, NULL );
		
		omp_set_num_threads(noOfThreads);
				
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
			
			#pragma omp parallel for shared(lowerMatrix, permutationMatrix)
			for(int i = 0; i < col; i++) {
			  swap(lowerMatrix[col][i], lowerMatrix[swapRowNo][i]);
			}

			upperMatrix[col][col] = matrix[col][col];

			int chunk = ((matrixSize - col) / omp_get_max_threads()) + 1;
			#pragma omp parallel shared(matrixSize, matrix, lowerMatrix, upperMatrix, permutationMatrix, col, chunk)
			{
				#pragma omp for schedule(static, chunk)
				for(int i = col+1; i < matrixSize; i++) {
					lowerMatrix[i][col] = matrix[i][col] / upperMatrix[col][col];
					upperMatrix[col][i] = matrix[col][i];
				}

				#pragma omp for schedule(static, chunk)
				for (int i = col + 1; i < matrixSize; ++i){
					for(int j = col+1; j < matrixSize; j++){
				  		matrix[i][j] -= lowerMatrix[i][col] * upperMatrix[col][j];
					}
				}
			}
		}
		
		gettimeofday( &tend, NULL );
		
		pexectime = (tend.tv_sec - tstart.tv_sec) * 1000.0; // sec to ms
  		pexectime += (tend.tv_usec - tstart.tv_usec) / 1000.0; // us to ms
  		
  		string stats = to_string(matrixSize)+", "+ to_string(noOfThreads)+", "+to_string(pexectime/1000.0) + ", "+ to_string(exectime/1000.0) +"\n";
  		outputFile << stats;
  		
	}
	
}
