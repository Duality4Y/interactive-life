#include "game.h"

Game::Game(int width, int height, int cell_width, int cell_height)
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "SDL could not initialize SDL_Error: %s\n", SDL_GetError());
        exit(-1);
    }

    SDL_DisplayMode current;
    printf("possible configurations: %d\n", SDL_GetNumVideoDisplays());
    if(SDL_GetDisplayMode(0, 0, &current) != 0)
    {
        fprintf(stderr, "couldn't get display mode\n");
        exit(-1);
    }
    
    printf("current info: \n"
           "format: %d\n"
           "width: %d\n"
           "height: %d\n"
           "refresh: %d\n",
           current.format,
           current.w,
           current.h,
           current.refresh_rate
           );

    uint32_t flags = SDL_WINDOW_SHOWN;
    if(this->fullscreen)
    {
        flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }
    else
    {
        // when not going fullscreen i'll hardcode size to this,
        // for debugging purposes.
        current.w = 800;
        current.h = 600;
    }

    this->window = SDL_CreateWindow("Interactive Life Simulation.",
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    current.w, current.h,
                                    flags);

    // create a sdl renderer to draw on.
    this->renderer = SDL_CreateRenderer(this->window, -1, 0);
    if(this->renderer == NULL)
    {
        fprintf(stderr, "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        exit(-1);
    }

    rect_t cell_dims = {0, 0, cell_width, cell_height};
    rect_t field_dims = {0, 0, width, height};
    this->lpane = LifePanel(field_dims, cell_dims);
}

Game::~Game()
{
    if(this->renderer != NULL)
    {
        SDL_DestroyRenderer(this->renderer);
        this->renderer = NULL;
    }
    if(this->window != NULL)
    {
        SDL_DestroyWindow(this->window);
        this->window = NULL;
    }
    SDL_Quit();
}

void Game::handle_input()
{
    static SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        this->lpane.handle_input(event);
        if(event.type == SDL_QUIT)
        {
            exit(0);
        }
        if(event.type == SDL_KEYUP)
        {
            switch(event.key.keysym.sym)
            {
                case SDLK_SPACE:
                    this->pause = !this->pause;
                    break;
                case SDLK_q:
                    exit(0);
                    break;
            }
        }
    }
}

void Game::process()
{
    if(!this->pause)
    {
        this->lpane.process();
    }
    
    // clear background
    SDL_SetRenderDrawColor(this->renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(this->renderer);

    // draw everything
    this->lpane.draw(this->renderer);
    
    // draw to window.
    SDL_RenderPresent(this->renderer);
}

// enters a infinite loop.
void Game::run()
{
    while(1)
    {
        this->handle_input();
        this->process();
        // SDL_Delay(1000 / this->framerate);
    }
}
