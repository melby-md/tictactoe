#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "minimax.h"

static int  get_single_digit(void);
static void print_board(int8_t *);

static int
get_single_digit(void)
{
	char buffer[3];
	char ch;

	if (fgets(buffer, 3, stdin) == NULL)
		return -1;

	if (buffer[0] == '\n')
		return -1;

	if (buffer[1] != '\n') {
		while ((ch = getchar()) != '\n' && ch != EOF);
		return -1;
	}

	return buffer[0] - '0';
}

static void
print_board(int8_t *arr)
{
	for (int i = 0; i < 9; i++) {
		putchar(' ');
		switch (arr[i]) {
		case -1:
			putchar('O');
			break;
		case 1:
			putchar('X');
			break;
		case 0:
			putchar(i+1+'0');
		}
		if (i == 8)
			putchar('\n');
		else if (i == 2 || i == 5)
			printf("\n-----------\n");
		else
			printf(" |");
	}
}

int
main(void)
{
	bool player, f;
	char mode, pos, win;
	int8_t arr[9] = {0};

	srand(time(NULL));
	player = rand() & 1;

	printf("%c beggins!\n", player ? 'X' : 'O');

	for (;;) {
		if (player){ 
			print_board(arr);
			printf("player's turn\nInsert a position [1-9]: ");
			pos = get_single_digit() - 1;

			if (pos >= 9 || pos < 0 || arr[pos] != 0) {
				printf("Insert a valid position\n");
				continue;
			}
		} else {
			pos = minimax(arr);
		}
		arr[pos] = player ? 1 : -1;	
		player = !player;

		if (win = winner(arr)) {
			print_board(arr);
			printf("%c wins!\n", win);
			break;
		} else if (f = full(arr)) {
			print_board(arr);
			printf("Tie!\n", win);
			break;
		}
	}

	return 0;
}
