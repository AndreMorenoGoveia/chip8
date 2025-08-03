#ifndef __CHIP8_H__
#define __CHIP8_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

/* tamanhos e constantes */
#define MEM_SIZE 4096
#define GFX_ROWS 32
#define GFX_COLS 64
#define GFX_SIZE (GFX_ROWS * GFX_COLS)
#define STACK_SIZE 16
#define KEY_SIZE 16

#define GFX_INDEX(row, col) ((row)*GFX_COLS + (col))
#define MAX_GAME_SIZE (0x1000 - 0x200)

/* fonte */
#define FONTSET_ADDRESS 0x00
#define FONTSET_BYTES_PER_CHAR 5

/* variáveis globais (visíveis entre módulos) */
extern uint16_t opcode;
extern uint8_t  memory[MEM_SIZE];
extern uint8_t  V[16];
extern uint16_t I;
extern uint16_t PC;
extern uint8_t  gfx[GFX_ROWS][GFX_COLS];
extern uint8_t  delay_timer;
extern uint8_t  sound_timer;
extern uint16_t stack[STACK_SIZE];
extern uint16_t SP;
extern uint8_t  key[KEY_SIZE];
extern bool     chip8_draw_flag;

/* funções expostas */
void chip8_initialize(void);
void chip8_loadgame(char *game);
void chip8_emulatecycle(void);
void chip8_tick(void);

/* helpers (podem ser estáticos em .c se não forem expostos) */
void draw_sprite(uint8_t x, uint8_t y, uint8_t n);

#endif /* __CHIP8_H__ */
