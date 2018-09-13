#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "ts.h"
#include "lcd.h"
#include "api_v4l2.h"

#define STOP        0
#define CAPTURING   1
#define START       2
#define PAUSE       3

unsigned char camera_state = START;

// 摄像头线程回调函数定义
void* camera_capture(void* arg)
{
	uint8_t *data=NULL;	
	FreamBuffer freambuf;
	
	printf("enter camera_capture function\n");
	while(1)
	{
		while(camera_state == CAPTURING)
		{
			linux_v4l2_get_fream(&freambuf);
			data = YUYV_To_RGB24(freambuf.buf,640,480);
		
			lcd_draw_bmp_for_buf(60,0,data,640,480);
			free(data);
			data=NULL;
		}
		sleep(1);
	}
}

int start_camera_app()
{
	// 初始化变量
	int start_x,start_y,end_x,end_y;
	pthread_t camera_thread;

	// 初始化硬件设备
	//lcd_init();
	//ts_init();
	if(linux_v4l2_device_init()!=0)
	{
		return 0;
	}	
	Init_Convert_Table(); //提前初始化表格
	printf("init table\n");

	// 初始化摄像头采集UI
	lcd_clear();
	lcd_show_bmp(0,0,60,480,"./image/head_pane.bmp");
	lcd_show_bmp(700,0,100,480,"./image/control_pane.bmp");
	lcd_show_bmp(700,190,100,100,"./image/start.bmp");
	lcd_show_bmp(700,70,100,100,"./image/return.bmp");

	// 监听触摸屏的输入来控制摄像头
	while(1)
	{
		if(ts_scan(&start_x,&start_y,&end_x,&end_y) == TS_CLICK)
		{
			if(end_x > 700 && end_x < 800 && end_y > 190 && end_y < 290)
			{	
				//打开摄像头开始采集图像
				if(camera_state == START)
				{
					lcd_show_bmp(700,190,100,100,"./image/stop.bmp");
					lcd_show_bmp(700,310,100,100,"./image/pause.bmp");
					linux_v4l2_start_capturing();
					// 创建摄像头捕捉线程
					if(pthread_create(&camera_thread,NULL,camera_capture,NULL) != 0)
					{
						perror("create camera thread failed");
						return -1;
					}
					camera_state = CAPTURING;
					printf("start capturing...\n");
				}
				//关闭摄像头停止采集图像
				else if(camera_state != STOP)
				{
					if(camera_state != PAUSE)
					{
						linux_v4l2_stop_capturing();
					}
					camera_state = START;
					// 停止摄像头捕捉线程
					pthread_cancel(camera_thread);
					pthread_join(camera_thread,NULL);
					// 重绘控制面板
					lcd_clear();
					lcd_show_bmp(0,0,60,100,"./image/head_pane.bmp");
					lcd_show_bmp(700,0,100,480,"./image/control_pane.bmp");
					lcd_show_bmp(700,190,100,100,"./image/start.bmp");
					lcd_show_bmp(700,70,100,100,"./image/return.bmp");	
				}
			}
			else if(end_x > 700 && end_x < 800 && end_y > 310 && end_y < 410 )
			{
				//暂停采集图像
				if(camera_state == CAPTURING)
				{
					camera_state = PAUSE;
					lcd_show_bmp(700,310,100,100,"./image/continue.bmp");	
					linux_v4l2_stop_capturing();
					printf("pause capture\n");
				}
				//继续采集图像
				else if(camera_state == PAUSE)
				{
					lcd_show_bmp(700,310,100,100,"./image/pause.bmp");
					linux_v4l2_start_capturing();
					camera_state = CAPTURING;
					printf("continue capture\n");
				}

			}
			// 退出摄像头应用
			else if(end_x > 700 && end_x < 800 && end_y > 70 && end_y < 170)
			{
				break;	
			}
		}
	}
	if(camera_state == CAPTURING || camera_state == PAUSE)
	{
		linux_v4l2_stop_capturing();
		pthread_cancel(camera_thread);
		pthread_join(camera_thread,NULL);
	}
	lcd_clear();
	camera_state = START;
	// 解除硬件设备初始化
	linux_v4l2_device_uinit();
//	ts_uninit();
//	lcd_uninit();	
//	pthread_exit(NULL);
	return 0;
}
