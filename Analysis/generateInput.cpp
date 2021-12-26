#include <iostream>
#include <fstream>

using namespace std;

ofstream inputFile;

const int lowerMatrixSize = 100;
const int upperMatrixSize = 1000;
const int noOfMatrices = 10;
const int incSize = 100;

int main(){
	remove("inputFile");
	inputFile.open("inputFile", ios::app);
	inputFile << noOfMatrices << "\n";
	for (int matrixSize = lowerMatrixSize; matrixSize <= upperMatrixSize; matrixSize += incSize){
		inputFile << matrixSize << "\n";
		for (int i = 0; i < matrixSize; i++){
    		for (int j = 0; j < matrixSize; j++){
        		inputFile <<  rand() % 100 << " ";
        	}
        	inputFile << "\n";
        }
	}
}
