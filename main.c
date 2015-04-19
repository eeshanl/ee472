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
char* menu = "Main Menu:     ";
//checks if the user wants to play again
int playAgain = 1;
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
        if(playAgain){ //priming the loop
          int sel = keymaster();     
          char* options = options_creator();
          char* menu0 = strcat*(menu, options[0]);
          char* menu1 = strcat*(menu, options[1]);
          char* menu2 = strcat*(menu, options[2]);
          //if select option is pressed, main menu is displayed
          if(sel == 5){
            selectPressed = 1;  
            //when the sel is the main menu displays the First option , Easy
            display_init();
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
                //need to reinitialize the display every time
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
        
        //now you need to call the different levels of the game according to which level has been chosen.
        chooseLevel();
      }
      fresh_key();
    }
}

//this method should help us choose what level the game should display, 
//according to the user's preferences.
void chooseLevel(){
    //find out what key was pressed and what the value of 
    //currentPos was at the time it was pressed
    //play the game needed accordingly
    int keyPressed = keymaster();
    if((keyPressed == 5) && (currentPos == 0)){
        //easy game is started
        startEasy();
    }else if((keyPressed == 5) && (currentPos == 1)){
        //TODO
        //startMedium();
    }else if((keyPressed == 5) && (currentPos == 2)){
        //TODO
        //startHard();
    }
}

//method plays the "Easy" version of DDR
//user can only play the game till the score reaches 10
//basically, this should display one char at a time,
//and call another method, check_input, which decides
//whether or not the score should be updated
int startEasy(){
    int score = 0;
    //display_init();
    while(score < 10){
        char combos[4];
        int i;
        //char* spaces = "    ";
        for(i = 0; i < 4; i++){
            display_init();
            combos[i] = string_combos(); //displays the combo on the screen
            display_string(combo[i]);
            //wait for some time for the user to enter the answer
            delay(300);
            //checks what the score is now
            score = score + check_input(combos[i]);
        }
    }
    //displays the score on the OLED Display
    RIT128x96x4StringDraw(score, 15, 44, 15);
    //display the new option of whether the user wants to play again
    display_init();
    char* again = "Play Again?                              Press sel to go!\0";
    display_string(again);
    int ok = keymaster(); 
    if(ok == 5){
        playAgain = 1;
    }else{
        playAgain = 0;
    }
    return playAgain;
}

//updating the score every time user inputs something
int check_input(char input){
    int key = keymaster();
    if((input == '^') && (key == 1)){
        return 1;
    }else if((input == 'v') && (key == 2)){
        return 1;
    }else if((input == '<') && (key == 3)){
        return 1;
    }else if((input == '>') && (key == 4)){
        return 1;
    }else{
        return 0;
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
