#include "header.h"
#define MAX 0x100000

unsigned long int timer1 = 0xFFFFFFFF;
unsigned long int randomSeed = 0xFFFFFFFF;
int debounce = 0;

unsigned long int getTimer1() {
  return timer1;
}

unsigned long int getSeed() {
  return randomSeed;
}

void init_GPIOE(void){
   NVIC_EN0_R |= 0x40000000;
   GPIO_PORTE_IS_R &= ~0x0F;
   GPIO_PORTE_IBE_R &= ~0x0F;
   GPIO_PORTE_IEV_R = 0x00;
   GPIO_PORTE_IM_R |= 0x0F;
   GPIO_PORTE_RIS_R = 0x0F;
   GPIO_PORTE_ICR_R |= 0x0F;
}

void init_GPIOF(void){
   //setup for port F interrupt
  NVIC_EN0_R |= 0x10;
   GPIO_PORTF_IS_R &= ~0x02;
   GPIO_PORTF_IBE_R &= ~0x02;
   GPIO_PORTF_IEV_R &= ~0x02;
   GPIO_PORTF_IM_R |= 0x02;
   GPIO_PORTF_RIS_R = 0x2;
  GPIO_PORTF_ICR_R |= 0x2;
}

//on key pad press, this needs to be called to trigger
//interrupt action
//trying to print enter if a key was pressed
//this should be handled by the ISR
void GPIOEventHandler(){
  //first clear the interrupt status
  GPIO_PORTE_ICR_R = 0x0F;
  GPIO_PORTF_ICR_R = 0x2;
  
  //each interrupt lasts for 10ms, so we need to delay it inside
  //here for another 90 ms
  volatile int i = 0;
  int key = getKey();
  //100 ms?
  while(i < 1000){
    if(key == getKey()){
      i++;
    }else{
      break;
    }
  }
  
  if(i == 1000){
    debounce = 1;
  }else{
    debounce = 0;
  }
  
} 


void Timer0IntHandler(){
  TIMER0_ICR_R |= 0x01;
  ADC0_PSSI_R |= 0x1;


//  LAB 3
//  TIMER0_ICR_R |= 0x01;
//  ADC0_PSSI_R |= 0x1;
//  int dist = ADC0_SSFIFO0_R;
//  TIMER1_TAILR_R = 0x50000 - 128 * dist;
//  debugHeight(dist);
}

void debugHeight(int dist){
  int value = dist;
  char myData[6];
  int a;
  int b;
  int c;
  int d;
  a = value/1000;
  b = (value%1000)/100;
  c = (value%1000%100)/10;
  d = (value % 1000 % 100 % 10);
  myData[0] = a + '0';
  myData[1] = b + '0';
  myData[2] = c + '0';
  myData[3] = d + '0';
  myData[4] = '\0';
  RIT128x96x4StringDraw(myData, 30, 24, 15);    
  
}


void Timer1EventHandler(void) {
  TIMER1_ICR_R |= 0x01;
  timer1--;
  //LAB 3
  //TIMER1_ICR_R |= 0x01;
  // LED_toggle();
}

void Timer2EventHandler(void) {
  TIMER2_ICR_R |= 0x01;
  randomSeed--;
}

int getDebounce() {
  return debounce;
}


void timer0_init() {
  
  SYSCTL_RCGC1_R |= SYSCTL_RCGC1_TIMER0;
  delay(1000);
  
  TIMER0_CTL_R &= ~(0x01);
  TIMER0_CFG_R = 0x0;
  TIMER0_TAMR_R = 0x02;
  TIMER0_TAILR_R = 500000;
  //TIMER0_TAILR_R = 0x70000; // for part of the lab
  TIMER0_IMR_R |= 0x1;
  TIMER0_RIS_R |= 0x01;
  TIMER0_ICR_R |= 0x01;
  NVIC_EN0_R |= 0x80000;
  TIMER0_CTL_R = 0x01;

}

void timer1_init() {
  
  SYSCTL_RCGC1_R |= SYSCTL_RCGC1_TIMER1;
  delay(1000);
  
  TIMER1_CTL_R &= ~(0x01);
  TIMER1_CFG_R = 0x0;
  TIMER1_TAMR_R = 0x02;
  TIMER1_TAILR_R = 500000;
  TIMER1_IMR_R |= 0x1;
  TIMER1_RIS_R |= 0x01;
  TIMER1_ICR_R |= 0x01;
  NVIC_EN0_R |= 0x200000;
  TIMER1_CTL_R = 0x01; 
  
}

void timer2_init() {
  
  SYSCTL_RCGC1_R |= SYSCTL_RCGC1_TIMER2;
  delay(1000);
  
  TIMER2_CTL_R &= ~(0x01);
  TIMER2_CFG_R = 0x0;
  TIMER2_TAMR_R = 0x02;
  TIMER2_TAILR_R = 10000;
  TIMER2_IMR_R |= 0x1;
  TIMER2_RIS_R |= 0x01;
  TIMER2_ICR_R |= 0x01;
  NVIC_EN0_R |= 0x800000;
  TIMER2_CTL_R = 0x01; 
}