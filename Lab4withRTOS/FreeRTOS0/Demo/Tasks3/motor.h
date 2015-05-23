
#ifndef MOTOR_H
#define MOTOR_H


void PWMinit();
void PORTD_init();
void vTaskControlMotor(void *vParameters);
void vTaskSpeaker(void *vParameters);
#endif