#include "life.h"

Life::Life(int width, int height)
{
    this->field_width = width;
    this->field_height = height;

    this->field = new int *[this->field_width];
    for(int x = 0; x < this->field_width; x++)
    {
        this->field[x] = new int[this->field_height];
        memset(this->field[x], '\0', this->field_height);
    }
    
    this->buffer = new int *[this->field_width];
    for(int x = 0; x < this->field_width; x++)
    {
        this->buffer[x] = new int[this->field_height];
        memset(this->buffer[x], '\0', this->field_height);
    }

    this->draw_glider(0, 0);
    this->draw_glider(width / 2 + 20, 0);
    this->field[this->field_width - 1][this->field_height - 1] = true;

    // seed field
    // time_t t;
    // srand((unsigned)time(&t));
    // for(int x = 0; x < this->field_width; x++)
    // {
    //     for(int y = 0; y < this->field_height; y++)
    //     {
    //         this->field[x][y] = rand() & 0x01;
    //     }
    // }
    // for(int i = 0; i < 3; i++)
    // {
    //     for(int j = 0; j < 3; j++)
    //     {
    //         this->field[i][j] = true;
    //     }
    // }
}

Life::~Life(){}

void Life::draw_glider(int x, int y)
{
    // draw in a little glider
    this->field[x + 1][y + 0] = true;
    this->field[x + 2][y + 1] = true;
    this->field[x + 0][y + 2] = true;
    this->field[x + 1][y + 2] = true;
    this->field[x + 2][y + 2] = true;
}

void Life::swapBuffers()
{
    int **temp;
    temp = field;
    field = buffer;
    buffer = temp;
}

void Life::progress()
{
    static int around;
    for(int x = 0; x < this->field_width; x++)
    {
        for(int y = 0; y < this->field_height; y++)
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

int Life::totalAround(int x, int y)
{
    int n = 0;
    for(int i = -1; i <= 1; i++)
    {
        for(int j = -1; j <= 1; j++)
        {
            int nx = (x + i + this->field_width) % this->field_width;
            int ny = (y + j + this->field_height) % this->field_height;
            if(this->field[nx][ny])
                n++;
        }
    }
    if(this->field[x][y])
        n--;

    return n;
}
