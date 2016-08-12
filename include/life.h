#ifndef __LIFE_H__
#define __LIFE_H__

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <vector>

using std::vector;

// this is for action you may want to take uppon cell positions.
// gets passed grid position and state of cell at position.

class Life
{
public:
    Life(int, int);
    ~Life();
    void progress();
    void swapBuffers();

    // function that check the state of cells around a cell.
    int totalAround(int, int);

    void draw_glider(int, int);

    int field_width, field_height;
    // field holds current state to be displayed.
    // vector< vector<int>> field;
    int **field;
    // buffer holds changes.
    // vector< vector<int>> buffer;
    int **buffer;

    static const int surviveAbility = 2;
    static const int reproductiveNumber = 3;

};

#endif