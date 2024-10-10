#ifndef TIMER_H
#define TIMER_H

#include <SDL3/SDL.h>

#include <time.h>

typedef enum {
	TIMER_STATE_START,
	TIMER_STATE_TIMING,
	TIMER_STATE_FINISH,
} timerStateEnum;

extern timerStateEnum timerState;
extern struct timespec timerStart;
extern struct timespec timerNow;
extern struct timespec timerBest;

void resetTimer(void);
void startStopTimer(void);

void processTimer(void);
void drawTimer(SDL_Renderer* renderer);

extern uint8_t timerSaveWait;

void SDLCALL timerSaveCallback(void* userdata, const char* const* fileList, int filter);
void SDLCALL timerOpenCallback(void* userdata, const char* const* fileList, int filter);

#endif // TIMER_H
