#include "keys.h"

#include <stdio.h>
#include <time.h>

#include <X11/Xlib.h>
#include <X11/keysym.h>

#include "timer.h"

char keyMap[32];
Display* disp = NULL;


key keys[] = {
	{
		.name = "START_STOP_TIMER",
		.keysym = XK_KP_End,
		.onPress = startStopTimer,
	},
	{
		.name = "RESET_TIMER",
		.keysym = XK_KP_Down,
		.onPress = resetTimer,
	},
};

void initHotkeys(void) {
	disp = XOpenDisplay(NULL);
	for(uint8_t i = 0; i < sizeof(keys)/sizeof(key); ++i) {
		keys[i].keycode = XKeysymToKeycode(disp, keys[i].keysym);
	}
}

void checkHotkeys(void) {
	// based on how sfml does keyboard handling
	// https://github.com/SFML/SFML/blob/758f0804b8e69a2983093fc1a6d655346bee355c/src/SFML/Window/Unix/KeyboardImpl.cpp#L582
	XQueryKeymap(disp, keyMap);
	for(size_t i = 0; i < sizeof(keys)/sizeof(key); ++i) {
		KeyCode k = keys[i].keycode;
		uint8_t keyState = (keyMap[k/8] & (1 << k%8));
		if(keyState != 0 && keys[i].lastKeyState != keyState) {
			(*keys[i].onPress)();
		}
		keys[i].lastKeyState = keyState;
	}
}

key* getHotkeyFromName(char* name) {
	for(uint8_t i = 0; i < sizeof(keys)/sizeof(key); ++i) {
		if(strncmp(keys[i].name, name, strlen(keys[i].name)) == 0) {
			return &keys[i];
		}
	}
	return NULL;
}
