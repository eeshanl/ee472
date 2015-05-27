// Contains all functions needed for the ADC's used by the distance sensors

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

#include "keypad.h"

// average values every half of a second
unsigned int avg0, avg1, avg2, avg3; // averages

// stores 64 instantaneous values for averaging
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

// initializes the ADC so that the sensor can be used to recieve distances
void ADCInit() {
  SYSCTL_RCGC0_R = 0x0;
  delay(100);
  SYSCTL_RCGC0_R |= 0x10000; // enables ADC0
  delay(100);
  ADC0_ACTSS_R = 0xF; // enables the SS0
  ADC_SSMUX0_R |= (0x0 | 0x10 | 0x200 | 0x3000); // set control for each mux
  ADC_SAC_R = 0x6; // set the oversampling to 64 per sample
}

// Task acquires the instantaneous values from each of the
// distance sensors and accumulates each one for their respective total
void vTaskADC(void *vParameters) {
  
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
    
    i++;
    vTaskDelay(8);
  }
}

// Pass in an integer and print out to the OLED screen
void printInt(int dist, int x, int y){
  int value = dist;
  char myData[5];
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
  RIT128x96x4StringDraw(myData, x, y, 15);
}

// Converts the digital value to millimeters. Decided to use an equation
// instead of a lookup table because we didn't want to allocate memory to
// a table of just values. The equation is also not that complex.
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

// Gets the average of approximately 4096 values every half a second.
// Chose to get average every half of a second because a lot can happen
// between that interval and a shorter interval would be more safe
void vTaskADCAverage(void *vParameters) {
  while(1) {
    avg0 = total0 / 64;
    avg1 = total1 / 64;
    avg2 = total2 / 64;
    avg3 = total3 / 64;
    vTaskDelay(500);
  }
}