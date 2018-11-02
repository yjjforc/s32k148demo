/**
  ******************************************************************************
  * File Name          : CanHandle.c
  * Description        : Code for 3-dragon CAN  数据处理
  * Author             : YJJ
  * Date               : 2018.9.5
  ******************************************************************************
  */
#ifndef _CAN_HANDLE_H
#define _CAN_HANDLE_H

#include "flexcan_driver.h"

#include "JL_Data_Structure.h"

unsigned long Can_Rece_Data(CanModuleHeader *CHead,flexcan_msgbuff_t *recemsg);

void CANRec_Layer(uint8_t Layer);
#endif