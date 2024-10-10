#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include <SDL3/SDL.h>

#include "keys.h"
#include "timer.h"
#include "text.h"

#define WINDOW_WIDTH 300
#define WINDOW_HEIGHT 100

int main(int argc, char** argv) {
	if(!SDL_Init(SDL_INIT_VIDEO)) {
		printf("could not init SDL: %s\n", SDL_GetError());
		exit(1);
	}
	SDL_Window* w = SDL_CreateWindow("deez", WINDOW_WIDTH, WINDOW_HEIGHT, 0);

	if(w == NULL) {
		printf("could not create window: %s\n", SDL_GetError());
		exit(1);
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(w, NULL);
	if(renderer == NULL) {
		printf("could not create rendere: %s\n", SDL_GetError());
		exit(1);
	}

	loadFont(renderer);

	uint8_t running = 1;

	SDL_Event e;
	while(running) {
		while(SDL_PollEvent(&e)) {
			switch(e.type) {
				case SDL_EVENT_QUIT:
					running = 0;
				default:
					break;
			}
		}

		SDL_SetRenderDrawColor(renderer, 0,0,0,255);
		SDL_RenderClear(renderer);
		checkHotkeys();

		processTimer();
		drawTimer(renderer);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyTexture(fontTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(w);
	SDL_Quit();
	return 0;
}
