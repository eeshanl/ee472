//*****************************************************************************
// Eeshan Londhe
// Application 2
// Prints ABCD then BCD then CD then D then " " then repeats this.
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
// Flashes ABCD with one character less every second to the OLED on the Stellaris evaluation board.
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
    
    // Prints "ABCD"
    // then   " BCD"
    // then   "  CD"
    // then   "   D"
    // then   "    "
    while(TRUE) {
      char str[5] = {'A', 'B', 'C', 'D', '\0'}; // initializes char array with "ABCD"
      RIT128x96x4StringDraw(str, 10, 24, 15); // prints "ABCD"
      delay(8000); // delay of 1 sec
      for (int i = 0; i < 4; i++) {
        str[i] = ' ';
        RIT128x96x4StringDraw(str, 10, 24, 15); // prints one less character every iteration of the foor loop
        delay(8000); // delay of 1 sec
      }
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
