/* boot.s  – entrada inicial do kernel bare‑metal
 * GNU assembler (arm-none-eabi-as) syntax
 */

    .section .text
    .global _start          /* ponto de entrada – o bootloader da Broadcom salta aqui */

_start:
    /* 1. Desactiva interrupções (I flag) */
    cpsid   i               /* Clear P‑state Interrupt Disable bit */

    /* 2. Configura a pilha.
     *  _stack_top é fornecido pelo linker script (link.ld). */
    ldr     r0, =_stack_top
    mov     sp, r0

    /* 3. Limpa a .bss  (r2 = 0, r0 = início, r1 = fim) */
    mov     r2, #0
    ldr     r0, =__bss_start
    ldr     r1, =__bss_end
1:  cmp     r0, r1
    it      lt              /* IT block: executa o próximo STR se r0 < r1   */
    strlt   r2, [r0], #4    /* escreve 0 e avança r0 */
    blt     1b

    /* 4. Chama main()  (conforme a convenção de chamada do ABI, r0‑r3 livres) */
    bl      main

/* Se main retornar, ficamos em loop para sempre */
hang:
    b       hang


/* ----------- Fim da seção de código ----------- */

/*  Reserva de pilha – opcional; você também pode colocar
 *  a pilha em RAM alta no linker script.
 */
    .section .bss
    .align  7               /* alinha em 128 bytes (poderia ser 8) */
    .global _stack_bottom
_stack_bottom:
    .space  0x4000          /* 16 KiB para a pilha */
    .global _stack_top
_stack_top:
