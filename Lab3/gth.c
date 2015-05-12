#include "header.h"

#define RANGE 50 // gives the range that is valid input for the Guess the Height Game

int state = 1; // state reprenting if you are in easy, medium, or hard mode
char* str = ""; // stores a string

// once the game starts, it allows the key presses to be used to play the game instead of controlling the menu
int gameTime = 0;

// stores teh score of the game
int score = 0;

// Passes in a timeLimit for the game. The timeLimit is changed based on difficulty
// game loop to play the Guess the height game
void testGame(unsigned long int timeLimit){

  // a flag used to display which state you are in while playing the game.
  // too high
  // too low
  // hold
  int safe = 0;

  // the time that the user must hold a specific height for
  unsigned long int finishTime = 0;

  // holds the value of the total time the player has to play the game.
  // used the getTimer1() method to return the value of the right now
  unsigned long int time = getTimer1() - timeLimit;

  // random number between 100 and 924
  int random = getRandomNumber();

  // game loop to play the game. exits when player wins (score == 9) or if they player exceeds the time limit
  while(score < 9 && getTimer1() > time){ //time taken to complete the game
    // reads the value from the distance sensor and saves it here
    int dist = ADC0_SSFIFO0_R;

    if(dist < random - RANGE) { // displays to the LCD display if you are higher than the desired height
      if (safe != 3) {
        clear_display();
        display_string("Too High\0");
      }
      safe = 3;
    } else if (dist > random + RANGE) { // displays to the LCD display if you are lower than the desired height
      if (safe != 2) {
        clear_display();
        display_string("Too Low\0");
      }
      safe = 2;
    } else { // // displays to the LCD display if you at the desired height
      if (safe != 1) {
        clear_display();
        display_string("Hold\0"); // prints "Hold" when you are at the desired height
      }
      if (safe == 1) {
        // if you held at the desired height for the required time, then you get one more point
        if (getTimer1() < finishTime) {

          random = getRandomNumber(); // resets the random number for the distance that the player is trying to get to
          score++;
          safe = 0;
        }
      } else {
        finishTime = getTimer1() - 12;
        safe = 1;
      }
    }
  }
}

// random number generator for the height
int getRandomNumber(){
  return getSeed() % 824 + 100; // gives a range between 100 and 924
}

// Once the game is over, the scroll Main Menu is displayed again.
// In particular, it displays the Easy mode on the Main Menu.
void goToStateOne() {
  state = 1; // state is one
  gameTime = 0; // sets gameTime back to 0 so that the main menu can be accessed
  clear_display();
  state = 1;
  str = "Main Menu:                              Easy\0";
  display_string(str);
}

//changes the speed of the game according to the level of difficulty the
//user chooses to play
//easy ==> time is 999999 (basically unlimitted time)
//medium ==> time is 500
//hard ==> time is 400
unsigned long int chooseLevel(int level){
  if (level == 1) {
    return 999999;
  } else if (level == 2) {
    return 500;
  } else {
    return 400;
  }
}


//pre: takes in the level of the game the user wants to play
void startGame(int level) {
  int playAgain = 1;
  //loops till the user wants to play again
  while (playAgain) { //playAgain set to 1 to prime the loop
    //starts countDown for the game to begin
    countDown();

    clear_display();
    testGame(chooseLevel(level));

    WinOrLose();
    playAgain = replay(playAgain);
  }
}

// Asks the user if they want to play the game again
int replay(int playAgain){
    display_string("Replay?                                 Y: <   N: >");
    int key = 1;
    int press; // records what key the user pressed.
    // loop gives time for the user to press left or right in order to choose Y or N to play again
    while(key) {
      press = keymaster();
      if (press == 2 || press == 1) {
        key = 0;
      }
    }
    if (press == 2) {
      clear_display();
    } else {
      playAgain = 0;
    }
    score = 0;
    return playAgain;
}

//post: executes the countDown that is displayed on the LCD screen
//      before every new game the user chooses to play
void countDown(){
  clear_display();
  char* str;
  str = "Game will begin                         in: \0";
  display_string(str);
  delay(8000);
  for (int i = 5; i > 0; i--) { // counts down from 5 to 1
    char count[2];
    count[0] = i + '0';
    count[1] = '\0';
    display_string(count);
    //delay so user can read the message displayed on the screen
    delay(8000);
    //shifts the cursor back to the left so it can override the
    //number just written on the Screen
    cursorLeft();
  }
}

// pre: Passes in int win and score representing if the player won the game or not and what their score was
// post: Displays a message to the user indicating if they won or lost and what their score was
void WinOrLose() {
  clear_display();
  char point[4];
  point[0] = score + '0';
  point[1] = '/';
  point[2] = 9 + '0';
  point[3] = '\0';
  if ((score == 9)) { // player won
    display_string("YOU WIN!!                                 Score: \0");
    display_string(point);
  } else { // player lost
    display_string("YOU LOSE!!                                Score: \0");
    display_string(point);
  }
  delay(16000);
  clear_display();
}

void mainMenuForGTH() {
  int key = keymaster(); // captures the user key input
  if (gameTime == 0) {
    if (state == 1 && key == 4) {
      // if game is on easy and down key (key == 4) is pressed
      // change state to medium
      clear_display(); //clear LCD to display new text
      state = 2; // medium state
      str = "Main Menu:                              Medium\0";
      display_string(str);
    } else if (state == 1 && key == 3) {
      // if the game is on easy and up key is pressed
      // change state to hard
      clear_display();
      state = 3; // hard state
      str = "Main Menu:                              Hard\0";
      display_string(str);
    } else if (state == 2 && key == 4) {
      // if the game is on medium and down key is pressed
      // change state to hard
      clear_display();
      state = 3; // hard state
      str = "Main Menu:                              Hard\0";
      display_string(str);
    } else if (state == 2 && key == 3) {
      // if the game is on medium and up key is pressed
      // change state to easy
      clear_display();
      state = 1; // easy state
      str = "Main Menu:                              Easy\0";
      display_string(str);
    } else if (state == 3 && key == 4) {
      // if the game is on hard and down key is pressed
      // change state to easy
      clear_display();
      state = 1; // easy state
      str = "Main Menu:                              Easy\0";
      display_string(str);
    } else if (state == 3 && key == 3) {
      // if the game is on hard and up key is pressed
      // change state to medium
      clear_display();
      state = 2; // medium state
      str = "Main Menu:                              Medium\0";
      display_string(str);
    }
  }
  // if the select key is selected, game is started.
  if (key == 5) {
    gameTime = -1; //gameTime reset to -1
    clear_display();
    startGame(state);
    goToStateOne(); //Main Menu is displayed again, once the game is over.
  }
}
