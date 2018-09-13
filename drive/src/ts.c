#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <linux/input.h>
#include "ts.h"

int fd_ts;

int ts_init()
{
	fd_ts = open("/dev/event0",O_RDWR);
	if(fd_ts == -1)
	{
		perror("open file failed");
		return -1;
	}
	printf("fd_ts = %d\n",fd_ts);
}

int ts_scan(int* start_x,int* start_y,int* end_x,int* end_y)
{
	int i,j;
	int x,y;
	ssize_t r_ret;
	int event_ret = TS_NORMAL;
	struct input_event ts_buf;
	time_t cur_time_s_out;
	suseconds_t cur_time_us_out;
	
	static unsigned char has_CLICK = !TS_CLICKED;
	static time_t pre_time_s_out;
	static suseconds_t pre_time_us_out;
	unsigned int time_ms_interval;
	
	while(1)
	{
 		r_ret = read(fd_ts,&ts_buf,sizeof(ts_buf));
		if(r_ret == -1)
		{
			perror("write ts failed");
			return -1;
		}
		
		if(ts_buf.type == EV_ABS || ts_buf.type == EV_KEY)
		{
			if(ts_buf.code == ABS_X)
			{
				x = ts_buf.value;
			}
			else if(ts_buf.code == ABS_Y)
			{
				y = ts_buf.value;
			}
			else if(ts_buf.code == ABS_PRESSURE || ts_buf.code == BTN_TOUCH)
			{
				if(ts_buf.value > 0)
				{
					*start_x = x;
					*start_y = y;
					printf("IN\n");
				}
				else
				{
					*end_x = x;
					*end_y = y;
					
					cur_time_s_out = ts_buf.time.tv_sec;
					cur_time_us_out = ts_buf.time.tv_usec;
					
					if(has_CLICK == TS_CLICKED)
					{
						time_ms_interval = (cur_time_s_out - pre_time_s_out) * 1000 + ((cur_time_us_out - pre_time_us_out) / 1000);
						printf("double click time interval is %ld\n",time_ms_interval);
					}
					pre_time_s_out = cur_time_s_out;
					pre_time_us_out = cur_time_us_out;
					
					printf("OUT:pre(%d-%d)\n",pre_time_s_out,pre_time_us_out);
					printf("OUT:cur(%d-%d)\n",cur_time_s_out,cur_time_us_out);
					
					break;
				}
			}
		}
	}
	
	printf("start_x=%d,\tstart_y=%d\n",*start_x,*start_y);
	printf("end_x=%d,\tend_y=%d\n",*end_x,*end_y);
	
	if(abs(*end_x - *start_x)<=TS_MIN_OFFSET && abs(*end_y-*start_y)<=TS_MIN_OFFSET)
	{
		if(has_CLICK == TS_CLICKED && time_ms_interval < TS_DOUBLE_CLICK_VAL)
		{
			event_ret = TS_DOUBLE_CLICK;
			has_CLICK = !TS_CLICKED;
		}
		else 
		{
			event_ret = TS_CLICK;
			has_CLICK = TS_CLICKED;
		}
	}
	else
	{
		event_ret = TS_SLIDE;
	}
	
	printf("event_ret = %d\n",event_ret);

	return event_ret;
}

int ts_get_direction(int start_x,int start_y,int end_x,int end_y)
{
	int x_offset = end_x - start_x;
	int y_offset = end_y - start_y;
	int direction = TS_NORMAL;

	// detect left and left up (on secound quadrant)
	if(x_offset < 0 && y_offset < 0)
	{
		if(abs(x_offset) >= abs(y_offset))
		{
			direction = TS_LEFT;
		}
		else
		{
			direction = TS_UP;
		}
	}
	// detect right up and up (on first quadrant)
	else if(x_offset > 0 && y_offset < 0)
	{
		if(abs(x_offset) >= abs(y_offset))
		{
			direction = TS_RIGHT;
		}
		else
		{
			direction = TS_UP;
		}
	}
	// detect left down and down (on third quadrant)
	else if(x_offset < 0 && y_offset > 0)
	{
		if(abs(x_offset) > abs(y_offset))
		{
			direction = TS_LEFT;
		}
		else
		{
			direction = TS_DOWN;
		}	
	}
	// detect right down and down (on forth quadrant)
	else if(x_offset > 0 && y_offset > 0)
	{
		if(abs(x_offset) >= abs(y_offset))
		{
			direction = TS_RIGHT;
		}
		else
		{
			direction = TS_DOWN;
		}
	}
	// detect right and left(on x axes)
	else if(y_offset == 0 && x_offset != 0)
	{
		if(x_offset > 0)
		{
			direction =TS_RIGHT;
		}
		else
		{
			direction = TS_LEFT;
		}	
	}
	// detect down and up(on y axes)
	else if(x_offset == 0 && y_offset != 0)
	{
		if(y_offset > 0)
		{
			direction = TS_DOWN;
		}
		else
		{
			direction = TS_UP;
		}
	}
	
	printf("direction = %d\n",direction);
	return direction;
}

int ts_uninit()
{
	close(fd_ts);
	
	return 0;
}

