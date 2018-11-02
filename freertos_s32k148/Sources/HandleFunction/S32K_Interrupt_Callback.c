/**
  ******************************************************************************
  * File Name          : S32K_Interrupt_Callback.h
  * Description        : Code for 3-dragon S32K 涓柇鍥炶皟鍑芥暟
  * Author             : YJJ
  * Date               : 2018.9.5
  ******************************************************************************
  */
 
#include "S32K_Interrupt_Callback.h"
#include "Log.h"
#include "delay.h"
#include "JL_Data_Structure.h"
#include "Flash_Handle.h"
extern flexcan_msgbuff_t rxDataBuff;
extern QueueHandle_t TxQueue;
extern unsigned long ulValueToSend;
extern CRece_BUFF CRBuffer;

static void CCopyData(flexcan_msgbuff_t *recemsg);

void flexcan0_Callback(uint8_t instance,
					   flexcan_event_type_t eventType,uint32_t buffIdx,
					   flexcan_state_t *flexcanState)
{
	switch(eventType)
	{
	case FLEXCAN_EVENT_RX_COMPLETE:\
	{

		FLEXCAN_DRV_Receive(INST_CANCOM1, RX_MAILBOX, &rxDataBuff);
     
     if (ulValueToSend==0xFFFFFFFF)
     {
        CCopyData(&CRBuffer.recemsg1);
        ulValueToSend=(uint32_t)&CRBuffer.recemsg1;	
     }
     else if (ulValueToSend==(uint32_t)&CRBuffer.recemsg1)
     {
        CCopyData(&CRBuffer.recemsg2);
        ulValueToSend=(uint32_t)(uint32_t)&CRBuffer.recemsg2;	
     }
     else if (ulValueToSend==(uint32_t)&CRBuffer.recemsg2)
     {
        CCopyData(&CRBuffer.recemsg3);
        ulValueToSend=(uint32_t)&CRBuffer.recemsg3;	
     }
     else
     {
     	#ifdef DEBUG
     	Log("have no buffer to CCopyData\r\n");
     	#endif
     }
     
     if(ulValueToSend!=0xFFFFFFFF)
     {
        xQueueSendFromISR( TxQueue, &ulValueToSend, 0 );
     }
     
	}
		break;
	case FLEXCAN_EVENT_TX_COMPLETE:
	{

	}
		break;
	default:
		break;
	}
}
void CCopyData(flexcan_msgbuff_t *recemsg)
{
 	recemsg->cs=rxDataBuff.cs;
	for (int i = 0; i < sizeof(rxDataBuff.data); i++)
	{
		recemsg->data[i]=rxDataBuff.data[i];
	}
	recemsg->dataLen=rxDataBuff.dataLen;
	recemsg->msgId=rxDataBuff.msgId;

}
int valutt=0;
 int flashaddr=0;
static uint16_t Recount=0;
static uint8_t flag4096='c';
static uint8_t flagstart='a';
static uint8_t start_flag=0;
static uint8_t flagstop='d';
static uint8_t receOldData=0;
static uint8_t flash_Wbuf[4096];
static uint8_t test_buf[10];
void uart_isr(void *driverState, uart_event_t event, void *userData)
{
	LPUART_DRV_GetData(INST_LPUART1);
  if(*lpuart1_State.rxBuff=='@'&&test_buf[0]=='@'&&test_buf[1]=='@')//判断是否是起始标志
  {
    LPUART_DRV_SendData(INST_LPUART1,&flagstart, 1);
    start_flag=1;
  }
  
  if(start_flag==1)//如果标志置位
  {
  if(*lpuart1_State.rxBuff=='#'&&test_buf[0]=='#'&&test_buf[1]=='#')//判断是否是结束标志
  {
    LPUART_DRV_SendData(INST_LPUART1,&flagstop, 1);
    Flash_WordWrite(flash_Wbuf,flashaddr-flashaddr%4096-1,flashaddr%4096);
    

    start_flag=0;
    //将剩余数据写入flash
  }
  else//开始接收数据
  {
    
    flash_Wbuf[Recount++]=*lpuart1_State.rxBuff;
    flashaddr++;//地址加1  为了判断是否是接收了4096个数据
      if(flashaddr%4096==0)//判断是否接收4096个数据
  {
     //开始写入flash 数据
     Flash_WordWrite(flash_Wbuf,flashaddr,4096);
     //发送写入完成标志
     LPUART_DRV_SendData(INST_LPUART1,&flag4096, 1);
    
  }
  
  if(Recount==4096)//判断接收数据是否是4096 清零
  {
    Recount=0;
  }
  }
  
  }
  if(valutt==2)
  {
    valutt=0;
  }
  test_buf[valutt++]=*lpuart1_State.rxBuff;//记录收到的值保存
  

  

// LPUART_DRV_SendData(INST_LPUART1,lpuart1_State.rxBuff, 1);
}
      