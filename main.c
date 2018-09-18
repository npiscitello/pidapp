// <TODO>: allow numpad as well as letter control

#include <string.h>
#include <sys/time.h>

#include "gui.h"
#include "defines.h"

float value[NUMWIN];

struct timeval time;

int main() {
  setup_windows();
  memcpy(value, DEFAULT_VALUE, sizeof(DEFAULT_VALUE));
  for( int i = 0; i < NUMWIN; i++ ) {
    set_window_value(i, value[i]);
  }

  // this loop needs to run full speed - we poll for timing and keypresses
  nodelay(stdscr, true);
  int in = 0;
  do {
    // our increment/decrement value is calculated based on window size to ensure the slider always
    // moves when you hit a button
    switch( in ) {
      case 'q':
        (value[KP] >= MIN[KP] + incdec[KP]) ? 
          (value[KP] -= incdec[KP]) : 
          (value[KP] = MIN[KP]);
        break;
      case 'w':
        (value[KP] <= MAX[KP] - incdec[KP]) ? 
          (value[KP] += incdec[KP]) : 
          (value[KP] = MAX[KP]);
        break;

      case 'a':
        (value[KI] >= MIN[KI] + incdec[KI]) ? 
          (value[KI] -= incdec[KI]) : 
          (value[KI] = MIN[KI]);
        break;
      case 's':
        (value[KI] <= MAX[KI] - incdec[KI]) ? 
          (value[KI] += incdec[KI]) : 
          (value[KI] = MAX[KI]);
        break;

      case 'z':
        (value[KD] >= MIN[KD] + incdec[KD]) ? 
          (value[KD] -= incdec[KD]) : 
          (value[KD] = MIN[KD]);
        break;
      case 'x':
        (value[KD] <= MAX[KD] - incdec[KD]) ? 
          (value[KD] += incdec[KD]) : 
          (value[KD] = MAX[KD]);
        break;

      case 'c':
        (value[SETPT] >= MIN[SETPT] + incdec[SETPT]) ? 
          (value[SETPT] -= incdec[SETPT]) : 
          (value[SETPT] = MIN[SETPT]);
        break;
      case 'e':
        (value[SETPT] <= MAX[SETPT] - incdec[SETPT]) ? 
          (value[SETPT] += incdec[SETPT]) : 
          (value[SETPT] = MAX[SETPT]);
        break;

      case 'd':
        memcpy(value, DEFAULT_VALUE, sizeof(DEFAULT_VALUE));
        break;
      default:
        break;
    }

    // fill in sliders and values
    update_input_windows(value);

    // figure out if we need to update the output
    gettimeofday(&time, NULL);
    if( time.tv_usec % UPDATE_PER * 1000 == 0 ) {
      // update stuff
      update_output_window(value);
    }
  } while( (in = getch()) != ' ' );

  endwin();
  return 0;
}
