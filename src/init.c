#include "chip8.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define IS_BIT_SET(byte, bit) (((0x80 >> (bit)) & (byte)) != 0x0)

/* fonte padrão */
unsigned char chip8_fontset[80] = 
{ 
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F 
};

uint16_t opcode;
uint8_t  memory[MEM_SIZE];
uint8_t  V[16];
uint16_t I;
uint16_t PC;
uint8_t  gfx[GFX_ROWS][GFX_COLS];
uint8_t  delay_timer;
uint8_t  sound_timer;
uint16_t stack[STACK_SIZE];
uint16_t SP;
uint8_t  key[KEY_SIZE];
bool     chip8_draw_flag;

static inline uint8_t randbyte() { return (rand() % 256); }

void chip8_initialize() {
    int i;

    PC     = 0x200;
    opcode = 0;
    I      = 0;
    SP     = 0;

    memset(memory, 0, sizeof(uint8_t)  * MEM_SIZE);
    memset(V,      0, sizeof(uint8_t)  * 16);
    memset(gfx,    0, sizeof(uint8_t)  * GFX_SIZE);
    memset(stack,  0, sizeof(uint16_t) * STACK_SIZE);
    memset(key,    0, sizeof(uint8_t)  * KEY_SIZE);

    for (i = 0; i < 80; i++) {
        memory[FONTSET_ADDRESS + i] = chip8_fontset[i];
    }
    
    chip8_draw_flag = true; 
    delay_timer = 0;
    sound_timer = 0;
    srand((unsigned)time(NULL));
}
