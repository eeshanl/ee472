#include "inc/lm3s8962.h"

#ifndef HEADER_H
#define HEADER_H

void delay(unsigned long aValue);
void LED_toggle();
void LED_init();
void key_init();
int getKey();
int is_a_key();
int keymaster();
int debounce();
int fresh_key();
void display_init();
void pulseE();
void portBD_init();
void display_write(int reset, char w);
void display_string(char* g);
void clear_display();

#endif
