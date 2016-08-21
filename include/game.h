#ifndef __GAME_H__
#define __GAME_H__

#include <SDL2/SDL.h>

#include <life.h>
#include <rect.h>
#include <lifepanel.h>

class Game
{
public:
    Game(int, int, int, int);
    ~Game();

    void handle_input();
    void process();
    void run();

    SDL_Window *window;
    SDL_Renderer *renderer;

    LifePanel lpane;
    int pause = false;
    int fullscreen = true;
};

#endif