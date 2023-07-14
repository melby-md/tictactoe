CC = $(CROSS)cc -std=c99
#WINDRES = $(CROSS)windres
SDL_CONFIG = sdl2-config
WARNINGS = -Wall -Wextra -Wvla -Wno-unused-parameter -Wno-unused-function
LDLIBS = $$($(SDL_CONFIG) --libs)

all: tictactoe

tictactoe: minimax.o main.o
	$(CC) $(LDFLAGS) -o $@ minimax.o main.o $(LDLIBS)

.c.o:
	$(CC) $(CFLAGS) $(WARNINGS) -c -o $@ $<

main.o: main.c
	$(CC) $(CFLAGS) $(WARNINGS) $$($(SDL_CONFIG) --cflags) -c -o $@ $<

clean:
	rm -f tictactoe tictactoe.exe main.o minimax.o cli.o

minimax.o main.o cli.o: minimax.h

.PHONY: all clean
