// header file containing prototypes of initialization functions

#ifndef ADC_H
#define ADC_H

void ADCInit();
void vTaskADC(void *vParameters);
void vTaskADCAverage(void *vParameters);
void debugHeight(int dist, int y);

#endif
