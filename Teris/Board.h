#pragma once
#define ROW 22
#define COL 20

class Block;

class Board
{
public:
    Board();
    ~Board();
    void start();
    void refresh();
    int  keyDown();
    void keyUp();
    void keyLeft();
    void keyRight();
    void activeNextBlock();
    Block* randomBlock();
    int popRows();
    void setOutput(void* hOut) { _hOut = hOut; }
    bool isOver() { return _end; }
private:
    void setupBlock(Block* block);
    void removeBlock(Block* block);
    bool isValidMove();
private:
    Block* _current;
    Block* _next;
    bool _end;
    int _score;
    void* _hOut;
    static unsigned char space[ROW][COL];
};

