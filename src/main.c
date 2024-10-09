#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include <SDL3/SDL.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define WINDOW_WIDTH 300
#define WINDOW_HEIGHT 100

//char charMap[] = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

uint8_t getCharMapIndex(char c) {
	// will probably change this function in the future if I want to make font loading different
	return c - ' ';
}

typedef enum {
	TEXT_ALIGN_LEFT,
	TEXT_ALIGN_RIGHT,
	TEXT_ALIGN_CENTER,
} align_enum;

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 16

void drawStr(SDL_Renderer* renderer, SDL_Texture* fontTexture, float x, float y, float scale, char* str, align_enum align) {
	uint32_t i = 0;
	switch(align) {
		case TEXT_ALIGN_LEFT:
		default:
			break;
		case TEXT_ALIGN_RIGHT:
			x -= CHAR_WIDTH*scale * strlen(str);
			break;
		case TEXT_ALIGN_CENTER:
			x -= (CHAR_WIDTH/2)*scale * strlen(str);
			break;
	}
	while(*str != '\0') {
		SDL_FRect srcRect = {
			.x = CHAR_WIDTH*getCharMapIndex(*str),
			.y = 0,
			.w = CHAR_WIDTH,
			.h = CHAR_HEIGHT,
		};
		SDL_FRect dstRect = {
			.x = x+(i*CHAR_WIDTH*scale),
			.y = y,
			.w = CHAR_WIDTH*scale,
			.h = CHAR_HEIGHT*scale,
		};
		SDL_RenderTexture(renderer, fontTexture, &srcRect, &dstRect);

		++i;
		++str;
	}
}

//https://stackoverflow.com/questions/53708076/what-is-the-proper-way-to-use-clock-gettime

void timespecDiff(struct timespec t1, struct timespec t2, struct timespec* tDiff) {
	tDiff->tv_nsec = t2.tv_nsec - t1.tv_nsec;
	tDiff->tv_sec = t2.tv_sec - t1.tv_sec;
	if(tDiff->tv_sec > 0 && tDiff->tv_nsec < 0) {
		tDiff->tv_nsec += 1000000000;
		--tDiff->tv_sec;
	} else if(tDiff->tv_sec < 0 && tDiff->tv_nsec > 0) {
		tDiff->tv_nsec -= 1000000000;
		++tDiff->tv_sec;
	}
}

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

	int fontW, fontH, fontC;
	uint8_t* fontData = stbi_load("font.png", &fontW, &fontH, &fontC, 4); // maybe could just include the image in the executable to not have to deal with files
	SDL_Surface* fontSurface = SDL_CreateSurface(fontW, fontH, SDL_PIXELFORMAT_RGBA32);
	uint8_t* surfacePtr = fontSurface->pixels;
	uint8_t* dataPtr = fontData;
	for(uint8_t y = 0; y < fontH; ++y) {
		memcpy(surfacePtr, dataPtr, fontW*4);
		surfacePtr += fontSurface->pitch;
		dataPtr += fontW*4;
	}

	SDL_Texture* fontTexture = SDL_CreateTextureFromSurface(renderer, fontSurface);
	SDL_SetTextureScaleMode(fontTexture, SDL_SCALEMODE_NEAREST);

	SDL_DestroySurface(fontSurface);

	uint8_t running = 1;

	struct timespec start;
	struct timespec now;

	clock_gettime(CLOCK_REALTIME, &start);
	now = start;

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

		struct timespec diff;
		timespecDiff(start, now, &diff);

		uint8_t centiseconds = diff.tv_nsec / 10000000;
		uint8_t seconds = diff.tv_sec % 60;
		uint8_t minutes = (diff.tv_sec / 60) % 60;
		uint16_t hours = (diff.tv_sec / 3600);

		char str[16];
		sprintf(str, "%02i:%02i:%02i.%02i", hours, minutes, seconds, centiseconds);

		SDL_SetTextureColorMod(fontTexture, 0,255,0);
		drawStr(renderer, fontTexture, 0,0,3, str, TEXT_ALIGN_LEFT);
		SDL_RenderPresent(renderer);
		clock_gettime(CLOCK_REALTIME, &now);
	}

	SDL_DestroyTexture(fontTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(w);
	SDL_Quit();
	return 0;
}
