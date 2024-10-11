#ifndef KEYS_H
#define KEYS_H

#include <stdint.h>

typedef struct {
	char* name;
	int keycode;
	uint8_t lastKeyState;
	void (*onPress)(void);
} key;

void checkHotkeys(void);

key* getHotkeyFromName(char* name);

#endif // KEYS_H
