/**
  ******************************************************************************
  * File Name          : ShowModule.h
  * Description        : Code for 3-dragon 显示模块 
  * Author             : YJJ
  * Date               : 2018.9.5
  ******************************************************************************
  */
 
#ifndef _SHOWMODULE_H
#define _SHOWMODULE_H


#include "device_registers.h"           // Device header
#include "JL_Data_Structure.h"
void ViewShow(User_Header * UHead,CanModuleHeader * CHead,uint32_t IDItemValue);
void  show_init(uint8_t Layer);
void show_Layer(uint8_t Layer,uint32_t ulReceivedValue);

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
void Show_pic_Handle(SubShowModuleHeader *PSSMHead,\
					 SubModuleShowPicDataContent *PSSPDCont);//图标显示处理函数

/**
  * @brief View_Num_Handle 
  * @param submodhead:Can子模块指针
  * @retval NULL
  */
 /* -----*/
/*---------------------------------------------------------------------------*/
void Show_Num_Handle(SubShowModuleHeader *PSSMHead,\
					 SubModuleShowNumFontDataContent *PSSNFDCont,\
					 SubModuleCANNumDataContent *PSCNDCont,uint8_t Tcount,User_Header *Uhead);//数字显示函数


/**
  * @brief View_Font_Handle 
  * @param submodhead:Can子模块指针
  * @retval NULL
  */
 /* -----*/
/*---------------------------------------------------------------------------*/
void Show_Font_Handle(SubShowModuleHeader *PSSMHead,\
					 SubCanModuleHeader *PSCMHead,\
					 SubModuleShowNumFontDataContent *PSSNFDCont,\
					 SubModuleCANFontDataContent *PSCFDCont,User_Header*Uhead);//汉字显示处理函数

void Show_Animated_Handle(SubShowModuleHeader *PSSMHead,\
                       SubCanModuleHeader *PSCMHead,\
                       SubModuleShowAnimatedDataContent *PSSMHeadADCont,\
                       SubModuleCANNumDataContent *PSCNDCont);


void Show_Bar_Handle(SubShowModuleHeader *PSSMHead,\
                       SubCanModuleHeader *PSCMHead,\
                       SubModuleShowBarDataContent *PSSMHeadBDCont,\
                       SubModuleCANNumDataContent *PSCNDCont);
           
           
#endif
