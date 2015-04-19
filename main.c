// main

#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "drivers/rit128x96x4.h"
#include "header.h"

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif

#define TRUE 1
#define FALSE 0



//*****************************************************************************
//
//
//*****************************************************************************
int
main(void)
{
    //
    // Set the clocking to run directly from the crystal.
    //
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_8MHZ);

    //
    // Initialize the OLED display.
    //
    RIT128x96x4Init(1000000);
    LED_init();
    key_init();
    display_init();
    
    char* monk = "Let's Play DDR!                         Press sel to go!\0";
    display_string(monk);
    
    // main loop
    while(TRUE) {
      
      fresh_key();
    }
}

void LED_init() {
  SYSCTL_RCGC2_R |= 0x00000020;
  delay(100);
  GPIO_PORTF_DIR_R |= 0x00000001;
  GPIO_PORTF_AFSEL_R &= 0x00000000;
  GPIO_PORTF_DEN_R |= 0x00000001;
  GPIO_PORTF_DATA_R |= 0x00000001;
  
}

void LED_toggle() {
  GPIO_PORTF_DATA_R ^= 0x00000001;
}

//  software delay
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
