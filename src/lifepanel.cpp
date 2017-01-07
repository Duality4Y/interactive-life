#include "lifepanel.h"


LifePanel::LifePanel()
{
    rect_t cell_dims = {0, 0, 1, 1};
    rect_t field_dims = {1, 1, 1, 1};
    this->init(field_dims, cell_dims, 0);
}

LifePanel::LifePanel(rect_t field_dims, rect_t cell_dims, uint8_t color_mode_param)
{
    this->init(field_dims, cell_dims, color_mode_param);
}

void LifePanel::init(rect_t field_dims, rect_t cell_dims, uint8_t color_mode_param)
{
    this->color_mode = color_mode_param;
    this->field_dims = field_dims;
    this->cell_dims = cell_dims;
    this->life = Life(this->field_dims.w, this->field_dims.h);
    this->life.draw_glider(1, 1);
}

void LifePanel::handle_input(SDL_Event event)
{
    if(event.type == SDL_KEYUP)
    {
        switch(event.key.keysym.sym)
        {
            case SDLK_c:
                this->life.clear();
                break;
            case SDLK_SPACE:
                this->paused = !this->paused;
                break;
            default:
                break;
        }
    }
    if(event.type == SDL_MOUSEBUTTONDOWN)
    {
        this->lmclick = true;
        this->lmdelay = 1;
    }
    if(event.type == SDL_MOUSEBUTTONUP)
    {
        this->lmclick = false;
        this->lmdelay = 0;
    }

    SDL_GetMouseState(&(this->mx), &(this->my));
    // detect if mouse is inside this panel.
    if((this->mx > this->field_dims.x) &&
       (this->my > this->field_dims.y) &&
       (this->mx < (this->field_dims.x + (this->field_dims.w * this->cell_dims.w))) &&
       (this->my < (this->field_dims.y + (this->field_dims.h * this->cell_dims.h))))
    {
        this->m_life_panel = true;
    }
    else
    {
        this->m_life_panel = false;
    }
}

void LifePanel::draw_cell_relative(int x, int y, int state)
{
    static int rx, ry;
    rx = x / this->cell_dims.w;
    ry = y / this->cell_dims.h;

    this->life.set_cell(rx, ry, state);    
}

void LifePanel::draw_cell(int x, int y, int state)
{
    this->life.set_cell(x, y, state);
}

void LifePanel::draw_cell_line(int x0, int y0, int x1, int y1)
{
    int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
    int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
    int err = (dx>dy ? dx : -dy)/2, e2;

    for(;;)
    {
        // setPixel(x0,y0);
        // this->life.field[x0][y0] = true;
        this->draw_cell(x0, y0, true);
        if (x0==x1 && y0==y1)
        {
            break;
        }
        e2 = err;
        if (e2 >-dx)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dy)
        {
            err += dx;
            y0 += sy;
        }
    }
}

void LifePanel::draw_point(SDL_Renderer *renderer, rect_t rect, uint8_t cr = 0xFF, uint8_t cg = 0xFF, uint8_t cb = 0xFF)
{
    static SDL_Rect r;
    r.x = rect.x;
    r.y = rect.y;
    r.w = rect.w;
    r.h = rect.h;
    SDL_SetRenderDrawColor(renderer, cr, cg, cb, 0xFF);
    SDL_RenderFillRect(renderer, &r);
    // draw border if cells biger then 2x2
    if(rect.w > 2 && rect.h > 2)
    {
        SDL_SetRenderDrawColor(renderer, 0x00,0x00, 0x00, 0xFF);
        SDL_RenderDrawRect(renderer, &r);
    }
}

void LifePanel::draw_field_border(SDL_Renderer *renderer, rect_t rect)
{
    static SDL_Rect r;

    r.x = rect.x;
    r.y = rect.y;
    r.w = rect.w;
    r.h = rect.h;
    
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderDrawRect(renderer, &r);
}

void LifePanel::process()
{
    if(!this->paused)
        this->life.progress();

    if(this->m_life_panel)
    {
        SDL_ShowCursor(SDL_DISABLE);
    }
    else
    {
        SDL_ShowCursor(SDL_ENABLE);
    }
}

void LifePanel::draw(SDL_Renderer *renderer)
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
        if (this->lmclick) this->draw_point(renderer, rect);

        // on left click draw, draw lines if gaps between draws.
        if(this->lmclick)
        {
            if (this->lmdelay>0) {
              this->lmdelay--;
            } else {
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
        }
        this->pmx = this->mx;
        this->pmy = this->my;
    }

    rect.x = this->field_dims.x;
    rect.y = this->field_dims.y;
    rect.w = this->field_dims.w * this->cell_dims.w;
    rect.h = this->field_dims.h * this->cell_dims.h;

    //if (this->lmdelay>0) this->draw_field_border(renderer, rect);

    for(int x = 0; x < this->life.field_width; x++)
    {
        for(int y = 0; y < this->life.field_height; y++)
        {
            if(this->life.field[x][y])
            {
		uint8_t cr = 255;
		uint8_t cg = 255;
		uint8_t cb = 255;

		int ar = this->life.totalAround(x, y);

		if (color_mode==1) {
			cr = (uint8_t) ((uint16_t) (255*ar)/8); //<-- I like this one! -R
			cg = 255-cr;
			cb = 0;
		} else if(color_mode==2) {
                	int ar4 = ar; //For those who like bluestuffthingblearch
                	if (ar4>4) {
                	  ar4 = 4;
                	}
			cr = (uint8_t) ((uint16_t) (255*ar4)/4);
                	cg = 255-cr;
			if (ar>4) {
        	          int arB = ar-4;
        	          cb = (uint8_t) ((uint16_t) (255*arB)/4);
        	          cr = 255-cb;
        	        }
		} else if(color_mode==3) {
			cb = 255;
			int nope = (uint8_t) ((uint16_t) (255*ar)/8);
			cr = 255 - nope;
			cg = 255 - nope;
		}
                rect.x = this->field_dims.x + (this->cell_dims.w * x);
                rect.y = this->field_dims.y + (this->cell_dims.h * y);
                rect.w = this->cell_dims.w;
                rect.h = this->cell_dims.h;
                this->draw_point(renderer, rect, cr,cg,cb);
            }
        }
    }
}
