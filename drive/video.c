#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include "lcd.h"
#include "ts.h"

#define LIST_SIZE   100
#define CMD_LEN     100
#define NO_PLAY     0
#define PLAYING     1
#define STOP        2

char** pCurrent_video = NULL;

int get_video_list(const char* dir_path, char* pvideo_list_buf[], int count);
void play(char* file_name);
void stop();
void go_on();
void quit();
void next_clip();
void pre_clip();

int start_video_app()
{
	/* 1.get video file on current directory*/
	char* video_list_buf[LIST_SIZE] = {"\0"};

	int file_cnt = get_video_list("./",video_list_buf,LIST_SIZE);	
	if(file_cnt <= 0)
	{
		printf("No such type of file or error in current directory\n");
		return -1;
	}
	printf("get %d clips in current directory\n",file_cnt);

	pCurrent_video = video_list_buf;

	//printf("auto play the first clip: %s\n",video_list_buf[0]);
	//play(video_list_buf[0]);
	
	/* 2.initalize the interface of video player */
	lcd_init();
	ts_init();
	

	int x0,y0;
	int x,y;
	int ts_ret;
	unsigned char player_state = NO_PLAY;

	while(1)
	{
		/* 3.wait for touch and udpate state*/
		ts_ret = ts_scan(&x0,&y0,&x,&y);
		if(ts_ret == TS_CLICK)
		{
			printf("current playing clip:%s\n",*pCurrent_video);
			if(x > 150 && x < 250 && y > 240 && y < 340)
			{
				if(strcmp(*pCurrent_video,video_list_buf[0]) == 0)
				{
					printf("this is the first clip\n");
				}
				else if(file_cnt == 1)
				{
					printf("there only one clip on current directory\n");
				}
				else
				{
					pre_clip();
				}
			}
			else if(x > 550 && x < 650 && y > 240 && y < 340)
			{
				if(strcmp(*pCurrent_video,video_list_buf[file_cnt-1]) == 0)
				{
					printf("this is the last clip\n");
				}
				else if(file_cnt == 1)
				{
					printf("there only one clip on current directory\n");
				}
				else
				{
					next_clip();
				}
			}
			else if(x > 350 && x < 450 && y > 240 && y < 340)
			{
				if(player_state == PLAYING)
				{	
					lcd_show_bmp(350,240,100,100,"play.bmp");
					stop();
					player_state = STOP;
				}
				else 
				{
					if(player_state == STOP)
					{
						lcd_show_bmp(350,240,100,100,"stop.bmp");
						go_on();
						player_state = PLAYING;
					}
					else
					{
						lcd_show_bmp(350,240,100,100,"stop.bmp");
						play(*pCurrent_video);
						player_state = PLAYING;
					}
				}
			}
		}
		else if(ts_ret == TS_SLIDE)
		{
			break;
		}
	}
	
	quit();
	ts_uninit();
	lcd_uninit();

	return 0;
}

int get_video_list(const char* dir_path, char* pvideo_list_buf[],int count)
{
	int file_num = 0;
	DIR* op = opendir(dir_path);
	if(op == NULL)
	{
		perror("open directory failed");
		return -1;
	}

	struct dirent* dp;
	
	while(1)
	{
		dp = readdir(op);
		if(dp == NULL && file_num <= count)
		{
			break;
		}
		else if(strstr(dp->d_name,".mp4") != NULL)
		{
			pvideo_list_buf[file_num] = dp->d_name;
			printf("file_name:%s\n",pvideo_list_buf[file_num]);
			file_num ++;
		}
	}
	closedir(op);
	
	printf("first clip:%s\n",pvideo_list_buf[0]);
	return file_num;
}

void play(char* filename)
{
	char cmd[CMD_LEN] = "";
	sprintf(cmd,"mplayer %s &",filename);
	//char* cmd = strcat(strcat("madplay ",filename)," &");
	printf("cmd = %s\n",cmd);
	system(cmd);
}

void stop()
{
	printf("video stop\n");
	system("killall -STOP mplayer");
}

void go_on()
{
	printf("video continue\n");
	system("killall -CONT mplayer");
}

void quit()
{
	system("killall -KILL mplayer");
}

void next_clip()
{
	quit();
	pCurrent_video ++;
	printf("current video:%s\n",*pCurrent_video);
	play(*pCurrent_video);
}

void pre_clip()
{
	quit();
	pCurrent_video --;
	printf("current video:%s\n",*pCurrent_video);
	play(*pCurrent_video);
}

