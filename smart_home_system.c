#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include "lcd.h"
#include "ts.h"

int get_file_list(const char* dir_path, char* pfile_list_buf[],char* search_str,int count);
void login();
void desktop();
int start_picture_app();
int start_camera_app();
int start_music_app();
int start_game_app();
int start_led_app();

int main()
{
	int start_x,start_y,end_x,end_y;
	int ts_envent,direction;

	lcd_init();
	ts_init();

	/* 1.show boot image*/
	login();

	/* 2.enter desktop system*/
	desktop();
	
	while(1)
	{
		ts_envent = ts_scan(&start_x,&start_y,&end_x,&end_y);
		if(ts_envent == TS_CLICK)
		{
			if(end_x > 100 && end_x < 200 && end_y > 100 && end_y < 200)
			{
				lcd_show_bin_bmp(100,100,100,100,"./image/picture.bmp");
				start_picture_app();
				desktop();
			}
			else if(end_x > 100 && end_x < 200 && end_y > 350 && end_y < 450)
			{
				lcd_show_bin_bmp(100,350,100,100,"./image/camera.bmp");
				start_camera_app();
				desktop();
			}
			else if(end_x > 350 && end_x < 450 && end_y > 100 && end_y < 200)
			{
				lcd_show_bin_bmp(350,100,100,100,"./image/led.bmp");
				start_led_app();
				desktop();
			}
			else if(end_x > 600 && end_x < 700 && end_y > 100 && end_y < 200)
			{
				lcd_show_bin_bmp(600,100,100,100,"./image/music.bmp");
				start_music_app();
				desktop();
			}
			else if(end_x > 600 && end_x < 700 && end_y > 350 && end_y < 450)
			{
				lcd_show_bin_bmp(600,350,100,100,"./image/player.bmp");
				start_player_app();
				desktop();	
			}
			else if(end_x > 350 && end_x < 450 && end_y > 350 && end_y < 450)
			{
				lcd_show_bin_bmp(350,350,100,100,"./image/game.bmp");
				lcd_uninit();
		GAME:	ts_uninit();
				int ret = start_game_app();//ret = 1:game over;ret = 2:manual exit;
				printf("ret = %d\n",ret);
				if(ret == 1)
				{
					int x0,y0,x,y;
					ts_init();
					while(ts_scan(&x0,&y0,&x,&y) != TS_CLICK);
					if(x > 650 && x < 750 && y > 90 && y < 160)
					{
						lcd_init();
						ts_init();
						desktop();
					}
					else if(x > 650 && x < 750 && y > 320 && y < 390)
					{
						goto GAME;
					}
				}
				else if(ret == 2)
				{	
					lcd_init();
					ts_init();
					desktop();
				}
			}
		}
		/*
		else if(ts_envent == TS_SLIDE)
		{
			direction = ts_get_direction(start_x,start_y,end_x,end_y);
			switch(direction)
			{
				
			}
		}	*/

	}
	
	ts_uninit();
	lcd_uninit();

	return 0;
}

void login()
{
	lcd_clear();
	int i;
	for(i = 0;i < 2;i ++)
	{
		lcd_show_bmp(350,190,100,100,"./image/logo_1.bmp");
		usleep(500*1000);
		lcd_show_bmp(350,190,100,100,"./image/logo_2.bmp");
		usleep(500*1000);
		lcd_show_bmp(350,190,100,100,"./image/logo_3.bmp");
		usleep(500*1000);
		lcd_show_bmp(350,190,100,100,"./image/logo_4.bmp");
		usleep(500*1000);
	}
}

void desktop()
{
	lcd_show_bmp(0,0,800,480,"./image/desktop.bmp");
	lcd_show_bmp(100,100,100,100,"./image/picture.bmp");
	lcd_show_bmp(100,350,100,100,"./image/camera.bmp");
	lcd_show_bmp(350,100,100,100,"./image/led.bmp");
	lcd_show_bmp(350,350,100,100,"./image/game.bmp");
	lcd_show_bmp(600,100,100,100,"./image/music.bmp");
	lcd_show_bmp(600,350,100,100,"./image/player.bmp");
}

int start_picture_app()
{
	int x0,y0,x,y;
	char* picture_list[10];
	int picture_cnt = get_file_list("./",picture_list,".bmp",10);
	lcd_show_bmp(0,0,800,480,picture_list[0]);
	int cur_pic_index = 0;

	while(1)
	{
		if(ts_scan(&x0,&y0,&x,&y) == TS_SLIDE)
		{
			switch(ts_get_direction(x0,y0,x,y))
			{
				case TS_LEFT:
					if(cur_pic_index == 0)
					{
						cur_pic_index = picture_cnt - 1;
					}
					lcd_show_bmp(0,0,800,480,picture_list[cur_pic_index]);
					break;
				case TS_RIGHT:
					if(cur_pic_index == picture_cnt -1)
					{
						cur_pic_index = 0;
					}
					lcd_show_bmp(0,0,800,480,picture_list[cur_pic_index]);
					break;
				case TS_UP:
					return 0;
			}
		}
	}
}

int start_player_app()
{
	int x0,y0,x,y;
	char* video_list[10];
	char cmd[20];
	int video_cnt = get_file_list("./",video_list,".mp4",10);
	sprintf(cmd,"mplayer %s &",video_list[0]);

	system(cmd);
	int cur_video_index = 0;

	while(1)
	{
		if(ts_scan(&x0,&y0,&x,&y) == TS_SLIDE)
		{
			switch(ts_get_direction(x0,y0,x,y))
			{
				case TS_LEFT:
					if(cur_video_index == 0)
					{
						cur_video_index = video_cnt - 1;
					}
					//lcd_show_bmp(0,0,800,480,video_list[cur_pic_index]);
					break;
				case TS_RIGHT:
					if(cur_video_index == video_cnt -1)
					{
						cur_video_index = 0;
					}
					//lcd_show_bmp(0,0,800,480,video_list[cur_pic_index]);
					break;
				case TS_UP:
					system("killall -KILL mplayer");
					return 0;
			}
		}
	}
}
int get_file_list(const char* dir_path, char* pfile_list_buf[],char* search_str,int count)
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
		else if(strstr(dp->d_name,search_str) != NULL)
		{
			pfile_list_buf[file_num] = dp->d_name;
			printf("file_name:%s\n",pfile_list_buf[file_num]);
			file_num ++;
		}
	}
	closedir(op);
	
	printf("first file:%s\n",pfile_list_buf[0]);
	return file_num;
}
