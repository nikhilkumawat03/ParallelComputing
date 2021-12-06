#include <iostream>
#include <bits/stdc++.h>
using namespace std;

void findLU(vector<vector<float>>);

int main() {
    int n, temp;
    cin >> n;
    vector<vector<float>> matrix(n, vector<float>(n, 5));
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            cin >> temp;
            matrix[i][j]=temp;
        }
    }
    findLU(matrix);
}

void findLU(vector<vector<float>> matrix) {
    int n = matrix.size();
    
    //Initialization
    vector<float> p(n, 0);
    vector<vector<float>> u(n, vector<float>(n, 0));
    vector<vector<float>> l(n, vector<float>(n, 0));
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            if(i==j) {
                l[i][j] = 1;
            }
        }
    }
    
    for(int i=0; i<n; i++) {
        p[i] = i;
    }

    for(int k=0; k<n; k++) {
        int maxEle=0;
        int k1;
        for(int i=k; i<n; i++) {
            if(maxEle < (matrix[i][k] < 0 ? matrix[i][k]*-1 : matrix[i][k])) {
                maxEle = ((matrix[i][k] < 0 ? matrix[i][k]*-1 : matrix[i][k]));
                k1 = i;
            }
        }
        if(maxEle==0) {
            cout << "LU can't be calculated.\n";
            return; //error
        }
        swap(p[k], p[k1]);
        matrix[k].swap(matrix[k1]);
        for(int i=0; i<k; i++) {
            swap(l[k][i], l[k1][i]);
        }
        u[k][k] = matrix[k][k];
        
        for(int i=k+1; i<n; i++) {
        	l[i][k] = matrix[i][k]/u[k][k];
            u[k][i] = matrix[k][i];
            for(int j=k+1; j<n; j++) {
                matrix[i][j] -= l[i][k]*u[k][j];
            }
        }
    }
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            cout << u[i][j] << " ";
        }
        cout << endl;
    }
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            cout << l[i][j] << " ";
        }
        cout << endl;
    }
}
