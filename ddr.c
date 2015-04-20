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
      //if (state == 1) {
         goToStateOne();
      //}
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
//   char countdown[2];
//   RIT128x96x4StringDraw("Game will begin in:\0", 15, 34, 15);
//   for (int i = 5; i > 0 ; i--) {
//     countdown[0] = i + '0';
//     countdown[1] = '\0';
//     RIT128x96x4StringDraw(countdown, 15, 44, 15);
//     delay(4000);
//   }
//   RIT128x96x4StringDraw("                         \0", 15, 34, 15);
//   RIT128x96x4StringDraw("                         \0", 15, 44, 15);
//   RIT128x96x4StringDraw("GO!!\0", 15, 34, 15);
//   delay(4000);
//   RIT128x96x4StringDraw("                         \0", 15, 34, 15);
   startGame(level);
}

void startGame(int level) {
   RIT128x96x4Init(1000000);
   char myData[2];
   int speed = 0;
   if (level == 1) {
     speed = 500000;
   } else if (level == 2) {
     speed = 250000;
   } else {
     speed = 100000;
   }
   int playAgain = 1;
   while (playAgain) {
      int rNums[] = {3,2,1,3,3,2,4,2,1,4,2,3,1,2,1,3,4,2,3,4,2,1,3,4,2,1,3,4,2,2,3,4,3,2,1};
      int length = sizeof(rNums) / sizeof(rNums[0]);
      //int length = 35;
      volatile int score = 0;
      //int win = 0;S
      volatile int r;
      int i = 0;
      while(i <= length && score < 9) {
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
            if (r == keymaster()) {
               j = speed;
               scoreUpdate = 1;
            }
            j++;
         }

         if (scoreUpdate != 0) {
            score++;
         }

         myData[0] = score + '0';
         myData[1] = '\0';
         RIT128x96x4StringDraw(myData, 15, 44, 15);
         if (i >= 35) {
            i = 0;
         } else {
            i++;
         }
         
      }
        RIT128x96x4StringDraw("CONGRATS YOU WIN!\0", 15, 44, 15);
        //RIT128x96x4StringDraw("YOU SUCK YOU LOST!\0", 15, 44, 15);
        clear_display();
        display_string("Play again?                              Y: <   N: >");
        int key = 1;
        int press;
        while(key) {
           press = keymaster();
           if (press == 2 || press == 1) {
              key = 0;
           }
        }
        RIT128x96x4StringDraw("                        \0", 15, 44, 15);
        if (press == 2) {
           clear_display();
        } else {
           playAgain = 0;
        }
      }
   }
