# Lab 5: EE 472 Final Project Using FreeRTOS, BlueTooth for making the RoboTank System 
- Please view this file in a markdown previewer

In this lab, we learned how to incorporate bluetooth connectivity to make the RoboTank system controllable wirelessly. We combined all subsystems including the sensors, pwms, user-interface, keypad buttons, bluetooth modem, blueTerm SSP application, LEDs to make our RoboTank drive in manual, autonomous and semi-autonomous modes. Currently, we have a menu on the OLED display that allows the user to choose between manual, semi-autonomous and autonomous mode. Once a mode is entered, the RoboTank is ready to drive. In autonomous mode the tank moves by itself, avoiding obstacles. In manual mode, the user can use the keypad buttons to drive the RoboTank in different directions. While driving, the distance sensor values are printed to the OLED display, showing the distances in millimeters. In our semi-autonomous mode the user is allowed to control the tank manually, but when the tank is too close to an obstacle, the tank turns and prevents collision. In the autonous mode, the tank drives itself. By default it will go straight, but, if there is an obstacle that is sensed by the sensors, it will change its direction based on where the obstacle of the The tank also beeps when a distance value is too small, indicating to the user that it is about to have a collision.

## Lab Objectives

1. Introduce a preemptive priority scheduled RTOS (FreeRTOS) which may include a number of steps to complete first.
2. Amend requirements specifications to reflect your groups unique RoboTank requirements that were refined and differentiate your prototype RoboTank
3. Amend design specifications including the functional and architectural design that describe and define your implementation of the RoboTank
4. Utilize UML diagrams to model some of the dynamic aspects of the system.
5. Implement task creation as well as resources sharing and inter-task communication protection requirements with the RTOS.
6. Iterate previous requirements specifications, design, and implementations of previous support for the  keypad, external GPIO interfaced LCD display, timers, ISRs, A/D sensors, and other infrastructure to meet the changes required for this lab.
7. Add a system clock interrupt for use by the RTOS as the OS “heartbeat” tick time base.
8. Add additional A/D conversion channels for a total of four A/D channels of sampling.
9. Add support for two PWMs which will be used in the final lab to drive the RoboTank's two motors via an H-bridge hardware module.
Note: The actual hardware will not be provided until the final lab.  Therefore you will have to formulate and prove your testing using other methods (e.g. oscilloscope, LEDs).
10. Test the new features and capabilities of the system

## Group Members & Contributions

#### Ruchira Kulkarni
- Created README
- Worked on motor control methods
- Worked on combining all the code into FreeRTOS

#### Eeshan Londhe
- Commented code thoroughly
- Worked on initializing the PWM signals.
- Worked on the four distance sensors and ADC's

#### Denny Ly
- Created flow diagrams and schematic
- Made the main-menu/user-interface
- Worked on combining all the code into FreeRTOS

<b>Overall, we distributed the work fairly evenly.</b>

## Hardware Setup
We attached the H-bridge to the Stellaris board using 5 GPIO pins for control of the two motors on the RoboTank. Also, we gave the H-bridge two PWM signals to drive the two motors. The speaker was also initialized and connected to a PWM signal to produce sounds at various frequencies. See Lab Report for full Hardware Schematic.

#### H-bridge Setup
- Vm is connected to 5V
- Vcc is connected to 3.3V
- All three GND's are connected to GND
- Output pins A01, A02, B01, B02 are connected to two motors.
- PWMA is connected to PWM2 and PWMB is connected to PWM3
- AIN2, AIN1, STBY, BIN1, BIN2, are connected to PD4, PD5, PD6, PD7, PD3 respectively

#### Sensor Setup
- Power of each sensor is connected to 5V
- Ground of each sensor is connected to ground
- The CTL wires on all 4 sensors are connected on pins ADC 0 - 3.

#### Speaker Setup
- Is internally wired to PWM0

## Included Files

#### main.c
- This file the includes all tasks run on the RTOS. The tasks we had are:
  - vTaskADC
  - vTaskADCAverage
  - vTaskControlMotor
  - vTaskSpeaker
  - vTaskDisplay
  - vPrintDistance

#### keypad.c
- Contains all initializations and other functions to use the key's on the LM3S8962 Micro-controller.

#### motor.c
- Contains the PWM initializations as well as the code that drives the motor. Also includes code that produces frequencies on the speaker.

#### adc.c
- Contains all initializations for all 4 ADC's as well as code that measures and performs functions with the distance sensors.

#### display.c
- Contains the code for the user-interface we made on the OLED display

#### InterruptSetup.c
- Contains all functions needed GPIO interrupts as well as event handlers for them

#### startup_ewarm.c
- Contains the Vector Table

## Notes

See report documentation for further implementation details of this project.
