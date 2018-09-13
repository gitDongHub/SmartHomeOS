#ifndef __TS_H__
#define __TS_H__

extern int fd_ts;

#define TS_CLICKED       1

#define TS_NORMAL		 0
#define TS_CLICK         1
#define TS_DOUBLE_CLICK  2
#define TS_SLIDE         3 
#define TS_LONG_PRESS    4

#define TS_MIN_OFFSET    5
#define TS_DOUBLE_CLICK_VAL  200   //200ms
#define TS_LONGPRESS_TIME    1000  //1s

#define TS_UP            4
#define TS_DOWN          5
#define TS_LEFT          6
#define TS_RIGHT         7
#define TS_RIGHT_UP      8
#define TS_LEFT_UP       9
#define TS_RIGHT_DOWN    10
#define TS_LEFT_DOWN     11

int ts_init();
int ts_scan(int* start_x,int* start_y,int* end_x,int* end_y);
int ts_uninit();


#endif
