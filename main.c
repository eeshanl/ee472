// main

#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "drivers/rit128x96x4.h"
#include "header.h"
#include <string.h>
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

int currentPos = 0;
int selectPressed = 0;
char* menu = "Main Menu:                            ";
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
      int sel = keymaster();     
      char* options = options_creator();
      char* menu0 = strcat*(menu, options[0]);
      char* menu1 = strcat*(menu, options[1]);
      char* menu2 = strcat*(menu, options[2]);
      //if select option is pressed, main menu is displayed
      if(sel == 5){
        selectPressed = 1;  
        display_init();
        //when the user selects select, the main menu initially displays Easy
        //char* menu = "Main Menu:                            1 : Easy\0";
        //char* menu0 = strcat*(menu, options[0]);
        display_string(menu0);
      }
      
      //This scroll menu is like a state machine
      //Here, currentPos refers to the "State" of the menu
      //so, if Easy is displayed on the 2nd line, ==> currentPos = 0
      // if Medium is displayed on the 2nd Line ==> currentPos = 1
      // if Hard if displayed ==> currentPos = 2
      //
      //As we only want the user to use the up and down keys when
      //the user has "chosen" to play the game, selectPressed flag needs
      //to be checked
      //
      
      if(selectPressed){
        int pressed = keymaster();
        if((pressed == 2) && (currentPos == 0)){
            display_init();
            display_string(menu1);
            currentPos = 1;
        }else if((pressed == 2) && (currentPos == 1)){
            display_init();
            display_string(menu2);
            currentPos = 2;
        }else if((pressed == 1) && (currentPos == 0)){
            display_init();
            display_string(menu2);
            currentPos = 2;
        }else if((pressed == 1) && (currentPos == 1)){
            display_init();
            display_string(menu0);
            currentPos = 0;
        }else if((pressed == 2) && (currentPos == 2)){
            display_init();
            display_string(menu0);
            currentPos = 0;
        }else if((pressed == 1) && (currentPos == 2)){
            display_init();
            display_string(menu1);
            currentPos = 1;
        }
      }
      fresh_key();
    }
}

//makes a character array of the three levels of the game.
//options is an array of three character pointers
char* options_creator() {
    char* options[3] = {"1: Easy\0", "2: Medium\0", "3: Hard\0"}; 
    return options;
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
