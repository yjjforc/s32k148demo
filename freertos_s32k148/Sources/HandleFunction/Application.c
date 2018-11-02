/**
  ******************************************************************************
  * File Name          : Application.c
  * Description        : Code for 3-dragon Application 应用
  * Author             : YJJ
  * Date               : 2018.9.5
  ******************************************************************************
  */


#include "Application.h"

 /* --------------------------------------------------------------------------*/
/*
* APP 应用层调用
*功能点：
*       1.应用层初始化
*       2.构建处理机制 数据流
*
*/
/* ---------------------------------------------------------------------------*/
/* *APP应用初始化-------------*/
/**
  * @brief APP_Init 
  * @param void   
  * @retval NULL
  */
 /* -----*/

void *Canhead_list;//CAN功能列表全局指针
void *viewhead_list;//view显示功能列表全局指针

void APP_Init(void)
{

    #include "cJUser.h"
    #include "cJCAN.h"
    #include "SendimageData.h"

    Can_Config();
    User_Config();  
}


