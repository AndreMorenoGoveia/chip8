#include "chip8.h"

extern uint8_t  delay_timer;
extern uint8_t  sound_timer;

void chip8_tick() {
    delay_timer = delay_timer > 0 ? delay_timer - 1 : 0;
    sound_timer = sound_timer > 0 ? sound_timer - 1 : 0;
    if (sound_timer == 0) {
        //system("powershell.exe \"[console]::beep(440,100)\" &");
    }
}