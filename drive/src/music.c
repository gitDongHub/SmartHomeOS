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

char** pCurrent_music = NULL;

int get_music_list(const char* dir_path, char* pMusic_list_buf[], int count);
void play(char* file_name);
void stop();
void go_on();
void quit();
void next_song();
void pre_song();

int start_music_app()
{
	/* 1.get music file on current directory*/
	char* music_list_buf[LIST_SIZE] = {"\0"};

	int file_cnt = get_music_list("./",music_list_buf,LIST_SIZE);	
	if(file_cnt <= 0)
	{
		printf("No such type of file or error in current directory\n");
		return -1;
	}
	printf("get %d songs in current directory\n",file_cnt);

	pCurrent_music = music_list_buf;

	//printf("auto play the first song: %s\n",music_list_buf[0]);
	//play(music_list_buf[0]);
	
	/* 2.initalize the interface of music player */
	//lcd_init();
	//ts_init();
	
	lcd_show_bmp(0,0,800,480,"./image/interface.bmp");
	lcd_show_bmp(150,240,100,100,"./image/pre.bmp");
	lcd_show_bmp(350,240,100,100,"./image/play.bmp");
	lcd_show_bmp(550,240,100,100,"./image/next.bmp");

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
			printf("current playing song:%s\n",*pCurrent_music);
			if(x > 150 && x < 250 && y > 240 && y < 340)
			{
				if(strcmp(*pCurrent_music,music_list_buf[0]) == 0)
				{
					printf("this is the first song\n");
				}
				else if(file_cnt == 1)
				{
					printf("there only one song on current directory\n");
				}
				else
				{
					pre_song();
				}
			}
			else if(x > 550 && x < 650 && y > 240 && y < 340)
			{
				if(strcmp(*pCurrent_music,music_list_buf[file_cnt-1]) == 0)
				{
					printf("this is the last song\n");
				}
				else if(file_cnt == 1)
				{
					printf("there only one song on current directory\n");
				}
				else
				{
					next_song();
				}
			}
			else if(x > 350 && x < 450 && y > 240 && y < 340)
			{
				if(player_state == PLAYING)
				{	
					lcd_show_bmp(350,240,100,100,"./image/play.bmp");
					stop();
					player_state = STOP;
				}
				else 
				{
					if(player_state == STOP)
					{
						lcd_show_bmp(350,240,100,100,"./image/stop_music.bmp");
						go_on();
						player_state = PLAYING;
					}
					else
					{
						lcd_show_bmp(350,240,100,100,"./image/stop_music.bmp");
						play(*pCurrent_music);
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
	//ts_uninit();
	//lcd_uninit();

	return 0;
}

int get_music_list(const char* dir_path, char* pMusic_list_buf[],int count)
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
		else if(strstr(dp->d_name,".mp3") != NULL)
		{
			pMusic_list_buf[file_num] = dp->d_name;
			printf("file_name:%s\n",pMusic_list_buf[file_num]);
			file_num ++;
		}
	}
	closedir(op);
	
	printf("first song:%s\n",pMusic_list_buf[0]);
	return file_num;
}

void play(char* filename)
{
	char cmd[CMD_LEN] = "";
	sprintf(cmd,"madplay %s &",filename);
	//char* cmd = strcat(strcat("madplay ",filename)," &");
	printf("cmd = %s\n",cmd);
	system(cmd);
}

void stop()
{
	printf("music stop\n");
	system("killall -STOP madplay");
}

void go_on()
{
	printf("music continue\n");
	system("killall -CONT madplay");
}

void quit()
{
	system("killall -KILL madplay");
}

void next_song()
{
	quit();
	pCurrent_music ++;
	printf("current music:%s\n",*pCurrent_music);
	play(*pCurrent_music);
}

void pre_song()
{
	quit();
	pCurrent_music --;
	printf("current music:%s\n",*pCurrent_music);
	play(*pCurrent_music);
}

