// header file containing prototypes of initialization functions

#ifndef ADC_H
#define ADC_H

void ADCInit();
void vTaskADC(void *vParameters);
void vTaskADCAverage(void *vParameters);
void printInt(int dist, int x, int y);
unsigned short int LookupDistanceTable(unsigned short int d);

extern unsigned int dist0, dist1, dist2, dist3;


#endif
