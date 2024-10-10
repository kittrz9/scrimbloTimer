#ifndef TIMER_H
#define TIMER_H

#include <SDL3/SDL.h>

typedef enum {
	TIMER_STATE_START,
	TIMER_STATE_TIMING,
	TIMER_STATE_FINISH,
} timerStateEnum;

extern timerStateEnum timerState;
extern struct timespec timerStart;
extern struct timespec timerNow;

void processTimer(void);
void drawTimer(SDL_Renderer* renderer);

#endif // TIMER_H
