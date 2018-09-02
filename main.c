// <TODO>: allow numpad as well as letter control

#include <string.h>

#include "gui.h"
#include "defines.h"

int main() {
  setup_windows();

  /*
  // load in default values
  memcpy(value, DEFAULT_VALUE, sizeof(DEFAULT_VALUE));

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
      step[i] = (value[i] - MIN[i]) / incdec[i];
      switch( i ) {
        // intentional fallthroughs
        case KP:
        case KI:
        case KD:
          // protect from "-0.00"
          mvwprintw(win[i].hdl, win[i].height / 2 - 1, win[i].width / 2, "%05.2f", 
              (value[i] < incdec[i]) ? 0 : value[i]);

          // print slider
          for( int j = 0; j < step[i]; j++ ) {
            mvwprintw(win[i].hdl, win[i].height / 2, WINDOW_MARGIN + 1 + j, "=");
          }
          mvwprintw(win[i].hdl, win[i].height / 2, WINDOW_MARGIN + 1 + step[i], "O");
          for( int j = step[i] + 1; j < win[i].width - (2 * (WINDOW_MARGIN + 1)); j++ ) {
            mvwprintw(win[i].hdl, win[i].height / 2, WINDOW_MARGIN + 1 + j, "-");
          }
          break;

        case SETPT:
          // print slider
          for( int j = 0; j < step[i]; j++ ) {
            mvwprintw(win[i].hdl, WINDOW_MARGIN + 1 + j, (win[i].width / 2) + 1, "|");
          }
          mvwprintw(win[i].hdl, WINDOW_MARGIN + 1 + step[i], (win[i].width / 2) + 1, "O");
          for( int j = step[i] + 1; j < win[i].height - (2 * (WINDOW_MARGIN + 1)); j++ ) {
            mvwprintw(win[i].hdl, WINDOW_MARGIN + 1 + j, (win[i].width / 2) + 1, "H");
          }
          break;

        case OUTPUT:
          break;
      }
      wrefresh(win[i].hdl);
    }
  } while( (in = getch()) != ' ' );

  */

  getch();
  endwin();
  return 0;
}
