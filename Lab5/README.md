# Lab 5: EE 472 Final Project Using FreeRTOS, BlueTooth for making the RoboTank System
- Please view this file in a markdown previewer

In this lab, we learned how to incorporate bluetooth connectivity to make the RoboTank system controllable wirelessly. We combined all subsystems including the sensors, pwms, user-interface, keypad buttons, bluetooth modem, blueTerm SSP application, LEDs to make our RoboTank drive in manual, autonomous and semi-autonomous modes. Currently, we have a menu on the OLED display that allows the user to choose between manual, semi-autonomous and autonomous mode. Once a mode is entered, the RoboTank is ready to drive. In autonomous mode the tank moves by itself, avoiding obstacles. In manual mode, the user can use the keypad buttons to drive the RoboTank in different directions. While driving, the distance sensor values are printed to the OLED display, showing the distances in millimeters. In our semi-autonomous mode the user is allowed to control the tank manually, but when the tank is too close to an obstacle, the tank turns and prevents collision. In the autonomous mode, the tank drives itself. By default it will go straight, but, if there is an obstacle that is sensed by the sensors, it will change its direction based on where the obstacle of the The tank also beeps when a distance value is too small, indicating to the user that it is about to have a collision. We also incorporate an LED system for turn and brake lights for the robo tank system.

## Lab Objectives

1. Assemble the hardware for our RoboTank system.
2. Configure and incorporate a Bluetooth module into your existing code for our RoboTank
3. Amend design specifications including the functional and architectural design that describe and define your implementation of the RoboTank
4. Write a simple Bluetooth communication interface between the two systems
5. Iterate previous requirements specifications, design, and implementations of previous support for the  keypad, external GPIO interfaced LCD display, timers, ISRs, A/D sensors, and other infrastructure to meet the changes required for this lab.
6. Test the new features and capabilities of the system

## Group Members & Contributions

#### Ruchira Kulkarni
- Created README
- Worked on motor control methods in autonomous mode
- Worked on setting up the uart functionality
- Wired the tank

#### Eeshan Londhe
- Commented code thoroughly
- Worked on strategically developing the placement of the sensors used for autonomous and semi-autonomous mode
- Worked with LED lights for signaling turns and brakes
- helped with wiring the tank

#### Denny Ly
- Created flow diagrams and schematic
- Worked on developing a strategy for motor movements in the autonomous mode of the tank
- Worked on setting up the uart
- Developed semi-autonomous mode

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
- There are 3 sensors in the front of the RoboTank(one directly in front, one on the front-left and one on the front-right) and 1 sensor in the back of the tank

#### Speaker Setup
- Is internally wired to PWM0
- Beeps whenever an object is too close to the tank

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

#### uart.c
- Contains all functions needed for bluetooth communication between app and the tank via UART

## Notes

See report documentation for further implementation details of this project.
