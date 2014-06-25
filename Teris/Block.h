#pragma once
#include <memory.h>
class Block
{
public:
    Block(int type)
    {
        memcpy(_meta, Block::_shape[type], sizeof(int)*4*2);
    }
    virtual ~Block()
    {
    }
    virtual void rotateLeft()
    {
        for (size_t i = 0; i < 4; i++)
        {
            int tmp = _meta[i][0];
            _meta[i][0] = _meta[i][1];
            _meta[i][1] = -tmp;
        }
    }
    virtual void rotateRight()
    {
        for (size_t i = 0; i < 4; i++)
        {
            int tmp = _meta[i][0];
            _meta[i][0] = -_meta[i][1];
            _meta[i][1] = tmp;
        }
    }
    virtual void moveDown() { offsetY++; }
    virtual void moveLeft() { offsetX--; }
    virtual void moveRight() { offsetX++; }
    virtual void moveUp() { offsetY--; }
    virtual void getMeta(int i, int& x, int& y)
    {
        x = _meta[i][0] + offsetX;
        y = _meta[i][1] + offsetY;
    }
    virtual void setOffset(int x, int y)
    {
        offsetX = x;
        offsetY = y;
    }
    virtual int getMaxX()
    {
        int max = _meta[0][0];
        for (size_t i = 1; i < 4; i++)
        {
            if (_meta[i][0] > max)
            {
                max = _meta[i][0];
            }
        }
        return max+offsetX;
    }
    virtual int getMinX()
    {
        int min = _meta[0][0];
        for (size_t i = 1; i < 4; i++)
        {
            if (_meta[i][0] < min)
            {
                min = _meta[i][0];
            }
        }
        return min+offsetX;
    }
    virtual int getMaxY()
    {
        int max = _meta[0][1];
        for (size_t i = 1; i < 4; i++)
        {
            if (_meta[i][1] > max)
            {
                max = _meta[i][1];
            }
        }
        return max+offsetY;
    }
    virtual int getMinY()
    {
        int min = _meta[0][1];
        for (size_t i = 1; i < 4; i++)
        {
            if (_meta[i][1] < min)
            {
                min = _meta[i][1];
            }
        }
        return min+offsetY;
    }

protected:
    int _meta[4][2];
    int offsetX;
    int offsetY;
    static int _shape[7][4][2];
};


