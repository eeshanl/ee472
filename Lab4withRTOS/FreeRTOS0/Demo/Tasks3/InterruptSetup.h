// header file containing prototypes of keypad functions

#ifndef INTERRUPTSETUP_H
#define INTERRUPTSETUP_H



unsigned long int getTimer1();
void init_GPIOE();
void init_GPIOF();
void GPIOEventHandler();
extern int getDebounce();
#endif
