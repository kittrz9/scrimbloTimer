#ifndef FILE_DIALOG_H
#define FILE_DIALOG_H

#include <SDL3/SDL.h>

typedef enum {
	WAITING_OPEN,
	WAITING_SAVE,
	WAITING_NONE,
	WAITING_EXIT,
} waitingStateEnum;

extern waitingStateEnum waitingState;
void timerLoadDialog(SDL_Window* w);
void timerSaveDialog(SDL_Window* w);

#endif // FILE_DIALOG_H
