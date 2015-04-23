#include "header.h"

//state is a global int that represents the state, or the level the
//game is in,
//the states are as follows:
//state = 1 ==> easy
//state = 2 ==> medium
//state = 3 ==> hard
int state = 1;
int gameTime = 0;
char* str = "";

//post: opens up a wrap around mainMenu for the game of ddr and
//      prints it out on the LCD Screen. Allows the user
//      to choose which level (easy, medium or hard) he/she
//      wants to play.
//      up and down keys are used to scroll through the menu
//      once the user reaches an option he/she wants to play, 
//      select (key == 5) is pressed for selection of the level
void mainMenu() {
  int key = keymaster();
  if (gameTime == 0) {
    if (state == 1 && key == 4) {
      // if game is on easy and down key (key == 4) is pressed
      // change state to medium
      clear_display(); //clear LCD to display new text
      state = 2;
      str = "Main Menu:                              Medium\0";
      display_string(str);
    } else if (state == 1 && key == 3) { 
      // if the game is on easy and up key is pressed
      // change state to hard
      clear_display();
      state = 3;
      str = "Main Menu:                              Hard\0";
      display_string(str);
    } else if (state == 2 && key == 4) {
      // if the game is on medium and down key is pressed
      // change state to hard
      clear_display();
      state = 3;
      str = "Main Menu:                              Hard\0";
      display_string(str);
    } else if (state == 2 && key == 3) {
      // if the game is on medium and up key is pressed
      // change state to easy
      clear_display();
      state = 1;
      str = "Main Menu:                              Easy\0";
      display_string(str);
    } else if (state == 3 && key == 4) {
      // if the game is on hard and down key is pressed
      // change state to easy
      clear_display();
      state = 1;
      str = "Main Menu:                              Easy\0";
      display_string(str);
    } else if (state == 3 && key == 3) {
      // if the game is on hard and up key is pressed
      // change state to medium
      clear_display();
      state = 2;
      str = "Main Menu:                              Medium\0";
      display_string(str);
    }
  }
  // if the select key is selected, game is started.
  if (key == 5) {
    gameTime = -1; //gameTime reset to -1
    game(state); // game is chosen according to selected state
    goToStateOne(); //Main Menu is displayed again, once the game is over
  }
}

// Once the game is over, the scroll Main Menu is displayed again
void goToStateOne() {
  state = 1;
  gameTime = 0;
  clear_display();
  state = 1;
  str = "Main Menu:                              Easy\0";
  display_string(str);
}

//post: executes an instance of the game of DDR to be played according to the level chosen
void game(int level) {
  clear_display(); //clears the LCD Display according to display a new game of DDR
  startGame(level); //starts the DDR display for the game, after clearing the display
}

//pre: takes in the level of the game the user wants to play 
void startGame(int level) {
  //changes the speed of the game according to the level of difficulty the
  //user chooses to play
  //easy ==> speed = 250000
  //medium ==> speed = 100000
  //hard ==> speed = 75000
  int speed = 0;
  if (level == 1) {
    speed = 250000;
  } else if (level == 2) {
    speed = 100000;
  } else {
    speed = 75000;
  }
  
  //flag to determine whether the user won the game
  int win = 1;
  //flag to determine whether the user wants to play again
  int playAgain = 1;
  
  //loops till the user wants to play again
  while (playAgain) { //playAgain set to 1 to prime the loop
    //starts countDown for the game to begin
    countDown();
    //array that helps "randomize" the sequence of the arrows displayed for the game
    //on the LCD screen
    int rNums[] = {3,2,1,3,1,2,4,2,1,3,1,3,4,1,2,4,3,2,3,4,2,1,3,4,2,1,3,4,2,1,3,4,3,2,1};
    int length = sizeof(rNums) / sizeof(rNums[0]);
    volatile int score = 0;
    //r holds the random number
    volatile int r;
    int i = 1;
    //display the characters till the score = 9
    //the user wins the game when the score is 9
    while(i < length && score < 9) {
      r = rNums[i];
      clear_display();
      if (r == 3) {
        display_string("^");
      } else if (r == 4) {
        display_string("v");
      } else if (r == 2) {
        display_string("<");
      } else { // rand = 1
        display_string(">");
      }
      volatile unsigned long j = 0;
      //scoreUpdate keeps track of whether the key needs to be updated or not
      int scoreUpdate = 0;
      
      while(j < speed) {
        int key = keymaster();
        //if the correct key is pressed, the score needs to be updated
        if (r == key) {
          j = speed;
          scoreUpdate = 1;
        } else if (key != 0) {
          j = speed;
          scoreUpdate = 0;
        }
        j++;
      }
      
      //update score if scoreUpdate = 1
      if (scoreUpdate != 0) {
        score++;
      }
      
      if (i >= length) {
        i = 0;
      } else {
        i++;
      }
      
      //break out of the loop if wrong guess is made
      //by setting i to a high value
      if (scoreUpdate == 0) {
        win = 0;
        i = 99;
      }
      
    }
    WinOrLose(win, score);
    display_string("Replay?                                 Y: <   N: >");
    int key = 1;
    int press;
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
  }
}

//post: executes the countDown that is displayed on the LCD screen
//      before every new game the user chooses to play
void countDown() {
  clear_display();
  char* str;
  str = "Game will begin                         in: \0";
  display_string(str);
  delay(8000);
  for (int i = 5; i > 0; i--) {
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

// post: Displays a message to the user 
void  WinOrLose(int win, int score) {
  clear_display();
  char point[4];
  point[0] = score + '0';
  point[1] = '/';
  point[2] = 9 + '0';
  point[3] = '\0';
  if ((win == 1) && (score == 9)) {
    display_string("YOU WIN!!                                 Score: \0");
    display_string(point);
  } else {
    display_string("YOU LOSE!!                                Score: \0");
    display_string(point);
  }
  delay(16000);
  clear_display();
}
