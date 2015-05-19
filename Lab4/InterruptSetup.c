/**
 * This file contains all intializations and event handlers for all interrupts used in this lab
*/

#include "InterruptSetup.h"
#include "keypad.h"
#include "main.h"

#include "inc/lm3s8962.h"
#include "drivers/rit128x96x4.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/cpu.h"


unsigned long int timer1 = 0xFFFFFFFF; // Timer 1. Used as a clock to time how long events are taking.
unsigned long int randomSeed = 0xFFFFFFFF; // a value used as a random seed. Timer2 decrements this value.
int debounce = 0; //

// method to return the value of the timer1 global variable to be accecible in other files.
unsigned long int getTimer1() {
  return timer1;
}

// returns the value of the randomSeed global variable to be accecible in other files.
// Using timer interrupts we change a value of randomSeed to act as a psuedo random number generator.
unsigned long int getSeed() {
  return randomSeed;
}

// Initializes GPIO Port E for interrupts for the direction keys
void init_GPIOE(void){
   // enables interrupts on the vector table for Port E.
   NVIC_EN0_R |= 0x40000000;

   // enables looking at edge
   GPIO_PORTE_IS_R &= ~0x0F;

   // enables looking at 1 or both edges. Enabled 1 edge.
   GPIO_PORTE_IBE_R &= ~0x0F;

   // enables port E at negedge
   GPIO_PORTE_IEV_R = 0x00;

   // enables the interrupt for required bits
   GPIO_PORTE_IM_R |= 0x0F;

   // Raw interrupt status enabled
   GPIO_PORTE_RIS_R = 0x0F;

   // clear interrupt status
   GPIO_PORTE_ICR_R |= 0x0F;
}

// setup for port F interrupt for the select key
void init_GPIOF(void){
   // enables interrupts on the vector table for Port F bit 1.
   NVIC_EN0_R |= 0x10;

   // enables looking at edge
   GPIO_PORTF_IS_R &= ~0x02;

   // enables looking at 1 or both edges. Enabled 1 edge.
   GPIO_PORTF_IBE_R &= ~0x02;

   // enables port F at negedge
   GPIO_PORTF_IEV_R &= ~0x02;

   // enables the interrupt for required bits
   GPIO_PORTF_IM_R |= 0x02;

   // Raw interrupt status enabled
   GPIO_PORTF_RIS_R = 0x2;

   // clear interrupt status
   GPIO_PORTF_ICR_R |= 0x2;
}

// Interrupt Event Handler for GPIO ports for all of the keypad keys.
// Incorrporates debounce from Lab 2 upon the interrupt being triggered
void GPIOEventHandler(){
  //first clear the interrupt status
  GPIO_PORTE_ICR_R = 0x0F;
  GPIO_PORTF_ICR_R = 0x2;

  //each interrupt lasts for 10ms, so we need to delay it inside
  //here for another 90 ms
  volatile int i = 0;
  int key = getKey();

  // checks whether the key pressed is the same for roughly 100 ms.
  while(i < 1000){
    if(key == getKey()){
      i++;
    }else{
      break;
    }
  }

  // setting the debounce flag = 1 if i was 1000, else debounce is 0. meaning the key press was not valid.
  if(i == 1000){
    debounce = 1;
  }else{
    debounce = 0;
  }

}

// Timer0 event handler to sample the ADC value from the sensor.
void Timer0IntHandler(){
//  // clears interrupt status
//  TIMER0_ICR_R |= 0x01;
//
//  // samples the adc value so that data is read from the sensor
//  ADC0_PSSI_R |= 0x1;

/* This blinks the led based off of the distance value read from the sensor. */
//  LAB 3
  TIMER0_ICR_R |= 0x01;
  
  
  ADC_SSMUX0_R = 0x0;
  delay(100);
  ADC0_PSSI_R |= 0x1;
  int dist0 = ADC0_SSFIFO0_R;
  
  ADC_SSMUX0_R = 0x1;
  delay(10);
  ADC0_PSSI_R |= 0x1;
  int dist1 = ADC0_SSFIFO0_R;
  
  ADC_SSMUX0_R = 0x2;
  delay(10);
  ADC0_PSSI_R |= 0x1;
  int dist2 = ADC0_SSFIFO0_R;
  
  ADC_SSMUX0_R = 0x3;
  delay(10);
  ADC0_PSSI_R |= 0x1;
  int dist3 = ADC0_SSFIFO0_R;
  
  debugHeight(dist0, 24);
  debugHeight(dist1, 34);
  debugHeight(dist2, 44);
  debugHeight(dist3, 54);
}

// A method used to debug our code. We pass in the distance value read from the sensor and print it
// out to the OLED screen
void debugHeight(int dist, int y){
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

  // prints to the OLED
  RIT128x96x4StringDraw(myData, 30, y, 15);

}

// Timer 1 event handler. used to decrement the timer1 variable. We use this variable to model a clock to
// time events in our menu and game.
void Timer1EventHandler(void) {
//  TIMER1_ICR_R |= 0x01; // clears interrupt status
//  timer1--;

  /* Toggles the LED when hovering over the sensor */
  //LAB 3
  TIMER1_ICR_R |= 0x01;
   LED_toggle();
}

// Timer 2 event handler. Used the decrement the timee2 variable. We use this variable to act as a
// random number seed that we use for a random number generator.
void Timer2EventHandler(void) {
  TIMER2_ICR_R |= 0x01;
  randomSeed--;
}

// getter method to return the debounce flag.
int getDebounce() {
  return debounce;
}

// initializes the timer0
void timer0_init() {

  // enables the timer0.
  SYSCTL_RCGC1_R |= SYSCTL_RCGC1_TIMER0;
  delay(1000);

  // turns of the timer0
  TIMER0_CTL_R &= ~(0x01);

  // timer0 configuration set to 0
  TIMER0_CFG_R = 0x0;

  // timer A mode. Set to periodic mode
  TIMER0_TAMR_R = 0x02;

  // sets the period of timer0
  TIMER0_TAILR_R = 500000;
  //TIMER0_TAILR_R = 0x70000; // for part of the lab

  // interrupt mask. enables the mask for the interrupt
  TIMER0_IMR_R |= 0x1;

  // timer0 raw interrupt status
  TIMER0_RIS_R |= 0x01;

  // clears the timer0 interrupt
  TIMER0_ICR_R |= 0x01;

  // enables the interrupt in the vector table
  NVIC_EN0_R |= 0x80000;

  // timer0 configured. set to 1
  TIMER0_CTL_R = 0x01;

}

// initializes the timer1
void timer1_init() {

  // enables the timer1.
  SYSCTL_RCGC1_R |= SYSCTL_RCGC1_TIMER1;
  delay(1000);

  // turns of the timer1
  TIMER1_CTL_R &= ~(0x01);

  // timer1 configuration set to 1
  TIMER1_CFG_R = 0x0;

  // timer A mode. Set to periodic mode
  TIMER1_TAMR_R = 0x02;

  // sets the period of timer1
  TIMER1_TAILR_R = 500000; // about 10 ms

  // interrupt mask. enables the mask for the interrupt
  TIMER1_IMR_R |= 0x1;

  // timer1 raw interrupt status
  TIMER1_RIS_R |= 0x01;

  // clears the timer1 interrupt
  TIMER1_ICR_R |= 0x01;

  // enables the interrupt in the vector table
  NVIC_EN0_R |= 0x200000;

  // timer1 configured. set to 1
  TIMER1_CTL_R = 0x01;

}

// initializes the timer2
void timer2_init() {

  // enables the timer2
  SYSCTL_RCGC1_R |= SYSCTL_RCGC1_TIMER2;
  delay(1000);

  // turns of the timer1
  TIMER2_CTL_R &= ~(0x01);

  // timer2 configuration set to 1
  TIMER2_CFG_R = 0x0;

  // timer A mode. Set to periodic mode
  TIMER2_TAMR_R = 0x02;

  // sets the period of timer2
  TIMER2_TAILR_R = 10000; // set to a fast value so that the randomSeed number can decrement quickly to get a more "random" number

  // interrupt mask. enables the mask for the interrupt
  TIMER2_IMR_R |= 0x1;

  // timer2 raw interrupt status
  TIMER2_RIS_R |= 0x01;

  // clears the timer2 interrupt
  TIMER2_ICR_R |= 0x01;

  // enables the interrupt in the vector table
  NVIC_EN0_R |= 0x800000;

  // timer2 configured. set to 1
  TIMER2_CTL_R = 0x01;
}