#include <ncurses.h>
#include <string.h>

// window macros
#define WINDOW_MARGIN 2
#define NUMWIN 5
#define COEFF_LINES 5
#define SETPT_COLS  10

#define KP 0
#define KI 1
#define KD 2
#define SETPT 3
#define OUT 4
// display is calibrated for coefficient values between 0 (inclusive) and 100 (exclusive)
const float MIN[NUMWIN] =     {     0,     0,     0,    -5,    -6};
const float MAX[NUMWIN] =     {    10,  14.5,    29,    20,    21};
const float DEFAULT[NUMWIN] = {     0,     0,     0,     0,     0};

int main() {
  int stdscr_height = 0, stdscr_width = 0;
  WINDOW* windows[NUMWIN];
  int height[NUMWIN], width[NUMWIN], step[NUMWIN];
  float values[NUMWIN], incdec[NUMWIN];

  // start curses and get screen size
  initscr();
  getmaxyx(stdscr, stdscr_height, stdscr_width);
  // disable echo and hide the cursor
  noecho();
  curs_set(0);
  // required for some reason...?
  refresh();

  // spin up windows
  windows[KP] = newwin(COEFF_LINES, stdscr_width / 3, 0, 0);
  windows[KI] = newwin(COEFF_LINES, stdscr_width / 3, 0, stdscr_width / 3);
  windows[KD] = newwin(COEFF_LINES, 0, 0, 2 * (stdscr_width / 3));
  windows[SETPT] = newwin(0, SETPT_COLS, COEFF_LINES, 0);
  windows[OUT] = newwin(0, 0, COEFF_LINES, SETPT_COLS);

  // get window dims and calculate coeff_step
  for( int i = 0; i < NUMWIN; i++ ) {
    //wborder(windows[i], '|', '|', '-', '-', '.', '.', '\'', '\'');
    //wrefresh(windows[i]);
    getmaxyx(windows[i], height[i], width[i]);
    incdec[i] = (MAX[i] - MIN[i]) / (width[i] - (2 * WINDOW_MARGIN) - 1);
  }

  // draw slider borders and labels in K windows
  mvwprintw(windows[KP], height[KP] / 2 - 1, width[KP] / 2 - 4, "Kp: ");
  mvwprintw(windows[KP], height[KP] / 2 - 1, 5, "q");
  mvwprintw(windows[KP], height[KP] / 2 - 1, width[KP] - 6, "w");

  mvwprintw(windows[KI], height[KI] / 2 - 1, width[KI] / 2 - 4, "Ki: ");
  mvwprintw(windows[KI], height[KI] / 2 - 1, 5, "a");
  mvwprintw(windows[KI], height[KI] / 2 - 1, width[KI] - 6, "s");

  mvwprintw(windows[KD], height[KD] / 2 - 1, width[KD] / 2 - 4, "Kd: ");
  mvwprintw(windows[KD], height[KD] / 2 - 1, 5, "z");
  mvwprintw(windows[KD], height[KD] / 2 - 1, width[KD] - 6, "x");

  for( int i = 0; i < NUMWIN; i++ ) {
    if( i == KP || i == KI || i == KD ) {
      mvwprintw(windows[i], height[i] / 2 - 1, 2, "<-");
      mvwprintw(windows[i], height[i] / 2, 1, "|");
      mvwprintw(windows[i], height[i] / 2 - 1, width[i] - 4, "->");
      mvwprintw(windows[i], height[i] / 2, width[i] - 2, "|");
      wrefresh(windows[i]);
    }
  }

  // load in default values
  memcpy(values, DEFAULT, sizeof(DEFAULT));

  int in = 0;
  do {
    switch( in ) {
      case 'q':
        if( step[KP] > 0 ) {
          step[KP]--;
          values[KP] -= incdec[KP];
        }
        break;
      case 'w':
        if( step[KP] < width[KP] - (2 * WINDOW_MARGIN) - 1 ) {
          step[KP]++;
          values[KP] += incdec[KP];
        }
        break;
      case 'a':
        if( step[KI] > 0 ) {
          step[KI]--;
          values[KI] -= incdec[KI];
        }
        break;
      case 's':
        if( step[KI] < width[KI] - (2 * WINDOW_MARGIN) - 1 ) {
          step[KI]++;
          values[KI] += incdec[KI];
        }
        break;
      case 'z':
        if( step[KD] > 0 ) {
          step[KD]--;
          values[KD] -= incdec[KD];
        }
        break;
      case 'x':
        if( step[KD] < width[KD] - (2 * WINDOW_MARGIN) - 1 ) {
          step[KD]++;
          values[KD] += incdec[KD];
        }
        break;
      default:
        break;
    }

    // fill in sliders and values
    for( int i = 0; i < NUMWIN; i++ ) {
      if( i == KP || i == KI || i == KD ) {

        // protect from "-0.00"
        mvwprintw(windows[i], height[i] / 2 - 1, width[i] / 2, "%05.2f", 
            (values[i] < incdec[i]) ? 0 : values[i]);

        // print slider
        for( int j = 0; j < step[i]; j++ ) {
          mvwprintw(windows[i], height[i] / 2, 2 + j, "=");
        }
        mvwprintw(windows[i], height[i] / 2, 2 + step[i], "O");
        for( int j = step[i] + 1; j < width[i] - 4; j++ ) {
          mvwprintw(windows[i], height[i] / 2, 2 + j, "-");
        }
        wrefresh(windows[i]);
      }
    }
  } while( (in = getch()) != ' ' );

  endwin();
  return 0;
}
