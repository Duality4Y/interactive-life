#ifndef __PATTERN_PANEL_H__
#define __PATTERN_PANEL_H__

#include <SDL2/SDL.h>

#include <life.h>
#include <lifepanel.h>
#include <rect.h>

class PatternPanel: public LifePanel
{
public:
    PatternPanel();
    PatternPanel(rect_t, rect_t);
    virtual void handle_input(SDL_Event);
    virtual void process();
    virtual void draw(SDL_Renderer *);

    int clicked = true;
};

#endif