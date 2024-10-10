#include "fileDialog.h"

#include <stdio.h>
#include <stdlib.h>

#include "timer.h"

#define FORMAT_VERSION_STRING "SCRIMBLO_TIME_FORMAT_V0"

waitingStateEnum waitingState;

// have to do these shenanigans because of dumb async stuff
typedef struct {
	waitingStateEnum* waitingStatePtr;
	struct timespec* timerBestPtr;
} callbackPointerStruct;

callbackPointerStruct callbackPtrs = {
	.waitingStatePtr = &waitingState,
	.timerBestPtr = &timerBest,
};

void SDLCALL timerSaveCallback(void* userdata, const char* const* fileList, int filter) {
	callbackPointerStruct* pointers = (callbackPointerStruct*)userdata;
	if(fileList == NULL) {
		printf("SDL returned a NULL file list, an error probably ocurred\n");
		*(pointers->waitingStatePtr)= WAITING_EXIT;
		return;
	}
	if(fileList[0] == NULL) {
		printf("SDL returned empty file list, save dialog might've been cancelled\n");
		*(pointers->waitingStatePtr)= WAITING_EXIT;
		return;
	}

	// only care about the first option given
	FILE* f = fopen(fileList[0], "wb");

	if(f == NULL) {
		printf("could not open file to save to\n");
		return;
	}

	fprintf(f, FORMAT_VERSION_STRING "\n%li\n%li", pointers->timerBestPtr->tv_sec, pointers->timerBestPtr->tv_nsec);

	fclose(f);

	*(pointers->waitingStatePtr)= WAITING_EXIT;

	return;
}

void SDLCALL timerOpenCallback(void* userdata, const char* const* fileList, int filter) {
	callbackPointerStruct* pointers = (callbackPointerStruct*)userdata;
	if(fileList == NULL) {
		printf("SDL returned a NULL file list, an error probably ocurred\n");
		*(pointers->waitingStatePtr)= WAITING_NONE;
		return;
	}
	if(fileList[0] == NULL) {
		printf("SDL returned empty file list, open dialog might've been cancelled\n");
		*(pointers->waitingStatePtr)= WAITING_NONE;
		return;
	}

	// only care about the first option given
	FILE* f = fopen(fileList[0], "rb");

	char str[64];
	fgets(str, 64, f);
	if(strncmp(str, FORMAT_VERSION_STRING, strlen(FORMAT_VERSION_STRING)) == 0) {
		fgets(str, 64, f);
		pointers->timerBestPtr->tv_sec = atoi(str);
		fgets(str, 64, f);
		pointers->timerBestPtr->tv_nsec = atoi(str);
	} else {
		printf("invalid times file\n");
	}

	fclose(f);

	*(pointers->waitingStatePtr)= WAITING_NONE;

	return;
}

void timerLoad(SDL_Window* w) {
	waitingState = WAITING_OPEN;
	SDL_ShowOpenFileDialog(&timerOpenCallback, &callbackPtrs, w, NULL, 0, NULL, 0);
}

void timerSave(SDL_Window* w) {
	waitingState = WAITING_SAVE;
	SDL_ShowSaveFileDialog(&timerSaveCallback, &callbackPtrs, w, NULL, 0, NULL);
}
