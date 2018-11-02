/**
  ******************************************************************************
  * File Name          : MyPNVIC_Config.h
  * Description        : Code for 3-dragon S32K NVIC 中断向量控制 封装参考 stm32hal
  * Author             : YJJ
  * Version            : 0.0.1
  * Date               : 2018.6.12
  ******************************************************************************
  */
 
#ifndef MYPNVIC_CONFIG_H
#define MYPNVIC_CONFIG_H

#include "device_registers.h"
 #include "core_cm4.h"


// #define NVIC_PRIORITYGROUP_0         0x00000007U /*!< 0 bits for pre-emption priority
//                                                       4 bits for subpriority */
// #define NVIC_PRIORITYGROUP_1         0x00000006U /*!< 1 bits for pre-emption priority
//                                                       3 bits for subpriority */
// #define NVIC_PRIORITYGROUP_2         0x00000005U !< 2 bits for pre-emption priority
//                                                       2 bits for subpriority 
// #define NVIC_PRIORITYGROUP_3         0x00000004U /*!< 3 bits for pre-emption priority
//                                                       1 bits for subpriority */
// #define NVIC_PRIORITYGROUP_4         0x00000003U /*!< 4 bits for pre-emption priority
//                                                       0 bits for subpriority */



void S32K_NVIC_SetPriority(IRQn_Type IRQn,  uint8_t priority);
void S32K_NVIC_EnableIRQ(IRQn_Type IRQn);
void S32K_NVIC_DisableIRQ(IRQn_Type IRQn);
void S32K_NVIC_SystemReset(void);





#endif