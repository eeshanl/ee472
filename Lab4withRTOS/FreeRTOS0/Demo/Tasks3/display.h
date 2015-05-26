#ifndef DISPLAY_H
#define DISPLAY_H

void vTaskDisplay(void *vParameters);
void vPrintDistance(void *vParameters);
void printState2Or3(int y);
void printADCString();

extern int state;

#endif