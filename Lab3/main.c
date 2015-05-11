#include "header.h"
//*****************************************************************************
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

int
main(void) {
  // Set the clocking to run directly from the crystal.
  SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);

  //
  // Initialize the OLED display.
  //
  RIT128x96x4Init(1000000);
  //speaker_init();
  LED_init();
  ADC_init();
  key_init();
  timer0_init();
  timer1_init();
  timer2_init();
  init_GPIOE();
  init_GPIOF();
  display_init(); // Initializes the LCD Display
  char* str = "Let's Play GTH!                         Press sel to go!\0";
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
      mainMenuForGTH(); // starts the main menu
    }
//    if (getTimer1() < 0xFFFFFFFF - 12) {
//      display_string("time\0");
//    }
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
  SYSCTL_RCGC0_R = 0x0;
  delay(100);
  SYSCTL_RCGC0_R |= 0x10000;
  delay(100);
  ADC0_ACTSS_R = 0x1;
}

void speaker_init() {
  SYSCTL_RCGC2_R |= 0x00000040;
  delay(100);
  GPIO_PORTF_DIR_R |= 0x00000002;
  GPIO_PORTG_AFSEL_R &= 0x00000000;
  GPIO_PORTG_DEN_R |= 0x2;
  GPIO_PORTG_PUR_R |= 0x2;
  GPIO_PORTG_DATA_R |= 0x2;
}
