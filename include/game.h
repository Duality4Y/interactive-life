#ifndef __GAME_H__
#define __GAME_H__

#include <life.h>
#include <SDL2/SDL.h>

typedef struct
{
    int x;
    int y;
    int w; 
    int h;
} rect_t;

class LifePane
{
public:
    LifePane();
    LifePane(rect_t, rect_t);
    void init(rect_t, rect_t);
    void handle_input(SDL_Event);
    void process();
    void draw_cell(int, int, int);
    void draw(SDL_Renderer *);

    int x, y, width, height;
    
    Life life;
    rect_t field_dims;
    rect_t cell_dims;
};

class Game
{
public:
    Game(int, int, int, int);
    ~Game();

    void draw_point(int, int, int, int);
    void draw_line(int, int, int, int);
    void draw_field_border(int, int, int, int);
    void draw_cell_line(int, int, int, int);
    void handle_input();
    void process();
    void run();

    SDL_Window *window;
    SDL_Renderer *renderer;



    Life life;
    LifePane lpane;
    int life_window_width, life_window_height;
    int cell_width;
    int cell_height;

    int mx = 0, my = 0, pmx = 0, pmy = 0;
    int m_life_pane = false;
    int lmclick = false;
    int framerate = 20;
};

#endif