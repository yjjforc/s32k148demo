/**
  ******************************************************************************
  * File Name          : LCD_Basic_Elements.c
  * Description        : Code for 3-dragon 基本元素 显示 
  * Author             : YJJ
  * Date               : 2018.10.26
  ******************************************************************************
  */
 /*此C文件是此项目特殊的底层接口
  *分为 画线 画圆 矩形 画笔粗度
  */
 
//画点
//x,y:坐标
//POINT_COLOR:此点的颜色
#include "LCD_Basic_Elements.h"
#include "SendimageData.h"
#include "JL_Data_Structure.h"
extern SPISendBuf SSendBuf;
static uint8_t Ssize=0;
static uint32_t Scolor=0;
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint32_t color)
{
 
    
    if(Scolor!=color||Ssize!=w)
    {
      SViewModeTwo(w*h);//发送字符数组表头 size大小=像素个数*有效数据长度
            for(uint16_t i=0,j=0; i<1024;i++)
      {
        SSendBuf.masterDataSend[j++]=color>>24;
        SSendBuf.masterDataSend[j++]=color>>16;
        SSendBuf.masterDataSend[j++]=color>>8;
        SSendBuf.masterDataSend[j++]=color;
      }

      if(w*h*4<4096)//如果发送的数据少于4096
      {
      	Spi_SendData(SSendBuf.masterDataSend,SSendBuf.masterDataReceive,w*h*4);
      }
      else
      {
        for (uint8_t i = 0; i < (w*h*4)/4096; i++)
        {
          Spi_SendData(SSendBuf.masterDataSend,SSendBuf.masterDataReceive, 4096);
        }
        if (((w*h*4)%4096)!=0)//如果还有剩余
        {
          Spi_SendData(SSendBuf.masterDataSend,SSendBuf.masterDataReceive,(w*h*4)%4096);
        }	
        
      }
    
      Scolor=color;
      Ssize=w;
    }
    SendimageHeader sendimageheader;
    sendimageheader.Start_Addr=AHead.UHead1->FPGA_deputy_addr;//NumFlash_deputy_addr+Num_W*Num_H*20 为偏移量地址
    sendimageheader.Pic_Len=w*h;//获取所有需要显示的像素点长度
    sendimageheader.pic_X=x;//其实坐标从起始的解析文件正写入特定的坐标 此坐标是数字在whindow中的显示坐标经过偏移
    sendimageheader.pic_Y=y;
    sendimageheader.pic_W=w;
    sendimageheader.pic_H=h;//横坐标不变 除非超过最大汉字显示范围
    SViewModeOne(&sendimageheader);//发送表头开始搬运
 //4个像素为一个点
}

//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t w,uint16_t h,uint32_t color)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出  //是否要减去一个点的长度
	{  
		LCD_DrawPoint(uRow,uCol,w,h,color);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}  
//画矩形	  
//(x1,y1),(x2,y2):矩形的对角坐标
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t w,uint16_t h,uint32_t color)
{
	LCD_DrawLine(x1,y1,x2,y1, w,h, color);
	LCD_DrawLine(x1,y1,x1,y2, w,h, color);
	LCD_DrawLine(x1,y2,x2,y2, w,h, color);
	LCD_DrawLine(x2,y1,x2,y2, w,h, color);
}


void LCD_Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r,uint16_t w,uint16_t h,uint32_t color)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD_DrawPoint(x0+a,y0-b, w,h, color);             //5
 		LCD_DrawPoint(x0+b,y0-a, w,h, color);             //0           
		LCD_DrawPoint(x0+b,y0+a, w,h, color);             //4               
		LCD_DrawPoint(x0+a,y0+b, w,h, color);             //6 
		LCD_DrawPoint(x0-a,y0+b, w,h, color);             //1       
 		LCD_DrawPoint(x0-b,y0+a, w,h, color);             
		LCD_DrawPoint(x0-a,y0-b, w,h, color);             //2             
    LCD_DrawPoint(x0-b,y0-a, w,h, color);             //7     	         
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
}
