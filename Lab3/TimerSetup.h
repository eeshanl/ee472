#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "inc/lm3s8962.h"
#include "drivers/rit128x96x4.h"
#include "driverlib/cpu.h"

#ifndef TIMERSETUP_H
#define TIMERSETUP_H

extern void Timer0IntHandler(void);
extern void timer0_init();
extern void delay(unsigned long aValue);
extern void LED_init();
extern void LED_toggle();

#endif