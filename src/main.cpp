#include <stdio.h>
#include <stddef.h>

extern "C"
{
    #include <SDL2/SDL.h>
}

#include <game.h>

int main(void)
{
    Game game = Game(128, 80, 10, 10);
    game.run();

    return 0;
}