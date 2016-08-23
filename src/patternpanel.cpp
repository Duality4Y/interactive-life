#include "patternpanel.h"

PatternPanel::PatternPanel():
LifePanel()
{}

PatternPanel::PatternPanel(rect_t field_dims, rect_t cell_dims):
LifePanel(field_dims, cell_dims)
{}

void PatternPanel::handle_input(SDL_Event event)
{

}

void PatternPanel::process()
{

}

void PatternPanel::draw(SDL_Renderer *renderer)
{
    static rect_t rect;

    if(this->m_life_panel)
    {
        // draw a point where the mouse should be.
        int px = this->mx - (this->mx % this->cell_dims.w);
        int py = this->my - (this->my % this->cell_dims.h);
        rect.x = px;
        rect.y = py;
        rect.w = this->cell_dims.w;
        rect.h = this->cell_dims.h;
        this->draw_point(renderer, rect);

        // on left click draw, draw lines if gaps between draws.
        if(this->lmclick)
        {
            // draw point, and fill with line if delta movement big enough.
            int lx = ((this->mx - this->field_dims.x) / this->cell_dims.w);
            int ly = ((this->my - this->field_dims.y) / this->cell_dims.h);
            this->draw_cell(lx, ly, 1);
            if(this->mx != this->pmx || this->my != this->pmy)
            {
                int lpx = ((this->pmx - this->field_dims.x) / this->cell_dims.w);
                int lpy = ((this->pmy - this->field_dims.y) / this->cell_dims.h);
                this->draw_cell_line(lx, ly, lpx, lpy);
            }
        }
        this->pmx = this->mx;
        this->pmy = this->my;
    }

    rect.x = this->field_dims.x;
    rect.y = this->field_dims.y;
    rect.w = this->field_dims.w * this->cell_dims.w;
    rect.h = this->field_dims.h * this->cell_dims.h;

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