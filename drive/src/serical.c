#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <termios.h>
#include <string.h>
#include <stdlib.h>
#include "ts.h"
#include "lcd.h"

#define OFF 0
#define ON  1

#define DEVICE_NAME "/dev/s3c2410_serial3"
int green_LED_state = OFF;
int blue_LED_state = OFF;

int ser_fd;
int set_serial_uart1(int ser_fd)
{

	struct termios termios_new;
	bzero( &termios_new, sizeof(termios_new));
	/*ԭʼģʽ*/
	cfmakeraw(&termios_new);

	/*������Ϊ115200*/
	termios_new.c_cflag=(B115200);
	termios_new.c_cflag |= CLOCAL | CREAD;
	
	/*8λ����λ*/
	termios_new.c_cflag &= ~CSIZE;
	termios_new.c_cflag |= CS8;

	/*����żУ��λ*/
	termios_new.c_cflag &= ~PARENB;

	/*1λֹͣλ*/
	termios_new.c_cflag &= ~CSTOPB;
	/*������ڻ�����*/
	tcflush( ser_fd,TCIOFLUSH);
	termios_new.c_cc[VTIME] = 0;
	termios_new.c_cc[VMIN] = 4;
	tcflush ( ser_fd, TCIOFLUSH);
	/*��������ʹ��*/
	tcsetattr( ser_fd ,TCSANOW,&termios_new);
}

	
int start_led_app(void)
{
	int x0,y0,x,y;
	int ts_event;
	char message[6];
	char cmd[2];

	//�򿪴���+��ʼ��
	ser_fd = open(DEVICE_NAME,O_RDWR);
	if(ser_fd == -1)
	{
		printf("serical open failed!\n");
		return -1;
	}
	set_serial_uart1(ser_fd);
	
	if(fcntl(ser_fd,F_SETFL,0)<0)
	{
		exit(1);
	}
	tcdrain(ser_fd);   //ͨ���ն˿���
	tcflush( ser_fd, TCIOFLUSH); //ˢ�»�����
	write(ser_fd,"00",2);
	lcd_show_bmp(0,0,400,480,"./image/black.bmp");
	lcd_show_bmp(400,0,400,480,"./image/black.bmp");
	
	while(1)
	{
		//���ڵĶ�д
		/*
		//��message�����������
		memset(message,0,6);
		read(ser_fd,message,6);
		printf("message = %s\n",message);
		tcflush( ser_fd, TCIOFLUSH); //ˢ�»�����*/
		printf("enter while\n");
		ts_event = ts_scan(&x0,&y0,&x,&y); 
		if(ts_event == TS_CLICK)
		{
			if(x > 0 && x < 400)
			{
				green_LED_state  = !green_LED_state;
				if(green_LED_state == OFF)
				{
					lcd_show_bmp(0,0,400,480,"./image/black.bmp");
				}
				else
				{
					lcd_show_bmp(0,0,400,480,"./image/green.bmp");
				}
			}
			else if(x > 400 && x < 800)
			{
				blue_LED_state = !blue_LED_state;
				if(blue_LED_state == OFF)
				{
					lcd_show_bmp(400,0,400,480,"./image/black.bmp");
				}
				else
				{
					lcd_show_bmp(400,0,400,480,"./image/blue.bmp");	
				}
			}
			sprintf(cmd,"%d%d",blue_LED_state,green_LED_state);			
			write(ser_fd,cmd,2);
		}
		else if(ts_event == TS_SLIDE)
		{
			break;
		}
	}
	
}
