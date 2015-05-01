#include "TimerSetup.h"

void Timer0IntHandler(void) {
  TIMER0_ICR_R |= 0x01;
  //ADC0_PSSI_R = 0x1;
  //if(ADC0_SSFIFO0_R >= 0x1000000000){
    LED_toggle();
  //}
}

//SYSCTL_RCGC1_R = SYSCTL_RCGC1_TIMER0;
//  delay(1000);
//  TIMER0_CTL_R &= ~(0x01);
//  TIMER0_CFG_R = 0x0;
//  TIMER0_TAMR_R = 0x02;
//  TIMER0_TAILR_R = 0x00100000;
//  TIMER0_IMR_R |= 0x1;
//  TIMER0_CTL_R = 0x01;
//  TIMER0_ICR_R |= 0x01;
//  NVIC_EN0_R |= 0x80000;

void timer0_init() {
  
  SYSCTL_RCGC1_R = SYSCTL_RCGC1_TIMER0;
  delay(1000);
  TIMER0_CTL_R &= ~(0x01);
  TIMER0_CFG_R = 0x0;
  TIMER0_TAMR_R = 0x02;
  TIMER0_TAILR_R = 0x00100000;
  TIMER0_IMR_R |= 0x1;
  TIMER0_CTL_R = 0x01;
  TIMER0_ICR_R |= 0x01;
  NVIC_EN0_R |= 0x80000;

  
}