#include "header.h"

int lastKey = 0;

void key_init() {
  SYSCTL_RCGC2_R |= 0x00000010;
  delay(100);
  GPIO_PORTE_AFSEL_R &= 0x00000000;
  GPIO_PORTE_DEN_R |= 0xF;
  GPIO_PORTF_DEN_R |= 0x00000002;
  GPIO_PORTE_PUR_R |= 0xF;
  GPIO_PORTF_PUR_R |= 0x2;
}

int fresh_key(){
//   if(keymaster() && flag == 0){
//        flag = 1;
//        LED_toggle();
//   }else if(keymaster() && flag){
//        flag = 0;
//        while(keymaster());
//    }
//  return flag;
  int key = getKey();
  if (lastKey == key) {
    return 0;
  } else {
    lastKey = key;
    return 1;
  }
}

int is_a_key() {
  
  int bit1 = GPIO_PORTF_DATA_R & 0x2;
  if (GPIO_PORTE_DATA_R != 0xF || bit1 == 0) {
    return 1;
  } else {
    return 0;
  }
}

int getKey() {
  //assign int values to different keys
  //1==> up
  //2==> down
  //3==>left
  //4==>right
  //5==>select
  //0 if no key is pressed
  
  int up = ~GPIO_PORTE_DATA_R & 0x1;
  int down = ~GPIO_PORTE_DATA_R & 0x2;
  int left = ~GPIO_PORTE_DATA_R & 0x4;
  int right = ~GPIO_PORTE_DATA_R & 0x8;
  int select = ~GPIO_PORTF_DATA_R & 0x2;
  
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



int keymaster() {
//  while (fresh_key()) {
//    if (debounce()) {
//        return getKey();
//    }
//  }
//  lastKey = 0;
//  return 0;
//
  
  while (is_a_key()) {
    if (debounce()) {
      if (fresh_key()) {
        return lastKey;
      }
    }
  }
    lastKey = 0;
    return 0;

  
  
//  if(is_a_key() && debounce() && fresh_key()) {
//    return lastKey;
//  }
//  lastKey = 0;
//  return 0;
//  
  
  
}

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
