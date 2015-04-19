#include "header.h"
//for the random number generator
#include <stdlib.h>
#include <time.h>

void pulseE() {
  GPIO_PORTD_DATA_R ^= 0x20;
  delay(100);
  GPIO_PORTD_DATA_R ^= 0x20;
}

void portBD_init() {
  SYSCTL_RCGC2_R |= 0xA;
  delay(100);
  GPIO_PORTD_DATA_R &= 0x0;
  GPIO_PORTB_DATA_R &= 0x0;
  GPIO_PORTB_DIR_R |= 0xFF;
  GPIO_PORTB_AFSEL_R &= 0x00000000;
  GPIO_PORTB_DEN_R |= 0xFF;
  GPIO_PORTD_DIR_R |= 0xE0;
  GPIO_PORTD_AFSEL_R &= 0x00000000;
  GPIO_PORTD_DEN_R |= 0xE0;
}

void display_init(){
  portBD_init();
  pulseE();
 
  GPIO_PORTB_DATA_R |= 0x30;
  pulseE();
  
  GPIO_PORTB_DATA_R |= 0x30;
  pulseE();
  
  GPIO_PORTB_DATA_R |= 0x30;
  pulseE();

  GPIO_PORTB_DATA_R |= 0x38; //0x30
  pulseE();

  GPIO_PORTB_DATA_R &= 0;
  GPIO_PORTB_DATA_R |= 0x8;
  pulseE();

  GPIO_PORTB_DATA_R &= 0;
  GPIO_PORTB_DATA_R |= 0x1;
  pulseE();

  GPIO_PORTB_DATA_R &= 0;
  GPIO_PORTB_DATA_R |= 0x6;
  pulseE();

  GPIO_PORTB_DATA_R &= 0;
  GPIO_PORTB_DATA_R |= 0xF;//0xF
  pulseE();
 
}

void display_write(int reset, char w) {
  
  if (reset) {
    GPIO_PORTD_DATA_R |= 0x80; // turns only reset on
  } else {
    GPIO_PORTD_DATA_R &=0x7F; // Turns only reset off
  }
  delay(100);
  GPIO_PORTD_DATA_R ^= 0x20; // Flips enable
  delay(50);
  GPIO_PORTB_DATA_R = w; // Set the char
  delay(50);
  GPIO_PORTD_DATA_R ^= 0x20; // Unflips enable
  
}

void clear_display() {
  GPIO_PORTB_DATA_R &= 0;
  GPIO_PORTB_DATA_R |= 0x1;
  pulseE();
}

void display_string(char* g) { // Guess and Check all the way
  while (*g) {
    display_write(1, *g);
    g++;
  }
}

//to create the array of different options available to display on the LCD for Level 1
//returns a different action, 
char string_combos(){
  srand( time(NULL) ); // hackish but gets the job done.
  int x;
  x = rand() % 4; // everytime it is different because the seed is different.
  char easyCombos[4] = {'<','>'','^','v'};
  return easyCombos[x];                        
}

/* Possible options for randomizing:
    srand( time(NULL) ); // hackish but gets the job done.
    int x;
    x = rand() % 6; // everytime it is different because the seed is different.
    printf("%d", x);
    */
