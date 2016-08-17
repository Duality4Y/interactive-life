#include "life.h"

Life::Life(cell_t width, cell_t height)
{
    this->field_width = width;
    this->field_height = height;

    this->field = new cell_t *[this->field_width];
    for(cell_t x = 0; x < this->field_width; x++)
    {
        this->field[x] = new cell_t[this->field_height];
    }
    
    this->buffer = new cell_t *[this->field_width];
    for(cell_t x = 0; x < this->field_width; x++)
    {
        this->buffer[x] = new cell_t[this->field_height];
    }

    this->clear();

    this->draw_glider(0, 0);
    this->draw_glider(width / 2 + 20, 0);
    this->draw_replicator(20, 20);
    // this->field[this->field_width - 1][this->field_height - 1] = true;
}

Life::~Life(){}

void Life::clear()
{
    for(cell_t x = 0; x < this->field_width; x++)
    {
        this->field[x] = new cell_t[this->field_height];
        memset(this->field[x], '\0', this->field_height);
    }

    for(cell_t x = 0; x < this->field_width; x++)
    {
        this->buffer[x] = new cell_t[this->field_height];
        memset(this->buffer[x], '\0', this->field_height);
    }

}

void Life::draw_glider(cell_t px, cell_t py)
{
    const static cell_t glider_width = 3;
    const static cell_t glider_height = 3;

    cell_t glider[glider_width][glider_height]
    {
        {0, 0, 1},
        {1, 0, 1},
        {0, 1, 1}
    };

    for(int x = 0; x < glider_width; x++)
    {
        for(int y = 0; y < glider_height; y++)
        {
            this->draw_cell(px + x, py + y, glider[x][y]);
        }
    }

    // draw in a little glider
    // this->field[x + 1][y + 0] = true;
    // this->field[x + 2][y + 1] = true;
    // this->field[x + 0][y + 2] = true;
    // this->field[x + 1][y + 2] = true;
    // this->field[x + 2][y + 2] = true;
}

void Life::draw_replicator(cell_t px, cell_t py)
{
    const static cell_t replicator_width = 7;
    const static cell_t replicator_height = 7;

    cell_t replicator[replicator_width][replicator_height]
    {
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 0},
        {0, 0, 1, 0, 0, 1, 0},
        {0, 1, 0, 0, 0, 1, 0},
        {0, 1, 0, 0, 1, 0, 0},
        {0, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };

    for(int x = 0; x < replicator_width; x++)
    {
        for(int y = 0; y < replicator_height; y++)
        {
            this->draw_cell(px + x, py + y, replicator[x][y]);
        }
    }
}

void Life::draw_cell(cell_t x, cell_t y, cell_t state)
{
    this->field[x][y] = state;
}

void Life::swapBuffers()
{
    field_t temp;
    temp = field;
    field = buffer;
    buffer = temp;
}

void Life::progress()
{
    static cell_t around;
    
    if(this->paused)
    {
        return;
    }

    for(cell_t x = 0; x < this->field_width; x++)
    {
        for(cell_t y = 0; y < this->field_height; y++)
        {
            if(this->field[x][y])
            {
                around = this->totalAround(x, y);
                if(around == this->surviveAbility)
                {
                    this->buffer[x][y] = true;
                }
                else if(around == this->surviveAbility + 1)
                {
                    this->buffer[x][y] = true;
                }
                else
                {
                    this->buffer[x][y] = false;
                }
            }
            else
            {
                around = this->totalAround(x, y);
                if(around == this->reproductiveNumber)
                {
                    this->buffer[x][y] = true;
                }
                else
                {
                    this->buffer[x][y] = false;
                }
            }
        }
    }
    this->swapBuffers();
}

cell_t Life::totalAround(cell_t x, cell_t y)
{
    cell_t n = 0;
    for(cell_t i = -1; i <= 1; i++)
    {
        for(cell_t j = -1; j <= 1; j++)
        {
            cell_t nx = (x + i + this->field_width) % this->field_width;
            cell_t ny = (y + j + this->field_height) % this->field_height;
            if(this->field[nx][ny])
                n++;
        }
    }
    if(this->field[x][y])
        n--;

    return n;
}
