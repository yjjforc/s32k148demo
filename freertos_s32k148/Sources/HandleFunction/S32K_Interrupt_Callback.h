/**
  ******************************************************************************
  * File Name          : S32K_Interrupt_Callback.h
  * Description        : Code for 3-dragon S32K 中断回调函数
  * Author             : YJJ
  * Date               : 2018.9.5
  ******************************************************************************
  */
 
#ifndef S32K_INTERRUPT_CALLBACK_H
#define S32K_INTERRUPT_CALLBACK_H


#include "stdint.h"
#include "Cpu.h"


#define TX_MAILBOX  (1UL)
#define TX_MSG_ID   (1UL)
#define RX_MAILBOX  (0UL)
#define RX_MSG_ID   (2UL)

void flexcan0_Callback(uint8_t instance,
					   flexcan_event_type_t eventType,uint32_t buffIdx,
					   flexcan_state_t *flexcanState);



void uart_isr(void *driverState, uart_event_t event, void *userData);
 #endif
