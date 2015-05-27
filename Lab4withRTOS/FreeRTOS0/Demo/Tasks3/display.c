// Contains functions needed for the user interface
#include "adc.h"
#include "main.h"
#include "motor.h"
#include "keypad.h"
#include "display.h"

#include "inc/lm3s8962.h"
#include "drivers/rit128x96x4.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/cpu.h"

//Include hardware memory map, GPIO driver, and PWM driver
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"

#include "FreeRTOS.h"
#include "task.h"

int state = 0; // indicates which state you are in, in the menu
int pastKey = -1; // flag to see what the previous key press was
int fast = 0; // flag to indicate whether the tank will be driving in fast or slow mode. Fast = 1, slow = 0

// This method is the task that runs the User-Interface system for the RoboTank system on the OLED display.
// It starts off with a screen asking to press select, then asks the user to press manual mode
// or autonomous mode. Then once a mode is chosen, the user is prompted with an option to run in fast/slow mode.
// Then, once that is chosen the menu displays the distance values from the sensor.
void vTaskDisplay(void *vParameters) {
  while(1) {
    if (pastKey != currentKey) {
      pastKey = currentKey;
      int newState;
      int prev = state;
      
      
      /****************************/
      // This if/else structures controls what state to go into. The state
      // corresponds to what is being displayed on the OLED display.
      if (state == 0) {
        state = 1;
      } else if (state == 1) {
        if (currentKey == 5) {
          state = 2;
          RIT128x96x4Clear();
        }
      } else if (state == 2) {
        if (currentKey == 1 || currentKey == 2) {
          state = 3;
        } else if (currentKey == 5) {
          state = 4;
        }
      } else if (state == 3) {
        if (currentKey == 1 || currentKey == 2) {
          state = 2;
        } else if (currentKey == 5) {
          state = 5;
          GPIO_PORTD_DATA_R = 0x40;
        }
      } else if (state == 4) {
        if(currentKey == 1){
          fast = 1;
          state = 6;
          GPIO_PORTD_DATA_R = 0x40;
        } else if(currentKey == 2){
          fast = 0;
          state = 6;
          GPIO_PORTD_DATA_R = 0x40;
        } else{
          state = 4;
        }
      } else if (state == 5) {
        if (currentKey == 5) {
          state = 2;
          GPIO_PORTD_DATA_R &= ~(0x40);
        }
      } else if(state == 6){
        if (currentKey == 5) {
          state = 2;
          GPIO_PORTD_DATA_R &= ~(0x40);
        }
      } else{
        state = 5;
      }
      
      /****************************/
      
      // indicates whether the current state is a new state or not
      if(prev != state) {
        newState = 1;
      } else {
        newState = 0;
      }
      
      /****************************/
      // This if/else structure indicates what to print out to the OLED display
      // depending on what state it is in from the previous if/else statement.
      if (newState) {
        RIT128x96x4Clear();
        if (state == 1) {
          RIT128x96x4Clear();
          RIT128x96x4StringDraw("ROBO TANK!\0", 40, 0, 15);
          RIT128x96x4StringDraw("Press, SEL to start \0", 10, 10, 15);
          RIT128x96x4StringDraw("Denny Ly\0", 10, 30, 15);
          RIT128x96x4StringDraw("Eeshan Londhe\0", 10, 40, 15);
          RIT128x96x4StringDraw("Ruchira Kulkarni\0", 10, 50, 15);
        } else if (state == 2) {
          RIT128x96x4Clear();
          printState2Or3(40);
        } else if (state == 3) {
          RIT128x96x4Clear();
          printState2Or3(60);
        } else if(state == 4){
          RIT128x96x4Clear();
          RIT128x96x4StringDraw("Press up for 1\0", 30, 0, 15);
            RIT128x96x4StringDraw("1. Faster\0", 30, 34, 15);
          RIT128x96x4StringDraw("2. Slower\0", 30, 44, 15);
          RIT128x96x4StringDraw("Press down for 2\0", 30, 14, 15);
          } else if (state == 6) {
            RIT128x96x4Clear();
            RIT128x96x4StringDraw("Manual Mode \0", 33, 0, 15);
            printADCString();
          } else if (state == 5) {
            RIT128x96x4Clear();
            RIT128x96x4StringDraw("Autonomous Mode\0", 23, 0, 15);
            printADCString();
          }
        }
      
      /****************************/
      
      }
      vTaskDelay(10); // delay of about 10 ms
    }
  }
  
  // Prints to the OLED display the necessary strings for the menu where you can choose between
  // manual and autonomous mode
  void printState2Or3(int y) {
    RIT128x96x4StringDraw("Mode Selection:\0", 25, 0, 15);
    RIT128x96x4StringDraw("Press SEL to choose\0", 10, 10, 15);
    RIT128x96x4StringDraw(">\0", 0, y, 15);
    RIT128x96x4StringDraw("Manual Mode\0", 15, 40, 15);
    RIT128x96x4StringDraw("Autonomous Mode\0", 15, 60, 15);
  }
  
  // Prints to the OLED display the necessary string for the menu where the ADC distance values are being displayed
  void printADCString() {
    RIT128x96x4StringDraw("ADC 0:\0", 30, 24, 15);
    RIT128x96x4StringDraw("ADC 1:\0", 30, 34, 15);
    RIT128x96x4StringDraw("ADC 2:\0", 30, 44, 15);
    RIT128x96x4StringDraw("ADC 3:\0", 30, 54, 15);
    RIT128x96x4StringDraw("Press SEL to exit\0", 15, 74, 15);
  }
  
  // This task is used to print the instantaneous distances to the OLED display.
  // Only want to print to the OLED display once autonomous/manual mode is initiated
  void vPrintDistance(void *vParameters) {
    while(1) {
      if (state == 6 || state == 5) { // autonomous/manual mode states
        printInt(LookupDistanceTable(dist0), 70, 24);
        printInt(LookupDistanceTable(dist1), 70, 34);
        printInt(LookupDistanceTable(dist2), 70, 44);
        printInt(LookupDistanceTable(dist3), 70, 54);
      }
      vTaskDelay(50); // delay of about 50 ms
    }
  }