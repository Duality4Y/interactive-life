#include <stdio.h>

#include <game.h>

int main(void)
{
    Game game = Game(100, 50, 10, 10);
    game.run();

    return 0;
}