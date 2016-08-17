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

#include <lifepatterns.h>

typedef int** field_t;
typedef int cell_t;

using std::vector;

// this is for action you may want to take uppon cell positions.
// gets passed grid position and state of cell at position.

class Life
{
public:
    Life(cell_t, cell_t);
    ~Life();
    void progress();
    void clear();
    void swapBuffers();

    // function that check the state of cells around a cell.
    cell_t totalAround(cell_t, cell_t);

    void draw_cell(cell_t, cell_t, cell_t);
    void draw_replicator(cell_t, cell_t);
    void draw_glider(cell_t, cell_t);

    cell_t field_width, field_height;
    // field holds current state to be displayed.
    field_t field;
    // buffer holds changes.
    field_t buffer;

    int paused = false;

    static const int surviveAbility = 2;
    static const int reproductiveNumber = 3;

};

#endif