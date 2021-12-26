// Author: Nikhil Kumawat
// Date: 06/12/2021
// Program: LU Decomposition using Gaussian Elimination

#include <iostream>
#include <pthread.h>
#include <fstream>
#include <climits>
#include <vector>
#include <cmath>
#include "utils.h"
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

using namespace std;

typedef struct arrguments{
	int threadNo;
	int col;
}arg;

typedef struct swapping{
	int col;
	int start;
	int end;
	int swapRowNo;
}swapp;

struct timeval tstart, tend;
double pexectime, exectime;

fstream outputFile;

vector <vector <double>> matrix;
vector <vector <double>> lowerMatrix;
vector <vector <double>> upperMatrix;
vector <vector <double>> saveMatrix;
vector <vector <double>> permutationMatrix;
int matrixSize = 0;		//size of matrix.
int threadCount = 0;	//Number of threads.
int threads = 0;
int **rowSwap;

void *getLowerUpperMatrix(void *tmp){
	arg *args = (arg*)tmp;
	int col = args->col + 1;
	int totalRows = matrixSize - col;	//get total rows
	int rowsPerThread = totalRows / threadCount;
	if (rowsPerThread == 0)
		rowsPerThread = totalRows;
	int start = col + rowsPerThread*(args->threadNo), end = start + rowsPerThread - 1;
	
	//Condition for last thread allocation
	if (args->threadNo == threadCount-1 or end >= matrixSize)
		end = matrixSize-1;
		
	//cout << args->threadNo << " " << start << " " << end << endl;		//Debug
	for (int i = start; i <= end;++i){		//parallalize
		for (int j = col; j < matrixSize; ++j){
			matrix[i][j] -= lowerMatrix[i][col-1]*upperMatrix[col-1][j];
		}
	}
	pthread_exit(NULL);
}

void *getLowerUpperRow(void *argument){
	arg *args = (arg*)argument;
	int col = args->col+1;
	int totalRows = matrixSize - col;	//get total rows
	int rowsPerThread = totalRows / threadCount;
	if (rowsPerThread == 0)
		rowsPerThread = totalRows;
	int start = col + rowsPerThread*(args->threadNo), end = start + rowsPerThread - 1;
	
	//Condition for last thread allocation
	if (args->threadNo == threadCount-1 or end >= matrixSize)
		end = matrixSize-1;
		
	//cout << args->threadNo << " " << start << " " << end << endl;		//Debug
	for(int i = start; i <= end; i++) {	//This can be parallalize
    	lowerMatrix[i][col-1] = matrix[i][col-1] / upperMatrix[col-1][col-1];
        upperMatrix[col-1][i] = matrix[col-1][i];
	}
	pthread_exit(NULL);
}

void *parallelSwap(void *argument){
	swapp *args = (swapp*)argument;
	int start = args->start;
	int end = args->end;
	int swapRowNo = args->swapRowNo;
	int col = args->col;
	for(int i = start; i <= end; i++) {			//This can be parallalize
    	swap(lowerMatrix[col][i], lowerMatrix[swapRowNo][i]);
    }
    pthread_exit(NULL);
}
void *getMaxPivotPos(void *argument){
	
	arg *args = (arg*)argument;
	int col = args->col;
	int totalCols = matrixSize - col;	//get total rows
	int rowsPerThread = totalCols / threadCount;
	if (rowsPerThread == 0)
		rowsPerThread = totalCols;
	int start = col + rowsPerThread*(args->threadNo), end = start + rowsPerThread - 1;
	int swapRowNo = col;
	double maxPivot = 0;
	//Condition for last thread allocation
	if (args->threadNo == threadCount-1 or end >= matrixSize)
		end = matrixSize-1;
		
	//cout << args->threadNo << " " << start << " " << end << endl;		//Debug
	for (int i = start; i <= end; ++i){		//This can be parallalize.
		if (abs(matrix[i][col]) > maxPivot){
			maxPivot = abs(matrix[i][col]);
			swapRowNo = i;
		}
	}
	rowSwap[args->threadNo][0] = swapRowNo;
	rowSwap[args->threadNo][1] = maxPivot;
	pthread_exit(NULL);
}

void luDecomposition(){
	pthread_t t[threads];
	
	for (int noOfThreads = 1; noOfThreads <= threads; noOfThreads+=1){
		threadCount = noOfThreads;
		
		
		for (int i = 0; i < matrixSize; ++i){
			for (int j = 0; j < matrixSize; ++j){
				matrix[i][j] = saveMatrix[i][j];
			}
		}
		
		for (int i = 0; i < matrixSize; ++i){	//O(n)
			lowerMatrix[i][i] = 1;
			permutationMatrix[i][i] = 1;
		}
		
		gettimeofday( &tstart, NULL );
		for (int col = 0; col < matrixSize; ++col){
			double maxPivot = 0;
			int swapRowNo = col;
			
			//Get the requirment of threads.
			int remainingRows = matrixSize - (col + 1);
			while (remainingRows < threadCount)
				threadCount = threadCount / 2;
			
			//*******************************************************************************
			//Paralalising finding max. using getMaxPivotPos() function
			/*for (int i = col; i < matrixSize; ++i){		//This can be parallalize.
				if (abs(matrix[i][col]) > maxPivot){
					maxPivot = abs(matrix[i][col]);
					swapRowNo = i;
				}
			}*/
			/*print_matrix(matrix);*/
			for (int threadNo = 0; threadNo < threadCount; ++threadNo){
				arg *args = new arg();
				args->threadNo = threadNo;
				args->col = col;
				pthread_create(t+threadNo, NULL, &getMaxPivotPos, (void*)args);
			}
			
			for (int i = 0; i < threadCount; ++i){
				pthread_join(t[i], NULL);
				if (maxPivot < rowSwap[i][1]){
					maxPivot = rowSwap[i][1];
					swapRowNo = rowSwap[i][0];
				}
			}
			maxPivot = abs(matrix[swapRowNo][col]);
			if(maxPivot == 0) {
			    cout << "Given matrix is singular. LU decomposition can't be calculated.\n";
			    exit(1);
			}
			//**********************************************************************************
			
			swap(permutationMatrix[col], permutationMatrix[swapRowNo]);		//O(1)
			swap(matrix[col], matrix[swapRowNo]);							//O(1)
			
			//*********************************************************************************
			/*for(int i = 0; i < col; i++) {			//This can be parallalize
			    swap(lowerMatrix[col][i], lowerMatrix[swapRowNo][i]);
			}*/
			// Parallaising swapping using parallelSwap() function
			int cntThread = 0;
			for (int threadNo = 0; threadNo < threadCount; ++threadNo){
				swapp *args = new swapp();
				args->swapRowNo = swapRowNo;
				args->col = col;
				if (threadCount > col){
					args->start = 0;
					args->end = col-1;
				}
				else{
					args->start = threadNo*(col/threadCount);
					args->end = args->start + col/threadCount - 1;
					if (threadNo == threadCount-1)
						args->end = col-1;
				}
				pthread_create(t+threadNo, NULL, &parallelSwap, (void*)args);
				if (args->end == col-1){
					cntThread = threadNo;
					break;
				}
			} 
		   	for (int i = 0; i <= cntThread; ++i)
					pthread_join(t[i], NULL);
			//*********************************************************************************
			
			
			upperMatrix[col][col] = matrix[col][col];
			
			
			//*********************************************************************************
			// Getting lower upper column/row using getLowerUpperRow(void *arguments) function
			/*for(int i = col+1; i < matrixSize; i++) {	//This can be parallalize
				lowerMatrix[i][col] = matrix[i][col] / upperMatrix[col][col];
				upperMatrix[col][i] = matrix[col][i];
			}*/
			for (int threadNo = 0; threadNo < threadCount; ++threadNo){
				arg *args = new arg();
				args->threadNo = threadNo;
				args->col = col;
				pthread_create(t+threadNo, NULL, &getLowerUpperRow, (void*)args);
			}
			
			for (int i = 0; i < threadCount; ++i)
				pthread_join(t[i], NULL);
			//*********************************************************************************
			
			
			
			for (int threadNo = 0; threadNo < threadCount; ++threadNo){
				arg *args = new arg();
				args->threadNo = threadNo;
				args->col = col;
				pthread_create(t+threadNo, NULL, &getLowerUpperMatrix, (void*)args);
			}
			
			for (int i = 0; i < threadCount; ++i)
				pthread_join(t[i], NULL);
		}
		gettimeofday( &tend, NULL );
		
		pexectime = (tend.tv_sec - tstart.tv_sec) * 1000.0; // sec to ms
  		pexectime += (tend.tv_usec - tstart.tv_usec) / 1000.0; // us to ms
  		
  		string stats = to_string(matrixSize)+", "+ to_string(noOfThreads)+", "+to_string(pexectime/1000.0) + ", "+ to_string(exectime/1000.0) +"\n";
  		cout << stats;
  		outputFile << stats;
  		
  		/*if (noOfThreads == 1){
  			save_matrix(lowerMatrix, "lowerMatrix");
			save_matrix(upperMatrix, "upperMatrix");
			save_matrix(permutationMatrix, "permutationMatrix");
  		}*/
  		
		//cout << "MatrixSize: " << matrixSize << " ThreadCount: " << noOfThreads << " Time" << endl;
		
	}
}

int main(){
	ifstream inputFile;
	inputFile.open("inputFile", ios::in);
	outputFile.open("stats", ios::out);
	
	cout << "Enter maximum number of threads for Analysis: (1-4)" << endl;
	cin >> threads;
	rowSwap = new int*[threads];
	for (int i = 0; i < threads; ++i){
		rowSwap[i] = new int[2];
	}
	int noOfMatrices = 0;
	inputFile >> noOfMatrices;
	
    for (int itr = 0; itr < noOfMatrices; itr++){
    	inputFile >> matrixSize; 
    	matrix.resize(matrixSize, vector <double> (matrixSize, 0));
		upperMatrix.resize(matrixSize, vector <double> (matrixSize, 0));
		lowerMatrix.resize(matrixSize, vector <double> (matrixSize, 0));
		saveMatrix.resize(matrixSize, vector <double> (matrixSize, 0));
		permutationMatrix.resize(matrixSize, vector <double> (matrixSize, 0));
		
		for (int i = 0; i < matrixSize; ++i){
			for (int j=  0; j < matrixSize; ++j){
				inputFile >> matrix[i][j];
				saveMatrix[i][j] = matrix[i][j];
			}
		}
		//print_matrix(saveMatrix);
		//save_matrix(saveMatrix, "originalMatrix");
		gettimeofday( &tstart, NULL );
		
		findLU(matrix);		//Serial Computation
		
		gettimeofday( &tend, NULL );
		
		exectime = (tend.tv_sec - tstart.tv_sec) * 1000.0; // sec to ms
  		exectime += (tend.tv_usec - tstart.tv_usec) / 1000.0; // us to ms
  		cout << "*************" << endl;
		luDecomposition();
		
		matrix.clear();
		
		saveMatrix.clear();
		upperMatrix.clear();
		lowerMatrix.clear();
		permutationMatrix.clear();
	}
	outputFile.close();
	
}
