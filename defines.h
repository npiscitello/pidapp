#ifndef RETURN_CODES_H
#define RETURN_CODES_H

// return codes
#define RET_OK          0
#define RET_MALLOC_ERR  1

// window macros
#define NUMWIN          5
#define COEFF_LINES     4
#define SETPT_COLS      9

// Indexes
#define KP      0   // proportional
#define KI      1   // integral
#define KD      2   // derivative
#define SETPT   3   // setpoint
#define OUTPUT  4   // PID-controlled output

/* Horizontal OR vertical space between window edge and window content, in characters. The axis
 * depends on the shape of the window - horizontal space will be applied in short, wide windows
 * (coefficient windows) and vertical space will be applied in thin, tall windows (setpoint window).
 * Windows that are roughly square will have space applied on all sides (output window). */
#define WINDOW_MARGIN   1 

// calibrated for min/max values between 0 (inclusive) and 100 (exclusive)
const float MIN[NUMWIN] =           {     0,     0,     0,     0,     0};
const float MAX[NUMWIN] =           {    10,  14.5,    29,    20,    20};
const float DEFAULT_VALUE[NUMWIN] = {     5,  7.25,  14.5,    10,    10};

// milliseconds between output screen updates
//#define UPDATE_PER 250
#define UPDATE_PER 10000

#endif
