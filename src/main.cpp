#include <stdio.h>
#include <stddef.h>

extern "C"
{
    #include <SDL2/SDL.h>
}

#include <life.h>

class Game
{
public:
    Game(int, int, int, int);
    ~Game();

    void draw_point(int, int, int, int);
    void draw_line(int, int, int, int);
    void draw_grid();
    void draw_field_border(int, int, int, int);
    void handle_input();
    void process();
    void run();

    SDL_Window *window;
    SDL_Renderer *renderer;

    Life *life;
    int life_window_width, life_window_height;
    int cell_width;
    int cell_height;

    int mx, my;
    int m_life_pane = false;
    int lmclick = false;
    int framerate = 20;
};

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

    // hard code cell size of 5 can be adjusted
    // this->cell_width = this->cell_height = 5;
    // current.w = 800;
    // current.h = 600;
    // this->cell_width = (current.w * this->life_window_xscale) / width;
    // this->cell_height = (current.h * this->life_window_yscale) / height;
    this->cell_width = cell_width;
    this->cell_height = cell_height;
    this->life_window_width = width * this->cell_width;
    this->life_window_height = height * this->cell_height;
    printf("%d, %d\n", this->cell_width, this->cell_height);

    uint32_t flags = SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP;
    this->window = SDL_CreateWindow("Interactive Life Simulation.",
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    current.w, current.h,
                                    flags);

    this->renderer = SDL_CreateRenderer(this->window, -1, 0);
    if(this->renderer == NULL)
    {
        fprintf(stderr, "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        exit(-1);
    }

    this->life = new Life(width, height);

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
    SDL_Event e;
    while(SDL_PollEvent(&e))
    {
        if(e.type == SDL_QUIT)
        {
            exit(0);
        }
        if(e.type == SDL_KEYUP)
        {
            switch(e.key.keysym.sym)
            {
                case SDLK_SPACE:
                    this->life->paused = !this->life->paused;
                    break;
                case SDLK_q:
                    exit(0);
                    break;
            }
        }
        if(e.type == SDL_MOUSEBUTTONDOWN)
        {
            this->lmclick = true;
        }
        if(e.type == SDL_MOUSEBUTTONUP)
        {
            this->lmclick = false;
        }
    }
    SDL_GetMouseState(&(this->mx), &(this->my));
    // dectect if mouse in life field.
    if((this->mx < (this->life->field_width * this->cell_width)) &&
       (this->my < (this->life->field_height * this->cell_height)))
    {
        this->m_life_pane = true;
    }
    else
    {
        this->m_life_pane = false;
    }
}

void Game::draw_point(int x, int y, int width, int height)
{
    static SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = width;
    r.h = height;
    SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(this->renderer, &r);
    // draw border if cells biger then 2x2
    if(width > 2 && height > 2)
    {
        SDL_SetRenderDrawColor(this->renderer, 0x00,0x00, 0x00, 0xFF);
        SDL_RenderDrawRect(this->renderer, &r);
    }
}

void Game::draw_line(int x, int y, int x1, int y1)
{
    SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderDrawLine(this->renderer, x, y, x1, y1);
}

void Game::draw_field_border(int x, int y, int width, int height)
{
    static SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = width;
    r.h = height;
    SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderDrawRect(this->renderer, &r);
}

void Game::draw_grid()
{
    // draws lines to form grid
    for(int x = 0; x < this->life_window_width; x+=this->cell_width)
    {
        this->draw_line(x, 0, x, this->life_window_height);
    }
    for(int y = 0; y < this->life_window_height; y += this->cell_height)
    {
        this->draw_line(0, y, this->life_window_width, y);
    }
}

void Game::process()
{
    this->life->progress();

    // clear background
    SDL_SetRenderDrawColor(this->renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(this->renderer);

    // draw cells for life.
    for(int x = 0; x < this->life->field_width; x++)
    {
        for(int y = 0; y < this->life->field_height; y++)
        {
            if(this->life->field[x][y])
                this->draw_point(x * this->cell_width,
                                 y * this->cell_height,
                                 this->cell_width,
                                 this->cell_height);
        }
    }


    // draw life field border.
    this->draw_field_border(0, 0, this->life_window_width, this->life_window_height);

    // draw grid
    // this->draw_grid();

    // draw square for cell where mouse is over that cell
    if(this->m_life_pane)
    {
        SDL_ShowCursor(SDL_DISABLE);
        int px = this->mx - (this->mx % this->cell_width);
        int py = this->my - (this->my % this->cell_height);
        this->draw_point(px, py, this->cell_width, this->cell_height);
        if(this->lmclick)
        {
            // printf("mclick: %d, %d\n", this->mx / this->cell_width,
            //                            this->my / this->cell_height);
            int lx = this->mx / this->cell_width;
            int ly = this->my / this->cell_height;
            if((lx > 0 && lx < this->life->field_width) &&
               (ly > 0 && ly < this->life->field_height))
            {
                this->life->field[lx][ly] = 1;
            }
        }
    }
    else
    {
        SDL_ShowCursor(SDL_ENABLE);
    }
    
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

int main(void)
{
    Game game = Game(128, 64, 8, 8);
    game.run();

    return 0;
}