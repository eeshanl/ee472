#include "header.h"

int lastKey = 0;

int is_a_key() {
  int bit1 = GPIO_PORTF_DATA_R & 0x2;
  if (GPIO_PORTE_DATA_R != 0xF || bit1 == 0) {
    return 1;
  } else {
    return 0;
  }
}

int fresh_key(){
  int key = getKey();
  if (lastKey == key) {
    return 0;
  } else {
    lastKey = key;
    return 1;
  }
}

int keymaster() {
  while (is_a_key()) { //checks for valid key
    if (getDebounce()) { //checks if the same key is being pressed for 100 miliseconds
      if (fresh_key()) { //if the key has not been pressed before
        return lastKey;
      }
    }
  }
  lastKey = 0; //need to reset the lastKey to 0, if no key was pressed
  return 0;
}

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

void key_init() {
  SYSCTL_RCGC2_R |= 0x00000010;
  delay(100);
  GPIO_PORTE_AFSEL_R &= 0x00000000;
  GPIO_PORTE_DEN_R |= 0xF;
  GPIO_PORTF_DEN_R |= 0x00000002;
  GPIO_PORTE_PUR_R |= 0xF;
  GPIO_PORTF_PUR_R |= 0x2;
}