#include <stdbool.h>
#include <stdint.h>

#include "minimax.h"
#include "SDL.h"

int
circle(SDL_Renderer * renderer, int x, int y, int radius)
{
    int offsetx, offsety, d;
    int status;


    offsetx = 0;
    offsety = radius;
    d = radius -1;
    status = 0;

    while (offsety >= offsetx) {
        status += SDL_RenderDrawPoint(renderer, x + offsetx, y + offsety);
        status += SDL_RenderDrawPoint(renderer, x + offsety, y + offsetx);
        status += SDL_RenderDrawPoint(renderer, x - offsetx, y + offsety);
        status += SDL_RenderDrawPoint(renderer, x - offsety, y + offsetx);
        status += SDL_RenderDrawPoint(renderer, x + offsetx, y - offsety);
        status += SDL_RenderDrawPoint(renderer, x + offsety, y - offsetx);
        status += SDL_RenderDrawPoint(renderer, x - offsetx, y - offsety);
        status += SDL_RenderDrawPoint(renderer, x - offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}

static void
render(SDL_Renderer *renderer, int8_t arr[][3], int size) 
{
	int tile = size/3;
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawLine(renderer, size/3, 0, size/3, size);
	SDL_RenderDrawLine(renderer, size/3*2, 0, size/3*2, size);
	SDL_RenderDrawLine(renderer, 0, size/3, size, size/3);
	SDL_RenderDrawLine(renderer, 0, size/3*2, size, size/3*2);

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++) {
			SDL_Log("%d", arr[i][j]);
			switch (arr[i][j]) {
			case 1:
				SDL_RenderDrawLine(renderer, tile*i, tile*j, tile*i+tile, tile*j+tile);
				SDL_RenderDrawLine(renderer, tile*i+tile, tile*j, tile*i, tile*j+tile);
				break;
			case -1:
				circle(renderer, tile*i+tile/2, tile*j+tile/2, size/7);
			}
		}

	SDL_RenderPresent(renderer);
}

int
main(int argc, char *argv[])
{
	int8_t arr[3][3] = {0};
	int size = 512;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_ERROR, "SDL_Init()", SDL_GetError(), 0
		);
		SDL_Log("SDL_Init(): %s", SDL_GetError());
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow(
		"tictactoe",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		size,
		size,
		SDL_WINDOW_SHOWN
	);

	if (window == NULL) {
		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_ERROR, "SDL_CreateWindow()", SDL_GetError(), 0
		);
		SDL_Log("SDL_CreateWindow(): %s", SDL_GetError());
		return 1;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

	if (renderer == NULL) {
		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_ERROR, "SDL_CreateRenderer()", SDL_GetError(), window
		);
		SDL_Log("SDL_CreateRenderer(): %s", SDL_GetError());
		return 1;
	}

	render(renderer, arr, size);

	SDL_Event e;
	bool quit;
	while (!quit) {
		SDL_WaitEvent(&e);
		switch (e.type) {

		case SDL_KEYDOWN: 
			switch (e.key.keysym.sym) {
			case 'r':
				break;
			case 'q':
				quit = true;
				continue;
			}
			break;

		case SDL_QUIT:
			quit = true;
			continue;

		case SDL_MOUSEBUTTONDOWN:
			int8_t *flat_arr = (int8_t *)arr;
			if (full(flat_arr) || winner(flat_arr))
					continue;
			int x, y;
			SDL_GetMouseState(&x, &y);
			if (arr[x/(size/3)][y/(size/3)])
				continue;
			arr[x/(size/3)][y/(size/3)] = 1;
			if (full(flat_arr) || winner(flat_arr))
				break;
			flat_arr[minimax(flat_arr)] = -1;
			break;

		default:
			continue;
		}

		render(renderer, arr, size);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
