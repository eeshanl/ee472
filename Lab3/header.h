// header file containing prototype of all used methods

#include "inc/lm3s8962.h"
#include "drivers/rit128x96x4.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/cpu.h"

#ifndef HEADER_H
#define HEADER_H

extern void LED_toggle();
extern void LED_init();
extern void key_init();
extern void display_init();
extern void pulseE();
extern void portBD_init();
extern void display_write(int reset, char w);
extern void display_string(char* g);
extern void clear_display();
extern void shiftRight();
extern void shiftLeft();
extern void cursorLeft();
extern void cursorRight();
extern void startGame(int level);
extern int checkInput(char input, int key);
extern void goToStateOne();
extern void countDown();
extern void WinOrLose();
extern void mainMenuForGTH();
extern void game(unsigned long int timeLimit);
extern int getRandomNumber();
extern int replay();

extern void Timer0IntHandler(void);
extern void Timer1EventHandler(void);
extern void timer0_init();
extern void delay(unsigned long aValue);
extern void ADC_init();
extern void timer1_init();
extern void debugHeight(int dist);
extern int getKey();
extern int is_a_key();
extern int keymaster();
extern int fresh_key();
extern void init_GPIOE();
extern void GPIOEventHandler();
extern void init_GPIOF(void);
extern int getDebounce();
extern unsigned long int getTimer1();
extern unsigned long int getSeed();
extern void timer2_init();
extern void Timer2EventHandler(void);

#endif
