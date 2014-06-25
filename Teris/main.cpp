#include <stdio.h>
#include <process.h>
#include <conio.h>
#include <Windows.h>
#include <thread>
#include "Block.h"
#include "Board.h"

bool quit(false);
int row(0);

enum Direction
{
    UP = 72,
    LEFT = 75,
    RIGHT = 77,
    DOWN = 80
};

void initConsole(HANDLE hOut)
{
    //system("chcp 437 > null");
    SetConsoleOutputCP(437);
    SetConsoleTitle("Teris"); // 设置窗口标题

    SMALL_RECT rc = { 0, 0, 41, 23 }; // 重置窗口位置和大小
    SetConsoleWindowInfo(hOut, true, &rc);

    COORD size = { 42, 24 };
    SetConsoleScreenBufferSize(hOut, size);   // 重新设置缓冲区大小

    CONSOLE_SCREEN_BUFFER_INFO binfo;
    GetConsoleScreenBufferInfo(hOut, &binfo);

    SetConsoleCursorPosition(hOut, { 0, 22 });
}

void task(HANDLE handle)
{
    int speed = 1000;
    while (!quit)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(speed));
        keybd_event(VK_DOWN, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
        keybd_event(VK_DOWN, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
    }
}

int main(int argc, char* argv[])
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);

    if (hStdIn == INVALID_HANDLE_VALUE)
        return 1;
    if (hStdOut == INVALID_HANDLE_VALUE) 
        return 1;

    initConsole(hStdOut);

    Board* board = new Board();
    board->setOutput(hStdOut);
    board->start();

    std::thread thr(task, hStdOut);

    INPUT_RECORD inRec;
    DWORD numRead;
    
    while (!quit)
    {
        ReadConsoleInput(hStdIn, &inRec, 1, &numRead);
        if (inRec.EventType == KEY_EVENT && inRec.Event.KeyEvent.bKeyDown)
        {
            switch (inRec.Event.KeyEvent.wVirtualKeyCode)
            {
            case VK_DOWN:
                //system("cls");
                row += board->keyDown();
                //quit = board->show();
                break;
            case VK_UP:
                //system("cls");
                board->keyUp();
                //quit = board->show();
                break;
            case VK_LEFT:
                //system("cls");
                board->keyLeft();
                //quit = board->show();
                break;
            case VK_RIGHT:
                //system("cls");
                board->keyRight();
                //quit = board->show();
                break;
            default:
                quit = true;
                break;
            }
            quit = board->isOver();
        }
    }

    printf("Game Over.\n");

    thr.join();
    getchar();

    CloseHandle(hStdIn);
    CloseHandle(hStdOut);
    return 0;
}

