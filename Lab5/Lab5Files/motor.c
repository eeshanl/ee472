// Functions used to drive the motor as well as the speaker

#include "adc.h"
#include "main.h"

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
#include "motor.h"
#include "keypad.h"
#include "display.h"
#include "uart.h"

int currentKey; // flag to check current key pressed.
unsigned long ulPeriod; // used for controlling the speaker
int r = 0;

//PWMinit sets up the Pulse Width Modulator
void PWMinit() {
  //Digitally enables PWM0
  //PWM0 Generator outputs to the PWM0 and PWM1
  GPIO_PORTF_DEN_R |= 0x00000000;
  //Enables the PWM clock by writing a value of 0x0010.0000
  //to the RCGC0 register in the System Control module.
  SYSCTL_RCGC0_R |= 0x00100000;
  //delay for 50 units
  delay(50);
  //Enable the clock to the appropriate GPIO module via the
  //RCGC2 register in the System Control module.
  SYSCTL_RCGC2_R |= 0x00000060;
  //delay for 50 units
  delay(50);
  //Configures the Run-Mode Clock Configuration (RCC) register in the System Control module
  SYSCTL_RCC_R |= 0x100000;
  //delay for 50 units
  delay(50);
  //Setting the PWM divide (USEPWMDIV) and set the divider (PWMDIV) to divide by 2 (000)
  //bits (17-14) are set
  SYSCTL_RCC_R &= ~(0xE0000);
  //delay for 50 units
  delay(50);
  //enable pin F0 to use its alternate function
  GPIO_PORTF_AFSEL_R = 0x1;
  //enable pin G1 to use its alternate function
  GPIO_PORTG_AFSEL_R = 0x2;
  //Digitally enables Port G1
  GPIO_PORTG_DEN_R |= 0x2;
  //to configure the PWM0 generator for countdown mode with immediate
  //updates to their parameters
  //PWM0 Control set to Read mode
  PWM_0_CTL_R = 0x00000000;
  //PWM0 Generator A Control set to write
  PWM_0_GENA_R = 0x0000008C;
  //PWM0 Generator B Control set to write
  PWM_0_GENB_R = 0x0000080C;
  //load value of PWM0 set to 0x18F
  PWM_0_LOAD_R = 0x0000018F;
  //PWM0 Compare A set to 0x12B
  PWM_0_CMPA_R = 0x0012B;
  //PWM0 Compare B set to 0x63
  PWM_0_CMPB_R = 0x00000063;
  //PWM0 Control set to Write mode
  PWM_0_CTL_R = 0x00000001;
  //PWM0 is enabled
  PWM_ENABLE_R = 0x00000003;

  //The following sets up the PWM1 Generator
  //which outputs to the PWM2 and PWM3

  //Enable the clock to the GPIO PORT B module via the
  //RCGC2 register in the System Control module.
  SYSCTL_RCGC2_R |= 0x2;
  //delay for 50 units
  delay(50);
  //Configures the Run-Mode Clock Configuration (RCC) register in the System Control module
  SYSCTL_RCC_R |= 0x100000;
  //delay for 50 units
  delay(50);
  //Setting the PWM divide (USEPWMDIV) and set the divider (PWMDIV) to divide by 2 (000)
  //bits (17-14) are set
  SYSCTL_RCC_R &= ~(0xE0000);
  //delay for 50 units
  delay(50);
  //Digitally enables Port B0 and B1
  GPIO_PORTB_DEN_R |= 0x3;
  //enables pins B0 and B1 to use their alternate functions
  GPIO_PORTB_AFSEL_R |= 0x3;
  //to configure the PWM1 generator for countdown mode with immediate
  //updates to their parameters
  PWM_1_CTL_R = 0x00000000;
  //PWM1 Generator A Control (Port B0) set to read mode
  PWM_1_GENA_R = 0x0000008C;
  //PWM1 Generator B Control (Port B1) set to read mode
  PWM_1_GENB_R = 0x0000080C;
  //load value of PWM1 set to 0xFFFF
  PWM_1_LOAD_R = 0xFFFF;
  //PWM1 Compare A set to 0xFFFFF
  PWM_1_CMPA_R = 0xFFFFF;
  //PWM0 Compare B set to 0xFFFFF
  PWM_1_CMPB_R = 0xFFFFF;
  //PWM1 Generator set to write mode
  PWM_1_CTL_R = 0x00000001;
  //enable the PWM1 Generator (PWM2 and PWM3)
  PWM_ENABLE_R |= 0xC;
}

//initializes GPIO PORT D
void PORTD_init(){
  //Enable the clock to the GPIO PORT D module via the
  //RCGC2 register in the System Control module.
  SYSCTL_RCGC2_R |= 0x8;
  //delays for about 1/80th of a second
  delay(100);
  //LED is at pin 0
  //sets the direction bit for LED to 1 so it can write
  //which enables it to display the light
  GPIO_PORTD_DIR_R |= 0x000000DC; //1101 1100
  //sets the alternate function select to 0 to turn it off
  GPIO_PORTD_AFSEL_R &= 0x000000DC;
  //sets the Digital Enabler to 1
  GPIO_PORTD_DEN_R |= 0x000000DC;
  //Sets the Data for PORT A to 1
  //GPIO_PORTD_DATA_R |= (0x000000F8);
  GPIO_PORTD_DATA_R &= 0;
  //GPIO_PORTD_DATA_R |= 0xD0;
}

//initializes GPIO PORT C
void PORTC_init(){
  //Enable the clock to the GPIO PORT D module via the
  //RCGC2 register in the System Control module.
  SYSCTL_RCGC2_R |= 0x4;
  //delays for about 1/80th of a second
  delay(100);
  //LED is at pin 5,6,7
  //sets the direction bit for LED to 1 so it can write
  //which enables it to display the light
  GPIO_PORTC_DIR_R |= 0x000000E0; //1110 0000
  //sets the alternate function select to 0 to turn it off
  GPIO_PORTC_AFSEL_R &= ~(0x000000E0);
  //sets the Digital Enabler to 1
  GPIO_PORTC_DEN_R |= 0x000000E0;
  //Sets the Data for PORT A to 1
  //GPIO_PORTD_DATA_R |= (0x000000F8);
  GPIO_PORTC_DATA_R = 0x0;
  //GPIO_PORTD_DATA_R |= 0xD0;
}

//VALUE FOR WHICH THE CMPA and CMPB TURNS OFF : 0xFFFD

//vTaskContolMotor is a Task that controls the movements of the motor
//the motor can move right, left, forward, backward, upRight, upLeft,
//downRight and downLeft
//PD7 ==> AOUT
//PD3 ==> AIN
//PD4 ==> BOUT
//PD2 ==> BIN
//PD6 ==> STDBY
void vTaskControlMotor(void *vParameters) {
  //the following commands for the controlling the tank through
  // the blue tooth app "blueTerm"
  //'w' pressed to go forward
  //'s' pressed to go back
  //'a' pressed to go left
  //'d' pressed to go right
  //'q' pressed to go upLeft
  //'e' pressed to go upRight
  //'z' pressed to go back left
  //'x' pressed to go back right
  //if any other character is pressed, the tank stops
  while(1) {
    //gets the currentKey pressed by the user
    currentKey = keymaster();
    if (state == 6) { // (!auton) {
      //if the up key is pressed, tank should move forward
      if (currentKey == 1 || bluetoothSignal == 'w') {
        goForward();
      } else if (currentKey == 2 || bluetoothSignal == 's') {//if down is pressed
        goBackWard();
      } else if (currentKey == 3 || bluetoothSignal == 'a') { //if left is pressed
        goLeft();
      } else if (currentKey == 4 || bluetoothSignal == 'd') { //if right is pressed
        goRight();
      } else if(currentKey == 6 || bluetoothSignal == 'q'){ // if the upLeft key is pressed
        goUpLeft();
      } else if(currentKey == 7 || bluetoothSignal == 'e'){ // if the upRight key is pressed
        goUpRight();
      } else if(currentKey == 8 || bluetoothSignal == 'z'){ //if the backleft key is pressed
        goBackLeft();
      } else if(currentKey == 9 || bluetoothSignal == 'x'){//if the backRight key is pressed
        goBackRight();
      } else {
        stopTank();
      }
    }
    if (bluetoothSignal == 'r') {
      state = 2;
    }
    //delays time between task
    vTaskDelay(20);
  }
}

void stopTank(){
  //enable the stdby
  GPIO_PORTD_DATA_R &= 0x40; //0100 0000
  //changes the PD3 and PD PD7 and PD5 and PD4 to 1
  //so no motion occurs //1001 1100
  GPIO_PORTD_DATA_R |= 0x9C;
  if (state == 5 || state == 6) {
    GPIO_PORTC_DATA_R = 0x40; // 0100 0000
  }
}

//Task that controls the motor actions of the tank when it is
//set to autonomous mode
void vAutoMotor(void *vParameters) {
  // avg0 ==> value sampled by the front sensor
  // avg1 ==> value sampled by the back sensor
  // avg2 ==> value sampled by the front-left sensor
  // avg3 ==> value sampled by the front-right sensor
  //true if the last move made by the motor was back,front,left or right
  while(1) {
    if (state == 5) { // state 5 represents the motor in autnomous mode
      if (avg0 > 400) { //avg0 ==> the sensor in the front of the tank
        //if the avg0 is less than 400 units, it beeps and indicates the
        //tank to do one of the following actions:
        if (avg1 > 400 && avg2 > 350 && avg3 > 350) {
          //if all other sensors are close as well, go left
          goLeft();
        } else if (avg2 > 350 && avg3 > 350) {
          //if both left and right side are open, then turn to the side
          //farther away from an obstacle
          if (avg2 > avg3) {
            //right side of tank is closer to obstacle than left
            goLeft();
          } else { // this implies that avg2 < avg3
            //left side of tank is closer to obstacle than right
            goRight();
          }
        } else if (avg2 > 350) { //if the left side is open
          goLeft();
        } else if (avg3 > 350) { //if the right side is open
          goRight();
        } else {
          if (avg2 > avg3) {
            goLeft();
          } else { // avg2 < avg3
            goRight();
          }
        }
      } else { // dist0 < 500
        if (avg2 > 350 && avg3 > 350) {
          if (avg2 > avg3) {
            goLeft();
          } else { // avg2 < avg3
            goRight();
          }
        } else if (avg2 > 350) {
          //if right sensor is close and left is clear
          //then go upLeft
          goUpLeft();
        } else if (avg3 > 350) {
          //if left sensor is close and right is clear
          goUpRight();
        } else { // By default if there is no obstruction
        //the tank moves forward
          goForward();
        }
      }
    }
    //delays time between task
    vTaskDelay(10);
  }
}
//ADC3 left          ADC2 Right

//this task is responsible for controlling tank actions in semi-autonomous mode
//if the tank is close to an obstacle it will being moving in the oposite direction to
//avoid collisions.
void vSemiMotor(void *vParameters) {
  while(1) {
    if (state == 9) { // (!auton) {
      //if the up key is pressed, tank should move forward
      if (avg0 > 400 || avg1 > 400 || avg2 > 400 || avg3 > 400) {
        if (avg1 > 400) {
          if (avg0 > 400) {
            if (avg3 > 400 && avg2 > 400) {
              goRight(); // tentative
            } else if (avg3 > 400) {
              goRight();
            } else if (avg2 > 400) {
              goLeft();
            } else {
              goLeft(); //tentative
            }
          } else { // avg0 <= 400
            if (avg3 > 400 && avg2 > 400) {
              goRight();
            } else if (avg3 > 400) {
              goUpRight();
            } else if (avg2 > 400) {
              goUpRight();
            } else {
              goForward();
            }
          }
        } else { // avg1 <= 400
          if (avg0 > 400) {
            if (avg3 > 400 && avg2 > 400) {
              goRight(); // tentative
            } else if (avg3 > 400) {
              goRight();
            } else if (avg2 > 400) {
              goLeft();
            } else {
              goRight(); // tentative
            }
          } else { // avg0 <= 500
            if (avg3 > 400 && avg2 > 400) {
              goRight(); // tentative
            } else if (avg3 > 400) {
              goUpRight();
            } else { // avg2 > 500
              goUpLeft();
            }
          }
        }
      } else {
        if (currentKey == 1 || bluetoothSignal == 'w') {
          goForward();
        } else if (currentKey == 2 || bluetoothSignal == 's') {//if down is pressed
          goBackWard();
        } else if (currentKey == 3 || bluetoothSignal == 'a') { //if left is pressed
          goLeft();
        } else if (currentKey == 4 || bluetoothSignal == 'd') { //if right is pressed
          goRight();
        } else if(currentKey == 6 || bluetoothSignal == 'q'){ // if the upLeft key is pressed
          goUpLeft();
        } else if(currentKey == 7 || bluetoothSignal == 'e'){ // if the upRight key is pressed
          goUpRight();
        } else if(currentKey == 8 || bluetoothSignal == 'z'){ //if the backleft key is pressed
          goBackLeft();
        } else if(currentKey == 9 || bluetoothSignal == 'x'){//if the backRight key is pressed
          goBackRight();
        } else {
          stopTank();
        }
      }
    }
    //delays time between task
    vTaskDelay(20);
  }
}

//changes the CMPA and CMPB for the wheels to move
//forward. Ideally, both these values should be the
//same, but in our motor, the right wheel is faster
//than the left one when moving forward
void goForward(){
  GPIO_PORTC_DATA_R = 0x0;
  //STDBY is enabled and PD3 is true and PD7 is false
  GPIO_PORTD_DATA_R &= 0x40; //0100 0000
  //disables stdby and sets PD2 to 1 and PD4 to 0
  GPIO_PORTD_DATA_R |= 0x2C; //0010 1100
  //now both wheels move, so it can go forward
  if(auton){
    PWM_1_CMPA_R = 0x2FFF;
    PWM_1_CMPB_R = 0x2FFF;
  }else if(fast){//if the setting is on fast mode
    PWM_1_CMPA_R = 0xFF;
    PWM_1_CMPB_R = 0xFF;
  }else{//if the setting is on slow mode
    PWM_1_CMPA_R = 0x2FFF;
    PWM_1_CMPB_R = 0x2FFF;
  }
}

//changes the CMPA and CMPB for the wheels to move
//backward. Both of them are the same value to make
//sure that the tank moves back in a straight line
void goBackWard(){
  GPIO_PORTC_DATA_R = 0x0;
  //STDBY is enabled(active low) and PD3 is true and PD7 is false
  GPIO_PORTD_DATA_R &= 0x40;
  //disables stdby and sets PD2 to 0 and PD4 to 1 and PD7 = 1 and PD3 = 0
  GPIO_PORTD_DATA_R |= 0x90; //1001 0000
  //now both wheels can move backward
  if(auton){
    PWM_1_CMPA_R = 0x2FFF;
    PWM_1_CMPB_R = 0x2FFF;
  }else if(fast){//if the setting is on fast mode
    PWM_1_CMPA_R = 0xFF;
    PWM_1_CMPB_R = 0xFF;
  }else{//if the setting is on slow mode
    PWM_1_CMPA_R = 0x2FFF;
    PWM_1_CMPB_R = 0x2FFF;
  }
}

//changes the CMPA ==> Right motor and CMPB ==> left motor for the wheels to move
//in the upLeft direction. To go slightly left, the right wheel is faster than
//the left wheel and both wheels move forward.
void goUpLeft(){
  if (state == 5 || state == 6) {
    GPIO_PORTC_DATA_R = 0x20;
  }
  //enable the stdby
  GPIO_PORTD_DATA_R &= 0x40;
  //changes the PD5 and PD3 TO 1 and disables stdby
  GPIO_PORTD_DATA_R |= 0x2C;
  //makes the wheels go in the UpLeft direction
  if(auton){//if the setting is on fast mode
    PWM_1_CMPA_R = 0x7FF;
    PWM_1_CMPB_R = 0x7FFF;
  } else if(fast){//if the setting is on fast mode
    PWM_1_CMPA_R = 0xFF;
    PWM_1_CMPB_R = 0x2FFF;
  } else {//if the setting is on slow mode
    PWM_1_CMPA_R = 0x2FFF;
    PWM_1_CMPB_R = 0x7FFF;
  }
}

//changes the CMPA ==> Right motor and CMPB ==> left motor for the wheels to move
//in the upLeft direction. To go slightly left, the right wheel is faster than
//the left wheel and both wheels move backward.
void goBackLeft(){
  //enable the stdby
  if (state == 5 || state == 6) {
    GPIO_PORTC_DATA_R = 0x20;
  }
  GPIO_PORTD_DATA_R &= 0x40;
  //changes the PD7 and PD4 TO 1 and disables stdby
  GPIO_PORTD_DATA_R |= 0x90;
  //makes the wheels go in the backLeft direction
  if(fast){//if the setting is on fast mode
    PWM_1_CMPA_R = 0xFF;
    PWM_1_CMPB_R = 0x2FFF;
  }else{//if the setting is on slow mode
    PWM_1_CMPA_R = 0x2FFF;
    PWM_1_CMPB_R = 0x7FFF;
  }
}

//changes the CMPA ==> Right motor and CMPB ==> left motor for the wheels to move
//in the upLeft direction. To go slightly right, the left wheel is faster than
//the right wheel and both wheels move forward.
void goUpRight(){
  if (state == 5 || state == 6) {
    GPIO_PORTC_DATA_R = 0x80; // 0010 0000
  }
  //enable the stdby
  GPIO_PORTD_DATA_R &= 0x40;
  //changes the PD5 and PD3 TO 1 and disables stdby
  GPIO_PORTD_DATA_R |= 0x2C; //01101000
  //makes the wheels go in the UpRight direction
  if(auton){//if the setting is on fast mode
    PWM_1_CMPA_R = 0x7FF;
    PWM_1_CMPB_R = 0x7FFF;
  } if(fast){ //if the setting is on fast mode
    PWM_1_CMPA_R = 0x2FFF;
    PWM_1_CMPB_R = 0xFF;
  }else{ //if the setting is on slow mode
    PWM_1_CMPA_R = 0x7FFE;
    PWM_1_CMPB_R = 0x2FFF;
  }
}

//changes the CMPA ==> Right motor and CMPB ==> left motor for the wheels to move
//in the upLeft direction. To go slightly right, the left wheel is faster than
//the right wheel and both wheels move backward.
void goBackRight() {
  if (state == 5 || state == 6) {
    GPIO_PORTC_DATA_R = 0x80;
  }
  //enable the stdby
  GPIO_PORTD_DATA_R &= 0x40;
  //changes the PD7 and PD4 TO 1 and disables stdby
  GPIO_PORTD_DATA_R |= 0x90;
  //makes the wheels go in the backRight direction
  if(fast){ //if the setting is on fast mode
    PWM_1_CMPA_R = 0x2FFF;
    PWM_1_CMPB_R = 0xFF;
  }else{ //if the setting is on slow mode
    PWM_1_CMPA_R = 0x7FFE;
    PWM_1_CMPB_R = 0x2FFF;
  }
}

void goLeft(){
  if (state == 5 || state == 6) {
    GPIO_PORTC_DATA_R = 0x20;
  }
  //enable the stdby
  GPIO_PORTD_DATA_R &= 0x40;
  //disable the stdby mode and enables the PD2 and PD7
  GPIO_PORTD_DATA_R |= 0x84; //1000 0100
  //changes the CMPA of the right wheel to make it slower
  //for a proper right turn
  if(auton){
    PWM_1_CMPA_R = 0x2FFF;
    PWM_1_CMPB_R = 0x2FFF;
  } else if(fast){//if the setting is on fast mode
    PWM_1_CMPA_R = 0xFF;
    PWM_1_CMPB_R = 0xFF;
  }else{//if the setting is on slow mode
    PWM_1_CMPA_R = 0x2FFF;
    PWM_1_CMPB_R = 0x2FFF;
  }
}

void goRight(){
  if (state == 5 || state == 6) {
    GPIO_PORTC_DATA_R = 0x80;
  }
  //enable the stdby mode
  GPIO_PORTD_DATA_R &= 0x40;
  //disables the stdby mode, and enables the PD4 and PD5 bits
  GPIO_PORTD_DATA_R |= 0x18; // 0001 1000
  //changes the CMPB of the left wheel to make it slower
  //for a proper left turn
  if(auton){
    PWM_1_CMPA_R = 0x2FFF;
    PWM_1_CMPB_R = 0x2FFF;
  } else if(fast){//if the setting is on fast mode
    PWM_1_CMPA_R = 0xFF;
    PWM_1_CMPB_R = 0xFF;
  }else{//if the setting is on slow mode
    PWM_1_CMPA_R = 0x2FFF;
    PWM_1_CMPB_R = 0x2FFF;
  }
}

//this initializes the speaker on the Stellaris board.
//Speaker is hooked up to PWM0
void speakerInit(){
  //enable the speaker peripheral that is connected to PWM0
  SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM);
  //Set GPIO Port: G Enabled
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
  //Tell Port G, Pin 1, to take input from PWM 0
  GPIOPinTypePWM(GPIO_PORTG_BASE, GPIO_PIN_1);
  //Set a 4400 Hz frequency as u1Period
  ulPeriod = SysCtlClockGet() / (4440);
  //Configure PWM0 in up-down count mode, no sync to clock
  PWMGenConfigure(PWM0_BASE, PWM_GEN_0,
  PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
  //Set u1Period (4400 Hz) as the period of PWM0
  PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, ulPeriod);
  //Set PWM0, output 1 to a duty cycle of 1/8
  PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, ulPeriod / 16);
  //Activate PWM0
  PWMGenEnable(PWM0_BASE, PWM_GEN_0);
}

//Task that plays a sound that gets within a range of 900 - 1200 distance within the distance sensor
void vTaskSpeaker(void *vParameters) {
  while(1) {
    //if the state is 5 or 6, this implies that the motor is not in standby(as determined from the menu options)
    if((state == 5 || state == 6) && (dist0 > 600 || dist1 > 600 || dist2 > 600 || dist3 > 600)){

       GPIO_PORTF_DEN_R |= 0x00000001;
      //Set u1Period (4400 Hz) as the period of PWM0
//      PWM_0_CMPA_R = 0x00012B;
       PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, ulPeriod);
      //Set the output of the speaker to true, so sound is heard
      PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT, true);
      //delay for 100 units, so sound can be heard
      delay(100);
      //Set u1Period (4400 Hz * 2) as the period of PWM0
      PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, ulPeriod*2);
      //delay(100);
    }else{
      //if this is  not in the range of the sensor, set output to false
      //so sound is not heard anymore
//        GPIO_PORTF_DEN_R ^= 0x1;
//        PWM_0_CMPA_R = 0x0;
      PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT, false);
    }
    //delay task for 10 units
    vTaskDelay(10);
  }
}
