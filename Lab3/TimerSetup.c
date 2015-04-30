#include "TimerSetup.h"

void Timer0IntHandler(void) {
  LED_toggle();
  TIMER0_ICR_R |= 0x01;
}

void timer0_init() {
  
//  SYSCTL_RCGC1_R |= 0x10000;
//  delay(50);
//  TIMER0_CTL_R &= ~(0x01);
//  TIMER0_CFG_R = 0x0;
//  TIMER0_TAMR_R = 0x2;
//  TIMER0_TAILR_R = 500000;
//  TIMER0_IMR_R |= 0x01;
//  TIMER0_RIS_R |= 0x01;
//  TIMER0_ICR_R |= 0x01;
//  NVIC_EN0_R |= 0x80000;
//  TIMER0_CTL_R |= 0x01;
//  CPUcpsie();
  
  CPUcpsid();
  SYSCTL_RCGC1_R = 0x10000;
  delay(100);
  TIMER0_CTL_R = 0x0;
  TIMER0_CFG_R = 0x0;
  TIMER0_TAMR_R = 0x2;
  TIMER0_TAILR_R = 5000000;
  TIMER0_IMR_R = 0x01;
  TIMER0_CTL_R = 0x02;
  //TIMER0_RIS_R |= 0x01;
  TIMER0_ICR_R = 0x01;
  NVIC_EN0_R = 0x80000;
  CPUcpsie();
  TIMER0_CTL_R |= 0x01;
}