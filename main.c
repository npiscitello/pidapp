// <TODO>: allow numpad as well as letter control

#include <ncurses.h>
#include <string.h>

// window macros
#define NUMWIN          5
#define COEFF_LINES     4
#define SETPT_COLS      9

/* Horizontal OR vertical space between window edge and window content, in characters. The axis
 * depends on the shape of the window - horizontal space will be applied in short, wide windows
 * (coefficient windows) and vertical space will be applied in thin, tall windows (setpoint window).
 * Windows that are roughly square will have space applied on all sides (output window). */
#define WINDOW_MARGIN   1 

// Indexes
#define KP      0
#define KI      1
#define KD      2
#define SETPT   3
#define INPUT     4
// calibrated for min/max values between 0 (inclusive) and 100 (exclusive)
const float MIN[NUMWIN] =           {     0,     0,     0,     0,     0};
const float MAX[NUMWIN] =           {    10,  14.5,    29,    20,    20};
const float DEFAULT_VALUE[NUMWIN] =  {     5, 7.25,  14.5,    10,    10};

int main() {
  int stdscr_height = 0, stdscr_width = 0;
  struct metadata {
    WINDOW* hdl;
    int height;
    int width;
  };
  struct metadata win[NUMWIN]; 
  // these go outside the struct b/c they're not really window metadata
  int step[NUMWIN];
  float value[NUMWIN], incdec[NUMWIN];


  // start curses and get screen size
  initscr();
  getmaxyx(stdscr, stdscr_height, stdscr_width);
  // disable echo and hide the cursor
  noecho();
  curs_set(0);
  // required for some reason...?
  refresh();

  // spin up windows
  win[KP].hdl     = newwin(COEFF_LINES, stdscr_width / 3, 0, 0);
  win[KI].hdl     = newwin(COEFF_LINES, stdscr_width / 3, 0, stdscr_width / 3);
  win[KD].hdl     = newwin(COEFF_LINES, 0, 0, 2 * (stdscr_width / 3));
  win[SETPT].hdl  = newwin(0, SETPT_COLS, COEFF_LINES, 0);
  win[INPUT].hdl    = newwin(0, 0, COEFF_LINES, SETPT_COLS);

  for( int i = 0; i < NUMWIN; i++ ) {
#ifdef BORDERS
    wborder(win[i].hdl, '|', '|', '-', '-', '.', '.', '\'', '\'');
    wrefresh(win[i].hdl);
#endif
    getmaxyx(win[i].hdl, win[i].height, win[i].width);
    switch( i ) {
      // intentional fall-throughs
      case KP:
      case KI:
      case KD:
        incdec[i] = (MAX[i] - MIN[i]) / (win[i].width - (2 * WINDOW_MARGIN + 2) - 1);
        break;
      // intentional fall-through
      case SETPT:
      case INPUT:
        incdec[i] = (MAX[i] - MIN[i]) / (win[i].height - (2 * WINDOW_MARGIN + 2) - 1);
        break;
    }
  }

  // draw slider borders and labels in K windows
  mvwprintw(win[KP].hdl, win[KP].height / 2 - 1, win[KP].width / 2 - 4, "Kp: ");
  mvwprintw(win[KP].hdl, win[KP].height / 2 - 1, WINDOW_MARGIN + 4, "q");
  mvwprintw(win[KP].hdl, win[KP].height / 2 - 1, win[KP].width - (WINDOW_MARGIN + 5), "w");

  mvwprintw(win[KI].hdl, win[KI].height / 2 - 1, win[KI].width / 2 - 4, "Ki: ");
  mvwprintw(win[KI].hdl, win[KI].height / 2 - 1, WINDOW_MARGIN + 4, "a");
  mvwprintw(win[KI].hdl, win[KI].height / 2 - 1, win[KI].width - (WINDOW_MARGIN + 5), "s");

  mvwprintw(win[KD].hdl, win[KD].height / 2 - 1, win[KD].width / 2 - 4, "Kd: ");
  mvwprintw(win[KD].hdl, win[KD].height / 2 - 1, WINDOW_MARGIN + 4, "z");
  mvwprintw(win[KD].hdl, win[KD].height / 2 - 1, win[KD].width - (WINDOW_MARGIN + 5), "x");

  for( int i = 0; i < NUMWIN; i++ ) {
    if( i == KP || i == KI || i == KD ) {
      mvwprintw(win[i].hdl, win[i].height / 2 - 1, WINDOW_MARGIN + 1, "<-");
      mvwprintw(win[i].hdl, win[i].height / 2, WINDOW_MARGIN, "|");
      mvwprintw(win[i].hdl, win[i].height / 2 - 1, win[i].width - (WINDOW_MARGIN + 1) - 2, "->");
      mvwprintw(win[i].hdl, win[i].height / 2, win[i].width - (WINDOW_MARGIN + 1), "|");
      wrefresh(win[i].hdl);
    }
  }

  // draw slider borders and labels in setpoint and output windows (this is difficult b/c vertical)
  mvwprintw(win[SETPT].hdl, WINDOW_MARGIN, (win[SETPT].width / 2) + 1, "-");
  mvwprintw(win[SETPT].hdl, win[SETPT].height - (2 * WINDOW_MARGIN), (win[SETPT].width / 2) + 1, "-");
  mvwprintw(win[SETPT].hdl, WINDOW_MARGIN + 1, (win[SETPT].width / 2) - 1, "^");
  mvwprintw(win[SETPT].hdl, WINDOW_MARGIN + 2, (win[SETPT].width / 2) - 1, "e");
  mvwprintw(win[SETPT].hdl, win[SETPT].height - (2 * WINDOW_MARGIN) - 1, (win[SETPT].width / 2) - 1, "v");
  mvwprintw(win[SETPT].hdl, win[SETPT].height - (2 * WINDOW_MARGIN) - 2, (win[SETPT].width / 2) - 1, "c");
  mvwprintw(win[SETPT].hdl, (win[SETPT].height / 2) - 2, (win[SETPT].width / 2) - 1, "S");
  mvwprintw(win[SETPT].hdl, (win[SETPT].height / 2) - 1, (win[SETPT].width / 2) - 1, "E");
  mvwprintw(win[SETPT].hdl, (win[SETPT].height / 2) + 0, (win[SETPT].width / 2) - 1, "T");
  mvwprintw(win[SETPT].hdl, (win[SETPT].height / 2) + 1, (win[SETPT].width / 2) - 1, "P");
  mvwprintw(win[SETPT].hdl, (win[SETPT].height / 2) + 2, (win[SETPT].width / 2) - 1, "T");
  wrefresh(win[SETPT].hdl);

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

        case INPUT:
          break;
      }
      wrefresh(win[i].hdl);
    }
  } while( (in = getch()) != ' ' );

  endwin();
  return 0;
}
