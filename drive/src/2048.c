#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <string.h>
#include <math.h>
#include<stdlib.h>

int value_2048[4][4]={0};//定义2048的值的数组 
int PIC_NUM_2048[4][4]={0};//定义2048值的图片符号的数组 
int fd[14]={0};//每一个数字图片的返回值
int num=3; //定义一个随机数专用变量 
int num_suiji(int x);//声明随机函数 
int pic[14][200*120]={0};//记录0到8196图片色数据的数组集合 



void SZ_CSH()//数组初始化 
{
	int i,j;
	
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			value_2048[i][j]=0;
		}
	}
	printf("value_2048 init over\n");
	value_2048[num_suiji(4)][num_suiji(4)]=2*(num_suiji(2)+1);
	num=num%rand()+rand();
	value_2048[num_suiji(4)][num_suiji(4)]=2*(num_suiji(2)+1);
	int sum;
	while(1)
	{
		num=num+rand();
		for(i=0;i<4;i++)
		{
			for(j=0;j<4;j++)
			{
				if(value_2048[i][j]==0) sum++;
			}
		}
		if(sum==15) value_2048[num_suiji(4)][num_suiji(4)]=2*(num_suiji(2)+1);
		if(sum==14) break;
		sum=0;
	}
}



void PIC_READ()//0到8192的图片显示 （共14张） 
{
	fd[0]=open("/home/pic0.bmp",O_RDWR);//0 
	fd[1]=open("/home/pic1.bmp",O_RDWR);//2
	fd[2]=open("/home/pic2.bmp",O_RDWR);//4
	fd[3]=open("/home/pic3.bmp",O_RDWR);//8
	fd[4]=open("/home/pic4.bmp",O_RDWR);//16
	fd[5]=open("/home/pic5.bmp",O_RDWR);//32
	fd[6]=open("/home/pic6.bmp",O_RDWR);//64
	fd[7]=open("/home/pic7.bmp",O_RDWR);//128
	fd[8]=open("/home/pic8.bmp",O_RDWR);//256
	fd[9]=open("/home/pic9.bmp",O_RDWR);//512
	fd[10]=open("/home/pic10.bmp",O_RDWR);//1024
	fd[11]=open("/home/pic11.bmp",O_RDWR);//2048
	fd[12]=open("/home/pic12.bmp",O_RDWR);//4096
	fd[13]=open("/home/pic13.bmp",O_RDWR);//8192
}



void PIC_CLOSE()//图片文件关闭 
{
	int i;
	for(i=0;i<14;i++)
	{
		close(fd[i]);
	} 
} 



int PIC_XIANSHI()
{
	int i,j;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			PIC_NUM_2048[i][j]=value_check(i,j);
		}
	}
	
	
	int fd_pic;
	int a;
	j=0;
	fd_pic=open("/dev/fb0",O_RDWR);
	for(j=0;j<120;j++)
	{	
		lseek(fd_pic,4*800*j+0,SEEK_SET);
		a=PIC_NUM_2048[0][0];
		write(fd_pic,&pic[a][(120-j)*200],200*sizeof(int));
	}
	for(i=0,j=120;j<240;j++,i++)
	{
		lseek(fd_pic,4*800*j+0,SEEK_SET);
		a=PIC_NUM_2048[1][0];
		write(fd_pic,&pic[a][(120-i)*200],200*sizeof(int));
	}
	for(i=0,j=240;j<360;j++,i++)
	{
		lseek(fd_pic,4*800*j+0,SEEK_SET);
		a=PIC_NUM_2048[2][0];
		write(fd_pic,&pic[a][(120-i)*200],200*sizeof(int));
	}
	for(i=0,j=360;j<480;j++,i++)
	{
		lseek(fd_pic,4*800*j+0,SEEK_SET);
		a=PIC_NUM_2048[3][0];
		write(fd_pic,&pic[a][(120-i)*200],200*sizeof(int));
	}
	for(j=0;j<120;j++)
	{	
		lseek(fd_pic,4*800*j+4*200,SEEK_SET);
		a=PIC_NUM_2048[0][1];
		write(fd_pic,&pic[a][(120-j)*200],200*sizeof(int));
	}
	for(i=0,j=120;j<240;j++,i++)
	{
		lseek(fd_pic,4*800*j+4*200,SEEK_SET);
		a=PIC_NUM_2048[1][1];
		write(fd_pic,&pic[a][(120-i)*200],200*sizeof(int));
	}
	for(i=0,j=240;j<360;j++,i++)
	{
		lseek(fd_pic,4*800*j+4*200,SEEK_SET);
		a=PIC_NUM_2048[2][1];
		write(fd_pic,&pic[a][(120-i)*200],200*sizeof(int));
	}
	for(i=0,j=360;j<480;j++,i++)
	{
		lseek(fd_pic,4*800*j+4*200,SEEK_SET);
		a=PIC_NUM_2048[3][1];
		write(fd_pic,&pic[a][(120-i)*200],200*sizeof(int));
	}
	for(j=0;j<120;j++)
	{	
		lseek(fd_pic,4*800*j+4*400,SEEK_SET);
		a=PIC_NUM_2048[0][2];
		write(fd_pic,&pic[a][(120-j)*200],200*sizeof(int));
	}
	for(i=0,j=120;j<240;j++,i++)
	{
		lseek(fd_pic,4*800*j+4*400,SEEK_SET);
		a=PIC_NUM_2048[1][2];
		write(fd_pic,&pic[a][(120-i)*200],200*sizeof(int));
	}
	for(i=0,j=240;j<360;j++,i++)
	{
		lseek(fd_pic,4*800*j+4*400,SEEK_SET);
		a=PIC_NUM_2048[2][2];
		write(fd_pic,&pic[a][(120-i)*200],200*sizeof(int));
	}
	for(i=0,j=360;j<480;j++,i++)
	{
		lseek(fd_pic,4*800*j+4*400,SEEK_SET);
		a=PIC_NUM_2048[3][2];
		write(fd_pic,&pic[a][(120-i)*200],200*sizeof(int));
	}
	for(j=0;j<120;j++)
	{	
		lseek(fd_pic,4*800*j+4*600,SEEK_SET);
		a=PIC_NUM_2048[0][3];
		write(fd_pic,&pic[a][(120-j)*200],200*sizeof(int));
	}
	for(i=0,j=120;j<240;j++,i++)
	{
		lseek(fd_pic,4*800*j+4*600,SEEK_SET);
		a=PIC_NUM_2048[1][3];
		write(fd_pic,&pic[a][(120-i)*200],200*sizeof(int));
	}
	for(i=0,j=240;j<360;j++,i++)
	{
		lseek(fd_pic,4*800*j+4*600,SEEK_SET);
		a=PIC_NUM_2048[2][3];
		write(fd_pic,&pic[a][(120-i)*200],200*sizeof(int));
	}
	for(i=0,j=360;j<480;j++,i++)
	{
		lseek(fd_pic,4*800*j+4*600,SEEK_SET);
		a=PIC_NUM_2048[3][3];
		write(fd_pic,&pic[a][(120-i)*200],200*sizeof(int));
	}
	close(fd_pic);
}



int value_check(int x,int y)
{
	switch(value_2048[x][y])
	{
		case 0: return 0;
		case 2: return 1;
		case 4: return 2;
		case 8: return 3;
		case 16: return 4;
		case 32: return 5;
		case 64: return 6;
		case 128: return 7;
		case 256: return 8;
		case 512: return 9;
		case 1024: return 10;
		case 2048: return 11;
		case 4096: return 12;
		case 8192: return 13;
	}
}


int PIC_ZHUANHUAN()//把图片资料读取出来并存入pic数组保存 
{
	int i,j,b,c,f,a;
	char num[14][200*120*3];
	for(i=0;i<14;i++)
	{
		lseek(fd[i],54,SEEK_SET);
		read(fd[i],num[i],200*120*3);
	}
	for(j=0;j<14;j++)
	{
		for(i=0;i<200*120*3;i++)
		{
			if(i%3==1)
			{
				b=num[j][i];
			}
			if(i%3==2)
			{
				c=num[j][i];
				f=i/3;
				pic[j][f]=c*256*256+b*256+a;
			}
			if(i%3==0)
			{
				a=num[j][i];
			}
		}
	}
	//int hum[14][200*120];
	/*for(i=0;i<14;i++)
	{
		for(j=0;j<200*120;j++)
		{
			hum[i][j]=pic[i][200*120-j-1];
		}
	} 
	for(i=0;i<14;i++)
	{
		for(j=0;j<200*120;j++)
		{
			pic[i][j]=hum[i][j];
		}
	} */
} 



int num_suiji(int x)//创建一个0到x（包括0不包括x）的随机数 
{
	int a,b;
	srand(time(NULL)+num);
	a=rand();
	b=a%x;
	num=num+rand();
	return b;
}



void CAOZUO(int a)
{
	int y,x,i;
	int zum=0;
	int b=0;
	if(a==3)//上 
	{
		for(y = 0; y < 4; y++) 
		{     
	        for(x = 0; x < 4; x++) 
			{
	            if(value_2048[x][y] == 0)    ;
	            else 
				{
	                for(i = x + 1; i < 4; i++) 
					{
	                    //if(i==4) break;
						if(value_2048[i][y] == 0);
	                    else if(value_2048[x][y] == value_2048[i][y]) 
						{
	                        value_2048[x][y] += value_2048[i][y];
	                        value_2048[i][y] = 0;
	                        x = i;
	                        zum++;
	                        break;
	                    }
	                    else 
						{
	                        break;
	                    }
	                } 
	            }
	        }
	    }
	}
	if(a==4) //下 
	{
		for(y = 0; y < 4; y++)  
		{
	        for(x = 3; x >= 0; x--) 
			{
	            if(value_2048[x][y] == 0)  ;
	            else 
				{
	                for(i = x - 1; i >= 0; i--) 
					{
	                    //if(i==-1) break;
						if(value_2048[i][y] == 0)    ;
	                    else if(value_2048[x][y] == value_2048[i][y]) 
						{
	                        value_2048[x][y] += value_2048[i][y];
	                        value_2048[i][y] = 0;
	                        x = i;
	                        zum++;
	                        break;
	                    }
	                    else  break;
	                }
	            }
	        }
    	}
    	
	}
	if(a==2)//左 
	{
		for(x = 0; x < 4; x++)  
	    {
	        for(y = 0; y < 4; y++) 
			{
	            if(value_2048[x][y] == 0)      ;
	            else 
				{
	                for(i = y + 1; i < 4; i++) 
					{
	                    //if(i==4) break;
						if(value_2048[x][i] == 0)
	                        ;
	                    else if(value_2048[x][y] == value_2048[x][i]) 
						{
	                        value_2048[x][y] += value_2048[x][i];
	                        value_2048[x][i] = 0;
	                        y = i;
	                        zum++;
	                        break;
	                    }
	                    else      break;
	                }
	            }
	        }
	    }
	}	
    if(a==1)//右 
    {
	    for(x = 0; x < 4; x++)  
		{ 
		    for(y = 3; y >= 0; y--) 
			{
	            if(value_2048[x][y] == 0)      ;
	            else 
				{
	                for(i = y - 1; i >= 0; i--) 
					{
	                   // if(i==-1) break;
						if(value_2048[x][i] == 0)       ;
	                    else if(value_2048[x][y] == value_2048[x][i]) 
						{
	                        value_2048[x][y] += value_2048[x][i];
	                        value_2048[x][i] = 0;
	                        y = i;
	                        zum++;
	                        break;
	                    }
	                    else          break;
	                }
	            }
	        }
		}
	}
	
	
	
	
	
	
	if(a==2)//左 
	{
		for(x=3;x>=0;x--)
		{
			for(y=0;y<4;y++)
			{
				if(value_2048[x][y]==0)
				{
					for(i=y+1;i<4;i++)
					{
						if(value_2048[x][i]==0) ;
						if(value_2048[x][i]!=0)
						{
							value_2048[x][y]=value_2048[x][i];
							value_2048[x][i]=0;
							zum++;
							break; 
						}
					}
				}
			}	
		} 
		if(zum!=0)
		{
			while(1)
			{
				b=num_suiji(4);
				if(value_2048[b][3]==0) 
				{
					value_2048[b][3]=2*(num_suiji(2)+1);
					break;
				}
			}
		}
	}
	if(a==1)//右 
	{
		for(x=0;x<4;x++)
		{
			for(y=3;y>=0;y--)
			{
				if(value_2048[x][y]==0)
				{
					for(i=y-1;i>=0;i--)
					{
						if(value_2048[x][i]==0) ;
						if(value_2048[x][i]!=0)
						{
							value_2048[x][y]=value_2048[x][i];
							value_2048[x][i]=0;
							zum++;
							break; 
						}
					}
				}
			}	
		} 
		if(zum!=0)	
		{
			while(1)
			{
				b=num_suiji(4);
				if(value_2048[b][0]==0) 
				{
					value_2048[b][0]=2*(num_suiji(2)+1);
					break;
				}
			}
		}
	}
	if(a==3)//上 
	{
		for(y=0;y<4;y++)
		{
			for(x=0;x<4;x++)
			{
				if(value_2048[x][y]==0)
				{
					for(i=x+1;i<4;i++)
					{
						if(value_2048[i][y]==0) ;
						if(value_2048[i][y]!=0)
						{
							value_2048[x][y]=value_2048[i][y];
							value_2048[i][y]=0;
							zum++;
							break; 
						}
					}
				}
			}	
		} 
		//printf("%d\n",zum);
		if(zum!=0)
		{
			while(1)
			{
				b=num_suiji(4);
				if(value_2048[3][b]==0) 
				{
					value_2048[3][b]=2*(num_suiji(2)+1);
					break;
				}
			}
		}
	}
	if(a==4)//下 
	{
		for(y=0;y<4;y++)
		{
			for(x=3;x>=0;x--)
			{
				if(value_2048[x][y]==0)
				{
					for(i=x-1;i>=0;i--)
					{
						if(value_2048[i][y]==0) ;
						if(value_2048[i][y]!=0)
						{
							value_2048[x][y]=value_2048[i][y];
							value_2048[i][y]=0;
							zum++;
							break; 
						}
					}
				}
			}	
		} 
		if(zum!=0)
		{
			while(1)
			{
				b=num_suiji(4);
				if(value_2048[0][b]==0) 
				{
					value_2048[0][b]=2*(num_suiji(2)+1);
					break;
				}
			}
		}
	}
}



int start_game_app()
{
	int i,j;//循环用数 
	int a,b;
	
	
	 
	SZ_CSH();
	printf("cnm\n");
	PIC_READ();
	PIC_ZHUANHUAN();
	PIC_CLOSE(); 
	PIC_XIANSHI();
	int ts_fd;
	ts_fd=open("/dev/event0",O_RDWR);
	struct input_event read_buf;
	int x,y,x1,y1;
	float x_,y_,k;
	int r_ret;
	int cao=0;
	int empty=0;
	int numu=0;
	int dame=0;
	int double_click=0;

	while(1)
	{
		while(1)
		{
			y=0;
			read(ts_fd,&read_buf,sizeof(read_buf));
			if(read_buf.type==3&&read_buf.code==0) x=read_buf.value;
			if(read_buf.type==3&&read_buf.code==1) y=read_buf.value;
			if(y!=0) break;
		}
		while(1)
		{
			read(ts_fd,&read_buf,sizeof(read_buf));			
			if(read_buf.type==3&&read_buf.code==0) x1=read_buf.value;
			if(read_buf.type==3&&read_buf.code==1) y1=read_buf.value;
			if(x1==0&&y1==0)
			{
				usleep(200000);
				r_ret=read(ts_fd,&read_buf,sizeof(read_buf));
				if(read_buf.type==3&&read_buf.code==0) x1=read_buf.value;
				if(read_buf.type==3&&read_buf.code==1) y1=read_buf.value;
				r_ret=read(ts_fd,&read_buf,sizeof(read_buf));
				if(read_buf.type==3&&read_buf.code==0) x1=read_buf.value;
				if(read_buf.type==3&&read_buf.code==1) y1=read_buf.value;
				if(x1==0&&y1==0)
				{
					numu++;
					break;
				}
			} 
			if(read_buf.type==1&&read_buf.code==BTN_TOUCH)
			{
				if(read_buf.type==1&&read_buf.code==BTN_TOUCH)				
					if(read_buf.value==0) 
					{
						usleep(400000);
						r_ret=read(ts_fd,&read_buf,sizeof(read_buf));
						if(read_buf.value==0) 
						{
							double_click=0;
							break;
						}	
					}
			}
		}
		//printf("x=%d,y=%d,x1=%d,y1=%d\n",x,y,x1,y1);
	//	printf("num=%d\n",numu);
		
		if(numu==0)
		{
			x_=1.0*(x1-x);
			y_=1.0*(y1-y);
			k=y_/x_;
			if(k<=1&&k>=-1)
			{
				if(x_>0) cao=1;//右
				if(x_<0) cao=2;//左 
			}
			if(k<-1||k>1)
			{
				if(y_>0) cao=4;//下 
				if(y_<0) cao=3;//上
			}
		}
		//printf("%d\n",cao);
		if(numu!=0)
		{
			double_click++;
			if(double_click==2) return 2;
			numu=0;
		}
		x1=0;
		y1=0;
		CAOZUO(cao);
		cao=0;
		PIC_XIANSHI();
		for(i=0;i<4;i++)
		{
			for(j=0;j<4;j++)
			{
				if(value_2048[i][j]==0) empty++;
				if(value_2048[i][j]==2048) //判断胜利 
				{
					int fd_sha,fd_dame;
					char fuck[700*300*3];
					int p,v,m,f;
					int kao[700*300];
					fd_sha=open("/home/win.bmp",O_RDWR);
					lseek(fd_sha,54,SEEK_SET);
					read(fd_sha,fuck,700*300*3);
					close(fd_sha);
					fd_dame=open("/dev/fb0",O_RDWR);
					for(i=0;i<700*300*3;i++)
					{
						if(i%3==1)
						{
							p=fuck[i];
						}
						if(i%3==2)
						{
							v=fuck[i];
							f=i/3;
							kao[f]=v*256*256+p*256+m;
						}
						if(i%3==0)
						{
							m=fuck[i];
						}
					}
					for(i=0;i<300;i++)
					{
						//j=i/700;
						lseek(fd_dame,(390-i)*800*4+50*4,SEEK_SET);
						write(fd_dame,&kao[i*700],700*sizeof(int));
					}
					return 1;
				}
			}
		}
		if(empty==0) //判断失败 
		{
			for(i=0;i<4;i++)
			{
				for(j=1;j<4;j++)
				{
					if(value_2048[i][j]==value_2048[i][j-1]) dame++;
				}
			}
			for(j=0;j<4;j++)
			{
				for(i=1;i<4;i++)
				{
					if(value_2048[i][j]==value_2048[i-1][j]) dame++;
				}
			}
			if(dame==0)
			{
				int fd_sha,fd_dame;
				char fuck[700*300*3];
				int p,v,m,f;
				int kao[700*300];
				fd_sha=open("/home/shadiao.bmp",O_RDWR);
				lseek(fd_sha,54,SEEK_SET);
				read(fd_sha,fuck,700*300*3);
				close(fd_sha);
				fd_dame=open("/dev/fb0",O_RDWR);
				for(i=0;i<700*300*3;i++)
				{
					if(i%3==1)
					{
						p=fuck[i];
					}
					if(i%3==2)
					{
						v=fuck[i];
						f=i/3;
						kao[f]=v*256*256+p*256+m;
					}
					if(i%3==0)
					{
						m=fuck[i];
					}
				}
				for(i=0;i<300;i++)
				{
					//j=i/700;
					lseek(fd_dame,(390-i)*800*4+50*4,SEEK_SET);
					write(fd_dame,&kao[i*700],700*sizeof(int));
				}
				return 1;
			}
			dame=0;
		}
		empty=0;
	}
}
