#ifndef HEADER
#define HEADER

#include <iostream>
#include <vector> 
#include <math.h>
#include <conio.h>
#include <windows.h>
#include <iomanip>
#include <sstream>
#include <cstring>
#include "ColorCode.h"

#endif

const double P_INFINITY = 1e5;
const double N_INFINITY = -1e5;

using namespace std;

typedef struct matrixParam
{
    vector<vector<float>> matA;
    vector<vector<float>> b;
    vector<vector<float>> matB;
    vector<vector<float>> d;
} matrixParam;

/**************************************************/
vector<vector<float>> MultiplyMatrix(vector<vector<float>>, vector<vector<float>>);
vector<vector<float>> AddMatrix(vector<vector<float>>, vector<vector<float>>);
vector<vector<float>> SubtractMatrix(vector<vector<float>>*, vector<vector<float>>*);
vector<vector<float>> SymmetrixMatrix(vector<vector<float>>*);

/* 
 * Check if the matrix is diagonally dominant
 * @returns
 * 0 if the matrix is not diagonally dominant, 
 * 1 if the matrix is column diagonally dominant, 
 * 2 if the matrix is row diagonally dominant, 
 * 3 if the matrix is row and column diagonally dominant
 */
int DiagonallyDominant(int, vector<vector<float>>*);

// Calculate Euclide norm, column norm and row norm of the matrix
void Norm(vector<vector<float>>*, vector<float>&);

/*
 * Convert the matrix A|b in Ax = b to matrix B and d in x = Bx + d
 * B = I - A
 * d = b
 */
void BuildPhi(matrixParam&);

// Convert float to string with given decimal precision
string FloatToString(float, int);
/**************************************************/

vector<vector<float>> MultiplyMatrix(vector<vector<float>> matA, vector<vector<float>> matB)
{
    vector<vector<float>> ans;
    double sum;

    for (int i = 0; i < matA.size(); i++) {
        ans.push_back({});        

        for (int j = 0; j < matB[0].size(); j++) {
            sum = 0;
            
            for (int k = 0; k < matB.size(); k++) {
                sum += matA[i][k]*matB[k][j];
            }

            ans[i].push_back(sum);
        }
    }

    return ans;
}

vector<vector<float>> AddMatrix(vector<vector<float>> matA, vector<vector<float>> matB)
{
    vector<vector<float>> ans;

    for (int i = 0; i < matA.size(); i++) {
        ans.push_back({});

        for (int j = 0; j < matA[0].size(); j++) {
            ans[i].push_back(matA[i][j] + matB[i][j]);
        }
    }

    return ans;
}

vector<vector<float>> SubtractMatrix(vector<vector<float>>* matA, vector<vector<float>>* matB)
{
    vector<vector<float>> ans;

    for (int i = 0; i < (*matA).size(); i++) {
        ans.push_back({});

        for (int j = 0; j < (*matA)[0].size(); j++) {
            ans[i].push_back((*matA)[i][j] - (*matB)[i][j]);
        }
    }

    return ans;
}

vector<vector<float>> SymmetrixMatrix(vector<vector<float>>* mat)
{
    vector<vector<float>> ans;

    for (int j = 0; j < (*mat)[0].size(); j++) {
        ans.push_back({});

        for (int i = 0; i < (*mat).size(); i++) {
            ans[j].push_back((*mat)[i][j]);
        }
    }

    return ans;
}

int DiagonallyDominant(int size, vector<vector<float>> *mat)
{
    float dEntry = 0;
    double rowSum = 0, colSum = 0;
    bool isRowDD = true, isColDD = true;
    int i = 0;

    // Check row and column diagonally dominant
    while (i < size && (isRowDD || isColDD)) {
        dEntry = abs((*mat)[i][i]);
        rowSum = 0;
        colSum = 0;

        for (int j = 0; j < size; j++) {
            rowSum += abs((*mat)[i][j]);
            colSum += abs((*mat)[j][i]);
        }

        if (dEntry < rowSum - dEntry)
            isRowDD = false;
        if (dEntry < colSum - dEntry)
            isColDD = false;

        i++;
    }

    if      (!isRowDD && !isColDD)  return 0;
    else if (!isRowDD && isColDD)   return 1;
    else if (isRowDD && !isColDD)   return 2;
    else                            return 3;
}

void Norm(vector<vector<float>>* mat, vector<float>& norm)
{
    float ansEuc = 0;
    float ansCol = N_INFINITY, ansRow = N_INFINITY;
    float colSum = 0, rowSum = 0;

    for (int i = 0; i < (*mat).size(); i++) {
        rowSum = 0;

        for (int j = 0; j < (*mat)[0].size(); j++) {
            ansEuc += (*mat)[i][j]*(*mat)[i][j];
            rowSum += abs((*mat)[i][j]);
        }

        if (rowSum > ansRow) ansRow = rowSum;
    }
    for (int j = 0; j < (*mat)[0].size(); j++) {
        colSum = 0;

        for (int i = 0; i < (*mat).size(); i++) {
            colSum += abs((*mat)[i][j]);
        }

        if (colSum > ansCol) ansCol = colSum;
    }

    ansEuc = sqrt(ansEuc);

    norm = {ansEuc, ansCol, ansRow};
}

void BuildPhi(matrixParam& matrices)
{
    int size = matrices.matA.size();

    matrices.matB = matrices.matA;
    matrices.d = matrices.b;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrices.matB[i][j] = -matrices.matB[i][j];
        }
        matrices.matB[i][i] += 1;
        matrices.d[i][0] = matrices.d[i][0];
    }
}

string FloatToString(float num, int decimal)
{
    stringstream stream;
    stream << fixed << setprecision(decimal) << num;

    return stream.str();
}
