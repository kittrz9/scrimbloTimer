#include "fileDialog.h"

#include <stdio.h>
#include <stdlib.h>

#include "timer.h"

#include "timesFiles.h"

enum {
	MSGBOX_YES,
	MSGBOX_NO,
};

waitingStateEnum waitingState = WAITING_NONE;
SDL_MessageBoxButtonData msgBoxButtons[] = {
	{
		.flags = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,
		.buttonID = MSGBOX_YES,
		.text = "yeah",
	},
	{
		.flags = SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT,
		.buttonID = MSGBOX_NO,
		.text = "nope",
	}
};

SDL_MessageBoxData msgBox = {
	.flags = SDL_MESSAGEBOX_INFORMATION,
	.window = NULL,
	.title = "scrimbloTimer",
	.message = "waga baba bobo",
	.numbuttons = 2,
	.buttons = msgBoxButtons,
	.colorScheme = NULL,
};

int buttonResult;


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
	msgBox.message = "would you like to load a personal best?";
	SDL_ShowMessageBox(&msgBox, &buttonResult);
	if(buttonResult == MSGBOX_YES) {
		waitingState = WAITING_OPEN;
		SDL_ShowOpenFileDialog(&timerOpenCallback, &callbackPtrs, w, NULL, 0, NULL, 0);
	}
}

void timerSaveDialog(SDL_Window* w) {
	msgBox.message = "would you like to save your personal best?";
	SDL_ShowMessageBox(&msgBox, &buttonResult);
	if(buttonResult == MSGBOX_YES) {
		waitingState = WAITING_SAVE;
		SDL_ShowSaveFileDialog(&timerSaveCallback, &callbackPtrs, w, NULL, 0, NULL);
	} else {
		waitingState = WAITING_EXIT;
	}
}
