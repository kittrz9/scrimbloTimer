#ifndef KEYS_H
#define KEYS_H

#include <stdint.h>

typedef struct {
	int keycode;
	uint8_t lastKeyState;
	void (*onPress)(void);
} key;

void checkHotkeys(void);

#endif // KEYS_H
