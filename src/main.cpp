#include <stdio.h>
#include <stddef.h>

extern "C"
{
    #include <SDL2/SDL.h>
}

#include <game.h>

int main(void)
{
    Game game = Game(128/4, 64/2, 8, 8);
    game.run();

    return 0;
}