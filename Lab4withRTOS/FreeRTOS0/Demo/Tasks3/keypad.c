/* This file is responsible for the actions related to the key presses on the board */
#include "keypad.h"
#include "InterruptSetup.h"
#include "main.h"

#include "inc/lm3s8962.h"
#include "drivers/rit128x96x4.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/cpu.h"

// global int that stores the value of the last key pressed
// lastKey is initially set to 0, as no key is pressed in the beginning
int lastKey = 0;

//post: returns whether the current data bit in the GPIO ports
//      corresponds to a key
//      1 ==> key
//      0 ==> not a key
int is_a_key() {
  int bit1 = GPIO_PORTF_DATA_R & 0x2;
  if (GPIO_PORTE_DATA_R != 0xF || bit1 == 0) {
    return 1;
  } else {
    return 0;
  }
}

//returns an int value that corresponds to a new instance of a key
//press. The lastKey flag is used to help figure this out.
//return value 1 ==> key is a fresh key
//return value 0 ==> key is not a fresh key
int fresh_key(){
  int key = getKey();
  if (lastKey == key) {
    return 0;
  } else {
    lastKey = key;
    return 1;
  }
}

//post: returns an int value of the key being pressed
int keymaster() {
  while (is_a_key()) { //checks for valid key
    if (debounce) { //checks if the same key is being pressed for 100 miliseconds
      if (fresh_key()) { //if the key has not been pressed before
        return lastKey;
      }
    }
  }
  lastKey = 0; //need to reset the lastKey to 0, if no key was pressed
  return 0;
}

//returns an int value that corresponds to one of the following values:
//      the assignment is as follows:
//      return value 1 ==> up
//      return value 2 ==> down
//      return value 3 ==> left
//      return value 4 ==> right
//      return value 5 ==> select
//      return value 6 ==> upLeft
//      return value 7 ==> upRight
//      return value 8 ==> backLeft
//      return value 9 ==> backRight
//      return value 0 ==> no key pressed or invalid presses
int getKey() {
  // sets the keys to their ports on the board
  int up = ~GPIO_PORTE_DATA_R & 0x1;
  int down = ~GPIO_PORTE_DATA_R & 0x2;
  int left = ~GPIO_PORTE_DATA_R & 0x4;
  int right = ~GPIO_PORTE_DATA_R & 0x8;
  int select = ~GPIO_PORTF_DATA_R & 0x2;
  int upLeft = up && left;
  int upRight = up && right;
  int downLeft = down && left;
  int downRight = down && right;
  int upDown = up && down;
  int leftRight = left && right;
  
  // checks which key is pressed and returns the int it corresponds to
  if (upDown || leftRight) {
    return 0;
  } else if (upLeft) {
    return 6;
  } else if (upRight) {
    return 7;
  } else if (downLeft) {
    return 8;
  } else if (downRight) {
    return 9;
  } else if (up) {
    return 1;
  } else if (down) {
    return 2;
  } else if (left) {
    return 3;
  } else if (right) {
    return 4;
  } else if (select) {
    return 5;
  } else {
    return 0;
  }
}

//post: initializes the keys on the board to their repective ports on the board
void key_init() {
  SYSCTL_RCGC2_R |= 0x00000010; // enables port E
  delay(100);
  
  GPIO_PORTE_AFSEL_R &= 0x0; // turns off alternate functions for port E
  
  GPIO_PORTE_DEN_R |= 0xF; // turns on digital enablers for buttons in port E
  GPIO_PORTE_PUR_R |= 0xF; // turns on the pull up registers in port E
  
  SYSCTL_RCGC2_R |= 0x20; // enables port F
  delay(100);
  
  GPIO_PORTF_AFSEL_R &= 0x0; // turns off alternate functions for port F
  
  GPIO_PORTF_DEN_R |= 0x2; // turns on digital enablers for buttons in port F
  GPIO_PORTF_PUR_R |= 0x2; // turns on the pull up registers in port F
}
