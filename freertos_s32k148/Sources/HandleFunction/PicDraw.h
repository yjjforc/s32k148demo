/**
  ******************************************************************************
  * File Name          : PicDraw.h
  * Description        : Code for 3-dragon 显示区域图像处理
  * Author             : YJJ
  * Date               : 2018.9.5
  ******************************************************************************
  */
 

#ifndef PICDRAW_H
#define PICDRAW_H

#include "stdint.h"
#include "flexcan_driver.h"
#include "JL_Data_Structure.h"
typedef struct 
{
	uint8_t color1;
	uint8_t color2;
	uint8_t color3;
	uint8_t alpha_channel;
	
}PixelRGB888alp;


typedef struct 
{
	uint16_t MaxX;
	uint16_t MaxY;
	uint16_t MaxW;
	uint16_t MaxH;
}AreaSize;



uint8_t PicAreaFill(PixelRGB888alp *Picpixel ,AreaSize *areasize);

void Clear_Region(SubShowModuleHeader *PSSMHead);


#endif
