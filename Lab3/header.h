// header file containing prototype of all used methods

#include "inc/lm3s8962.h"
#include "drivers/rit128x96x4.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/cpu.h"

#ifndef HEADER_H
#define HEADER_H

void LED_toggle();
void LED_init();
void key_init();
void display_init();
void pulseE();
void portBD_init();
void display_write(int reset, char w);
void display_string(char* g);
void clear_display();
void shiftRight();
void shiftLeft();
void cursorLeft();
void cursorRight();
void startGame(int level);
int checkInput(char input, int key);
void goToStateOne();
void countDown();
void WinOrLose();
void mainMenuForGTH();
void testGame(unsigned long int timeLimit);
int getRandomNumber();
int replay();

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
int getDebounce();
unsigned long int getTimer1();
unsigned long int getSeed();
extern void timer2_init();
extern void Timer2EventHandler(void);
void speaker_init();











#endif