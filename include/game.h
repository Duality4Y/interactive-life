#ifndef __GAME_H__
#define __GAME_H__

#include <SDL2/SDL.h>

#include <life.h>
#include <rect.h>
#include <lifepanel.h>

class Game
{
public:
    Game(int, uint8_t);
    ~Game();

    void handle_input();
    void process();
    void draw();
    void run();

    SDL_Window *window;
    SDL_Renderer *renderer;

    LifePanel lpane;
    int fullscreen = true;
    double framerate = 20;
    uint8_t color_mode = 0;
};

#endif
