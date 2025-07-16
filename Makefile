FONTES = boot.s src/main.c
ALVO = kernel

# Interface serial a utilizar
TTY = /dev/ttyUSB0

# Configuração JTAG
OCD_CFG = jtag.cfg

#
# Arquivos de saída 
#
EXEC = ${ALVO}.elf
MAP = ${ALVO}.map
IMAGE = ${ALVO}.img
HEXFILE = ${ALVO}.hex
LIST = ${ALVO}.list
OBJ = $(FONTES:.s=.o)
OBJETOS = $(OBJ:.c=.o)

PREFIXO = arm-none-eabi-
LDSCRIPT = linker.ld
AS = ${PREFIXO}as
LD = ${PREFIXO}ld
GCC = ${PREFIXO}gcc
OBJCPY = ${PREFIXO}objcopy
OBJDMP = ${PREFIXO}objdump

# Opções do compilador e linker
OPTS = -g 
LDOPTS =

# Para RPi Zero
#OPTS += -mfpu=vfp -march=armv6zk -mtune=arm1176jzf-s -g 
#Para PPi 2
OPTS += -march=armv7-a -mtune=cortex-a7 
OPTS += --specs=nano.specs

# para usar newlib nano
#LDOPTS += -L/usr/lib/arm-none-eabi/newlib -lc_nano -lm
# para usar newlib
#LDOPTS += -L/usr/lib/arm-none-eabi/newlib -lc -lm
# para usar libgcc
#LDOPTS += -L/usr/lib/gcc/arm-none-eabi/13.2.1 -lgcc

all: ${EXEC} ${IMAGE} ${LIST} ${HEXFILE}

#
# Gerar executável
#
${EXEC}: ${OBJETOS}
	${LD} -T ${LDSCRIPT} -Map=${MAP} -o $@ ${OBJETOS} ${LDOPTS}

#
# Gerar imagem
#
${IMAGE}: ${EXEC}
	${OBJCPY} ${EXEC} -O binary ${IMAGE}

#
# Gerar intel Hex
#
${HEXFILE}: ${EXEC}
	${OBJCPY} ${EXEC} -O ihex ${HEXFILE}

#
# Gerar listagem
#
${LIST}: ${EXEC}
	${OBJDMP} -d ${EXEC} > ${LIST}

#
# Compilar arquivos em C
#
.c.o:
	${GCC} ${OPTS} -c -o $@ $<

#
# Montar arquivos em assembler
#
.s.o:
	${AS} -g -o $@ $<

#
# Limpar tudo
#
clean:
	rm -f *.o ${EXEC} ${MAP} ${LIST} ${IMAGE} ${HEXFILE}

#
# Executar openocd
#
ocd:
	@if pgrep openocd >/dev/null ; then \
		echo "openocd já está executando" ; \
	else openocd -f ${OCD_CFG} & \
	fi

#
# Gdb via serial
#
gdb: ${EXEC}
	@if pgrep openocd >/dev/null; then \
		gdb-multiarch ${EXEC} \
			-ex "target extended-remote: 3333" \
			-ex "load"; \
		else gdb-multiarch -b 115200 ${EXEC} \
		                -ex "target remote ${TTY}" \
	                   -ex "load"; \
	fi

#
# Executar qemu
#
qemu:
	qemu-system-arm -s -M virt -kernel ${EXEC} &
	gdb-multiarch -ex "set architecture arm" ${EXEC} \
				-ex "target extended-remote :1234" \
				-ex "load"FONTES = boot.s src/main.c
ALVO = kernel

# Interface serial a utilizar
TTY = /dev/ttyUSB0

# Configuração JTAG
OCD_CFG = jtag.cfg

#
# Arquivos de saída 
#
EXEC = ${ALVO}.elf
MAP = ${ALVO}.map
IMAGE = ${ALVO}.img
HEXFILE = ${ALVO}.hex
LIST = ${ALVO}.list
OBJ = $(FONTES:.s=.o)
OBJETOS = $(OBJ:.c=.o)

PREFIXO = arm-none-eabi-
LDSCRIPT = linker.ld
AS = ${PREFIXO}as
LD = ${PREFIXO}ld
GCC = ${PREFIXO}gcc
OBJCPY = ${PREFIXO}objcopy
OBJDMP = ${PREFIXO}objdump

# Opções do compilador e linker
OPTS = -g 
LDOPTS =

# Para RPi Zero
#OPTS += -mfpu=vfp -march=armv6zk -mtune=arm1176jzf-s -g 
#Para PPi 2
OPTS += -march=armv7-a -mtune=cortex-a7 
OPTS += --specs=nano.specs

# para usar newlib nano
#LDOPTS += -L/usr/lib/arm-none-eabi/newlib -lc_nano -lm
# para usar newlib
#LDOPTS += -L/usr/lib/arm-none-eabi/newlib -lc -lm
# para usar libgcc
#LDOPTS += -L/usr/lib/gcc/arm-none-eabi/13.2.1 -lgcc

all: ${EXEC} ${IMAGE} ${LIST} ${HEXFILE}

#
# Gerar executável
#
${EXEC}: ${OBJETOS}
	${LD} -T ${LDSCRIPT} -Map=${MAP} -o $@ ${OBJETOS} ${LDOPTS}

#
# Gerar imagem
#
${IMAGE}: ${EXEC}
	${OBJCPY} ${EXEC} -O binary ${IMAGE}

#
# Gerar intel Hex
#
${HEXFILE}: ${EXEC}
	${OBJCPY} ${EXEC} -O ihex ${HEXFILE}

#
# Gerar listagem
#
${LIST}: ${EXEC}
	${OBJDMP} -d ${EXEC} > ${LIST}

#
# Compilar arquivos em C
#
.c.o:
	${GCC} ${OPTS} -c -o $@ $<

#
# Montar arquivos em assembler
#
.s.o:
	${AS} -g -o $@ $<

#
# Limpar tudo
#
clean:
	rm -f *.o src/*.o ${EXEC} ${MAP} ${LIST} ${IMAGE} ${HEXFILE}

#
# Executar openocd
#
ocd:
	@if pgrep openocd >/dev/null ; then \
		echo "openocd já está executando" ; \
	else openocd -f ${OCD_CFG} & \
	fi

#
# Gdb via serial
#
gdb: ${EXEC}
	@if pgrep openocd >/dev/null; then \
		gdb-multiarch ${EXEC} \
			-ex "target extended-remote: 3333" \
			-ex "load"; \
		else gdb-multiarch -b 115200 ${EXEC} \
		                -ex "target remote ${TTY}" \
	                   -ex "load"; \
	fi

#
# Executar qemu
#
qemu:
	qemu-system-arm -s -M virt -kernel ${EXEC} &
	gdb-multiarch -ex "set architecture arm" ${EXEC} \
				-ex "target extended-remote :1234" \
				-ex "load"