#include "header.h"

char* str = "";
int state = 1;
int gameTime = 0;

void mainMenu() {
  //state 1 is easy
  // state 2 is medium
  // state 3 is hard
  
  int key = keymaster();
  if (gameTime == 0) {
    if (state == 1 && key == 4) {
      clear_display();
      state = 2;
      str = "Main Menu:                              Medium\0";
      display_string(str);
    } else if (state == 1 && key == 3) {
      clear_display();
      state = 3;
      str = "Main Menu:                              Hard\0";
      display_string(str);
    } else if (state == 2 && key == 4) {
      clear_display();
      state = 3;
      str = "Main Menu:                              Hard\0";
      display_string(str);
    } else if (state == 2 && key == 3) {
      clear_display();
      state = 1;
      str = "Main Menu:                              Easy\0";
      display_string(str);
    } else if (state == 3 && key == 4) {
      clear_display();
      state = 1;
      str = "Main Menu:                              Easy\0";
      display_string(str);
    } else if (state == 3 && key == 3) {
      clear_display();
      state = 2;
      str = "Main Menu:                              Medium\0";
      display_string(str);
    }
  }
  if (key == 5) {
    gameTime = -1;
    game(state);
    goToStateOne();
  }
  
}

void goToStateOne() {
  state = 1;
  gameTime = 0;
  clear_display();
  state = 1;
  str = "Main Menu:                              Easy\0";
  display_string(str);
}

void game(int level) {
  clear_display();
  
  startGame(level);
}



void startGame(int level) {
  int speed = 0;
  if (level == 1) {
    speed = 250000;
  } else if (level == 2) {
    speed = 100000;
  } else {
    speed = 75000;
  }
  int win = 1;
  int playAgain = 1;
  
  while (playAgain) {
    countDown();
    int rNums[] = {3,2,1,3,1,2,4,2,1,3,1,3,4,1,2,4,3,2,3,4,2,1,3,4,2,1,3,4,2,1,3,4,3,2,1};
    int length = sizeof(rNums) / sizeof(rNums[0]);
    volatile int score = 0;
    volatile int r;
    int i = 1;
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
      int scoreUpdate = 0;
      
      while(j < speed) {
        int key = keymaster();
        if (r == key) {
          j = speed;
          scoreUpdate = 1;
        } else if (key != 0) {
          j = speed;
          scoreUpdate = 0;
        }
        j++;
      }
      
      if (scoreUpdate != 0) {
        score++;
      }
      
      if (i >= length) {
        i = 0;
      } else {
        i++;
      }
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

void countDown() {
  clear_display();
  char* str;
  str = "Game will begin                         in: \0";
  display_string(str);
  delay(8000);
  for (int i = 5; i > 0; i--) {
    //clear_display();
    char count[2];
    count[0] = i + '0';
    count[1] = '\0';
    display_string(count);
    
    delay(8000);
    cursorLeft();
  }
}


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