#define HEADER

#include <iostream>
#include <vector> 
#include <math.h>
#include <conio.h>
#include <windows.h>
#include <iomanip>
#include <sstream>
#include <cstring>
#include "Packages/ColorCode.h"

#include "Packages/Functions.cpp"
#include "Packages/Console.cpp"

void EditDecimalPrecision();
void CheckDiagonallyDominant(int, vector<vector<float>>*, vector<string>&);
void CalculateNorms(int, vector<vector<float>>*, vector<string>&);
void Option678(int, vector<string>&);
void Options678Guide(int, int, int&);
void Classical(vector<vector<float>>&, float&, int);
void Seidel(vector<vector<float>>&, float&, int);

int k, k_Classical, k_Seidel;
float e;
int size = 0;
matrixParam matrices;               // Store matrices used in these methods: A, B, b and d
int normType;
vector<vector<float>> x_Classical;  // Answer using Classical iteration
vector<vector<float>> x_Seidel;     // Answer using Seidel iteration

Message message;

int main()
{
    FormatConsole();
    DrawBorder();

    vector<vector<float>> logs;     // Tổng hợp kết quả của từng bước lặp
    
    vector<float> norm {-1, -1, -1};
    int choice = 1, state = 0;
    char buttonPressed;
    vector<string> outputLines;
    vector<string> currentMatrixInStrings;
    vector<string> outputLogs;
    int logsPage = 0;
    int inputFile;

    while (state != -1) {
        ClearInputBox(); // Clear input box

        switch (state) {
            case 0:
                ClearMenu();
                DrawMenu(choice);
                SetDecimalPrecision(precision, true);
                ResetInputCursor(); // Move to input box

                buttonPressed = getch();
                switch (buttonPressed) {
                    case 72: // up arrow
                        choice -= 1;
                        if (choice < 1) choice = 9;
                        break;
                    case 80: // down arrow
                        choice += 1;
                        if (choice > 9) choice = 1;
                        break;
                    case 75: // left arrow
                        break;
                    case 77: // right arrow
                        break;
                    case 13: // enter
                        if (choice == 9) state = -1;
                        else state = 1;
                        break;
                    case 9: // tab
                        EditDecimalPrecision();

                        ClearOutputBox();
                        ClearInputBox();

                        break;
                    default:
                        break;
                }
                break;
            
            case 1:
                SetDecimalPrecision(precision, false);
                ResetInputCursor();
                outputLines.clear();
                outputLogs.clear();
                currentMatrixInStrings.clear();

                switch (choice) {
                    case 1:
                        inputFile = ReadCoefMatrix(size, matrices);
                        if (inputFile == -1) {
                            outputLogs.push_back(SetStringColor("Không tìm thấy file hoặc file không chứa ma trận!", warningColor));
                            outputLogs.push_back(SetStringColor("Không tồn tại file input.txt hoặc file không chứa ma trận!", warningColor));
                            DrawOutputBox(&outputLogs);
                            break;
                        }
                        else if (inputFile == 0) {
                            outputLogs.push_back(SetStringColor("Không tìm thấy file hoặc file không chứa ma trận!", warningColor));
                            outputLogs.push_back(SetStringColor("Chương trình sẽ tự động đọc từ file input.txt", warningColor));
                            outputLogs.push_back(SetStringColor("Chọn lựa chọn in ma trận đang xét để xem ma trận đọc từ file input.txt!", tableColor));
                            DrawOutputBox(&outputLogs);
                            break;
                        }
                        SaveMatrix(&matrices);
                        ShowMatrix(size, &matrices, currentMatrixInStrings, outputLogs);
                        if (size <= MAX_SIZE) DrawOutputBox(&currentMatrixInStrings);
                        else DrawOutputBox(&outputLogs);

                        break;
                    case 2:                        
                        ClearInputBox();
                        InputCoefMatrix(size, matrices);
                        SaveMatrix(&matrices);

                        system("cls");
                        DrawBorder();
                        ShowMatrix(size, &matrices, currentMatrixInStrings, outputLogs);
                        if (size <= MAX_SIZE) DrawOutputBox(&currentMatrixInStrings);
                        else DrawOutputBox(&outputLogs);

                        break;
                    case 3:
                        ShowMatrix(size, &matrices, currentMatrixInStrings, outputLogs);
                        if (size > 0 && size <= MAX_SIZE) DrawOutputBox(&currentMatrixInStrings);
                        else DrawOutputBox(&outputLogs);

                        break;
                    case 4:
                        CheckDiagonallyDominant(size, &matrices.matA, outputLines);
                        DrawOutputBox(&outputLines);

                        break;
                    case 5:
                        CalculateNorms(size, &matrices.matA, outputLines);
                        DrawOutputBox(&outputLines);

                        break;
                    case 6:                        
                        Option678(6, outputLines);
                        DrawOutputBox(&outputLines);
                        Options678Guide(size, normType, state);

                        break;
                    case 7:
                        Option678(7, outputLines);
                        DrawOutputBox(&outputLines);                       
                        Options678Guide(size, normType, state);

                        break;
                    case 8:
                        Option678(8, outputLines);
                        DrawOutputBox(&outputLines);                        
                        Options678Guide(size, normType, state);

                        break;
                }

                if (state != 2) state = 0;
                break;
            
            case 2:
                SetDecimalPrecision(precision, false);
                ClearOutputBox();

                string logLine = "";
                int maxCol = 8;

                outputLines.clear();
                if (choice == 6) {
                    outputLines.push_back(message.showAnswer[choice - 6] + to_string(k) + ":");
                    k_Classical = k;
                    k_Seidel = k;
                }
                else if (choice == 7 || choice == 8) {
                    outputLines.push_back(message.showAnswer[choice - 6] + to_string(e) + ":");
                    if (k_Classical > k_Seidel) k = k_Classical;
                    else k = k_Seidel;
                }
                if (logsPage == k/8) maxCol = k%8 + 1;

                // k index line
                logLine = "k";
                for (int i = 0; i < maxCol; i++) {
                    logLine += "\033[5;" + to_string(OUTPUT_START_COLUMN + 8 + i*16) + "H " + to_string(i + logsPage*8) + "\t";
                }
                outputLines.push_back(logLine);

                // Append answer of Classical iteration method to output
                outputLines.push_back(SetStringColor("[Classical]", tableColor));
                ReadLogs(k_Classical, size, logs, choice, 1);
                AppendLogsToOutput(size, maxCol, logsPage, &logs, outputLines, 1, OUTPUT_START_COLUMN);
                outputLines.push_back("");

                // Append answer of Seidel iteration method to output
                outputLines.push_back(SetStringColor("[Seidel]", tableColor));
                ReadLogs(k_Seidel, size, logs, choice, 2);
                AppendLogsToOutput(size, maxCol, logsPage, &logs, outputLines, 2, OUTPUT_START_COLUMN);

                // Page number
                logLine = "\033[" + to_string(2*size + 10) + ";" + to_string(OUTPUT_START_COLUMN + 120) + "H";
                logLine += SetStringColor("< ", tableColor) + to_string(logsPage + 1) + SetStringColor("/" + to_string(k/8 + 1) + " >", tableColor);
                outputLines.push_back(logLine);

                // Draw table
                DrawOutputBox(&outputLines);
                MoveCursorTo(6, OUTPUT_START_COLUMN + 11);
                DrawLineOfText("-", 125, tableColor);
                MoveCursorTo(8 + size, OUTPUT_START_COLUMN + 8);
                DrawLineOfText("-", 128, tableColor);
                MoveCursorTo(9 + 2*size, OUTPUT_START_COLUMN);
                DrawLineOfText("-", 136, tableColor);

                ResetInputCursor();
                cout << "Nhấn các phím mũi tên trái và phải để chuyển trang!\n"
                     << "   Nhấn phím Tab để quay lại Menu!";
                
                buttonPressed = ' ';
                while (buttonPressed != 75 && buttonPressed != 77 && buttonPressed != 9) {
                    buttonPressed = getch();
                    if (buttonPressed == 75) {
                        logsPage -= 1;
                        if (logsPage < 0) logsPage = k/8;
                    }
                    else if (buttonPressed == 77) {
                        logsPage += 1;
                        if (logsPage > k/8) logsPage = 0;
                    }
                    else if (buttonPressed == 9) {
                        ClearInputBox();
                        state = 0;
                        logsPage = 0;
                    }
                }

                break;
        } 
    }
        
    return 0;
}

/******************************** Functions ********************************/

void EditDecimalPrecision()
{
    string precision_instr;

    cout << "Nhập số chữ số thập phân cần hiển thị (tối đa 6 chữ số): ";
    cin >> precision_instr;

    bool isNumber = IsNumber(precision_instr);

    if (!isNumber) {
        ClearInputBox();
        cout << "Vui lòng nhập vào 1 chữ số!\n"
             << "   Nhập số chữ số thập phân cần hiển thị (tối đa 6 chữ số): ";
        EditDecimalPrecision();
    }
    else {
        precision = stoi(precision_instr);
        if (precision > 6) {
            ClearInputBox();
            cout << "Để đảm bảo khả năng hiển thị của chương trình, số chữ số thập phân cần hiển thị không được vượt quá 6.\n"
                 << "   Nhập số chữ số thập phân cần hiển thị (tối đa 6 chữ số): ";
            EditDecimalPrecision();
        }
    }
}

void CheckDiagonallyDominant(int size, vector<vector<float>>* mat, vector<string>& lines)
{
    if ((*mat).size() == 0) 
        lines.push_back(SetStringColor("Chưa nhập ma trận!", warningColor));
    else {
        lines.push_back(message.diagonallyDominant[DiagonallyDominant(size, mat)]);
    }
}

void CalculateNorms(int size, vector<vector<float>>* mat, vector<string>& lines)
{
    if ((*mat).size() == 0) {
        lines.push_back(SetStringColor("Chưa nhập ma trận!", warningColor));
        return;
    }
    vector<float> ans;

    Norm(mat, ans);

    lines.push_back("Ta sẽ tính chuẩn cột, chuẩn hàng và chuẩn Euclide, tuy nhiên các thuật toán trong chương trình");
    lines.push_back("sẽ chỉ sử dụng chuẩn cột và chuẩn hàng:");
    lines.push_back("");
    lines.push_back("- Chuẩn cột:\t ||A||_1  = " + FloatToString(ans[1], precision));
    lines.push_back("- Chuẩn hàng:\t ||A||_oo = " + FloatToString(ans[2], precision));
    lines.push_back("- Chuẩn Euclide:\t ||A||_2  = " + FloatToString(ans[0], precision));
}

/*
 * There are many similar things to do in options 6, 7 and 8 and this function will manage these 3 options. 
 * First we create 2 matrix B and d in x = Bx + d. 
 * Then we calculate column norm and row norm of matrix B to check if these method can be used. 
 * If yes, solve the problems and append answer to output.
 */
void Option678(int opt, vector<string>& output)
{
    string curLine;
    float errorC, errorS;

    CreateLogs(size, opt);

    if (size == 0) {
        output.push_back(SetStringColor("Chưa nhập ma trận!", warningColor));
        DrawOutputBox(&output);
        return;
    }
    
    // Nhập vào k hoặc e tương ứng với lựa chọn
    switch (opt) {
        case 6:
            cout << "Nhập số lần lặp theo yêu cầu: k = ";
            cin >> k;
            break;
        case 7:
            cout << "Nhập sai số theo yêu cầu: epsilon = ";
            cin >> e;
            break;
        case 8:
            cout << "Nhập sai số theo yêu cầu: epsilon = ";
            cin >> e;
            break;
    }
    Classical(x_Classical, errorC, opt);
    Seidel(x_Seidel, errorS, opt);
    
    output.push_back("Ta biến đổi về dạng x = Bx + d với B và d xác định như sau:");
    for (int i = 0; i < size; i++) {
        curLine = "";
        if (i == size/2) curLine += "B = ";
        else curLine += "    ";
        
        for (int j = 0; j < size; j++) {
            if (matrices.matB[i][j] < 0) curLine += FloatToString(matrices.matB[i][j], precision) + " ";
            else curLine += " " + FloatToString(matrices.matB[i][j], precision) + " ";
        }

        if (i == size/2) curLine += ",  d = " + FloatToString(matrices.d[i][0], precision);
        else curLine += "       " + FloatToString(matrices.d[i][0], precision);

        output.push_back(curLine);
    }
    
    output.push_back("");
    
    if (normType == -1) {
        output.push_back(SetStringColor("Không tồn tại chuẩn thỏa mãn điều kiện lặp!", warningColor));
    }
    else if (normType == 1) {
        if (opt == 6) 
            output.push_back("Nghiệm gần đúng với số lần lặp k = " + to_string(k) + " theo chuẩn cột:");
        else if (opt == 7 || opt == 8)
            output.push_back("Nghiệm gần đúng với sai số epsilon = " + to_string(e) + " theo chuẩn cột:");
    }
    else if (normType == 2) {
        if (opt == 6) 
            output.push_back("Nghiệm gần đúng với số lần lặp k = " + to_string(k) + " theo chuẩn hàng:");
        else if (opt == 7 || opt == 8)
            output.push_back("Nghiệm gần đúng với sai số epsilon = " + to_string(e) + " theo chuẩn hàng:");
    }
    
    AppendAnswerToOutput(&x_Classical, &x_Seidel, output);

    if (opt == 6 && normType != -1) {
        output.push_back("");
        output.push_back("Sai số theo công thức hậu nghiệm của kết quả thu được:");
        output.push_back("\x1b[40;32mLặp đơn:\x1b[0m\t\t||X_k - X*|| < " + FloatToString(errorC, precision));
        output.push_back("\x1b[40;32mLặp Seidel:\x1b[0m\t||X_k - X*|| < " + FloatToString(errorS, precision));
    }
}

void Options678Guide(int size, int normUsed, int& state)
{
    char pressed;

    if (size > 0 && normUsed != -1) {
        pressed = ' ';
        while (pressed != 9 && pressed != 13) {
            ClearInputBox();
            if (size <= MAX_SIZE) {
                cout << "Có thể in kết quả các bước!\n"
                     << "   Nhấn Enter để in hoặc Tab để quay lại Menu!";
            }
            else {
                cout << "Không thể in kết quả các bước do số phương trình lớn hơn " + to_string(MAX_SIZE) + "\n"
                     << "   Có thể xem chi tiết ở các file logs trong thư mục Texts\n"
                     << "   Nhấn phím Tab để quay lại Menu!";    
            }
            pressed = getch();
            if (pressed == 13) state = 2; // Print logs
        }
    }
}

void Classical(vector<vector<float>>& x, float& error, int opt)
{
    vector<float> norm = {-1, -1, -1};
    vector<vector<float>> lastX;

    // Initialize vector x
    x.clear();
    k_Classical = 0;
    for (int i = 0; i < size; i++) {
        x.push_back({0});
    }
    AppendLogs(0, &x, opt, 1);

    // Check norm
    Norm(&(matrices.matB), norm);
    if (norm[1] < 1) normType = 1;
    else {
        if (norm[2] < 1) normType = 2;
        else normType = -1;
    }

    // Classical iteration
    if (normType == 1 || normType == 2) {
        bool condition;

        do {
            lastX = x;
            x = AddMatrix(MultiplyMatrix(matrices.matB, x), matrices.d);

            k_Classical += 1;
            AppendLogs(k_Classical, &x, opt, 1);

            switch (opt) {
                case 6:
                    condition = k_Classical < k;
                    break;
                case 7:
                    condition = ErrorClassical(norm[normType], normType, &x, &lastX) > e;
                    break;
                case 8:
                    lastX = SubtractMatrix(&x, &lastX);
                    Norm(&lastX, norm);
                    condition = norm[normType] > e;
            }
        } while (condition);

        error = ErrorClassical(norm[normType], normType, &x, &lastX);
    }
}

void Seidel(vector<vector<float>>& x, float& error, int opt)
{
    vector<float> norm = {-1, -1, -1};
    vector<vector<float>> lastX;
    double curSum;

    //Initialize vector x
    x.clear();
    k_Seidel = 0;
    for (int i = 0; i < size; i++) {
        x.push_back({0});
    }
    AppendLogs(0, &x, opt, 2);

    // Check norm
    Norm(&(matrices.matB), norm);
    if (norm[1] < 1) normType = 1;
    else {
        if (norm[2] < 1) normType = 2;
        else normType = -1;
    }
            
    // Seidel iteration
    if (normType == 1 || normType == 2) {
        bool condition;

        do {
            lastX = x;

            for (int r = 0; r < size; r++) {
                curSum = 0;
                for (int c = 0; c < size; c++) {
                    curSum += matrices.matB[r][c]*x[c][0];
                }
                x[r][0] = curSum + matrices.d[r][0];
            }

            k_Seidel += 1;
            AppendLogs(k_Seidel, &x, opt, 2);

            switch (opt) {
                case 6:
                    condition = k_Seidel < k;
                    break;
                case 7:
                    condition = ErrorSeidel(normType, &(matrices.matB), &x, &lastX) > e;
                    break;
                case 8:
                    lastX = SubtractMatrix(&x, &lastX);
                    Norm(&lastX, norm);
                    condition = norm[normType] > e;
            }
        } while (condition);

        error = ErrorSeidel(normType, &(matrices.matB), &x, &lastX);
    }
}
