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

int dist0, dist1, dist2, dist3;

// initializes the ADC so that the sensor can be used to recieve distances
void ADCInit() {
  // enables ADC0
  SYSCTL_RCGC0_R = 0x0;
  delay(100);
  SYSCTL_RCGC0_R |= 0x10000;
  delay(100);
  ADC0_ACTSS_R = 0x1;
}

void vTaskADC(void *vParameters)
{
//  xOLEDMessage xMessage;
//   
//  volatile unsigned long ul;  
//  const char *T1Text = "Task 1 is running\n\r";
//
//  xMessage.pcMessage = "Bon Jour, Task 1";
  
  while(1)
  {
      
      ADC_SSMUX0_R = 0x0;
      delay(10);
      ADC0_PSSI_R |= 0x1;
      delay(10);
      dist1 = ADC0_SSFIFO0_R; // ADC 1
      
      ADC_SSMUX0_R = 0x1;
      delay(10);
      ADC0_PSSI_R |= 0x1;
      delay(10);
      dist2 = ADC0_SSFIFO0_R; // ADC 2
      
      ADC_SSMUX0_R = 0x2;
      delay(10);
      ADC0_PSSI_R |= 0x1;
      delay(10);
      dist3 = ADC0_SSFIFO0_R; // ADC 3
      
      
      ADC_SSMUX0_R = 0x3;
      delay(10);
      ADC0_PSSI_R |= 0x1;
      delay(10);
      dist0 = ADC0_SSFIFO0_R; // ADC 0
      
      
//      if (dist0 > 500) {
//        GPIO_PORTF_DATA_R |= 0x00000001;
//      } else {
//        GPIO_PORTF_DATA_R &= ~(0x00000001);
//      }
      debugHeight(dist0, 24);
      debugHeight(dist1, 34);
      debugHeight(dist2, 44);
      debugHeight(dist3, 54);  

    
    //Send the message to the OLED gatekeeper for display.
      //xQueueSend( xOLEDQueue, &xMessage, 0 );
    
      //vTaskDelay(1000);
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