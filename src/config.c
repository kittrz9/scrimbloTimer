#include "config.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <X11/Xlib.h>

#include "keys.h"

#define CONFIG_PATH_MAX_LEN 256
#define CONFIG_FILE_NAME "scrimbloTimer.conf"

#define CONFIG_FORMAT_VERSION_STRING "SCRIMBLO_CONFIG_FORMAT_V0"

#define MAX_STR_LEN 256

#define CONFIG_DEFAULT \
CONFIG_FORMAT_VERSION_STRING "\n" \
"\n" \
"# setting up a hotkey works by putting \"HOTKEY\" on one line,\n" \
"# the hotkey's name on the next (found in src/keys.c),\n" \
"# and the key you want to set it to on the one after that (key names are in `/usr/include/X11/keysymdef.h`)\n" \
"# you will need to remove the \"XK_\" from the start of the key's name\n" \
"\n" \
"HOTKEY\n" \
"START_STOP_TIMER\n" \
"KP_End\n" \
"\n" \
"HOTKEY\n" \
"RESET_TIMER\n" \
"KP_Down\n" \
"\n" \


char configPath[CONFIG_PATH_MAX_LEN];

void loadConfigFile(void) {
	char* cfgDirPath = getenv("XDG_CONFIG_HOME");
	if(cfgDirPath == NULL) {
		char* homePath = getenv("HOME");
		if(strlen(homePath) > CONFIG_PATH_MAX_LEN - strlen("/.config/" CONFIG_FILE_NAME)) {
			printf("config file path too long\n");
			return;
		}
		strcpy(configPath, homePath);
		strcpy(configPath + strlen(homePath), "/.config/" CONFIG_FILE_NAME);
	} else {
		if(strlen(cfgDirPath) > CONFIG_PATH_MAX_LEN - strlen("/.config/" CONFIG_FILE_NAME)) {
			printf("config file path too long\n");
			return;
		}
		strcpy(configPath, cfgDirPath);
		strcpy(configPath + strlen(cfgDirPath), "/.config/" CONFIG_FILE_NAME);
	}
	FILE* f = fopen(configPath, "rb");
	if(f == NULL) {
		printf("could not find config file \"%s\", creating one there with a default config\n", configPath);
		f = fopen(configPath, "wb");
		if(f == NULL) {
			printf("could not create config file\n");
			return;
		}
		fputs(CONFIG_DEFAULT, f);
		fclose(f);
		return;
	}

	char str[MAX_STR_LEN];
	fgets(str, MAX_STR_LEN, f);
	if(strncmp(str, CONFIG_FORMAT_VERSION_STRING, strlen(CONFIG_FORMAT_VERSION_STRING)) != 0) {
		printf("invalid config file \"%s\"\n", configPath);
		fclose(f);
		return;
	}

	while(fgets(str, MAX_STR_LEN, f) != NULL) {
		if(str[0] == '#' || str[0] == '\n') { continue; } // comment or empty
		if(strncmp(str, "HOTKEY", strlen("HOTKEY")) == 0) {
			fgets(str, MAX_STR_LEN, f);
			key* k = getHotkeyFromName(str);
			if(k == NULL) {
				printf("invalid hotkey name \"%s\"\n", str);
				continue;
			}
			printf("hotkey found: %s\n", str);
			fgets(str, MAX_STR_LEN, f);
			printf("%s\n", str);
			// have to get rid of the newline for this function
			str[strlen(str)-1] = '\0';
			int keysym = XStringToKeysym(str);
			if(keysym == NoSymbol) {
				printf("invalid keysym \"%s\", keeping default\n", str);
			} else {
				k->keysym = keysym;
			}
			continue;
		}

		printf("invalid config instruction \"%s\"\n", str);
	}
}
