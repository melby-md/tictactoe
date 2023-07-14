#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "minimax.h"

static int minimax_util(const int8_t *, char, int *, int);
static int score(const int8_t *, int);

bool
full(const int8_t *arr)
{
	for (int i = 0; i < 9; i++) {
		if (!arr[i])
			return false;
	}
	return true;
}

int
minimax(const int8_t *arr)
{
	int move;
	minimax_util(arr, 'O', &move, 0);
	return move;
}

static int
minimax_util(const int8_t *arr, char player, int *move, int depth)
{
	int8_t tmp[9];
	int scores[10], moves[10];
	int n = 0;

	int s;
	if ((s = score(arr, depth)) != 0 || full(arr)) {
		return s;
	}

	for (int i = 0; i < 9; i++) {
		if (arr[i])
			continue;

		memcpy(tmp, arr, 9);
		tmp[i] = (player == 'X') ? 1 : -1;
		scores[n] = minimax_util(tmp, (player == 'X') ? 'O' : 'X', move, depth+1);
		moves[n] = i;
		n++;
	}

	int idx;
	if (player == 'O') {
		int max_score = -11;

		for (int i = 0; i < n; i++)
			if (scores[i] > max_score) {
				max_score = scores[i];
				idx = i;
			}

	*move = moves[idx];
	return max_score;
	} else {
		int min_score = 11;

		for (int i = 0; i < n; i++)
			if (scores[i] < min_score) {
				min_score = scores[i];
				idx = i;
			}

	*move = moves[idx];
	return min_score;
	}
}

static int
score(const int8_t *arr, int depth)
{
	switch (winner(arr)) {
	case 'O':
		return  10 - depth;
	case 'X':
		return depth - 10;
	}
	return 0;
}

char
winner(const int8_t *arr)
{
	int i;
	int8_t v, h;

	for (i = 0; i < 3; i ++) {
		h = arr[i*3] + arr[i*3+1] + arr[i*3+2]; 
		v = arr[i] + arr[i+3] + arr[i+6];
		if (h == 3 || v == 3)
			return 'X';
		else if (h == -3 || v == -3)
			return 'O';
	}

	switch (arr[0] + arr[4] + arr[8]) {
	case 3:
		return 'X';
	case -3:
		return 'O';
	}

	switch (arr[2] + arr[4] + arr[6]) {
	case 3:
		return 'X';
	case -3:
		return 'O';
	}

	return 0;
}
