#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "inc/lm3s8962.h"
#include "drivers/rit128x96x4.h"
#include "driverlib/cpu.h"

#ifndef TIMERSETUP_H
#define TIMERSETUP_H

extern void Timer0IntHandler(void);
extern void Timer1EventHandler(void);
extern void timer0_init();
extern void delay(unsigned long aValue);
extern void LED_init();
extern void LED_toggle();
extern void ADC_init();
extern void timer1_init();
extern void debugHeight(int dist);
extern void key_init();
extern int getKey();
extern int is_a_key();
extern int keymaster();
extern int fresh_key();
extern void init_GPIOE();
extern void GPIOEventHandler();
extern void init_GPIOF(void);
void mainMenu();
void goToStateOne();
void game(int level);
void startGame(int level);
void countDown();
unsigned long int getTimer1();
unsigned long int getSeed();
extern void timer2_init();
extern void Timer2EventHandler(void);
void speaker_init();

#endif