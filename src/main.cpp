#include <stdio.h>
#include <stddef.h>

extern "C"
{
    #include <SDL2/SDL.h>
}

#include <game.h>

int main(int argc, const char* argv[])
{
    if (argc != 3) {
      printf("Usage: %s <blocksize> <mode>\r\n", argv[0]);
      return 1;
    }
    uint32_t blocksize = atoi(argv[1]);
    uint8_t color_mode = atoi(argv[2]);

    if (blocksize<1) {
	printf("I am sorry Dave, I'm afraid I can't do that.\n");
	return 1;
    }

    if (color_mode>4) {
      printf("Notice: invalid color mode, using default (white).\n");
    }

    Game game = Game(blocksize, color_mode);
    game.run();

    return 0;
}
