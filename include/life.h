#ifndef __LIFE_H__
#define __LIFE_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

class Life
{
public:
    Life(size_t, size_t);
    ~Life();
    void progress();
    void swapBuffers();

    // function that check the state of cells around a cell.
    size_t totalAround(size_t, size_t);
    bool checkUpper(size_t, size_t);
    bool checkLower(size_t, size_t);
    bool checkLeft(size_t, size_t);
    bool checkRight(size_t, size_t);
    bool checkUpperLeft(size_t, size_t);
    bool checkUpperRight(size_t, size_t);
    bool checkLowerLeft(size_t, size_t);
    bool checkLowerRight(size_t, size_t);

    size_t field_width, field_height;
    // field holds current state to be displayed.
    bool **field;
    // buffer holds changes.
    bool **buffer;
};

#endif