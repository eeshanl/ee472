#include "header.h"

#define RANGE 50
int state = 1;
char* str = ""; 
unsigned long int timeForLevel = 0; //time limit for guessing the correct height

// once the game starts, it allows the key presses to be used to play the game instead of controlling the menu
int gameTime = 0;
int score = 0;
void testGame(unsigned long int timeLimit){
 
  int safe = 0;
  unsigned long int finishTime = 0;
  unsigned long int time = getTimer1() - timeLimit;
  int random = getRandomNumber();
 // int i = 0;
  //int r[] = {800, 100, 800, 100, 800, 100, 800, 100, 800, 100};
  //int random = r[0];
  while(score < 9 && getTimer1() > time){ //time taken to complete the game
    int dist = ADC0_SSFIFO0_R;
    
//    char myData[5];
//    int a = dist/1000;
//    int b = (dist%1000)/100;
//    int c = (dist%1000%100)/10;
//    int d = (dist % 1000 % 100 % 10);
//    myData[0] = a + '0';
//    myData[1] = b + '0';
//    myData[2] = c + '0';
//    myData[3] = d + '0';
//    myData[4] = '\0';

    
    if(dist < random - RANGE) {
      if (safe != 3) {
        clear_display();
        display_string("Too High\0");
      }
      safe = 3;
    } else if (dist > random + RANGE) {
      if (safe != 2) {
        clear_display();
        display_string("Too Low\0");
      }
      safe = 2;
    } else {
      if (safe != 1) {
        clear_display();
        display_string("Hold\0");
      }
      if (safe == 1) {
        if (getTimer1() < finishTime) {
          //i++;
          //random = r[i];
          random = getRandomNumber();
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

//TODO: GENERATE RANDOM NUMBER FOR THE HEIGHT
int getRandomNumber(){
  return getSeed() % 824 + 100;
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
//easy ==> height range = +-200
//medium ==> height range = +-100
//hard ==> height range = +-50
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
