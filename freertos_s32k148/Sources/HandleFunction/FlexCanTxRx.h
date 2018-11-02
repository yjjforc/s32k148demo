/**
  ******************************************************************************
  * File Name          : FlexCanTxRx.c
  * Description        : Code for 3-dragon Can 模块发送接收
  * Author             : YJJ
  * Version            : 0.0.1
  * Date               : 2018.9.8
  ******************************************************************************
  */
 
#ifndef FLEXCANTXRX_H
#define FLEXCANTXRX_H


#include "canCom1.h"

    
#define TX_MAILBOX  (1UL)
#define TX_MSG_ID   (1UL)
#define RX_MAILBOX  (0UL)
#define RX_MSG_ID   (2UL)    
    
    




void FlexCan_Init(void);


void SendCANData(uint32_t mailbox, uint32_t messageId, uint8_t * data, uint32_t len);






#endif
