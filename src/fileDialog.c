#include "fileDialog.h"

#include <stdio.h>
#include <stdlib.h>

#include "timer.h"

#include "timesFiles.h"

waitingStateEnum waitingState = WAITING_NONE;

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
	timerSaveToFile(fileList[0], pointers->timerBestPtr);

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
	timerLoadFromFile(fileList[0], pointers->timerBestPtr);

	*(pointers->waitingStatePtr)= WAITING_NONE;

	return;
}

void timerLoadDialog(SDL_Window* w) {
	waitingState = WAITING_OPEN;
	SDL_ShowOpenFileDialog(&timerOpenCallback, &callbackPtrs, w, NULL, 0, NULL, 0);
}

void timerSaveDialog(SDL_Window* w) {
	waitingState = WAITING_SAVE;
	SDL_ShowSaveFileDialog(&timerSaveCallback, &callbackPtrs, w, NULL, 0, NULL);
}
