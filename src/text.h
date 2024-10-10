#ifndef TEXT_H
#define TEXT_H

#include <SDL3/SDL.h>

uint8_t getCharMapIndex(char c);

extern SDL_Texture* fontTexture;

typedef enum {
	TEXT_ALIGN_LEFT,
	TEXT_ALIGN_RIGHT,
	TEXT_ALIGN_CENTER,
} align_enum;

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 16

void loadFont(SDL_Renderer* renderer);
void drawStr(SDL_Renderer* renderer, float x, float y, float scale, char* str, align_enum align);

#endif // TEXT_H
