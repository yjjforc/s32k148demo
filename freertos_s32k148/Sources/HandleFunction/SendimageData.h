/**
  ******************************************************************************
  * File Name          : SendimageData.h
  * Description        : Code for 3-dragon 发送显示参数数据->FPGA 
  * Author             : YJJ
  * Date               : 2018.9.5
  ******************************************************************************
  */
 
 #ifndef  SENDIMAGEDATA_H
 #define SENDIMAGEDATA_H

#include "stdint.h"



typedef struct 
{
  uint32_t Start_Addr;//起始地址
  uint32_t Pic_Len;//数据长度
  uint32_t pic_X;//起始坐标x
  uint32_t pic_Y;//起始坐标y
  uint32_t pic_W;//图片宽度
  uint32_t pic_H;//图片高度
}SendimageHeader;



typedef struct
{
  uint8_t Command_send_buf[20];
  uint8_t Command_rec_buf[20];
  uint8_t image_send_buf[16];
  uint8_t image_rec_buf[16];
  uint8_t masterDataSend[4096];
  uint8_t masterDataReceive[4096];
}SPISendBuf;

void SViewModeOne(SendimageHeader *sendimageheader);

void SViewModeTwo(uint32_t size);


#endif