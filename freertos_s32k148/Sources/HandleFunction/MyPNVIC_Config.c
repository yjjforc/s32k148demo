/**
  ******************************************************************************
  * File Name          : MyPNVIC_Config.c
  * Description        : Code for 3-dragon S32K NVIC 中断向量控制 封装参考 stm32hal
  * Author             : YJJ
  * Version            : 0.0.1
  * Date               : 2018.6.12
  ******************************************************************************
  */
 

#include "MyPNVIC_Config.h"
#include "interrupt_manager.h"


void S32K_NVIC_SetPriority(IRQn_Type IRQn,  uint8_t priority)
{ 
	INT_SYS_SetPriority(IRQn, priority);
}

void S32K_NVIC_EnableIRQ(IRQn_Type IRQn)
{
  /* Enable interrupt */
  INT_SYS_EnableIRQ(IRQn);
}

void S32K_NVIC_DisableIRQ(IRQn_Type IRQn)
{ 
  /* Disable interrupt */
  INT_SYS_DisableIRQ(IRQn);
}

void S32K_NVIC_SystemReset(void)
{
  /* System Reset */
  NVIC_SystemReset();
}

