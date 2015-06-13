// Prototypes for the display functions

#ifndef DISPLAY_H
#define DISPLAY_H

void vTaskDisplay(void *vParameters);
void vPrintDistance(void *vParameters);
void vBlinkLED(void* vParameters);
void printState2Or3(int y);
void printADCString();
void chooseSpeed();

extern int state;
extern int auton;
extern int fast;

#endif