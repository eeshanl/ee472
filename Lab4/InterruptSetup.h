// header file containing prototypes of keypad functions

#ifndef INTERRUPTSETUP_H
#define INTERRUPTSETUP_H



unsigned long int getTimer1();
void init_GPIOE();
void init_GPIOF();
void GPIOEventHandler();
void Timer0IntHandler();
void debugHeight(int dist, int y);
extern int getDebounce();
void timer0_init();
void timer1_init();
void timer2_init();

#endif
