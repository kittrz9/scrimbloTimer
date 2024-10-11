#include "timesFiles.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TIME_FORMAT_VERSION_STRING "SCRIMBLO_TIME_FORMAT_V0"

void timerLoadFromFile(char* filePath, struct timespec* pb) {
	FILE* f = fopen(filePath, "rb");

	char str[64];
	fgets(str, 64, f);
	if(strncmp(str, TIME_FORMAT_VERSION_STRING, strlen(TIME_FORMAT_VERSION_STRING)) == 0) {
		fgets(str, 64, f);
		pb->tv_sec = atoi(str);
		fgets(str, 64, f);
		pb->tv_nsec = atoi(str);
	} else {
		printf("invalid times file\n");
	}

	fclose(f);
}

void timerSaveToFile(char* filePath, struct timespec* pb) {
	FILE* f = fopen(filePath, "wb");

	if(f == NULL) {
		printf("could not open file to save to\n");
		return;
	}

	fprintf(f, TIME_FORMAT_VERSION_STRING "\n%li\n%li\n", pb->tv_sec, pb->tv_nsec);

	fclose(f);
}
