#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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

void drawStr(float x, float y, float scale, SDL_Surface* fontSurface, SDL_Surface* dstSurface, char* str, align_enum align) {
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
		SDL_BlitSurfaceScaled(fontSurface, &(SDL_Rect){.x=8*getCharMapIndex(*str),.y=0,.w=CHAR_WIDTH,.h=CHAR_HEIGHT}, dstSurface, &(SDL_Rect){.x=x+(i*CHAR_WIDTH*scale), .y=y, .w=CHAR_WIDTH*scale, .h=CHAR_HEIGHT*scale}, SDL_SCALEMODE_NEAREST);

		++i;
		++str;
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

	SDL_Surface* windowSurface = SDL_GetWindowSurface(w);
	if(windowSurface == NULL) {
		printf("could not get window surface: %s\n", SDL_GetError());
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
		SDL_FillSurfaceRect(windowSurface, NULL, 0);

		drawStr(0,0,2,fontSurface,windowSurface,"0:00.00", TEXT_ALIGN_LEFT);

		SDL_UpdateWindowSurface(w);
	}

	SDL_DestroyWindowSurface(w);
	SDL_DestroyWindow(w);
	SDL_Quit();
	return 0;
}
