# LAB 2: I/O and Peripherals

Created a [DDR Game](http://en.wikipedia.org/wiki/Dance_Dance_Revolution)  and menu on the LCD Display using the LMS3962 Micro-controller. A player has multiple difficulties (Easy, Medium, Hard) to choose from in the main menu of the game. DDR is a game that allows the player to choose the corresponding arrow sequence as they appear on the screen. We made a simple adaptation of this game on the LCD display. A new arrow appears on the screen as a certain amount of time elapses (varies with difficulty). Within that time period, the player must press the corresponding arrow key, if they press an incorrect key or do not press the arrow fast enough, they lose the game. To win the game, the player must reach 9 points. After the game is over, the display shows whether the player won or lost as well as there score out of 9. It then prompts the user if they want to play the game again or not.

## Group Members & Contributions

### Eeshan Londhe
- Created README
- Worked on creating the game main menu, game functionality, and LCD Display functionality

### Ruchira Kulkarni
- Commented code thoroughly
- Wired the LCD Display to the board
- Worked on creating the game functionality, keypad setup, and LCD Display set up

### Denny Ly
- Created flow diagrams and schematic
-  Worked on game functionality, initialization and functionality of the LCD Display, and initialization and functionality of the keypad buttons

Overall, we distributed the work fairly evenly.

## Hardware Setup
We attached the LCD display to the LMS3962 Micro-controller using the GPIO pins on Port B and D. To see the full schematic, please view the hardware schematic in the report document.

- The data bits correspond to bits 0 - 6 on Port B. Bit 7 on Port B is unused and is set to ground.
- The Enable, R/W, and RS is wired to bits 5, 6, and 7 on Port D.
- VDD is given 5V
- VCC is wired to a 100K Ohm Potentiometer that is used to vary the display brightness
- VSS is set to ground

## Included Files

### main.c
- This file contains the main function that is executed on the LM3S962 board. It runs the DDR game and menu on the LCD Display.

### header.h
- Contains all prototypes of all functions used in creating this overall program.

### keypad.c
- Contains all initializations and other functions to use the key's on the LMS3962 Micro-controller.

### display.c
- Contains all initializations and other functions to use the LCD Display on the LMS3962 Micro-controller. (See Hardware Setup for how the LCD Display is wired to the board)

### ddr.c
- Contains all functions needed to play the DDR game on the LCD Display

## Notes

See report documentation for further implementation details of this project. (Also please view this file in a <b>Markdown</b> previwer)
