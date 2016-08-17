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
    Game(int, int);
    ~Game();

    void draw_point(int, int, int, int);
    void draw_line(int, int, int, int);
    void draw_field_border(int, int, int, int);
    void handle_input();
    void process();
    void run();

    Life *life;
    int life_window_width, life_window_height;
    double life_window_xscale = (3.0 / 4.0);
    double life_window_yscale = (3.0 / 4.0);
    double cell_width;
    double cell_height;
    // is true if mouse on window pane.
    // else false
    int m_life_pane = false;

    SDL_Window *window;
    SDL_Renderer *renderer;
    int mx, my;
    int framerate = 20;
};

Game::Game(int width, int height)
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "SDL could not initialize SDL_Error: %s\n", SDL_GetError());
        exit(-1);
    }

    static SDL_DisplayMode current;
    if(SDL_GetDisplayMode(0, 0, &current) != 0)
    {
        fprintf(stderr, "couldn't get display mode\n");
        exit(-1);
    }

    // hard code cell size of 5 can be adjusted
    // this->cell_width = this->cell_height = 5;
    current.w = 800;
    current.h = 600;
    this->cell_width = (current.w * this->life_window_xscale) / width;
    this->cell_height = (current.h * this->life_window_yscale) / height;
    this->life_window_width = width * this->cell_width;
    this->life_window_height = height * this->cell_height;
    printf("%d, %d\n", (int)this->cell_width, (int)this->cell_height);

    uint32_t flags = SDL_WINDOW_SHOWN ;//| SDL_WINDOW_FULLSCREEN_DESKTOP;
    this->window = SDL_CreateWindow("Interactive Life Simulation.",
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    current.w, current.h,
                                    flags);

    this->renderer = SDL_CreateRenderer(this->window, 1, 0);
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
    if(this->m_life_pane)
    {
        printf("mouse in life pane: %d, %d         \r", this->mx, this->my);
    }
}

void Game::draw_point(int x, int y, int width, int height)
{
    static SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = width;
    r.h = height;
    SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0x00);
    SDL_RenderFillRect(this->renderer, &r);
    // draw border if cells biger then 2x2
    if(width > 2 && height > 2)
    {
        SDL_SetRenderDrawColor(this->renderer, 0x00,0x00, 0x00, 0x00);
        SDL_RenderDrawRect(this->renderer, &r);
    }
}

void Game::draw_field_border(int x, int y, int width, int height)
{
    static SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = width;
    r.h = height;
    SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0x00);
    SDL_RenderDrawRect(this->renderer, &r);
}

void Game::draw_line(int x, int y, int x1, int y1)
{
    SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0x00);
    SDL_RenderDrawLine(this->renderer, x, y, x1, y1);
}

void Game::process()
{
    this->life->progress();

    // clear background
    SDL_SetRenderDrawColor(this->renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(this->renderer);

    // draw cells for life.
    for(int x = 0; x < this->life->field_width; x++)
    {
        for(int y = 0; y < this->life->field_height; y++)
        {
            if(this->life->field[x][y])
                this->draw_point(x * this->cell_width, y * this->cell_height, this->cell_width, this->cell_height);
        }
    }


    // draw life field border.
    this->draw_field_border(0, 0, this->life_window_width, this->life_window_height);

    // draw grid
    this->draw_line(0, 0, this->life_window_width, this->life_window_height);
    for(int x = 0; x < this->life_window_width; x+=this->cell_width)
    {
        //this->draw_line(0, 0, this->life_window_width, this->life_window_height);
    }
    for(int y = 0; y < this->life->field_height; y++)
    {
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
    Game game = Game(128, 64);
    game.run();

    return 0;
}