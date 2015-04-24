#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"

#include "header.h"

//*****************************************************************************
// Main function that drives the microcontroller.
// LETS PLAY DDR!!!
//
// This program allows the user to play a DDR game using the keys on the board.
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

//*****************************************************************************
// Sets up a main menu for the game that the player can play and control using
// the keys on the board. The user can use the up and down keys to cycle
// through the menu and choose a difficulty. Then they can start the game at
// the specified difficulty. Functionality for playing again and at different
// difficulties is available.
//
//*****************************************************************************
int
main(void)
{
  // Set the clocking to run directly from the crystal.
  SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
  SYSCTL_XTAL_8MHZ);


  RIT128x96x4Init(1000000); // Initialize the OLED display.
  LED_init(); // Initializes the LED on the board
  key_init(); // Initializes all keys on the board. Up, Down, Left, Right, Select
  display_init(); // Initializes the LCD Display
  char* str = "Let's Play DDR!                         Press sel to go!\0";
  display_string(str); // prints out the initial message

  // main loop
  while(TRUE) {
    static int mainState = 0; // main state representing the easy screen of the main menu
    if (mainState == 0 && keymaster() == 5) { // select is pressed
      clear_display();
      str = "Main Menu:                              Easy\0";
      display_string(str);
      mainState = -1; // changes mainState to -1 so that after this time the mainMenu can be initiated
    }
    if (mainState != 0) {
      mainMenu(); // starts the main menu
    }

    /*
    if (keymaster()) {
      LED_toggle();
    }
    */

  }
}

// Initializes the LED on the board to its repective port on the board.
// LED uses Port F
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

// pre: passes in an unsigned long value representing the time for the delay
// post: causes a software delay
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
