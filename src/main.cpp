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
    Game(size_t, size_t);
    void process();
    void run();

    Life *life;
};

Game::Game(size_t width, size_t height)
{
    this->life = new Life(width, height);
}

void Game::process()
{
    this->life->progress();
}

// enters a infinite loop.
void Game::run()
{
    while(1)
    {
        this->process();
    }
}

int main(void)
{
    Game game = Game(3, 3);
    game.run();

    return 0;
}