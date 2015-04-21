//*****************************************************************************
// Eeshan Londhe
// Application 3
// Flashes AC and BD at different rates to the OLED
//*****************************************************************************

#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "drivers/rit128x96x4.h"

#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif

#define TRUE 1
#define FALSE 0

void delay(unsigned long aValue);

//*****************************************************************************
//
// Flashes A C at 1 second rate and B D at 2 second rate to the OLED on the Stellaris evaluation board.
//
//*****************************************************************************
int
main(void) {
    //
    // Set the clocking to run directly from the crystal.
    //
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_8MHZ);

    //
    // Initialize the OLED display.
    //
    RIT128x96x4Init(1000000);
    
    // Used 8000 to represent approximately a 1 second delay
    
    // Prints A and C at a 1 sec rate and prints B and D at a 2 second rate
    while(TRUE) {
      char str[5] = {'A', 'B', 'C', 'D', '\0'}; // initializes char array with "ABCD"
      RIT128x96x4StringDraw(str, 10, 24, 15); // prints "ABCD"
      delay(8000); // delay of 1 sec
      str[0] = ' ';
      str[2] = ' ';
      RIT128x96x4StringDraw(str, 10, 24, 15); // prints " B D"
      delay(8000); // delay of 1 sec
      str[0] = 'A';
      str[2] = 'C';
      str[1] = ' ';
      str[3] = ' ';
      RIT128x96x4StringDraw(str, 10, 24, 15); // prints "A C "
      delay(8000); // delay of 1 sec
      str[0] = ' ';
      str[2] = ' ';
      RIT128x96x4StringDraw(str, 10, 24, 15); // prints "    "
      delay(8000); // delay of 1 sec
    }
}

//  software delay
void delay(unsigned long aValue) {
    volatile unsigned long i = 0;

    volatile unsigned int j = 0;
    
    for (i = aValue; i > 0; i--) {
        for (j = 0; j < 100; j++);
    }

    return;
}
