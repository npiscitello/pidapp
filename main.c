// <TODO>: allow numpad as well as letter control

#include <string.h>

#include "gui.h"
#include "defines.h"

float value[NUMWIN];

int main() {
  setup_windows();
  memcpy(value, DEFAULT_VALUE, sizeof(DEFAULT_VALUE));
  //memcpy(value, MAX, sizeof(DEFAULT_VALUE));
  for( int i = 0; i < NUMWIN; i++ ) {
    set_window_value(i, value[i]);
  }

  /*
  int in = 0;
  do {
    switch( in ) {
      case 'q':
        if( step[KP] > 0 ) {
          value[KP] -= incdec[KP];
        }
        break;
      case 'w':
        if( step[KP] < win[KP].width - (2 * (WINDOW_MARGIN + 1)) - 1 ) {
          value[KP] += incdec[KP];
        }
        break;

      case 'a':
        if( step[KI] > 0 ) {
          value[KI] -= incdec[KI];
        }
        break;
      case 's':
        if( step[KI] < win[KI].width - (2 * (WINDOW_MARGIN + 1)) - 1 ) {
          value[KI] += incdec[KI];
        }
        break;

      case 'z':
        if( step[KD] > 0 ) {
          value[KD] -= incdec[KD];
        }
        break;
      case 'x':
        if( step[KD] < win[KD].width - (2 * (WINDOW_MARGIN + 1)) - 1 ) {
          value[KD] += incdec[KD];
        }
        break;

      case 'e':
        if( step[SETPT] > 0 ) {
          value[SETPT] -= incdec[SETPT];
        }
        break;
      case 'c':
        if( step[SETPT] < win[SETPT].height - (2 * (WINDOW_MARGIN + 1)) - 1 ) {
          value[SETPT] += incdec[SETPT];
        }
        break;

      case 'd':
        memcpy(value, DEFAULT_VALUE, sizeof(DEFAULT_VALUE));
        break;
      default:
        break;
    }

    // fill in sliders and values
    for( int i = 0; i < NUMWIN; i++ ) {
    }
  } while( (in = getch()) != ' ' );

  */

  getch();
  endwin();
  return 0;
}
