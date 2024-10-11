#ifndef TIMES_FILES_H
#define TIMES_FILES_H

#include <time.h>

void timerLoadFromFile(char* filePath, struct timespec* pb);
void timerSaveToFile(char* filePath, struct timespec* pb);

#endif // TIMES_FILES_H
