#ifndef __LCD_H__
#define __LCD_H__
#include "common.h"
#define DEBUG_MODE

#define BLACK   0x00000000
#define WHITE   0x00FFFFFF

#define COLORn  7
#define RED    	0x00FF0000
#define GREEN  	0x0000FF00
#define BLUE   	0x000000FF

#define ORANGE 	0x00FF7D00 
#define YEALLOW 0x00FFFF00
#define CYAN    0x0000FFFF
#define PURPLE  0x00FF00FF

#define LCD_WIDTH  800
#define LCD_HEIGHT 480
#define LCD_LEN LCD_WIDTH*LCD_HEIGHT*4

extern int fd_lcd;
extern int *p;

int lcd_init();
int lcd_show_bmp(int show_x,int show_y,int show_w,int show_h,const char* bmp_path_name);
int32_t lcd_draw_bmp_for_buf(uint32_t x,uint32_t y,uint8_t *pbmp_data,uint32_t w,uint32_t h);
int lcd_show_bin_bmp(int show_x,int show_y,int show_w,int show_h,const char* bmp_path_name);
int lcd_show_bmp_part(int show_x,int show_y,int start_x,int start_y,int end_x,int end_y,const char* bmp_path_name);
int lcd_draw_point(int x,int y,int color);
//int lcd_draw_dot(int x, int y,int size, int color);
int lcd_show_color(int color);
int lcd_clear();
int lcd_uninit(); 

void Init_Convert_Table(void);
uint8_t *YUYV_To_RGB24(uint8_t *yuyv_in,uint32_t w,uint32_t h);
#endif
