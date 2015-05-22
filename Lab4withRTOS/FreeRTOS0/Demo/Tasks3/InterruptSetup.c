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
    //LED_toggle();
  }else{
    debounce = 0;
  }

}

// getter method to return the debounce flag.
int getDebounce() {
  return debounce;
}
