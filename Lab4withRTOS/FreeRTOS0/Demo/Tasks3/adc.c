#include "adc.h"
#include "main.h"

#include "inc/lm3s8962.h"
#include "drivers/rit128x96x4.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/cpu.h"


#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"


unsigned int avg0, avg1, avg2, avg3; // averages

// stores 64 values for avg
unsigned int values0[64];
unsigned int values1[64];
unsigned int values2[64];
unsigned int values3[64];

// instantaneous distance values
unsigned int dist0, dist1, dist2, dist3;

// accumulation of the distance values
unsigned int total0 = 0;
unsigned int total1 = 0;
unsigned int total2 = 0;
unsigned int total3 = 0;


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
  
  PWM_1_LOAD_R = 100000;
  //PWM_1_LOAD_R = 0x0000018F;
  //PWM_1_CMPA_R = 0x0000012B;
  PWM_1_CMPA_R = 0x00000063;
  PWM_1_CMPB_R = 0x00000063;
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
  GPIO_PORTD_DATA_R |= 0xD0;
}


// initializes the ADC so that the sensor can be used to recieve distances
void ADCInit() {
  // enables ADC0
  SYSCTL_RCGC0_R = 0x0;
  delay(100);
  SYSCTL_RCGC0_R |= 0x10000;
//  ADC_SSCTL0_R |= 0x2;
  delay(100);
  ADC0_ACTSS_R = 0xF;
  ADC_SSMUX0_R |= (0x0 | 0x10 | 0x200 | 0x3000);

  ADC_SAC_R = 0x6;
}

void vTaskADC(void *vParameters) {
//  xOLEDMessage xMessage;
//   
//  volatile unsigned long ul;  
//  const char *T1Text = "Task 1 is running\n\r";
//
//  xMessage.pcMessage = "Bon Jour, Task 1";
  for (int i = 0; i < 64; i++) {
    values0[i] = 0;
    values1[i] = 0;
    values2[i] = 0;
    values3[i] = 0;
  }
  int i = 0;
  while(1) {
      
    if (i >= 64) {
      i = 0;
      //xSemaphoreGive(gateKeeper);
    }
    ADC0_PSSI_R |= 0x1;
    delay(5);
    dist0 = ADC0_SSFIFO0_R; // ADC 0
    
    dist1 = ADC0_SSFIFO0_R; // ADC 1
    
    dist2 = ADC0_SSFIFO0_R; // ADC 2
    
    dist3 = ADC0_SSFIFO0_R; // ADC 3
    
    total0 -= values0[i];
    values0[i] = dist0;
    total0 += values0[i];
    
    total1 -= values1[i];
    values1[i] = dist1;
    total1 += values1[i];
    
    total2 -= values2[i];
    values2[i] = dist2;
    total2 += values2[i];
    
    total3 -= values3[i];
    values3[i] = dist3;
    total3 += values3[i];
    
//    printInt(dist0, 24);
//    printInt(dist1, 34);
//    printInt(dist2, 44);
//    printInt(dist3, 54);
    
    i++;
    vTaskDelay(8);
  }
}

// A method used to debug our code. We pass in the distance value read from the sensor and print it
// out to the OLED screen
void printInt(int dist, int y){
  int value = dist;
  char myData[6];
  int a = value/1000;
  int b = (value%1000)/100;
  int c = (value%1000%100)/10;
  int d = (value % 1000 % 100 % 10);
  myData[0] = a + '0';
  myData[1] = b + '0';
  myData[2] = c + '0';
  myData[3] = d + '0';
  myData[4] = '\0';

  // prints to the OLED
  RIT128x96x4StringDraw(myData, 30, y, 15);
}

unsigned short int LookupDistanceTable(unsigned short int d) {
  int dis = d << 6;
  if (dis <= 0x2222) {
    return 800;
  } else if (dis >= 0xFFC0) {
    return 70;
  } else {
    dis *= -730;
    dis /= 56734;
    dis += 913;
    return dis;
  }
}

void vTaskADCAverage(void *vParameters) {
  while(1) {
    LED_toggle();
    avg0 = total0 / 64;
    avg1 = total1 / 64;
    avg2 = total2 / 64;
    avg3 = total3 / 64;
    printInt(LookupDistanceTable(avg0), 24);
    printInt(LookupDistanceTable(avg1), 34);
    printInt(LookupDistanceTable(avg2), 44);
    printInt(LookupDistanceTable(avg3), 54);
    vTaskDelay(500);
  }
}
