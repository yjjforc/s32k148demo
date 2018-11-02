/**
  ******************************************************************************
  * File Name          : PicAreaHandle.c
  * Description        : Code for 3-dragon 显示区域图像处理
  * Author             : YJJ
  * Date               : 2018.9.5
  ******************************************************************************
  */
 #include "PicDraw.h"
#include "SendimageData.h"
#include "JL_Data_Structure.h"
/* ---------------------------------------------------------------------------*/
/*
*显示区域图像处理 做自定义图像 RGB888
*功能点：
*		1.填充区域 全色填充
*		2.
*------------------------------------------------------------------------------*/
/**
  * @brief UpMachine_CANData_Analyses 
  * @param PixelRGB888alp: 像素点结构体

  * @retval NULL
  */
 /* -----*/
//全色填充


extern SPISendBuf SSendBuf;

uint8_t PicAreaFill(PixelRGB888alp *Picpixel ,AreaSize *areasize)
{
	
	int i =0;
	for(int j=0; j<1024;j++)
	{
		SSendBuf.masterDataSend[i++]=Picpixel->color1;
		SSendBuf.masterDataSend[i++]=Picpixel->color2;
		SSendBuf.masterDataSend[i++]=Picpixel->color3;
		SSendBuf.masterDataSend[i++]=Picpixel->alpha_channel;
	}
	if(areasize->MaxW*areasize->MaxH*4<=0)
	{
		return 0;
	}
	if(areasize->MaxW*areasize->MaxH*4<4096)//如果发送的数据少于4096
	{
		Spi_SendData(SSendBuf.masterDataSend,SSendBuf.masterDataReceive, areasize->MaxW*areasize->MaxH*4);
	}
	else//多于4096
	{
		for (uint8_t i = 0; i < (areasize->MaxW*areasize->MaxH*4)/4096; i++)
		{
			Spi_SendData(SSendBuf.masterDataSend,SSendBuf.masterDataReceive, 4096);
		}
		if (((areasize->MaxW*areasize->MaxH*4)%4096)!=0)//如果还有剩余
		{
			Spi_SendData(SSendBuf.masterDataSend,SSendBuf.masterDataReceive,(areasize->MaxW*areasize->MaxH*4)%4096);
		}		
	}
    SendimageHeader sendimageheader;
    sendimageheader.Start_Addr=AHead.UHead1->FPGA_deputy_addr;//NumFlash_deputy_addr+Num_W*Num_H*20 为偏移量地址
    sendimageheader.Pic_Len=areasize->MaxW*areasize->MaxH;//获取所有需要显示汉字的像素点长度
    sendimageheader.pic_X=areasize->MaxX;//其实坐标从起始的解析文件正写入特定的坐标 此坐标是数字在whindow中的显示坐标经过偏移
    sendimageheader.pic_Y=areasize->MaxY;
    sendimageheader.pic_W=areasize->MaxW;
    sendimageheader.pic_H=areasize->MaxH;//横坐标不变 除非超过最大汉字显示范围
    SViewModeOne(&sendimageheader);//发送表头开始搬运
  return 0; 
}

//清除区域 黑色清除
void Clear_Region(SubShowModuleHeader *PSSMHead)
{
	//擦除显示区域的部分
	PixelRGB888alp Picpixel;
	AreaSize areasize;
	Picpixel.color1=0x00;
	Picpixel.color2=0x00;
	Picpixel.color3=0x00;
	Picpixel.alpha_channel=0x00;
	areasize.MaxX=PSSMHead->MaxX;
	areasize.MaxY=PSSMHead->MaxY;
	areasize.MaxW=PSSMHead->MaxW;
	areasize.MaxH=PSSMHead->MaxH;

  SViewModeTwo(areasize.MaxW*areasize.MaxH);//发送字符数组表头 size大小=像素个数*有效数据长度

	PicAreaFill(&Picpixel,&areasize);//全色填充 清空区域
}
