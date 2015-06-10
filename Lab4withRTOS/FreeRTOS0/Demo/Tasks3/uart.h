// Prototypes for the display functions

#ifndef UART_H
#define UART_H

void uARTInit();
int UARTSend(const unsigned char *pucBuffer, unsigned long ulCount);
void UARTIntHandler(void);

extern char bluetoothSignal;
#endif