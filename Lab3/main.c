#include "header.h"
//*****************************************************************************
//main.c creates a game of 'Guess the Height' using the sensor and displayed 
//its menu on the LCD Display using the LM3S8962 Micro-controller. A player
//has multiple levels of difficulties (Easy, Medium, Hard) to choose from in the
//main menu of the game
//main.c enables the setting up of different GPIO Ports used throughout the game
//along with their respective interrupts, and their interrupt handlers
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
  //Initializes the LED needed for the blinking-sensor part of the assignment
  LED_init();
  //Initializes the ADC
  ADC_init();
  //Initializes the keys on the keypad on the Stellaris Board
  key_init();
  //Initializes Timer0 and enables Timer0 interrupts 
  timer0_init();
  //Initializes Timer1 and enables Timer1 interrupts 
  timer1_init();
  //Initializes Timer2 and enables Timer2 interrupts 
  timer2_init();
  //Initializes GPIO PORT E and enables its interrupts 
  init_GPIOE();
  //Initializes GPIO PORT F and enables its interrupts
  init_GPIOF();
  //Initializes the LCD Display
  display_init(); 
  //Inital display that shows up to start the game
  char* str = "Let's Play GTH!                         Press sel to go!\0";
  // prints out the initial message
  display_string(str); 
  // main loop
  while(TRUE) {
    // main state representing the easy screen of the main menu
    static int mainState = 0; 
    // if the user presses select, game should start
    if (mainState == 0 && keymaster() == 5) { 
      //clears the display on LCD to allow the new game's menu to show up
      clear_display();
      //the first part of the scroll down menu for the game
      str = "Main Menu:                              Easy\0";
      display_string(str);
      // changes mainState to -1 so that after this time the mainMenu can be initiated
      mainState = -1; 
    }
    //the the mainstate is not 0, show the main meu for the game of GTH
    //as the user has pressed select already
    if (mainState != 0) {
      mainMenuForGTH(); // starts the main menu
    }
  }
<<<<<<< HEAD


=======
>>>>>>> bb73b66fa9d0c5741986b08b7d7a54ad73a61c98
}

//delay takes in an unsigned long integer as a paramter
//aValue represents the amount of time of the delay
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

//function for initializing the LED on the board
void LED_init() {
  //sets the system clock
  SYSCTL_RCGC2_R |= 0x00000020;
  //delays for about 1/80th of a second
  delay(100);
  //LED is at pin 0
  //sets the direction bit for LED to 1 so it can write
  //which enables it to display the light
  GPIO_PORTF_DIR_R |= 0x00000001;
  //sets the alternate function select to 0 to turn it off
  GPIO_PORTF_AFSEL_R &= 0x00000000;
  //sets the Digital Enabler to 1
  GPIO_PORTF_DEN_R |= 0x00000001;
  //Sets the Data for PORT F to 1
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
