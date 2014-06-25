#include <stdio.h>
#include <string>
#include <time.h>
#include <Windows.h>
#include "Board.h"
#include "Block.h"

#define INITX 5
#define INITY 2
#define WAITX 14
#define WAITY 5


unsigned char Board::space[ROW][COL] = { 0 };

Board::Board()
{
    _end = false;
    _current = NULL;
    _score = 0;
    _next = randomBlock();

    // top&bottom boundary
    for (size_t i = 0; i < COL; i++)
    {
        space[0][i] = '*';
        space[ROW-1][i] = '*';
    }
    // left&right boundary, wait area
    for (size_t i = 0; i < ROW; i++)
    {
        space[i][0] = '*';
        space[i][11] = '*';
        space[i][COL-1] = '*';
    }

    unsigned char* ptr;

    ptr = *space;
    ptr += 2 * COL + 12;
    strcpy_s((char*)ptr, 6, "Next:");

    ptr = *space;
    ptr += 10 * COL + 12;
    strcpy_s((char*)ptr, 7, "Score:");

    ptr = *space;
    ptr += 12 * COL + 12;

    char score[6] = { 0 };
    _itoa_s(_score, score, 10);
    strcpy_s((char*)ptr, strlen(score) + 1, score);

}

Board::~Board()
{
    if (_current != NULL)
        delete _current;
    if (_next != NULL)
        delete _next;
}

void Board::start()
{
    refresh();

    _current = _next;
    _current->setOffset(INITX, INITY);
    setupBlock(_current);

    _next = randomBlock();
    _next->setOffset(WAITX, WAITY);
    setupBlock(_next);
}

Block* Board::randomBlock()
{
    LARGE_INTEGER nFrequency;

    if (::QueryPerformanceFrequency(&nFrequency))
    {
        LARGE_INTEGER nStartCounter;
        ::QueryPerformanceCounter(&nStartCounter);
        ::srand((unsigned)nStartCounter.LowPart);
    }
    
    int type = rand() % 7;
    return new Block(type);
}

void Board::activeNextBlock()
{
    if (_current != NULL)
        delete _current;
    
    removeBlock(_next);

    // Active a exist block, 
    // Put block to game area
    _current = _next;
    _current->setOffset(INITX, INITY);
    if (!isValidMove())
    {
        _end = true;
    }
    setupBlock(_current);

    // Ready a next block
    // Put next block to wait area
    _next = randomBlock();
    _next->setOffset(WAITX, WAITY);
    setupBlock(_next);
}

int Board::popRows()
{
    int count = 0;
    int cur_score = _score;
    int row = _current->getMaxY();
    int bonus = 0;
    unsigned char* head = *space + 20;

    while(row >= _current->getMinY())
    {
        unsigned char* ptr = *space + row * COL + 1;
        unsigned char* rowHead = ptr;
        unsigned char* rowTail = ptr + 9;

        int pop = 10;
        for (; ptr <= rowTail ; ptr++)
        {
            if (*ptr == 220)
            {
                pop--;
            }
        }

        // pop this row
        if (0 == pop)
        {
            cur_score += ++bonus;

            if (rowHead == head)
            {
                memset(rowHead, 0, 10);
            }
            else
            {
                unsigned char* prevRowHead = rowHead - COL;
                // Cover this row with the row upstair. 
                while (prevRowHead >= head)
                {
                    memcpy(rowHead, prevRowHead, 10);
                    rowHead = prevRowHead;
                    prevRowHead = rowHead - COL;
                }
            }
        }
        else
        {
            row--;
            count++;
            continue;
        }
    }

    if (_score > 99999)
    {
        _end = true;
    }
    else if (cur_score > _score)
    {
        unsigned char* p = *space;
        p += 12 * COL + 12;

        char score[6] = { 0 };
        _itoa_s(cur_score, score, 10);
        strcpy_s((char*)p, strlen(score) + 1, score);

        _score = cur_score;

        refresh();
    }

    return count;
}

int Board::keyDown()
{
    int n = 0;
    removeBlock(_current);
    _current->moveDown();
    if (isValidMove())
    {
        setupBlock(_current);
    }
    // down move is invalid, mean that block
    // should be fixed in this position
    else
    {
        // rollback previous step
        _current->moveUp();
        setupBlock(_current);

        n = popRows();
        activeNextBlock();
    }
    return n;
}

void Board::keyUp()
{
    removeBlock(_current);
    _current->rotateLeft();
    if (!isValidMove())
    {
        _current->rotateRight();
    }
    setupBlock(_current);
}

void Board::keyLeft()
{
    removeBlock(_current);
    _current->moveLeft();
    if (!isValidMove())
    {
        _current->moveRight();
    }
    setupBlock(_current);
}

void Board::keyRight()
{
    removeBlock(_current);
    _current->moveRight();
    if (!isValidMove())
    {
        _current->moveLeft();
    }
    setupBlock(_current);
}

bool Board::isValidMove()
{
    int x, y;
    for (size_t i = 0; i < 4; i++)
    {
        _current->getMeta(i, x, y);
        if (space[y][x] != 0)
            return false;
    }
    return true;
}

void Board::setupBlock(Block* block)
{
    int x, y;
    for (size_t i = 0; i < 4; i++)
    {
        block->getMeta(i, x, y);
        space[y][x] = 220;
        COORD pos = { 2*x, y };
        DWORD num;
        FillConsoleOutputCharacter(_hOut, space[y][x], 1, pos, &num);
    }
}

void Board::removeBlock(Block* block)
{
    for (size_t i = 0; i < 4; i++)
    {
        int x, y;
        block->getMeta(i, x, y);
        space[y][x] = 0;
        COORD pos = { 2*x, y };
        DWORD num;
        FillConsoleOutputCharacter(_hOut, space[y][x], 1, pos, &num);
    }
}

void Board::refresh()
{
    DWORD num;

    for (size_t i = 0; i < ROW; i++)
    {
        size_t j = 0;
        for (; j < COL; j++)
        {
            char c = space[i][j];
            FillConsoleOutputCharacter(_hOut, c, 1, { 2 * j, i }, &num);
            //printf("%c ", c);
        }
        //FillConsoleOutputCharacter(_hOut, '\n', 1, { 2 * j, i }, &num);
        //printf("\n");
    }
}

