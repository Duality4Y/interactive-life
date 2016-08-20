#ifndef __LIFE_H__
#define __LIFE_H__

#include <stdint.h>

// used type for field en indexes.
typedef int32_t** field_t;
typedef int32_t cell_t;

// a 2D field of points
// with a check functions for the game of life.
class Life
{
public:
    Life();
    Life(cell_t, cell_t);
    ~Life();

    void init(cell_t, cell_t);
    void progress();
    void clear();
    void swapBuffers();

    // function that check the state of cells around a cell.
    cell_t totalAround(cell_t, cell_t);

    void set_cell(cell_t, cell_t, cell_t);
    cell_t get_cell(cell_t, cell_t);
    void draw_replicator(cell_t, cell_t);
    void draw_glider(cell_t, cell_t);

    cell_t field_width, field_height;
    // field holds current state to be displayed.
    field_t field;
    // buffer holds changes.
    field_t buffer;

    static const int surviveAbility = 2;
    static const int reproductiveNumber = 3;

};

#endif