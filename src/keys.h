#ifndef KEYS_H
#define KEYS_H

#include <stdint.h>

#include <X11/Xlib.h>

typedef struct {
	char* name;
	int keysym;
	KeyCode keycode;
	uint8_t lastKeyState;
	void (*onPress)(void);
} key;

void checkHotkeys(void);
void initHotkeys(void);

key* getHotkeyFromName(char* name);

#endif // KEYS_H
