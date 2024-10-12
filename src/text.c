#include "text.h"

#include <SDL3/SDL.h>

#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SDL_Texture* fontTexture = NULL;
//char charMap[] = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

uint8_t getCharMapIndex(char c) {
	// will probably change this function in the future if I want to make font loading different
	return c - ' ';
}

void loadFont(SDL_Renderer* renderer) {
	int fontW, fontH, fontC;
	uint8_t* fontData = stbi_load("font.png", &fontW, &fontH, &fontC, 4); // maybe could just include the image in the executable to not have to deal with files
	if(fontData == NULL) {
		fontData = stbi_load("../font.png", &fontW, &fontH, &fontC, 4); // have to do this since I keep font.png out of the build folder when developing
		if(fontData == NULL) {
			printf("could not load font\n");
			exit(1);
		}
	}
	SDL_Surface* fontSurface = SDL_CreateSurface(fontW, fontH, SDL_PIXELFORMAT_RGBA32);
	uint8_t* surfacePtr = fontSurface->pixels;
	uint8_t* dataPtr = fontData;
	for(uint8_t y = 0; y < fontH; ++y) {
		memcpy(surfacePtr, dataPtr, fontW*4);
		surfacePtr += fontSurface->pitch;
		dataPtr += fontW*4;
	}

	fontTexture = SDL_CreateTextureFromSurface(renderer, fontSurface);
	SDL_SetTextureScaleMode(fontTexture, SDL_SCALEMODE_NEAREST);

	SDL_DestroySurface(fontSurface);
	stbi_image_free(fontData);
}

void drawStr(SDL_Renderer* renderer, float x, float y, float scale, char* str, align_enum align) {
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
