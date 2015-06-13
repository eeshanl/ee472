// Prototypes for the motor control functions

#ifndef MOTOR_H
#define MOTOR_H


void PWMinit();
void PORTD_init();
void PORTC_init();
void vTaskControlMotor(void *vParameters);
void vAutoMotor(void *vParameters);
void vSemiMotor(void *vParameters);
void vTaskSpeaker(void *vParameters);
void goUpLeft();
void goUpRight();
void goBackRight();
void goBackLeft();
void goForward();
void goBackWard();
void goLeft();
void goRight();
void speakerInit();
void stopTank();

extern int fast;
extern int currentKey;
extern unsigned int dist0, dist1, dist2, dist3;
extern int r;
#endif