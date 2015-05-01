#include "TimerSetup.h"



//*****************************************************************************
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif

// booleans. 1 is TRUE 0 is FALSE
#define TRUE 1 // defines TRUE to be 1
#define FALSE 0 // defines FALSE to be 0

int
main(void) {
  // Set the clocking to run directly from the crystal.
  SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
  SYSCTL_XTAL_8MHZ);

  LED_init();
  timer0_init();
  //ADC_init();
  //GPIO_PORTF_DATA_R &= 0x00000000;
  
  // main loop
  while(TRUE) {

  }
  
}

void delay(unsigned long aValue)
{
  volatile unsigned long i = 0;

  volatile unsigned int j = 0;

  for (i = aValue; i > 0; i--)
  {
    for (j = 0; j < 100; j++);
    }

  return;
}

void LED_init() {
  SYSCTL_RCGC2_R |= 0x00000020;
  delay(100);
  GPIO_PORTF_DIR_R |= 0x00000001;
  GPIO_PORTF_AFSEL_R &= 0x00000000;
  GPIO_PORTF_DEN_R |= 0x00000001;
  GPIO_PORTF_DATA_R |= 0x00000001;
}

// Toggles the LED on the board
void LED_toggle() {
  GPIO_PORTF_DATA_R ^= 0x00000001;
}

void ADC_init() {
  SYSCTL_RCGC0_R = 0x10000;
  ADC0_ACTSS_R = 0x1;
}
