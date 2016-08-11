#include "life.h"

Life::Life(size_t width, size_t height)
{
    this->field_width = width;
    this->field_height = height;

    this->field = new bool *[this->field_width];
    for(size_t x = 0; x < this->field_width; x++)
        this->field[x] = new bool[this->field_height];
    
    this->buffer = new bool *[this->field_width];
    for(size_t x = 0; x < this->field_width; x++)
        this->buffer[x] = new bool[this->field_height];


}

Life::~Life()
{
    // delete field
    for(size_t x = 0; x < this->field_width; x++)
        delete field[x];
    delete field;
    
    // delete buffer
    for(size_t x = 0; x < this->field_width; x++)
        delete buffer[x];
    delete buffer;
}

void Life::progress()
{

}

size_t Life::totalAround(size_t x, size_t y)
{
    return (this->checkUpper(x, y) +
            this->checkLower(x, y) +
            this->checkLeft(x, y) +
            this->checkRight(x, y) +
            this->checkUpperLeft(x, y) +
            this->checkUpperRight(x, y) +
            this->checkLowerLeft(x, y) +
            this->checkLowerRight(x, y));
}

bool Life::checkUpper(size_t x, size_t y)
{
    return this->field[x][(y + 1) % this->field_height];
}

bool Life::checkLower(size_t x, size_t y)
{
    return this->field[x][(y - 1) % this->field_height];
}

bool Life::checkLeft(size_t x, size_t y)
{
    return this->field[(x - 1) % this->field_width][y];
}

bool Life::checkRight(size_t x, size_t y)
{
    return this->field[(x + 1) % this->field_width][y];
}

bool Life::checkUpperLeft(size_t x, size_t y)
{
    return this->field[(x - 1) % this->field_width][(y + 1) % this->field_height];
}

bool Life::checkUpperRight(size_t x, size_t y)
{
    return this->field[(x + 1) % this->field_width][(y + 1) % this->field_height];
}

bool Life::checkLowerLeft(size_t x, size_t y)
{
    return this->field[(x - 1) % this->field_width][(y - 1) % this->field_height];
}

bool Life::checkLowerRight(size_t x, size_t y)
{
    return this->field[(x + 1) % this->field_width][(y - 1) % this->field_height];
}

