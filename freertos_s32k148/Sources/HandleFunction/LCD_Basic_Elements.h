/**
  ******************************************************************************
  * File Name          : LCD_Basic_Element.c
  * Description        : Code for 3-dragon 基本元素 显示 
  * Author             : YJJ
  * Date               : 2018.10.26
  ******************************************************************************
  */
 /*此C文件是此项目特殊的底层接口
  *分为 画线 画圆 矩形 画笔粗度
  */
 
#ifndef LCD_BASIC_ELEMENT_H
#define LCD_BASIC_ELEMENT_H

#include "Cpu.h"

void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint32_t color);

void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t w,uint16_t h,uint32_t color);


void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t w,uint16_t h,uint32_t color);

void LCD_Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r,uint16_t w,uint16_t h,uint32_t color);

#endif