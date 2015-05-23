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
#include "semphr.h"

#include "keypad.h"


//sets up the PWM
void PWMinit() {
  GPIO_PORTF_DEN_R |= 0x00000001;
  SYSCTL_RCGC0_R |= 0x00100000;
  delay(50);
  SYSCTL_RCGC2_R |= 0x00000060;
  delay(50);
  SYSCTL_RCC_R |= 0x100000;
  delay(50);
  SYSCTL_RCC_R &= ~(0xE0000);
  delay(50);
  GPIO_PORTF_AFSEL_R = 0x1;
  GPIO_PORTG_AFSEL_R = 0x2;
  GPIO_PORTG_DEN_R |= 0x2;
  PWM_0_CTL_R = 0x00000000;
  PWM_0_GENA_R = 0x0000008C;
  PWM_0_GENB_R = 0x0000080C;
  
  PWM_0_LOAD_R = 0x0000018F;
  PWM_0_CMPA_R = 0x0000012B;
  PWM_0_CMPB_R = 0x00000063;
  PWM_0_CTL_R = 0x00000001;
  PWM_ENABLE_R = 0x00000003;

  
  //setting up PWM2 and PWM3

  SYSCTL_RCGC2_R |= 0x2;
  delay(50);
  SYSCTL_RCC_R |= 0x100000;
  delay(50);
  SYSCTL_RCC_R &= ~(0xE0000);
  delay(50); 
  GPIO_PORTB_DEN_R |= 0x3;
  GPIO_PORTB_AFSEL_R |= 0x3;
  PWM_1_CTL_R = 0x00000000;
  PWM_1_GENA_R = 0x0000008C;
  PWM_1_GENB_R = 0x0000080C;
  
  PWM_1_LOAD_R = 0xFFFF;
  //PWM_1_LOAD_R = 0x0000018F;
  PWM_1_CMPA_R = 0x0000012B;
  //PWM_1_CMPA_R = 0x00000001;
  PWM_1_CMPB_R = 0x0000012B;
  PWM_1_CTL_R = 0x00000001;
  PWM_ENABLE_R |= 0xC; 
}

void PORTD_init(){
   SYSCTL_RCGC2_R |= 0x8;
  //delays for about 1/80th of a second
  delay(100);
  //LED is at pin 0
  //sets the direction bit for LED to 1 so it can write
  //which enables it to display the light
  GPIO_PORTD_DIR_R |= 0x000000F8;
  //sets the alternate function select to 0 to turn it off
  GPIO_PORTD_AFSEL_R &= 0x000000F8;
  //sets the Digital Enabler to 1
  GPIO_PORTD_DEN_R |= 0x000000F8;
  //Sets the Data for PORT A to 1
  //GPIO_PORTD_DATA_R |= (0x000000F8);
  GPIO_PORTD_DATA_R &= 0;
  //GPIO_PORTD_DATA_R |= 0xD0;
}

void vTaskControlMotor(void *vParameters) {
  while(1) {
    int press = keymaster();
    
    if (press == 1) {
      GPIO_PORTD_DATA_R = 0x68;
    } else if (press == 2) {
      GPIO_PORTD_DATA_R = 0xD0;
    } else if (press == 3) {
      GPIO_PORTD_DATA_R = 0x48;
    } else if (press == 4) {
      GPIO_PORTD_DATA_R = 0x60;
    } else {
      GPIO_PORTD_DATA_R &= ~(0x40);
    }
      //    else if (press == 2) {
//      
//    } else if (press == 3) {
//      
//    } else if (press == 4) {
//      
//    } else {
//      
//    }
    
    vTaskDelay(100);
  }
}






void vTaskSpeaker(void *vParameters) {
  
  
  
  //Set GPIO Port: G Enabled
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
   
    //Tell Port G, Pin 1, to take input from PWM 0
    GPIOPinTypePWM(GPIO_PORTG_BASE, GPIO_PIN_1);
   
    //Set a 440 Hz frequency as u1Period
    unsigned long ulPeriod = SysCtlClockGet() / 440;
   
    //Configure PWM0 in up-down count mode, no sync to clock
    PWMGenConfigure(PWM0_BASE, PWM_GEN_0,
                    PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);

    //Set u1Period (440 Hz) as the period of PWM0
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, ulPeriod);

    //Set PWM0, output 1 to a duty cycle of 1/8
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, ulPeriod / 16);

    //Activate PWM0
    PWMGenEnable(PWM0_BASE, PWM_GEN_0);   
   
  
  
  
  while(1) {
      PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, ulPeriod);
      PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT, true);
      delay(100);
      PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, ulPeriod*2);
      delay(100);
      PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT, false);
      delay(100);
      
      
      vTaskDelay(100);
  }
}