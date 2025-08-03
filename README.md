# CHIP-8

Para compilar o processador CHIP-8 e rodar uma ROM de um jogo, rode os comandos abaixo:

```sh
make
./play <rom>
```

## Estrutura do processador

O processador está dividido em 4 módulos principais, cada um com responsabilidade bem definida:

### Inicialização (init.c)

Responsável por preparar o estado inicial da máquina CHIP-8:

- Zera memória, registradores, pilha, display e teclado.
- Carrega o fontset padrão na memória (endereços começando em `0x00`).
- Inicializa os registradores especiais: `PC` é definido para o início dos programas (`0x200`), `I`, `SP`, timers e sinalizadores.
- Semente do gerador de números aleatórios é inicializada com `srand(time(NULL))`.

Variáveis globais definidas aqui e expostas via `extern`:
- `opcode`
- `memory`
- `V` (registradores V0..VF)
- `I`, `PC`
- `gfx` (tela)
- `delay_timer`, `sound_timer`
- `stack`, `SP`
- `key`
- `chip8_draw_flag`

Função principal:
```c
void chip8_initialize(void);
```

Também contém o `chip8_fontset` e o helper `randbyte()` usado por instruções aleatórias.

### Carregamento de ROM (load.c)

Responsável por abrir e ler a ROM do jogo para a memória em `0x200`:

Função:
```c
void chip8_loadgame(char *game);
```

Se não conseguir abrir o arquivo, exibe erro e finaliza. Lê até `MAX_GAME_SIZE` bytes (tamanho máximo possível) em `memory[0x200]`.

### Ciclo de Emulação e Timers (cycle.c)

Contém a lógica central de busca, decodificação e execução de opcodes, além da atualização dos timers:

Funções:
```c
void chip8_emulatecycle(void);
void chip8_tick(void);
```

- `chip8_emulatecycle`: busca o opcode atual, decodifica campos (`x`, `y`, `n`, `kk`, `nnn`) e executa de acordo com a tabela completa de instruções do CHIP-8 (incluindo desenho de sprites, controle de fluxo, operações aritméticas, manipulação de registradores, eventos de tecla, etc.).
  - Usa `draw_sprite` para renderizar gráficos com detecção de colisão.
  - Atualiza o program counter (`PC`) conforme cada instrução.
  - Sinaliza desenho com `chip8_draw_flag` quando necessário.

- `chip8_tick`: decrementa os timers `delay_timer` e `sound_timer`. Emite "BEEP!" quando `sound_timer` chega a zero.

### Arquivo de cabeçalho comum (chip8.h)

Define:
- Constantes (tamanhos de memória, display, pilha, etc.).
- Tipos e macros auxiliares.
- Declarações `extern` dos estados globais.
- Assinaturas das funções públicas:
  - `chip8_initialize`
  - `chip8_loadgame`
  - `chip8_emulatecycle`
  - `chip8_tick`
  - `draw_sprite` (usada internamente)

### Frontend gráfico e main (main.c)

Implementa a interface de saída e entrada usando OpenGL/GLUT:

- Inicializa a janela com as dimensões baseadas em `GFX_ROWS` e `GFX_COLS`, escaladas por `PIXEL_SIZE`.
- Traduz o estado `gfx` para pixels e desenha via `glDrawPixels`.
- Mapeia teclado convencional para o teclado hexadecimal do CHIP-8.
- Loop principal chama `chip8_emulatecycle()` constantemente e atualiza timers em taxa fixa (`CLOCK_HZ`).
- Usa `chip8_draw_flag` para atualizar a tela apenas quando necessário.

#### Mapeamento de teclas

O teclado físico é mapeado para o teclado hexadecimal do CHIP-8 da seguinte forma:

```
1 2 3 4    -> 1 2 3 C
q w e r    -> 4 5 6 D
a s d f    -> 7 8 9 E
z x c v    -> A 0 B F
```

Pressionar a tecla correspondente define o índice em `key[]` para 1; liberá-la zera-o.

#### Temporização

- A emulação usa um timer de 60Hz (`CLOCK_HZ`) para atualizar os timers de delay e som.
- O ciclo de execução (`chip8_emulatecycle`) roda continuamente via `glutIdleFunc`.
- A função `chip8_tick` é chamada apenas quando o tempo acumulado ultrapassa o intervalo de 1/60 segundo.



### Layout de memória importante

- Instruções são carregadas a partir de `0x200`.
- `chip8_fontset` está em `0x00`.
- Registrador de índice `I` é usado para endereçamento de fontes/sprites.
- `V[0xF]` é usado como flag de carry/collision em várias instruções.

### Erros e tratamento

- OpCodes desconhecidos disparam mensagem de erro e encerram o programa.
- Falha ao abrir a ROM também encerra com mensagem.

