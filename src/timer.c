#include "timer.h"

#include <stdio.h>
#include <time.h>

#include "text.h"

timerStateEnum timerState;

struct timespec timerStart;
struct timespec timerNow;
struct timespec timerDiff;

struct timespec timerBest;

enum {
	TIMER_RESULT_SAME,
	TIMER_RESULT_SLOWER,
	TIMER_RESULT_FASTER,
} timerResult;

// https://stackoverflow.com/questions/53708076/what-is-the-proper-way-to-use-clock-gettime
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

int timespecCmp(struct timespec t1, struct timespec t2) {
	if(t1.tv_sec == t2.tv_sec) {
		if(t1.tv_nsec < t2.tv_nsec) {
			return -1;
		} else if(t1.tv_nsec > t2.tv_nsec) {
			return 1;
		} else {
			return 0;
		}
	}
	if(t1.tv_sec < t2.tv_sec) {
		return -1;
	} else if(t1.tv_sec > t2.tv_sec) {
		return 1;
	} else {
		return 0;
	}
}

void startStopTimer(void) {
	switch(timerState) {
		case TIMER_STATE_START:
			clock_gettime(CLOCK_REALTIME, &timerStart);
			timerState = TIMER_STATE_TIMING;
			break;
		case TIMER_STATE_TIMING:
			timerState = TIMER_STATE_FINISH;
			int comparison = timespecCmp(timerDiff, timerBest);
			if((timerBest.tv_sec == 0 && timerBest.tv_nsec == 0) || comparison < 0) {
				timerBest = timerDiff;
				timerResult = TIMER_RESULT_FASTER;
			} else if(comparison > 0) {
				timerResult = TIMER_RESULT_SLOWER;
			} else {
				timerResult = TIMER_RESULT_SAME;
			}
			break;
		default:
			break;
	}
}

void resetTimer(void) {
	if(timerState == TIMER_STATE_TIMING || timerState == TIMER_STATE_FINISH) {
		clock_gettime(CLOCK_REALTIME, &timerStart);
		timerNow = timerStart;
		timerState = TIMER_STATE_START;
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
	timespecDiff(timerStart, timerNow, &timerDiff);

	uint8_t centiseconds = timerDiff.tv_nsec / 10000000;
	uint8_t seconds = timerDiff.tv_sec % 60;
	uint8_t minutes = (timerDiff.tv_sec / 60) % 60;
	uint16_t hours = (timerDiff.tv_sec / 3600);

	char str[16];
	sprintf(str, "%02i:%02i:%02i.%02i", hours, minutes, seconds, centiseconds);
	switch(timerState) {
		case TIMER_STATE_START:
		case TIMER_STATE_TIMING:
			SDL_SetTextureColorMod(fontTexture, 255,255,255);
			break;
		case TIMER_STATE_FINISH:
			switch(timerResult) {
				case TIMER_RESULT_FASTER:
					SDL_SetTextureColorMod(fontTexture, 0,255,0);
					break;
				case TIMER_RESULT_SLOWER:
					SDL_SetTextureColorMod(fontTexture, 255,0,0);
					break;
				case TIMER_RESULT_SAME:
					SDL_SetTextureColorMod(fontTexture, 0,0,255);
					break;
			}
			break;
		default:
			SDL_SetTextureColorMod(fontTexture, 255,255,255);
			break;
	}
	drawStr(renderer, 0,0,3, str, TEXT_ALIGN_LEFT);
	uint8_t pbCentiseconds = timerBest.tv_nsec / 10000000;
	uint8_t pbSeconds = timerBest.tv_sec % 60;
	uint8_t pbMinutes = (timerBest.tv_sec / 60) % 60;
	uint16_t pbHours = (timerBest.tv_sec / 3600);

	char str2[16];
	sprintf(str2, "PB: %02i:%02i:%02i.%02i", pbHours, pbMinutes, pbSeconds, pbCentiseconds);
	drawStr(renderer, 0,CHAR_HEIGHT*3,2, str2, TEXT_ALIGN_LEFT);
}

