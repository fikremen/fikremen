#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#include "gba.h"
#include "images/garbage.h"
#include "colorful.h"
#include "welcomeScreen.h"
#include "hand.h"
#include "cRFace.h"
#include "shieldPic.h"
#include "oscars.h"
#include "winScreen.h"
#include "loseScreen.h"






int hitLeftEdge(struct ball *ball, struct shield *shield) ;


/* TODO: */
// Include any header files for title screen or exit
// screen images generated by nin10kit. Example for the provided garbage
// image:
// #include "images/garbage.h"

/* TODO: */
// Add any additional states you need for your app. You are not requried to use
// these specific provided states.

int main(void) {
  int tempRight = 1;
  int tempLeft = 1;
  int tempUp = 1;
  int tempDown = 1;
  int lives = 5;
  int timer = 30;
  int counter = 60;

  int buttonWasDown[10] = {0};
	int buttonJustReleased[10] = {0};

  struct ball handBall;
	struct ball *bp = &handBall;
	bp->row = 10;  //todo make random
	bp->col = 10;  //todo make random
	bp->rd = 2;
	bp->cd = 2;
	bp->width = HAND_WIDTH;
  bp->height = HAND_HEIGHT;

  struct shield shield;
  struct shield *sp = &shield;
  sp->row = 100;  
	sp->col = 100;  
	sp->rd = 1;
	sp->cd = 1;
	sp->width = SHIELDPIC_WIDTH; //make concrete
  sp->height = SHIELDPIC_HEIGHT; //make concrete

  struct ball oldBall = handBall;
  struct shield oldshield = shield;
  //struct shield *osp = &oldshield;


  /* TODO: */
  // Manipulate REG_DISPCNT here to set Mode 3. //
  REG_DISPCNT = MODE3 | BG2_ENABLE;
  //drawImageDMA(10, 15, COLORFUL_WIDTH, COLORFUL_HEIGHT, colorful);
  //setPixel(10,50,RED);
  //drawRectDMA(20,60, 5,10,BLUE);
  //drawFullScreenImageDMA(garbage);
  //drawImageDMA(0,0,GOOGLE_WIDTH,GOOGLE_HEIGHT,google);

  // Save current and previous state of button input.
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS; 

  // Load initial application state
  enum gba_state state = START;

  while (1) {
    currentButtons = BUTTONS; // Load the current state of the buttons

    // Update button status
		for (int i=0; i<10; i++) {
			buttonJustReleased[i] = KEY_DOWN(i, BUTTONS) == 0 && buttonWasDown[i];
			buttonWasDown[i] = KEY_DOWN(i, BUTTONS) != 0;
		}


    /* TODO: */
    // Manipulate the state machine below as needed //
    // NOTE: Call waitForVBlank() before you draw

    switch (state) {
      case START:
        waitForVBlank();
        drawFullScreenImageDMA(welcomeScreen); 
        if (buttonJustReleased[BUTTON_START]) {
			    setBackground();
          state = PLAY;
		    }
        // state = ?
        break;
      case PLAY:
        oldBall = handBall;
        oldshield = shield;
        counter--;

        //ball bouncing around
        bp->row = bp->row + bp->rd;
        bp->col += bp->cd;
        if(bp->row < 0) {
          bp->row = 0;
          bp->rd = -bp->rd;
        }
        if(bp->row > (HEIGHT - bp->height)) {
          bp->row = HEIGHT - bp->height;
          bp->rd = -bp->rd;
        }
        if(bp->col < 0) {
          bp->col = 0;
          bp->cd = -bp->cd;
          }
        if(bp->col > (WIDTH - CRFACE_WIDTH - bp->width)) {
          bp->col = WIDTH - CRFACE_WIDTH - bp->width;
          bp->cd = -bp->cd;
          lives--;
        }


        //controlling the shield
        if(sp->row >= 0 && sp->row <= (HEIGHT - sp->height) && KEY_DOWN(BUTTON_UP,BUTTONS) && tempUp != 0) {
          if(sp->row != 0) {
            sp->row -= 1;
          }
        }
        if(sp->row >= 0 && sp->row <= (HEIGHT - sp->height) && KEY_DOWN(BUTTON_DOWN,BUTTONS) && tempDown != 0) {
          if(sp->row != HEIGHT - sp->height) {
            sp->row += 1;
          }
        }
        if(sp->col >= 0 && sp->col <= (WIDTH - CRFACE_WIDTH - sp->width) && KEY_DOWN(BUTTON_LEFT,BUTTONS) && tempLeft != 0) {
          if(sp->col != 0) {
            sp->col -= 1;
          }
        }
        if(sp->col >= 0 && sp->col <= (WIDTH - CRFACE_WIDTH - sp->width) && KEY_DOWN(BUTTON_RIGHT,BUTTONS) && tempRight != 0) {
          if(sp->col != WIDTH - CRFACE_WIDTH - sp->width) {
            sp->col += 1;
          }
        }
        
        //collisions
        //|| ((bp->col) == (sp->col + sp->width + 1) && KEY_DOWN(BUTTON_RIGHT,BUTTONS))
        tempRight = 1;
        tempLeft = 1;
        tempUp = 1;
        tempDown = 1;
        if ((bp->row) > (sp->row - bp->height) && (bp->row) < (sp->row + sp->height)) {
          if (((bp->col + bp->width) >= sp->col && (bp->col + bp->width) < sp->col + 2) || ((bp->col) == (sp->col + sp->width) && (bp->col) > sp->col + sp->width - 2) ) {
            bp->cd = -bp->cd;
            tempRight = 0;
            tempLeft = 0;
          }
        }

        if ((bp->col) > (sp->col - bp->width) && (bp->col) < (sp->col + sp->width)) {
          if (((bp->row + bp->height) >= sp->row && (bp->row + bp->height) < sp->row + 2) || ((bp->row) == (sp->row + sp->height) && (bp->row) > sp->row + sp->height - 2) ) {
            bp->rd = -bp->rd;
            tempUp = 0;
            tempDown = 0;
          }
        }

        //timer
        if (counter <= 0) {
          counter = 60;
          timer--;
        }

        // state = ?
        if (timer <= 0) {
          state = WIN;
        }
        if (lives <= 0) {
          state = LOSE;
        }
        if (buttonJustReleased[BUTTON_START]) {
          state = START;
        }
        
        


        //draw
        waitForVBlank(); //dont draw anything above this, do you calculations and then draw stuff under it
        undrawImageDMA(oldBall.row,oldBall.col,oldBall.width,oldBall.height,oscars);
        undrawImageDMA(150,5,150,10,oscars); //to undraw section for text
        undrawImageDMA(oldshield.row,oldshield.col,oldshield.width,oldshield.height,oscars);
        drawImageDMA(handBall.row,handBall.col,handBall.width,handBall.height,hand);
        drawImageDMA(shield.row,shield.col,shield.width,shield.height,shieldPic);
        char buffer[51];
			  sprintf(buffer, "Lives: %d", lives);
			  drawString(150, 5, buffer, YELLOW);

        char buffer2[51];
			  sprintf(buffer2, "Timer: %d", timer);
			  drawString(150, 75, buffer2, YELLOW);
        break;
      case WIN:
        drawImageDMA(0,0,WINSCREEN_WIDTH,WINSCREEN_HEIGHT,winScreen);
        if (buttonJustReleased[BUTTON_START]) {
          state = PLAY;
          tempRight = 1;
          tempLeft = 1;
          tempUp = 1;
          tempDown = 1;
          lives = 5;
          timer = 30;
          counter = 60;
          setBackground();
        }
        // state = ?
        break;
      case LOSE:
        drawImageDMA(0,0,LOSESCREEN_WIDTH,LOSESCREEN_HEIGHT,loseScreen);
        if (buttonJustReleased[BUTTON_START]) {
          state = PLAY;
          tempRight = 1;
          tempLeft = 1;
          tempUp = 1;
          tempDown = 1;
          lives = 10;
          timer = 30;
          counter = 60;
          setBackground();
        }
        
        // state = ?
        break;
    }

    previousButtons = currentButtons; // Store the current state of the buttons
  }

  UNUSED(previousButtons); // You can remove this once previousButtons is used

  return 0;
}

void setBackground(void) {
  drawImageDMA(0,0,OSCARS_WIDTH,OSCARS_HEIGHT,oscars); //todo change background
  drawImageDMA(0,200,CRFACE_WIDTH,CRFACE_HEIGHT,cRFace);
  drawImageDMA(40,200,CRFACE_WIDTH,CRFACE_HEIGHT,cRFace);
  drawImageDMA(80,200,CRFACE_WIDTH,CRFACE_HEIGHT,cRFace);
  drawImageDMA(120,200,CRFACE_WIDTH,CRFACE_HEIGHT,cRFace);

}

int hitLeftEdge(struct ball *ball, struct shield *shield) {
  if ((ball->row) > (shield->row - ball->height) && (ball->row) < (shield->row + shield->height)) {
    if ((ball->col + ball->width) >= shield->col && (ball->col + ball->width) < shield->col + 2) {
      return 1;
    }
  }
  return 0;
}
