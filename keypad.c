#include "header.h"


int flag = 0;

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
   if(keymaster() && flag == 0){
        flag = 1;
        LED_toggle();
   }else if(keymaster() && flag){
        flag = 0;
        while(keymaster());
    }
  return flag;
}

int is_a_key() {
  
  int boolean = 0;
  int bit1 = GPIO_PORTF_DATA_R & 0x2;
  if (GPIO_PORTE_DATA_R != 0xF || bit1 == 0) {
    boolean = 1;
  }
  return boolean;
}

int getKey() {
  //assign int values to different keys
  //o==> up
  //1==> down
  //2==>left
  //3==>right
  //4==>select
  //-1 if no key is pressed
  
  int up = ~GPIO_PORTE_DATA_R & 0x1;
  int down = ~GPIO_PORTE_DATA_R & 0x2;
  int left = ~GPIO_PORTE_DATA_R & 0x4;
  int right = ~GPIO_PORTE_DATA_R & 0x8;
  int select = ~GPIO_PORTF_DATA_R & 0x2;
  
  if (up) {
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
  
  int key = getKey();
  if (is_a_key() && (key != 0) && debounce()) {
    return key;
  } else {
    return 0;
  }
  
}

int debounce(){
  int key = getKey();   
  if(key != 0){
    delay(100);
    if(key = getKey()){
      return 1;
    }
  }
  return 0;
  
}
