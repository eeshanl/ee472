#include "header.h"

void pulseE() {
  GPIO_PORTD_DATA_R ^= 0x20;
  delay(50);
  GPIO_PORTD_DATA_R ^= 0x20;
}

//SET UP ACCORDING TO 
//post: initializes all the ports on the board
//      the 8 Data Bits (DB 7 - DB 0) 
//      
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

//post: initializes the display on the LCD Screen to enable 
//      writing to the display
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

// pre: takes in reset and the char as parameters
// post: writes a character to the LCD Display on reset
void display_write(int reset, char w) {
  if (reset) {
    GPIO_PORTD_DATA_R |= 0x80; // turns only reset on
  } else {
    GPIO_PORTD_DATA_R &=0x7F; // Turns only reset off
  }
  //delay for data to be interpreted
  delay(5);
  GPIO_PORTD_DATA_R ^= 0x20; // Flips enable
  delay(5);
  GPIO_PORTB_DATA_R = w; // Set the char
  delay(5);
  GPIO_PORTD_DATA_R ^= 0x20; // Unflips enable
}

//post: clears the current display on the LCD Display
void clear_display() {
  GPIO_PORTB_DATA_R = 0x1;
  GPIO_PORTD_DATA_R &= 0;
  //pulse, for data to be interpreted 
  pulseE();
}

//post: shifts entire display to the right on the LCD screen
void shiftRight() {
  GPIO_PORTB_DATA_R = 0x1C;
  GPIO_PORTD_DATA_R &= 0;
  pulseE();
}

//post: moves the cursor in the LCD Display to the right on the LCD Display
void cursorRight() {
  pulseE();
  GPIO_PORTB_DATA_R = 0x14;
  GPIO_PORTD_DATA_R &= 0;
  pulseE();
}

//post: moves the cursor in the LCD Display to the left on the LCD Display
void cursorLeft() {
  GPIO_PORTB_DATA_R = 0x10;
  GPIO_PORTD_DATA_R &= 0;
  pulseE();
}

//post: shifts entire display to the left on the LCD screen
void shiftLeft() {
  GPIO_PORTB_DATA_R = 0x14;
  GPIO_PORTD_DATA_R &= 0;
  pulseE();
}

//pre: takes in a String(array of char) to be printed out on the LCD
//post: displays the string on the LCD diplay
void display_string(char* g) {
  //iterating till you hit the null character (\0)
  while (*g) {
    display_write(1, *g);
    g++;
  }
}
