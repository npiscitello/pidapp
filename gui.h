#ifndef GUI_H
#define GUI_H

#include <ncurses.h>
#include <stdlib.h>
#include <math.h>

#include "defines.h"

struct metadata {
  WINDOW* hdl;
  int height;
  int width;
};

struct metadata win[NUMWIN]; 
// these go outside the struct b/c they're not really window metadata
int step[NUMWIN];
float incdec[NUMWIN];



void setup_windows(void) {
  // get overall dimensions
  int stdscr_height = 0, stdscr_width = 0;
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
  win[SETPT].hdl  = newwin(0, SETPT_COLS, COEFF_LINES, 0);
  win[KD].hdl     = newwin(COEFF_LINES, 0, 0, 2 * (stdscr_width / 3));
  win[OUTPUT].hdl    = newwin(0, 0, COEFF_LINES, SETPT_COLS);


  // calculate the value of a tick mark based on the window size
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
      case OUTPUT:
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

  return;
}



// set a value on the slider of the specified window
void set_window_value(int window, float value) {

  // the int implicitly floors it, but that's not necessarily what we want
  // besides, its better to be explicit
  int step = round((value - MIN[window]) / incdec[window]);
  switch( window ) {
    // intentional fallthroughs
    case KP:
    case KI:
    case KD:
      // protect from "-0.00"
      mvwprintw(win[window].hdl, win[window].height / 2 - 1, win[window].width / 2, "%05.2f", 
          (value < incdec[window]) ? 0 : value);

      // print slider
      for( int j = 0; j < step; j++ ) {
        mvwprintw(win[window].hdl, win[window].height / 2, WINDOW_MARGIN + 1 + j, "=");
      }
      mvwprintw(win[window].hdl, win[window].height / 2, WINDOW_MARGIN + 1 + step, "O");
      for( int j = step + 1; j < win[window].width - (2 * (WINDOW_MARGIN + 1)); j++ ) {
        mvwprintw(win[window].hdl, win[window].height / 2, WINDOW_MARGIN + 1 + j, "-");
      }
      break;

    case SETPT:
      // print slider
      for( int j = 0; j < step; j++ ) {
        mvwprintw(win[window].hdl, WINDOW_MARGIN + 1 + j, (win[window].width / 2) + 1, "|");
      }
      mvwprintw(win[window].hdl, WINDOW_MARGIN + 1 + step, (win[window].width / 2) + 1, "O");
      for( int j = step + 1; j < win[window].height - (2 * (WINDOW_MARGIN + 1)); j++ ) {
        mvwprintw(win[window].hdl, WINDOW_MARGIN + 1 + j, (win[window].width / 2) + 1, "H");
      }
      break;

    case OUTPUT:
      break;
  }
  wrefresh(win[window].hdl);

  return;
}

#endif
