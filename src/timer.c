#include "timer.h"

#include <stdio.h>
#include <time.h>

#include "text.h"

timerStateEnum timerState;

struct timespec timerStart;
struct timespec timerNow;

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

void processTimer(void) {
	switch(timerState) {
		case TIMER_STATE_START:
			break;
		case TIMER_STATE_TIMING:
			clock_gettime(CLOCK_REALTIME, &timerNow);
			break;
		default:
			break;
	}
}
void drawTimer(SDL_Renderer* renderer) {
	struct timespec diff;
	timespecDiff(timerStart, timerNow, &diff);

	uint8_t centiseconds = diff.tv_nsec / 10000000;
	uint8_t seconds = diff.tv_sec % 60;
	uint8_t minutes = (diff.tv_sec / 60) % 60;
	uint16_t hours = (diff.tv_sec / 3600);

	char str[16];
	sprintf(str, "%02i:%02i:%02i.%02i", hours, minutes, seconds, centiseconds);
	switch(timerState) {
		case TIMER_STATE_START:
		case TIMER_STATE_TIMING:
			SDL_SetTextureColorMod(fontTexture, 255,255,255);
			break;
		case TIMER_STATE_FINISH:
			SDL_SetTextureColorMod(fontTexture, 0,255,0);
			break;
		default:
			SDL_SetTextureColorMod(fontTexture, 255,255,255);
			break;
	}
	drawStr(renderer, 0,0,3, str, TEXT_ALIGN_LEFT);
}
