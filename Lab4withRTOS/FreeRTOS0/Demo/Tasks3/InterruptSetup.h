// header file containing prototypes of keypad functions

#ifndef INTERRUPTSETUP_H
#define INTERRUPTSETUP_H

void init_GPIOE();
void init_GPIOF();
void GPIOEventHandler();
extern int debounce;

#endif
