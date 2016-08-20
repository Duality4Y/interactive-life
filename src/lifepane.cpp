#include "lifepane.h"



LifePane::LifePane()
{
    rect_t cell_dims = {0, 0, 1, 1};
    rect_t field_dims = {1, 1, 1, 1};
    this->init(field_dims, cell_dims);
}

LifePane::LifePane(rect_t field_dims, rect_t cell_dims)
{
    this->init(field_dims, cell_dims);
}

void LifePane::init(rect_t field_dims, rect_t cell_dims)
{
    this->field_dims = field_dims;
    this->cell_dims = cell_dims;
    this->life = Life(this->field_dims.w, this->field_dims.h);
    this->life.draw_glider(1, 1);
}

void LifePane::handle_input(SDL_Event event)
{
    (void)event;
}

void LifePane::process()
{
    this->life.progress();
}

void LifePane::draw_cell(int x, int y, int state)
{
    static int rx, ry;
    rx = x / this->cell_dims.w;
    ry = y / this->cell_dims.h;

    this->life.set_cell(rx, ry, state);
}

void LifePane::draw_point(SDL_Renderer *renderer, rect_t rect)
{
    static SDL_Rect r;
    r.x = rect.x;
    r.y = rect.y;
    r.w = rect.w;
    r.h = rect.h;
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(renderer, &r);
    // draw border if cells biger then 2x2
    if(rect.w > 2 && rect.h > 2)
    {
        SDL_SetRenderDrawColor(renderer, 0x00,0x00, 0x00, 0xFF);
        SDL_RenderDrawRect(renderer, &r);
    }
}

void LifePane::draw_field_border(SDL_Renderer *renderer, rect_t rect)
{
    static SDL_Rect r;

    r.x = rect.x;
    r.y = rect.y;
    r.w = rect.w;
    r.h = rect.h;
    
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderDrawRect(renderer, &r);
}

void LifePane::draw(SDL_Renderer *renderer)
{
    static rect_t rect;

    rect.x = this->field_dims.x;
    rect.y = this->field_dims.y;
    rect.w = this->field_dims.w * this->cell_dims.w;
    rect.h = this->field_dims.h * this->cell_dims.h;

    this->draw_field_border(renderer, rect);

    // rect.w = this->cell_dims.w;
    // rect.h = this->cell_dims.h;
    // this->draw_point(renderer, rect);

    for(int x = 0; x < this->life.field_width; x++)
    {
        for(int y = 0; y < this->life.field_height; y++)
        {
            if(this->life.field[x][y])
            {
                rect.x = this->field_dims.x + (this->cell_dims.w * x);
                rect.y = this->field_dims.y + (this->cell_dims.h * y);
                rect.w = this->cell_dims.w;
                rect.h = this->cell_dims.h;
                this->draw_point(renderer, rect);
            }
        }
    }
}