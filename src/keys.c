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
		.keycode = XK_KP_End,
		.onPress = startStopTimer,
	},
	{
		.keycode = XK_KP_Down,
		.onPress = resetTimer,
	},
};

void checkHotkeys(void) {
	if(disp == NULL) {
		disp = XOpenDisplay(NULL);
	}
	// based on how sfml does keyboard handling
	// https://github.com/SFML/SFML/blob/758f0804b8e69a2983093fc1a6d655346bee355c/src/SFML/Window/Unix/KeyboardImpl.cpp#L582
	XQueryKeymap(disp, keyMap);
	for(size_t i = 0; i < sizeof(keys)/sizeof(key); ++i) {
		KeyCode k = XKeysymToKeycode(disp, keys[i].keycode);
		uint8_t keyState = (keyMap[k/8] & (1 << k%8));
		if(keyState != 0 && keys[i].lastKeyState != keyState) {
			(*keys[i].onPress)();
		}
		keys[i].lastKeyState = keyState;
	}
}
