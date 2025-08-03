#include "chip8.h"
#include <stdio.h>
#include <stdlib.h>

void chip8_loadgame(char *game) {
    FILE *fgame;

    fgame = fopen(game, "rb");
    
    if (NULL == fgame) {
        fprintf(stderr, "Unable to open game: %s\n", game);
        exit(42);
    }

    fread(&memory[0x200], 1, MAX_GAME_SIZE, fgame);

    fclose(fgame);
}
