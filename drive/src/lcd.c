#include "lcd.h"
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "common.h"
/*
 * function:initial lcd screen
 * param: none
 * return:none
 */

int fd_lcd;
int* p = NULL;

static int open_bmp(const char* bmp_path_name);
static int read_bmp_ifo(int fd_bmp,int* bmp_w,int* bmp_h,int* bmp_t);
static int read_bmp_data(int fd_bmp,char* bmp_path_name,int count);
static int close_bmp(int fd_bmp);

int lcd_init()
{
	// open lcd file
	fd_lcd = open("/dev/fb0",O_RDWR);
	if(fd_lcd == -1)
	{
		perror("open file failed");
		return -1;
	}
#if defined DEBUG_MODE
	printf("fd_lcd = %d\n",fd_lcd);
#endif

	// map lcd file with RAM
	p = mmap(NULL,LCD_LEN,PROT_READ|PROT_WRITE,MAP_SHARED,fd_lcd,0);
	if(p == NULL)
	{
		perror("map file failed");
		return -1;
	}
#if defined DEBUG_MODE
	printf("map pointer is %p\n",p);
#endif

	printf("Init LCD success!\n");

	return 0;
}

/*
 * function:show a bmp image on lcd screen at any position and scale to the
 *          specify size.
 * param:  	1.start_x:
 * 			2.start_y:
 * 			3.show_x:
 * 			4.show_y:
 * 			5.bmp_path_name:
 * return:  none
 */
int lcd_show_bmp(int show_x,int show_y,int show_w,int show_h,const char* bmp_path_name)
{
	int x,y,k;
	int bmp_w,bmp_h,bmp_t;

	// open bmp file 
	int fd_img = open_bmp(bmp_path_name);

	// read bmp file image information
	int bmp_buf_size = read_bmp_ifo(fd_img,&bmp_w,&bmp_h,&bmp_t);

	// read bmp file image data
	char bmp_buf[bmp_buf_size];
	read_bmp_data(fd_img,bmp_buf,bmp_buf_size);

	// display image
	int color;
	k = 0;
	for(y = (LCD_HEIGHT-bmp_h-show_y);y < (LCD_HEIGHT-show_y);y ++)
	{
		for(x = show_x;x < show_x+show_w;x ++) 
		{
			if(k <= bmp_buf_size - 3)
			{
				color = 0x000000000 | bmp_buf[k] | bmp_buf[k+1]<<8 | bmp_buf[k+2]<<16;
				lcd_draw_point(x,LCD_HEIGHT-1-y,color);
				k += 3;
			}
		}
	}
	
	// close img file
	close_bmp(fd_img);

	return 0;
}

/*
 * function:show a binbmp image on lcd screen at any position and scale to the
 *          specify size.
 * param:  	1.start_x:
 * 			2.start_y:
 * 			3.show_x:
 * 			4.show_y:
 * 			5.bmp_path_name:
 * return:  none
 */
int lcd_show_bin_bmp(int show_x,int show_y,int show_w,int show_h,const char* bmp_path_name)
{
	int x,y,k;
	int bmp_w,bmp_h,bmp_t;

	// open bmp file 
	int fd_img = open_bmp(bmp_path_name);

	// read bmp file image information
	int bmp_buf_size = read_bmp_ifo(fd_img,&bmp_w,&bmp_h,&bmp_t);

	// read bmp file image data
	char bmp_buf[bmp_buf_size];
	read_bmp_data(fd_img,bmp_buf,bmp_buf_size);

	// display image
	int color;
	k = 0;
	for(y = (LCD_HEIGHT-bmp_h-show_y);y < (LCD_HEIGHT-show_y);y ++)
	{
		for(x = show_x;x < show_x+show_w;x ++) 
		{
			if(k <= bmp_buf_size - 3)
			{
				color = 0x000000000 | bmp_buf[k] | bmp_buf[k]<<8 | bmp_buf[k]<<16;
				lcd_draw_point(x,LCD_HEIGHT-1-y,color);
				k += 3;
			}
		}
	}
	
	// close img file
	close_bmp(fd_img);

	return 0;
}
int32_t lcd_draw_bmp_for_buf(uint32_t x,uint32_t y,uint8_t *pbmp_data,uint32_t w,uint32_t h)
{
	uint32_t blue, green, red;
	uint32_t color;
	uint32_t bmp_type;
	uint32_t bmp_size;
	uint32_t x_s = x;
	uint32_t x_e ;	
	uint32_t y_e ;
	uint8_t buf[54]={0};
	uint8_t *pbmp_buf=pbmp_data;
	
	
	/* 设置显示x、y坐标结束位置 */
	x_e = x + w;
	y_e = y + h;
	
	
	for(;y < y_e; y++) 
	{
		for (;x < x_e; x++)
		{
				/* 获取红绿蓝颜色数据 */
				blue  = *pbmp_buf++;
				green = *pbmp_buf++;
				red   = *pbmp_buf++;
				
				/* 组成24bit颜色 */
				color = red << 16 | green << 8 | blue << 0;
				//Lcd_Show_Point(x, y_e-1-y, color);		//逆序显示		
				lcd_draw_point(x, y, color);
		}
		
		x = x_s;
	}
	
	return 0;
} 

/*
 * function:show a bmp image on lcd screen at any position and scale to the
 *          specify size.
 * param:  	1.start_x:
 * 			2.start_y:
 * 			3.show_x:
 * 			4.show_y:
 * 			5.bmp_path_name:
 * return:  none
 */
int lcd_show_bmp_part(int show_x,int show_y,int start_x,int start_y,int end_x,int end_y,const char* bmp_path_name)
{
 	int x,y,k;
	int bmp_w,bmp_h,bmp_t;

	// open bmp file 
	int fd_img = open_bmp(bmp_path_name);

	// read bmp file image information
	int bmp_buf_size = read_bmp_ifo(fd_img,&bmp_w,&bmp_h,&bmp_t);

	// read bmp file image data
	char bmp_buf[bmp_buf_size];
	read_bmp_data(fd_img,bmp_buf,bmp_buf_size);

	// display image
	int color;
	int show_w = end_x - start_x;
	int show_h = end_y - start_y;
	k = end_y * bmp_w * 3 + end_x - 3;
	for(y = show_y;y < show_y+show_h;y ++)
	{
		for(x = show_x;x < show_x+show_w;x ++) 
		{
			if(k > start_y * bmp_w + start_x -3)
			{
				color = 0x000000000 | bmp_buf[k-2] | bmp_buf[k-1]<<8 | bmp_buf[k]<<16;
				lcd_draw_point(x,y,color);
				k -= 3;
			}
		}
	}
	
	// close img file
	close_bmp(fd_img);
 		
	return 0;
}



/*
 * function:draw a dot on lcd at specify position
 * @param:
 * 			1.x:
 * 			2.y:
 * 			3.color:
 * @return: 0
*/
int lcd_draw_point(int x,int y,int color)
{
	*(p+y*LCD_WIDTH+x) = color;
	return 0;
}

/*
 * function:clear lcd
 * @param:  none
 * @return 	0
 * 
*/
int lcd_clear()
{
	lcd_show_color(BLACK);
}


int lcd_show_color(int color)
{
	int x,y;

	for(y = 0;y < LCD_HEIGHT;y ++)
	{
		for(x = 0;x < LCD_WIDTH;x ++)
		{
			lcd_draw_point(x,y,color);
		}
	}
	return 0;
}

/*
 * function:lcd uninit
 * param: none
 * return:0
*/
int lcd_uninit()
{
	munmap(p,LCD_LEN);
	close(fd_lcd);

	return 0;
}

/*
	功能描述：yuyv转RGB24算法（查表+公式），能够正常转换。
		使用时，需要先调用下面的表格初始化Init_Convert_Table函数，再进行转换即可。
	yuyv_in：YUYV画面数据的输入。
	w：yuyv画面的宽度
	h：yuyv画面的高度
	return：成功转换的RGB24数据。（尺寸跟原来的画面相同）
*/

//数组需要用int来定义，不然溢出时无法察觉
int32_t R[256][256];
int32_t G[256][256][256];
int32_t B[256][256];

void Init_Convert_Table(void)
{
	int32_t i=0,j=0,k=0;
	for(i=0; i<256; i++)
	{
		for(j=0; j<256; j++)
		{
			R[i][j] = i + 1.042*(j-128);
			R[i][j] = R[i][j]>255 ? 255 : R[i][j];
			R[i][j] = R[i][j]<0   ? 0   : R[i][j];

			B[i][j] = i + 1.772*(j-128);
			B[i][j] = B[i][j]>255 ? 255 : B[i][j];
			B[i][j] = B[i][j]<0   ? 0   : B[i][j];

			for(k=0; k<256; k++)
			{
				G[i][j][k] = i + 0.344*(j-128)-0.714*(k-128);
				G[i][j][k] = G[i][j][k]>255 ? 255 : G[i][j][k];
				G[i][j][k] = G[i][j][k]<0   ? 0   : G[i][j][k];
			}
		}
	}
}

/*
	注意：YUYV的尺寸，应该是w*h*2，而RGB24的是w*h*3，YUV420的则是w*h*1.5
*/
uint8_t *YUYV_To_RGB24(uint8_t *yuyv_in,uint32_t w,uint32_t h)
{
	uint8_t Y0, U;
	uint8_t Y1, V;

	uint8_t *RGB24 = malloc(w*h*3);

	int32_t yuv_offset,rgb24_offset;
	int32_t x=0,y=0;
	for(y=0; y<h; y++)
	{
		for(x=0; x<w; x+=2)
		{
			yuv_offset = ( w*y + x ) * 2; //yuyv一个像素点的偏移量
			rgb24_offset = ( w*y + x ) * 3; //lcd像素点偏移量
			
			//计算yuv分量
			Y0 = *(yuyv_in + yuv_offset + 0);
			U  = *(yuyv_in + yuv_offset + 1);
			Y1 = *(yuyv_in + yuv_offset + 2);
			V  = *(yuyv_in + yuv_offset + 3);

			//查表得出yuv分量中的RGB值（每次操作是两个像素点）
			*(RGB24 + rgb24_offset + 2) = R[Y0][V];
			*(RGB24 + rgb24_offset + 1) = G[Y0][U][V];
			*(RGB24 + rgb24_offset + 0) = B[Y0][U];

			*(RGB24 + rgb24_offset + 5) = R[Y1][V];
			*(RGB24 + rgb24_offset + 4) = G[Y1][U][V];
			*(RGB24 + rgb24_offset + 3) = B[Y1][U];
		}
	}
	return RGB24;
}

static int open_bmp(const char* bmp_path_name)
{
	int fd_bmp;
	unsigned char buf[54]={0};
	ssize_t r_ret;

	// open bmp file
	fd_bmp = open(bmp_path_name,O_RDONLY);
	if(fd_bmp == -1)
	{
		perror("open bmp file failed");
		return -1;
	}
	printf("fd_bmp = %d\n",fd_bmp);
	
	return fd_bmp;
}

static int read_bmp_ifo(int fd_bmp,int* bmp_w,int* bmp_h,int* bmp_t)
{ 
	// get bmp file information
	char buf[54] = {0};
	int r_ret=read(fd_bmp,buf,sizeof(buf));
	if(r_ret < 0)
	{
		perror("read bmp file header failed");
		return -1;
	}
	printf("read bmp file header = %d bytes\n",r_ret);

	//宽度
	*bmp_w = buf[18]|buf[19]<<8;
	printf("bmp_width=%d\r\n",*bmp_w);
	
	//高度
	*bmp_h =buf[22]| buf[23]<<8;
	printf("bmp_height=%d\r\n",*bmp_h);	
	
	//位深
	*bmp_t = buf[28]|buf[29]<<8;	
	printf("bmp_type=%d\r\n",*bmp_t);	
	unsigned int bmp_size = (*bmp_w)*(*bmp_h)*(*bmp_t/8);
	
	return bmp_size;
}

static int read_bmp_data(int fd_bmp,char* bmp_buf,int bmp_buf_size)
{
	int r_ret = read(fd_bmp,bmp_buf,bmp_buf_size);
	if(r_ret == -1)
	{
		perror("read bmp file image data failed");
		return -1;
	}
	printf("read bmp file data = %d bytes\n",r_ret);
	
	return r_ret;
}

static int close_bmp(int fd_bmp)
{
	close(fd_bmp);
}

