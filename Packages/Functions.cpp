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

#include "MathFuncs.cpp"

#define MAX_SIZE 10

typedef struct Message
{
    vector<string> showAnswer = {
        "Kết quả từng bước với số lần lặp k = ", 
        "Kết quả từng bước với sai số epsilon = ", 
        "Kết quả từng bước với epsilon = "
    };
    vector<string> diagonallyDominant = {
        "Ma trận không chéo trội!",
        "Ma trận chéo trội cột",
        "Ma trận chéo trội hàng",
        "Ma trận chéo trội hàng và chéo trội cột"
    };
} Message;

// Read the coefficient from file input.txt
int ReadCoefMatrix(int&, matrixParam&);  

// Get input from keyboard
void InputCoefMatrix(int&, matrixParam&);

// Print coefficient matrix in output box
void ShowMatrix(int, matrixParam*, vector<string>&, vector<string>&);

// Save coefficient matrix to file
void SaveMatrix(matrixParam*);

string SetStringColor(string, string);

bool IsNumber(string);

/* 
 * Create 2 file logsClassical.txt and logsSeidel.txt to store answers in each iteration
 * Only create the empty file
 */
void CreateLogs(int, int);

// Append answers from each iteration to the log files
void AppendLogs(int, vector<vector<float>>*, int, int);

// Read answers from log files and store in a 2-D mxn matrix with m is the number of variable x_i and n is the number of iterations
void ReadLogs(int, int, vector<vector<float>>&, int, int);

// This function will convert a matrix to a vector of strings, each element in this vector is a line in output box
void AppendMatrixToOutput(matrixParam*, vector<string>&);

// Similar to the above function, but this will help to print the answer of each method
void AppendAnswerToOutput(vector<vector<float>>*, vector<vector<float>>*, vector<string>&);

void AppendLogsToOutput(int, int, int, vector<vector<float>>*, vector<string>&, int, int);

float ErrorClassical(float, int, vector<vector<float>>*, vector<vector<float>>*);

float ErrorSeidel(int, vector<vector<float>>*, vector<vector<float>>*, vector<vector<float>>*);

/**************************************************/
int precision = 4;
string tableColor = GREEN;
string warningColor = RED;

int ReadCoefMatrix(int& size, matrixParam& matrices)
{
    FILE* fptr;
    float current;
    string fileName;
    int exist = 1;  // = 1 nghĩa là có thể mở được file yêu cầu
                    // = 0 nghĩa là không thể mở được file yêu cầu, có thể mở file input.txt
                    // = -1 nghĩa là không thể mở được file yêu cầu và file input.txt

    cout << "Nhập tên file (file cần phải tồn tại trong thư mục Texts, có thể tạo thêm thư mục con trong đó:\n   ";
    BLACK_GREEN;
    cout << "./Texts/";
    cin >> fileName;
    RESET_COLOR;
    fileName = "Texts/" + fileName;

    // Convert string to char*
    char fileName_char[fileName.size() + 1];
    strcpy(fileName_char, fileName.c_str());

    fptr = fopen(fileName_char, "r");
    if (fptr == NULL) {
        fptr = fopen("Texts/input.txt", "r");
        if (fptr == NULL) return -1;
        else exist = 0;
    }
    matrices.matA.clear();
    matrices.b.clear();

    fscanf(fptr, "%d", &size);
    if (size == 0) return -1;

    for (int i = 0; i < size; i++) {
        matrices.matA.push_back({});
        for (int j = 0; j < size; j++) {
            fscanf(fptr, "%f", &current);
            matrices.matA[i].push_back(current);
        }
        fscanf(fptr, "%f", &current);
        matrices.b.push_back({current});
    }

    fclose(fptr);

    BuildPhi(matrices); // Build matrix B and vector d in x = Bx + d

    return exist;
}

void InputCoefMatrix(int& size, matrixParam& matrices)
{
    float current;
    string size_instr;

    matrices.matA.clear();
    matrices.b.clear();

    cout << "Nhập kích thước của ma trận (ma trận nxn, chỉ cần nhập 1 số tự nhiên n): ";
    cin >> size_instr;
    if (!IsNumber(size_instr)) {
        return;
    }
    else {
        size = stoi(size_instr);
    }

    cout << "   Chỉ nhập các ký tự số và dấu \".\" để biểu diễn số chấm động, nếu nhập sai chương trình sẽ dừng\n"
         << "   Nhập ma trận hệ số mở rộng, các số hạng cách nhau bởi dấu cách hoặc kết thúc dòng (lần lượt từ trái qua phải, trên xuống dưới):\n";

    for (int i = 0; i < size; i++) {
        matrices.matA.push_back({});
        for (int j = 0; j < size; j++) {
            cin >> current;
            matrices.matA[i].push_back(current);
        }
        cin >> current;
        matrices.b.push_back({current});
    }

    BuildPhi(matrices); // Build matrix B and vector d in x = Bx + d
}

void ShowMatrix(int size, matrixParam* matrices, vector<string>& outputMatrix, vector<string>& outputLogs)
{
    if (size == 0) {
        outputLogs.push_back(SetStringColor("Chưa nhập ma trận!", warningColor));
    }
    else if (size <= MAX_SIZE) {
        outputMatrix.push_back("Ma trận đã nhập:");
        AppendMatrixToOutput(matrices, outputMatrix);
    }
    else {
        outputLogs.push_back("Nhập ma trận thành công!");
        outputLogs.push_back("Chương trình chỉ hiện thị ma trận có kích thước không vượt quá " + to_string(MAX_SIZE) + "x" + to_string(MAX_SIZE + 1));
        outputLogs.push_back("Đối với những ma trận có kích thước lớn hơn, có thể xem ở file " + SetStringColor("Texts/input_manual.txt", GREEN));
    }
}

void SaveMatrix(matrixParam* matrices)
{
    FILE* fptr;
    int size = matrices->matA.size();

    fptr = fopen("Texts/input_manual.txt", "w");

    fprintf(fptr, "%d\n", size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            fprintf(fptr, "%f ", matrices->matA[i][j]);
        }
        fprintf(fptr, "%f\n", matrices->b[i][0]);
    }
    
    fclose(fptr);
}

string SetStringColor(string str, string color)
{
    return "\x1b[40;" + color + str + "\x1b[0m";
}

bool IsNumber(string str)
{
    int i = 0;
    int len = str.length();

    while (i < len) {
        if (isdigit(str[i])) i += 1;
        else {
            return false;
        }
    }

    return true;
}

void CreateLogs(int size, int opt)
{
    FILE* fptr;

    string fileNameC = "Texts/logsOpt" + to_string(opt) + "_Classical.txt";
    string fileNameS = "Texts/logsOpt" + to_string(opt) + "_Seidel.txt";

    char fileNameC_char[fileNameC.size() + 1];
    char fileNameS_char[fileNameS.size() + 1];

    strcpy(fileNameC_char, fileNameC.c_str());
    strcpy(fileNameS_char, fileNameS.c_str());
    
    fptr = fopen(fileNameC_char, "w");
    fclose(fptr);
    fptr = fopen(fileNameS_char, "w");
    fclose(fptr);
}

void AppendLogs(int k, vector<vector<float>>* x, int opt, int method)
{
    FILE* fptr;
    int size = (*x).size();

    string fileName = "Texts/logsOpt" + to_string(opt) + "_";
    if (method == 1) fileName += "Classical.txt";
    else if (method == 2) fileName += "Seidel.txt";

    char fileName_char[fileName.size() + 1];
    strcpy(fileName_char, fileName.c_str());

    fptr = fopen(fileName_char, "a");
    
    fprintf(fptr, "%d\t|\t", k);
    // Convert the vector x to a string
    for (int i = 0; i < size; i++) {
        if ((*x)[i][0] < 0) {
            fprintf(fptr, "%f\t", (*x)[i][0]);
        }
        else {
            fprintf(fptr, " %f\t", (*x)[i][0]);
        }
    }
    fprintf(fptr, "\n");

    fclose(fptr);
}

void ReadLogs(int k, int size, vector<vector<float>>& logMatrix, int opt, int method)
{
    FILE* fptr;
    char* unused[10];
    float current;

    string fileName = "Texts/logsOpt" + to_string(opt) + "_";
    if (method == 1) fileName += "Classical.txt";
    else if (method == 2) fileName += "Seidel.txt";

    char fileName_char[fileName.size() + 1];
    strcpy(fileName_char, fileName.c_str());

    fptr = fopen(fileName_char, "r");
    
    logMatrix.clear();
    for (int i = 0; i < size; i++) {
        logMatrix.push_back({});
    }

    for (int i = 0; i <= k; i ++) {
        // Do not get the index and |
        fscanf(fptr, "%s", unused);
        fscanf(fptr, "%s", unused);
        
        // Data in the log file looks like a k*size matrix, but this function will read and store it in a size*k matrix
        // to make it easier to print.
        for (int j = 0; j < size; j++) {
            fscanf(fptr, "%f", &current);
            logMatrix[j].push_back(current);
        }
    }

    fclose(fptr);
}

void AppendMatrixToOutput(matrixParam* matrices, vector<string>& target)
{
    string line = "";
    string curNum;
    vector<vector<float>> mat_A = matrices->matA;
    vector<vector<float>> _b = matrices->b;
    
    for (int i = 0; i < mat_A.size(); i++) {
        for (int j = 0; j < mat_A[0].size(); j++) {
            curNum = FloatToString(mat_A[i][j], precision);

            if (mat_A[i][j] < 0) {
                line += curNum;
            }
            else {
                line += " " + curNum;
            }
            line += " ";
        }

        if (_b[i][0] < 0) {
            line += " |  " + FloatToString(_b[i][0], precision);
        }
        else {
            line += " |   " + FloatToString(_b[i][0], precision);
        }

        target.push_back(line);
        line = "";
    }
}

void AppendAnswerToOutput(vector<vector<float>>* xClassical, vector<vector<float>>* xSeidel, vector<string>& target)
{
    string line = "";
    string curNum;
    
    target.push_back("");
    target.push_back(SetStringColor("    |  Lặp đơn\t|  Lặp Seidel ", tableColor));
    target.push_back(SetStringColor("---------------------------------------", tableColor));

    for (int i = 0; i < (*xClassical).size(); i++) {
        curNum = FloatToString((*xClassical)[i][0], precision);
        line += "x" + to_string(i + 1) + SetStringColor("  |  ", tableColor);
        if ((*xClassical)[i][0] < 0) {
            line += curNum;
        }
        else {
            line += " " + curNum;
        }

        curNum = FloatToString((*xSeidel)[i][0], precision);
        line += SetStringColor("\t|  ", tableColor);
        if ((*xSeidel)[i][0] < 0) {
            line += curNum;
        }
        else {
            line += " " + curNum;
        }

        target.push_back(line);
        line = "";
    }
}

void AppendLogsToOutput(int size, int maxCol, int inPage, vector<vector<float>>* logs, vector<string>& target, int method, int startCol)
{
    string line;

    for (int i = 0; i < size; i++) {
        line = "x" + to_string(i + 1) + SetStringColor("  |  ", tableColor);
        if (inPage*8 <= (*logs)[i].size()) { 
            for (int j = 0; j < maxCol; j++) {
                if (j + inPage*8 < (*logs)[i].size()) {
                    line += "\033[" + to_string(i + 7 + (size + 2)*(method - 1)) + ";" + to_string(startCol + 8 + j*16) + "H";
                    if ((*logs)[i][j + inPage*8] >= 0) line += " ";
                    line += FloatToString((*logs)[i][j + inPage*8], precision) + " ";
                }
            }
        }
        target.push_back(line);
    }
}

float ErrorClassical(float normB, int normUsed, vector<vector<float>>* x_k, vector<vector<float>>* x_k1)
{
    float qcoef = normB/(1 - normB);
    vector<vector<float>> x_sub;
    vector<float> norm = {-1, -1, -1};

    x_sub = SubtractMatrix(x_k, x_k1);
    Norm(&x_sub, norm);

    return qcoef*norm[normUsed];
}

float ErrorSeidel(int normUsed, vector<vector<float>>* B, vector<vector<float>>* x_k, vector<vector<float>>* x_k1)
{
    float qCoef;
    vector<vector<float>> x_sub;
    vector<float> norm = {-1, -1, -1};
    int size = (*x_k).size();

    x_sub = SubtractMatrix(x_k, x_k1);
    Norm(&x_sub, norm);

    if (normUsed == 1) {
        float zeta = 0, S = 0;
        float upSum, downSum;

        for (int j = 0; j < size; j++) {
            upSum = 0;
            downSum = 0;
            for (int i = 0; i < j; i++) {
                upSum += abs((*B)[i][j]);
            }
            for (int i = j; i < size; i++) {
                downSum += abs((*B)[i][j]);
            }

            if (upSum/(1 - downSum) > zeta) zeta = upSum/(1 - downSum);
            if (downSum > S) S = downSum;
        }

        qCoef = zeta/((1 - S)*(1 - zeta));
        return qCoef*norm[normUsed];
    }
    
    else if (normUsed == 2) {
        float lambda = 0;
        float upSum, downSum;

        for (int i = 0; i < size; i++) {
            upSum = 0;
            downSum = 0;
            for (int j = i - 1; j < size; j++) {
                upSum += abs((*B)[i][j]);
            }
            for (int j = 0; j < i - 1; j++) {
                downSum += abs((*B)[i][j]);
            }

            if (upSum/(1 - downSum) > lambda) lambda = upSum/(1 - downSum);
        }

        qCoef = lambda/(1 - lambda);
        return qCoef*norm[normUsed];
    }
}

