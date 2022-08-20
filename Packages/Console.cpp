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

#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004

#define CONSOLE_WIDTH       185
#define CONSOLE_HEIGHT      50
#define INPUT_START_LINE    36
#define INPUT_START_COLUMN  4
#define OUTPUT_START_COLUMN 46

using namespace std;

/**************************************************/
void FormatConsole();
void SetConsoleSize(SHORT, SHORT);
void SetConsoleBufferSize(SHORT, SHORT);
void DisableResizeConsole();
void DisableButton(bool, bool, bool);
void ShowScrollBar(BOOL);
void EnableANSIEscapeSequences();

void MoveCursorTo(int, int);
void DrawLineOfText(string, int, string);
void DrawBorder();
void DrawMenu(int);
void SetDecimalPrecision(int, bool);
void DrawOutputBox(vector<string>*);
void DrawInputBox(string);
void ClearMenu();
void ClearOutputBox();
void ClearInputBox();
void ResetInputCursor();
/**************************************************/

string options[9] {
    "1.  Nhập ma trận A|B từ file (nên dùng)",
    "2.  Nhập ma trận A|B từ bàn phím",
    "3.  In ma trận đang xét",
    "4.  Kiểm tra tính chéo trội",
    "5.  Tính chuẩn của ma trận A",
    "6.  Giải HPT với số lần lặp cho trước",
    "7.  Giải HPT với sai số cho trước",
    "8.  ||x_(k+1) - x_k|| <= e cho trước",
    "9.  Thoát"
};

void FormatConsole()
{
    EnableANSIEscapeSequences();
    
    DisableResizeConsole();
    DisableButton(false, false, false);
    SetConsoleOutputCP(65001);
    SetConsoleTitle(TEXT("Giai gan dung he phuong trinh dai so tuyen tinh"));
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);

    //SetConsoleMode(hStdin, ~ENABLE_QUICK_EDIT_MODE);
    //ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
    SetConsoleBufferSize(200, 50);
    SetConsoleSize(CONSOLE_WIDTH, 45);
    //ShowScrollBar(0);
}

void SetConsoleSize(SHORT width, SHORT height)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    SMALL_RECT windowSize;
    windowSize.Top = 0;
    windowSize.Left = 0;
    windowSize.Right = width;
    windowSize.Bottom = height;
 
    SetConsoleWindowInfo(hStdout, 1, &windowSize);
}

void SetConsoleBufferSize(SHORT width, SHORT height)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD newSize;
    newSize.X = width;
    newSize.Y = height;

    SetConsoleScreenBufferSize(hStdout, newSize);
}

void DisableResizeConsole()
{
    HWND hWnd = GetConsoleWindow();
    SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_SIZEBOX);
}

void DisableButton(bool close, bool min, bool max)
{
    HWND hWnd = GetConsoleWindow();
    HMENU hMenu = GetSystemMenu(hWnd, false);
    
    if (close == 1)
    {
        DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
    }
    if (min == 1)
    {
        DeleteMenu(hMenu, SC_RESTORE, MF_BYCOMMAND);
    }
    if (max == 1)
    {
        DeleteMenu(hMenu, SC_MAXIMIZE, MF_BYCOMMAND);
    }
}

void ShowScrollBar(BOOL show)
{
    HWND hWnd = GetConsoleWindow();
    ShowScrollBar(hWnd, SB_BOTH, show);
}

void EnableANSIEscapeSequences()
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD consoleMode;
    GetConsoleMode(console, &consoleMode);
    consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(console, consoleMode); 

    //SMALL_RECT windowSize;
    //SetConsoleSize(260, 260);
    //ShowScrollBar(GetConsoleWindow(), SB_BOTH, false);
    //SetConsoleBufferSize(windowSize.Right - windowSize.Left + 100, windowSize.Bottom - windowSize.Top);
}

void MoveCursorTo(int line, int col)
{
    cout << "\033[" + to_string(line) + ";" + to_string(col) + "H";
}

void DrawLineOfText(string text, int repeat, string color)
{   
    cout << "\x1b[40;" + color;
    for (int i = 0; i < repeat; i++) {
        cout << text;
    }
    RESET_COLOR;
}

void DrawBorder()
{
    for (int i = 0; i < CONSOLE_WIDTH; i ++) {
        cout << "=";
    }
    MoveCursorTo(2, INPUT_START_COLUMN - 3);
    cout << "|| MENU                                    " << "|" << " OUTPUT";
    MoveCursorTo(2, CONSOLE_WIDTH - 1);
    cout << "||\n";
    for (int i = 0; i < CONSOLE_WIDTH; i ++) {
        cout << "=";
    }
    
    for (int i = 4; i < INPUT_START_LINE - 1; i++) {
        MoveCursorTo(i, 1);
        cout << "||";
        MoveCursorTo(i, OUTPUT_START_COLUMN - 2);
        cout << "|";
        MoveCursorTo(i, CONSOLE_WIDTH - 1);
        cout << "||";
    }

    MoveCursorTo(INPUT_START_LINE - 1, 1);
    for (int i = 0; i < CONSOLE_WIDTH; i ++) {
        cout << "=";
    }
}

void DrawMenu(int currentChoice)
{  
    for (int i = 1; i <= 9; i++) {
        MoveCursorTo(i + 3, INPUT_START_COLUMN);
        if (i == currentChoice) {
            WHITE_BLACK;
                cout << "                                       ";
                MoveCursorTo(i + 3, INPUT_START_COLUMN);
                cout << options[i-1];
            RESET_COLOR;
        }
        else {
            cout << options[i-1];
        }
    }
}

void SetDecimalPrecision(int precision, bool editable)
{
    MoveCursorTo(INPUT_START_LINE - 4, INPUT_START_COLUMN);

    BLACK_GREEN;
    cout << "Kết quả hiện thị " + to_string(precision) + " chữ số thập phân!";

    MoveCursorTo(INPUT_START_LINE - 3, INPUT_START_COLUMN);
    if (editable) {
        cout << "Nhấn phím Tab để thay đổi";
    }
    else {
        cout << "                                   ";
    }
    RESET_COLOR;
}

/*
 * This function will write text in output box. 
 * Data will be converted to lines, it needs to be calculated first that how much letters will be printed in one line. 
 * First empty the output box, then print each line in lines continuously
 @param vector<string>* lines
 */
void DrawOutputBox(vector<string>* lines)
{
    int lineNum = (*lines).size();

    ClearOutputBox();

    if (lineNum <= INPUT_START_LINE - 5) {
        for (int i = 0; i < lineNum; i++) {
            MoveCursorTo(4 + i, OUTPUT_START_COLUMN);
            cout << (*lines)[i];
        }
    } 
    else {
        MoveCursorTo(4, OUTPUT_START_COLUMN);
        cout << "Kết quả cần in quá lớn, hãy chọn ma trận có kích thước bé hơn!";
    }
}

void DrawInputBox(string text)
{
    ClearInputBox();
    ResetInputCursor();

    cout << text;
}

void ClearMenu()
{
    for (int i = 4; i < INPUT_START_LINE - 1; i++) {
        MoveCursorTo(i, INPUT_START_COLUMN - 1);
        cout << "                                        ";
    }
}

void ClearOutputBox()
{
    for (int i = 4; i < INPUT_START_LINE - 1; i++) {
        MoveCursorTo(i, OUTPUT_START_COLUMN - 1);
        for (int j = OUTPUT_START_COLUMN; j < CONSOLE_WIDTH; j++) {
            cout << " ";
            //cout << "o";
        }
    }
    ResetInputCursor();
}

void ClearInputBox()
{    
    for (int i = INPUT_START_LINE; i < CONSOLE_HEIGHT; i++) {
        MoveCursorTo(i, 1);
        cout << "\033[K";
    }
    
    ResetInputCursor();
}

void ResetInputCursor()
{
    MoveCursorTo(INPUT_START_LINE, INPUT_START_COLUMN);
}
