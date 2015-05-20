#include "inc/lm3s8962.h"
#include "drivers/rit128x96x4.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/cpu.h"

#include "main.h"
#include "adc.h"
#include "keypad.h"
#include "startup.c"

//*****************************************************************************
//main.c
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif


// booleans:
#define TRUE 1 // defines TRUE to be 1
#define FALSE 0 // defines FALSE to be 0


int main() {
  // Set the clocking to run directly from the crystal.
  SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);

  // Initialize the OLED display.
  RIT128x96x4Init(1000000);
  //Initializes the LED needed for the blinking-sensor part of the assignment
  //Initializes the ADC
  ADCInit();
  //Initializes the keys on the keypad on the Stellaris Board
  key_init();
  //Initializes Timer0 and enables Timer0 interrupts
  timer0_init();
  //Initializes GPIO PORT E and enables its interrupts
  init_GPIOE();
  //Initializes GPIO PORT F and enables its interrupts
  init_GPIOF();
  // main loop
  while(TRUE) {
//    if (keymaster()) {
//      RIT128x96x4StringDraw("HELLO\0", 30, 24, 15);
//    } else {
//      RIT128x96x4StringDraw("YOLLO\0", 30, 24, 15);
//    }
  }
}

//delay takes in an unsigned long integer as a paramter
//aValue represents the amount of time of the delay
void delay(unsigned long aValue) {
  volatile unsigned long i = 0;
  volatile unsigned int j = 0;
  for (i = aValue; i > 0; i--) {
    for (j = 0; j < 100; j++);
  }
  return;
}