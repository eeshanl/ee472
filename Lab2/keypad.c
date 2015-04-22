//This file is responsible for the actions related to the key presses on the board
#include "header.h"

//global int that stores the value of the last key pressed
//lastKey is initially set to 0, as no key is pressed in the beginning 
int lastKey = 0;

//post: initializes the keys on the board to their repective ports on the board
void key_init() {
  SYSCTL_RCGC2_R |= 0x00000010;
  delay(100);
  GPIO_PORTE_AFSEL_R &= 0x00000000;
  GPIO_PORTE_DEN_R |= 0xF;
  GPIO_PORTF_DEN_R |= 0x00000002;
  GPIO_PORTE_PUR_R |= 0xF;
  GPIO_PORTF_PUR_R |= 0x2;
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

//post: returns an int value that corresponds to the value a key has been assigned 
//      the assignment is as follows:
//      return value 1 ==> up
//      return value 2 ==> down
//      return value 3 ==> left
//      return value 4==> right
//      return value 5 ==> select
//      return value 0 ==> no key pressed
int getKey() {
  // sets the keys to their ports on the board
  int up = ~GPIO_PORTE_DATA_R & 0x1;
  int down = ~GPIO_PORTE_DATA_R & 0x2;
  int left = ~GPIO_PORTE_DATA_R & 0x4;
  int right = ~GPIO_PORTE_DATA_R & 0x8;
  int select = ~GPIO_PORTF_DATA_R & 0x2;
  
  // checks which key is pressed and returns the int it corresponds to 
  if (up != 0) {
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

//post: returns an int value of the key being pressed
//      while 
int keymaster() {
  while (is_a_key()) {
    if (debounce()) {
      if (fresh_key()) {
        return lastKey;
      }
    }
  }
  lastKey = 0;
  return 0;
}

//post: returns an int value 
int debounce() {
  int key = getKey();
  volatile unsigned long i = 0;
  volatile unsigned int j = 0;
  for (i = 10; i > 0; i--) {
    for (j = 0; j < 100; j++) {
      if (key != getKey()) {
        return 0;
      }
    }
  }
  return 1;
}
