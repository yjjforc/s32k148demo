/**
  ******************************************************************************
  * File Name          : ViewModule.h
  * Description        : Code for 3-dragon 显示模块 
  * Author             : YJJ
  * Date               : 2018.9.5
  ******************************************************************************
  */
 
#ifndef _VIEWMODULE_H
#define _VIEWMODULE_H


#include "device_registers.h"           // Device header
#include "JL_Data_Structure.h"
void ViewShow(void *Viewlist,void *CanList);


/* ---------------------------------------------------------------------------*/
/*
*图标 数字 汉字显示
*功能点：
*		1.依据子模块数据结构 做处理
*		
*------------------------------------------------------------------------------*/
/**
  * @brief View_pic_Handle 
  * @param submodhead:Can子模块指针
  * @retval NULL
  */
 /* -----*/
/*---------------------------------------------------------------------------*/
void View_pic_Handle(SubShowModuleHeader *subviemodhead,\
					 SubCanModuleHeader *subcanmodhead,\
					 SubModuleShowPicDataContent *viewpicdatacon,\
					 SubModuleCANPicDataContent *cannpicdacon);//图标显示处理函数

/**
  * @brief View_Num_Handle 
  * @param submodhead:Can子模块指针
  * @retval NULL
  */
 /* -----*/
/*---------------------------------------------------------------------------*/
void View_Num_Handle(SubShowModuleHeader *subviemodhead,\
					 SubCanModuleHeader *subcanmodhead,\
					 SubModuleNumFontDataContent *viewnumdatacon,\
					 SubModuleCANNumDataContent *cannumdacon);//数字显示函数


/**
  * @brief View_Font_Handle 
  * @param submodhead:Can子模块指针
  * @retval NULL
  */
 /* -----*/
/*---------------------------------------------------------------------------*/
void View_Font_Handle(SubShowModuleHeader *subviemodhead,\
					 SubCanModuleHeader *subcanmodhead,\
					 SubModuleNumFontDataContent *viewfontdatacon,\
					 SubModuleCANFontDataContent *canfontdatacon);//汉字显示处理函数
           
           
uint32_t Num_Addr_Choose(uint8_t number,uint16_t w,uint16_t h,uint8_t Num_kind);
#endif
