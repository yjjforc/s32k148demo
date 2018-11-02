/**
  ******************************************************************************
  * File Name          : UnicodeFont.h
  * Description        : Code for 3-dragon Unicode 汉字解析处理
  * Author             : YJJ
  * Date               : 2018.9.5
  ******************************************************************************
  */
 #ifndef UNICODE_FONT_H
#define UNICODE_FONT_H


#include "stdint.h"


/* ---------------------------------------------------------------------------*/
/*
*Unicode 汉字解析处理
*功能点：
*		1.根据Unicode值解析数据 边解析边传输 一次发送4096个字节
*		
*------------------------------------------------------------------------------*/
/**
  * @brief Font_Analyses 
  * @param buf:点阵字体数组 
  * @retval NULL
  */
 /* -----*/
/*---------------------------------------------------------------------------*/
void Font_Analyses(uint8_t *buf);
#include "JL_Data_Structure.h"
void Unicode_GetArray(uint32_t Unicode,uint16_t Font_W,uint16_t Font_H,SubModuleShowNumFontDataContent *viewfontdatacon);

//字符点阵数组
extern uint8_t Font_Buf[128];

#endif
