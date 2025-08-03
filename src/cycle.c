#include "chip8.h"
#include <stdio.h>
#include <stdlib.h>

#define unknown_opcode(op) \
    do { \
        fprintf(stderr, "Unknown opcode: 0x%x\n", opcode); \
        exit(42); \
    } while (0)

static inline uint8_t randbyte() { return (rand() % 256); }

extern uint8_t  memory[];
extern uint8_t  V[];
extern uint16_t I;
extern uint16_t PC;
extern uint8_t  gfx[][GFX_COLS];
extern uint16_t stack[];
extern uint16_t SP;
extern uint8_t  key[];
extern bool     chip8_draw_flag;
extern uint16_t opcode;

void draw_sprite(uint8_t x, uint8_t y, uint8_t n) {
    unsigned row = y, col = x;
    unsigned byte_index;
    unsigned bit_index;

    // set the collision flag to 0
    V[0xF] = 0;
    for (byte_index = 0; byte_index < n; byte_index++) {
        uint8_t byte = memory[I + byte_index];

        for (bit_index = 0; bit_index < 8; bit_index++) {
            uint8_t bit = (byte >> bit_index) & 0x1;
            uint8_t *pixelp = &gfx[(row + byte_index) % GFX_ROWS]
                                  [(col + (7 - bit_index)) % GFX_COLS];

            if (bit == 1 && *pixelp ==1) V[0xF] = 1;

            *pixelp = *pixelp ^ bit;
        }
    }
}

void chip8_emulatecycle() {
    int i;
    uint8_t x, y, n;
    uint8_t kk;
    uint16_t nnn;

    // fetch
    opcode = memory[PC] << 8 | memory[PC + 1];
    x   = (opcode >> 8) & 0x000F;
    y   = (opcode >> 4) & 0x000F;
    n   = opcode & 0x000F;
    kk  = opcode & 0x00FF;
    nnn = opcode & 0x0FFF;

    // decode and execute
    switch (opcode & 0xF000) {
        case 0x0000:
            switch (kk) {
                case 0x00E0:
                    memset(gfx, 0, sizeof(uint8_t) * GFX_SIZE);
                    chip8_draw_flag = true;
                    PC += 2;
                    break;
                case 0x00EE:
                    PC = stack[--SP];
                    break;
                default:
                    unknown_opcode(opcode);
            }
            break;
        case 0x1000:
            PC = nnn;
            break;
        case 0x2000:
            stack[SP++] = PC + 2;
            PC = nnn;
            break;
        case 0x3000:
            PC += (V[x] == kk) ? 4 : 2;
            break;
        case 0x4000:
            PC += (V[x] != kk) ? 4 : 2;
            break;
        case 0x5000:
            PC += (V[x] == V[y]) ? 4 : 2;
            break;
        case 0x6000:
            V[x] = kk;
            PC += 2;
            break;
        case 0x7000:
            V[x] += kk;
            PC += 2;
            break;
        case 0x8000:
            switch (n) {
                case 0x0: V[x] = V[y]; break;
                case 0x1: V[x] |= V[y]; break;
                case 0x2: V[x] &= V[y]; break;
                case 0x3: V[x] ^= V[y]; break;
                case 0x4:
                    V[0xF] = ((int)V[x] + (int)V[y]) > 255 ? 1 : 0;
                    V[x] = V[x] + V[y];
                    break;
                case 0x5:
                    V[0xF] = (V[x] > V[y]) ? 1 : 0;
                    V[x] = V[x] - V[y];
                    break;
                case 0x6:
                    V[0xF] = V[x] & 0x1;
                    V[x] >>= 1;
                    break;
                case 0x7:
                    V[0xF] = (V[y] > V[x]) ? 1 : 0;
                    V[x] = V[y] - V[x];
                    break;
                case 0xE:
                    V[0xF] = (V[x] >> 7) & 0x1;
                    V[x] <<= 1;
                    break;
                default:
                    unknown_opcode(opcode);
            }
            PC += 2;
            break;
        case 0x9000:
            if ((n) == 0x0)
                PC += (V[x] != V[y]) ? 4 : 2;
            else
                unknown_opcode(opcode);
            break;
        case 0xA000:
            I = nnn;
            PC += 2;
            break;
        case 0xB000:
            PC = nnn + V[0];
            break;
        case 0xC000:
            V[x] = randbyte() & kk;
            PC += 2;
            break;
        case 0xD000:
            draw_sprite(V[x], V[y], n);
            PC += 2;
            chip8_draw_flag = true;
            break;
        case 0xE000:
            switch (kk) {
                case 0x9E:
                    PC += (key[V[x]]) ? 4 : 2;
                    break;
                case 0xA1:
                    PC += (!key[V[x]]) ? 4 : 2;
                    break;
                default:
                    unknown_opcode(opcode);
            }
            break;
        case 0xF000:
            switch (kk) {
                case 0x07:
                    V[x] = delay_timer;
                    PC += 2;
                    break;
                case 0x0A: {
                    int j;
                    while (true) {
                        for (j = 0; j < KEY_SIZE; j++) {
                            if (key[j]) {
                                V[x] = j;
                                goto got_key_press;
                            }
                        }
                    }
                    got_key_press:
                    PC += 2;
                    break;
                }
                case 0x15:
                    delay_timer = V[x];
                    PC += 2;
                    break;
                case 0x18:
                    sound_timer = V[x];
                    PC += 2;
                    break;
                case 0x1E:
                    V[0xF] = (I + V[x] > 0xFFF) ? 1 : 0;
                    I = I + V[x];
                    PC += 2;
                    break;
                case 0x29:
                    I = FONTSET_BYTES_PER_CHAR * V[x];
                    PC += 2;
                    break;
                case 0x33:
                    memory[I]   = (V[x] % 1000) / 100;
                    memory[I+1] = (V[x] % 100) / 10;
                    memory[I+2] = (V[x] % 10);
                    PC += 2;
                    break;
                case 0x55:
                    for (i = 0; i <= x; i++) memory[I + i] = V[i];
                    I += x + 1;
                    PC += 2;
                    break;
                case 0x65:
                    for (i = 0; i <= x; i++) V[i] = memory[I + i];
                    I += x + 1;
                    PC += 2;
                    break;
                default:
                    unknown_opcode(opcode);
            }
            break;
        default:
            unknown_opcode(opcode);
    }
}
