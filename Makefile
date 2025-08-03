CC = gcc
CCFLAGS += -Wall -Wextra
LDFLAGS = -lGL -lglut

all: play

play: src/cycle.c src/init.c src/load.c src/main.c src/tick.c
	$(CC) $(CCFLAGS) src/cycle.c src/init.c src/load.c src/main.c src/tick.c -o play $(LDFLAGS)

clean:
	rm -f play
