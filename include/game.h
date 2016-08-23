#ifndef __GAME_H__
#define __GAME_H__

#include <SDL2/SDL.h>

#include <life.h>
#include <rect.h>
#include <lifepanel.h>
#include <patternpanel.h>

class Game
{
public:
    Game(int, int, int, int);
    ~Game();

    void handle_input();
    void process();
    void draw();
    void run();

    SDL_Window *window;
    SDL_Renderer *renderer;

    LifePanel lpanel;
    PatternPanel glider_panel;
    int fullscreen = true;
    double framerate = 30;
};

#endif