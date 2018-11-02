/**
  ******************************************************************************
  * File Name          : CanHandle.c
  * Description        : Code for 3-dragon CAN  数据处理 
  * Author             : YJJ
  * Date               : 2018.9.5
  ******************************************************************************
  */
 /* ---------------------------------------------------------------------------*/
/*
*处理从CAN接口传出的CAN数据  
*功能点：
*       1.将输出的数据对应在CANList中，动态解析数据
*         例如：解析速度 速度从接口中出来，包含 CANID STD 数据域选项 听过比对 canlist 将数据存入相应的地址空间，无需自定义结构体存入动态链表中
*              解析图标 左转右转直行 分3个值 当包含这个数据选项的CAN帧出来后，左转假设为0 ，右转1 直行 2  c
*              将其存入数据大小中
*       2.分开解析 图标 数字 汉字
*       读取CAN数据 
* */
/* ---------------------------------------------------------------------------*/
/* *从CAN接口中读取需要解析的数据-------------*/
/**
  * @brief UpMachine_CANData_Analyses 
  * @param flexcan_msgbuff_t:CAN接受结构体
  * @retval NULL
  */
 /* -----*/
//typedef struct {
//    uint32_t cs;                        /*!< Code and Status*/
//    uint32_t msgId;                     /*!< Message Buffer ID*/
//    uint8_t data[64];                   /*!< Data bytes of the FlexCAN message*/
//    uint8_t dataLen;                    /*!< Length of data in bytes */

//} ;flexcan_msgbuff_t
#include "CanHandle.h"

#include "JL_Data_Structure.h"
#include "LinkList.h"
//#include "FreeRTOS.h"
//#include "semphr.h"


extern QueueHandle_t RxQueue;


unsigned long Can_Rece_Data(CanModuleHeader *CHead,flexcan_msgbuff_t *recemsg)
{
	  
	 	uint8_t i,j,k=0;
	//首先遍历链表找到对应的CANID 数组在CAN帧的位置
    uint32_t SendValue=0Xffffffff;
    uint16_t ID=0;
    uint16_t item=0;
    
    
	for ( i = 0; i <CHead->SubModuleSum ; i++)
	{
    
		SubCanModuleHeader *PSCMHead =	(SubCanModuleHeader *)GetPosValue(CHead->Sublist, i);//得到此节点在CAN链表的数据

		if(PSCMHead->CANID==recemsg->msgId)
		{
      		#ifdef DEBUG
      		Log("CANID is %d\r\n",PSCMHead->CANID);
      		#endif
      		ID=PSCMHead->ID;//获取CAN数据中的ID 号
			//获取到具体的数据存储区域
			if (PSCMHead->DataMode==1)//图标显示 所有帧都按照特定的规范存储 图片显示只有一个子数据项标号
			{
				for ( j = 0; j<PSCMHead-> Data_ItemSize; j++)
				{

					SubModuleCANPicDataContent *canpicdacon=(SubModuleCANPicDataContent *)GetPosValue(PSCMHead->Contlist,j);
					canpicdacon->Data_ItemNumber=recemsg->data[PSCMHead->Data_Pos];//将获取到数值赋值给链表中的数据项编号
          item=canpicdacon->Data_ItemNumber;//获取需要显示的子数据项item
				}
			}
			if (PSCMHead->DataMode==2||PSCMHead->DataMode==4||PSCMHead->DataMode==5)//数字显示 所有帧都按规定存储
			{
				for ( j = 0; j<PSCMHead-> Data_ItemSize; j++)
				{
					SubModuleCANNumDataContent *PSCNDCont=(SubModuleCANNumDataContent *)GetPosValue(PSCMHead->Contlist,j);
					if (PSCMHead->Effective_Data==1)
					{
						PSCNDCont->Data=recemsg->data[PSCMHead->Data_Pos];//将获取到数值赋值给链表中的数据项编号 高位在前
					}
					else if (PSCMHead->Effective_Data==2)
					{
						PSCNDCont->Data=recemsg->data[PSCMHead->Data_Pos+1]<<8|recemsg->data[PSCMHead->Data_Pos];//将获取到数值赋值给链表中的数据项编号 高位在前
					}
					else if (PSCMHead->Effective_Data==3)
					{
						PSCNDCont->Data=recemsg->data[PSCMHead->Data_Pos+2]<<16|recemsg->data[PSCMHead->Data_Pos+1]<<8|recemsg->data[PSCMHead->Data_Pos];//将获取到数值赋值给链表中的数据项编号 高位在前
					}
					else if (PSCMHead->Effective_Data==4)
					{
						PSCNDCont->Data=recemsg->data[PSCMHead->Data_Pos+2]<<24|recemsg->data[PSCMHead->Data_Pos+2]<<16|recemsg->data[PSCMHead->Data_Pos+1]<<8|recemsg->data[PSCMHead->Data_Pos];//将获取到数值赋值给链表中的数据项编号 高位在前
					}
					else
					{
						Log("CANrec Num no efect\r\n");
					}					
				}
        	item=1;//数字显示不需要item
			}
			if (PSCMHead->DataMode==3)//汉字显示 所有帧都按规定存储
			{
				//汉字一次性传输不完分两次传输
				for ( j = 0; j<PSCMHead-> Data_ItemSize; j++)
				{
					SubModuleCANFontDataContent *PSCFDCont=(SubModuleCANFontDataContent *)GetPosValue(PSCMHead->Contlist,j);
					//先判断传输的汉字的总字节数 字节数在CAN帧的第一个data
					if (recemsg->data[0]>3)//汉字的字数已大于3 一帧传输不完 存储等待
					{
           
            			PSCMHead->Effective_Data=recemsg->data[0];
						for (  k = 2; k < 7; k=k+2)
						{
							PSCFDCont->unicode_pack[PSCFDCont->NData_len]=recemsg->data[k]|recemsg->data[k+1]<<8;//存储所有值
              				if(PSCFDCont->NData_len== PSCMHead->Effective_Data)	//
              				{
                				break;
              				}
							PSCFDCont->NData_len++;//有效长度加1
						}
            
					}
					else
					{
						for (k =2; k <recemsg->data[0]*2+1 ; k=k+2)//K值为之前已经采集到的数据长度 如果没有就为0，如果有说明上次有传输的数据则继续接收
						{
							PSCFDCont->unicode_pack[PSCFDCont->NData_len]=recemsg->data[k]|recemsg->data[k+1]<<8;//存储所有值
							PSCFDCont->NData_len++;//有效长度加1
						}
					}
				}	
        		item=0;//汉字显示不需要item				
			}
//			if (PSCMHead->DataMode==99)//layer跳转
//      		{
// 				for ( j = 0; j<PSCMHead-> Data_ItemSize; j++)
//				{
//
//					SubModuleCANPicDataContent *canpicdacon=(SubModuleCANPicDataContent *)GetPosValue(PSCMHead->Contlist,j);
//					canpicdacon->Data_ItemNumber=recemsg->data[PSCMHead->Data_Pos];//将获取到数值赋值给链表中的数据项编号
//          			//AHead.Show_View=canpicdacon->Data_ItemNumber;//改变要显示的图层
//          			ID=65535;
//          			item=65535;
//				}         
//      		}
     
      SendValue= ID<<16|item;
      if(SendValue!=0Xffffffff)
      {
        xQueueSend( RxQueue, &SendValue, 0 );
        
      }
      SendValue=0Xffffffff;
      
       
			//发送消息队列给显示模块 viewmodule 显示
		}
	}
  return 0;
}

void CANRec_Layer(uint8_t Layer)
{
    switch(Layer)
  {
    case 1:
      {
        Can_Rece_Data(AHead.CHead1,&CRBuffer.recemsg1);
      }break;
    case 2:
      {
        Can_Rece_Data(AHead.CHead2,&CRBuffer.recemsg1);
      }break;
    case 3:
      {
        Can_Rece_Data(AHead.CHead3,&CRBuffer.recemsg1);
      }break;
    default: break;
  }
}