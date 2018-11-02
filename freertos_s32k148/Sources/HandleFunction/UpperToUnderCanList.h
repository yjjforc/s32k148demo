/**
  ******************************************************************************
  * File Name          : UpperToUnderCanList.h
  * Description        : Code for 3-dragon 上位机 ->下位机 传输CAN解析文件
  * Author             : YJJ
  * Date               : 2018.9.5
  ******************************************************************************
  */
 
#ifndef _UPPERTOUNDERCANLIST_H
#define _UPPERTOUNDERCANLIST_H



/* ---------------------------------------------------------------------------*/
/* *解析上位机下发到MCU的CAN模块的数据，该数据存储在flash中-------------*/
/**
  * @brief UpMachine_CANData_Analyses 
  * @param void   
  * @retval NULL
  */
 /* -----*/
void UpMachine_CAN_Analyses(void *Canhead_list);    

#endif