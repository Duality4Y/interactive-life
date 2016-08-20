#ifndef __LIFEPANE_H__
#define __LIFEPANE_H__

#include <SDL2/SDL.h>

#include <life.h>
#include <rect.h>

class LifePane
{
public:
    LifePane();
    LifePane(rect_t, rect_t);
    void init(rect_t, rect_t);
    void handle_input(SDL_Event);
    void process();

    void draw(SDL_Renderer *);
    void draw_point(SDL_Renderer *, rect_t);
    void draw_line(SDL_Renderer *, rect_t);
    void draw_field_border(SDL_Renderer *, rect_t);
    void draw_cell(int, int, int);
    void draw_cell_line(SDL_Renderer *, rect_t);

    int x, y, width, height;
    
    Life life;
    rect_t field_dims;
    rect_t cell_dims;
};

#endif