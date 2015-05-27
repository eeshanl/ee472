
#ifndef MOTOR_H
#define MOTOR_H


void PWMinit();
void PORTD_init();
void vTaskControlMotor(void *vParameters);
void vTaskSpeaker(void *vParameters);
void goUpLeft();
void goUpRight();
void goBackRight();
void goBackLeft();
void goForward();
void goBackWard();
void speakerInit();

extern int fast;
extern int currentKey;
extern unsigned int dist0, dist1, dist2, dist3;

#endif