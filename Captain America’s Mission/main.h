#ifndef MAIN_H
#define MAIN_H

#include "gba.h"

// TODO: Create any necessary structs

enum gba_state {
  START,
  PLAY,
  WIN,
  LOSE,
};

struct ball {
		int row;
		int col;
		int rd;
		int cd;
    int width;
    int height;
	};

struct shield {
  int row;
  int col;
  int rd;
  int cd;
  int width;
  int height;
};

/*
* For example, for a Snake game, one could be:
*
* struct snake {
*   int heading;
*   int length;
*   int row;
*   int col;
* };
*
* Example of a struct to hold state machine data:
*
* struct state {
*   int currentState;
*   int nextState;
* };
*
*/

void setBackground(void);

#endif
