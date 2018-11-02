/**
  ******************************************************************************
  * File Name          : FlexCanTxRx.c
  * Description        : Code for 3-dragon Can 模块发送接收
  * Author             : YJJ
  * Version            : 0.0.1
  * Date               : 2018.9.8
  ******************************************************************************
  */
 

#include "FlexCanTxRx.h"
#include "S32K_Interrupt_Callback.h"
//CAN

extern flexcan_msgbuff_t rxDataBuff;

void FlexCan_Init(void)
{
	//配置CAN RX参数
  flexcan_data_info_t dataInfo =
  {
          .data_length = 8U,
          .msg_id_type = FLEXCAN_MSG_ID_STD,
          .enable_brs  = true,
          .fd_enable   = false,
          .fd_padding  = 0U
  };
  //测试can发送数据
  uint8_t senddata=1;

  //CAN初始化
  FLEXCAN_DRV_Init(INST_CANCOM1, &canCom1_State, &canCom1_InitConfig0);
  //配置过滤器种类
  FLEXCAN_DRV_SetRxMaskType(INST_CANCOM1, FLEXCAN_RX_MASK_GLOBAL);    /* Set the global mask as "don't care" for each message buffer */
  FLEXCAN_DRV_SetRxMbGlobalMask(INST_CANCOM1, FLEXCAN_MSG_ID_STD, 0U);
  //配置 CAN Rx Message buffer
  FLEXCAN_DRV_ConfigRxMb(INST_CANCOM1, RX_MAILBOX, &dataInfo, RX_MSG_ID);
  // FLEXCAN_DRV_SetRxMaskType(INST_CANCOM1,FLEXCAN_RX_MASK_INDIVIDUAL);//选择个人
  // FLEXCAN_DRV_SetRxIndividualMask(INST_CANCOM1,FLEXCAN_MSG_ID_STD,0x0001,0x0021);
  // 配置CALLBACK 中断回调
  FLEXCAN_DRV_InstallEventCallback(INST_CANCOM1,flexcan0_Callback, NULL);
  //开始CAN接收
  FLEXCAN_DRV_Receive(INST_CANCOM1, RX_MAILBOX, &rxDataBuff);
  //测试发送CAN数据
  SendCANData(TX_MAILBOX, TX_MSG_ID,  &senddata, 1UL);
}




void SendCANData(uint32_t mailbox, uint32_t messageId, uint8_t * data, uint32_t len)
{
    /* Set information about the data to be sent
     *  - 1 byte in length
     *  - Standard message ID
     *  - Bit rate switch enabled to use a different bitrate for the data segment
     *  - Flexible data rate enabled
     *  - Use zeros for FD padding
     */
    flexcan_data_info_t dataInfo =
    {
            .data_length = len,
            .msg_id_type = FLEXCAN_MSG_ID_STD,
            .enable_brs  = true,
            .fd_enable   = true,
            .fd_padding  = 0U
    };

    /* Configure TX message buffer with index TX_MSG_ID and TX_MAILBOX*/
    FLEXCAN_DRV_ConfigTxMb(INST_CANCOM1, mailbox, &dataInfo, messageId);

    /* Execute send non-blocking */
    FLEXCAN_DRV_Send(INST_CANCOM1, mailbox, &dataInfo, messageId, data);
}