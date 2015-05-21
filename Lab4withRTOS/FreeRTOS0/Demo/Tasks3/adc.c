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

int doAverage = 0; // flag for 4096

unsigned long int avg0, avg1, avg2, avg3;

// instantaneous distance values
unsigned long int dist0, dist1, dist2, dist3;

// accumulation of the distance values
unsigned long int total0 = 0;
unsigned long int total1 = 0;
unsigned long int total2 = 0;
unsigned long int total3 = 0;

int i = 0;

xSemaphoreHandle gateKeeper = 0;

// initializes the ADC so that the sensor can be used to recieve distances
void ADCInit() {
  // enables ADC0
  SYSCTL_RCGC0_R = 0x0;
  delay(100);
  SYSCTL_RCGC0_R |= 0x10000;
//  ADC_SSCTL0_R |= 0x2;
  delay(100);
  ADC0_ACTSS_R = 0x1;
  gateKeeper = xSemaphoreCreateMutex();
}

void vTaskADC(void *vParameters)
{
//  xOLEDMessage xMessage;
//   
//  volatile unsigned long ul;  
//  const char *T1Text = "Task 1 is running\n\r";
//
//  xMessage.pcMessage = "Bon Jour, Task 1";
  //int i = 0;  // counter to 4096
  while(1) {
      
    if (i >= 4096) {
      //doAverage = 1;
      i = 0;
      //xSemaphoreGive(gateKeeper);
    }
      
//    ADC_SSMUX0_R = 0x0;
//    delay(5);
//    ADC0_PSSI_R |= 0x1;
//    delay(5);
//    dist1 = ADC0_SSFIFO0_R; // ADC 1
//    
//    ADC_SSMUX0_R = 0x1;
//    delay(5);
//    ADC0_PSSI_R |= 0x1;
//    delay(5);
//    dist2 = ADC0_SSFIFO0_R; // ADC 2
//    
//    ADC_SSMUX0_R = 0x2;
//    delay(5);
//    ADC0_PSSI_R |= 0x1;
//    delay(5);
//    dist3 = ADC0_SSFIFO0_R; // ADC 3
//    
//    
//    ADC_SSMUX0_R = 0x3;
//    delay(5);
//    ADC0_PSSI_R |= 0x1;
//    delay(5);
//    dist0 = ADC0_SSFIFO0_R; // ADC 0
//    
//    total0 += dist0;
//    total1 += dist1;
//    total2 += dist2;
//    total3 += dist3;
    
//      if (dist0 > 500) {
//        GPIO_PORTF_DATA_R |= 0x00000001;
//      } else {
//        GPIO_PORTF_DATA_R &= ~(0x00000001);
//      }
    debugHeight(i, 24);
//    debugHeight(avg1, 34);
//    debugHeight(avg2, 44);
//    debugHeight(avg3, 54);
    i++;
    //Send the message to the OLED gatekeeper for display.
    //xQueueSend( xOLEDQueue, &xMessage, 0 );
    
  }
}

// A method used to debug our code. We pass in the distance value read from the sensor and print it
// out to the OLED screen
void debugHeight(int dist, int y){
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

void vTaskADCAverage(void *vParameters) {
  
  while(1) {
    if (xSemaphoreTake(gateKeeper, 1)) {
      avg0 = total0 / 1000;
      avg1 = total1 / 1000;
      avg2 = total2 / 1000;
      avg3 = total3 / 1000;
      doAverage = 0;
      total0 = 0;
      total1 = 0;
      total2 = 0;
      total3 = 0;
    }
  }
}
