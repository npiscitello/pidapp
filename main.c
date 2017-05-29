// <TODO> make labels and positioning more dynamic - be able to change one macro to change position
// of labels, etc.

#include <ncurses.h>

#define NUMWIN 5
#define KP 0
#define KI 1
#define KD 2
#define SETPT 3
#define OUT 4

// yes, I know, global vars are bad.
int maxheight = 0, maxwidth = 0;
WINDOW* windows[NUMWIN];
int values[NUMWIN], maxval[NUMWIN];
int height[NUMWIN], width[NUMWIN];

int main() {
  // start curses and get screen size
  initscr();
  getmaxyx(stdscr, maxheight, maxwidth);
  // disable echo and hide the cursor
  noecho();
  curs_set(0);
  // required for some reason...?
  refresh();

  // spin up windows
  windows[KP] = newwin(5, maxwidth / 3, 0, 0);
  windows[KI] = newwin(5, maxwidth / 3, 0, (maxwidth / 3));
  windows[KD] = newwin(5, 0, 0, 2 * (maxwidth / 3));
  windows[SETPT] = newwin(0, 10, 5, 0);
  windows[OUT] = newwin(0, 0, 5, 10);

  // get window dims
  for( int i = 0; i < NUMWIN; i++ ) {
    //wborder(windows[i], '|', '|', '-', '-', '.', '.', '\'', '\'');
    //wrefresh(windows[i]);
    getmaxyx(windows[i], height[i], width[i]);
  }

  // draw slider borders and labels in K windows
  mvwprintw(windows[KP], height[KP] / 2 - 1, width[KP] / 2 - 3, "Kp: ");
  mvwprintw(windows[KP], height[KP] / 2 - 1, 5, "q");
  mvwprintw(windows[KP], height[KP] / 2 - 1, width[KP] - 6, "w");

  mvwprintw(windows[KI], height[KI] / 2 - 1, width[KI] / 2 - 3, "Ki: ");
  mvwprintw(windows[KI], height[KI] / 2 - 1, 5, "a");
  mvwprintw(windows[KI], height[KI] / 2 - 1, width[KI] - 6, "s");

  mvwprintw(windows[KD], height[KD] / 2 - 1, width[KD] / 2 - 3, "Kd: ");
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

  // default values
  values[KP] = 0;
  values[KI] = 0;
  values[KD] = 0;

  int in = 0;
  do {
    switch( in ) {
      case 'q':
        if( values[KP] > 0 ) {
          values[KP]--;
        }
        break;
      case 'w':
        if( values[KP] < width[KP] - 5 ) {
          values[KP]++;
        }
        break;
      case 'a':
        if( values[KI] > 0 ) {
          values[KI]--;
        }
        break;
      case 's':
        if( values[KI] < width[KI] - 5 ) {
          values[KI]++;
        }
        break;
      case 'z':
        if( values[KD] > 0 ) {
          values[KD]--;
        }
        break;
      case 'x':
        if( values[KD] < width[KD] - 5 ) {
          values[KD]++;
        }
        break;
      default:
        break;
    }

    // fill in sliders and values
    for( int i = 0; i < NUMWIN; i++ ) {
      if( i == KP || i == KI || i == KD ) {
        mvwprintw(windows[i], height[i] / 2 - 1, width[i] / 2 + 1, "%03d", values[i]);
        for( int j = 0; j < values[i]; j++ ) {
          mvwprintw(windows[i], height[i] / 2, 2 + j, "=");
        }
        mvwprintw(windows[i], height[i] / 2, 2 + values[i], "O");
        for( int j = values[i] + 1; j < width[i] - 4; j++ ) {
          mvwprintw(windows[i], height[i] / 2, 2 + j, "-");
        }
        wrefresh(windows[i]);
      }
    }
  } while( (in = getch()) != ' ' );

  endwin();
  return 0;
}
