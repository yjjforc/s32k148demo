/**
  ******************************************************************************
  * File Name          : SendimageData.h
  * Description        : Code for 3-dragon 发送显示参数数据->FPGA 
  * Author             : YJJ
  * Date               : 2018.9.5
  ******************************************************************************
  */
 
#include "SendimageData.h"

#include "JL_Data_Structure.h"


/* ---------------------------------------------------------------------------*/
/*
*数据流发送模块 方式一 方式二
*功能点：
*		方式一.传输显示文件地址数据给FPGA FPGA做内存搬运
*		方式二.传输在MCU处理过的图片给FPGA存储 再做地址搬运工作
*将自定义的图片文件或者数据存入FPGA相应地址，调用显示 ，汉字需要先生成或传输再显示
*------------------------------------------------------------------------------*/
/**
  * @brief UpMachine_CANData_Analyses 
  * @param Start_Addr:显示模块嵌套链表
  * @param Pic_Len: Can模块嵌套链表
  * @param pic_x: Can模块嵌套链表
  * @param pic_y: Can模块嵌套链表
  * @param pic_w: Can模块嵌套链表
  * @param pic_h: Can模块嵌套链表
  * @retval NULL
  */
 /* -----*/

SPISendBuf SSendBuf;

void SViewModeOne(SendimageHeader *SiHead)
{

	SSendBuf.Command_send_buf[0]=0xFF;
	SSendBuf.Command_send_buf[1]=0x18;
	SSendBuf.Command_send_buf[2]=0xA2;
	SSendBuf.Command_send_buf[3]=0xD1;//表头

	SSendBuf.Command_send_buf[4]=SiHead->Start_Addr>>24;//32位图像起始地址
	SSendBuf.Command_send_buf[5]=SiHead->Start_Addr>>16;//32位图像起始地址
	SSendBuf.Command_send_buf[6]=SiHead->Start_Addr>>8;//32位图像起始地址
	SSendBuf.Command_send_buf[7]=SiHead->Start_Addr;//32位图像起始地址

	SSendBuf.Command_send_buf[8]=SiHead->Pic_Len>>24;//32位图像数据长度
	SSendBuf.Command_send_buf[9]=SiHead->Pic_Len>>16;//32位图像数据长度
	SSendBuf.Command_send_buf[10]=SiHead->Pic_Len>>8;//32位图像数据长度
	SSendBuf.Command_send_buf[11]=SiHead->Pic_Len;//32位图像数据长度        

	SSendBuf.Command_send_buf[12]=SiHead->pic_X>>8;//32位图像X
	SSendBuf.Command_send_buf[13]=SiHead->pic_X;//32位图像X
	SSendBuf.Command_send_buf[14]=SiHead->pic_Y>>8;//32位图像Y
	SSendBuf.Command_send_buf[15]=SiHead->pic_Y;//32位图像Y


	SSendBuf.Command_send_buf[16]=SiHead->pic_W>>8;//宽度
	SSendBuf.Command_send_buf[17]=SiHead->pic_W;
	SSendBuf.Command_send_buf[18]=SiHead->pic_H>>8;//长度
	SSendBuf.Command_send_buf[19]=SiHead->pic_H;

	Spi_SendData(SSendBuf.Command_send_buf,SSendBuf.Command_rec_buf, 20);

  //Log("SSendBuf==%s\r\n",SSendBuf.Command_send_buf);
}  

/*------------------------------------------------------------------------------*/
/**
  * @brief SViewModeTwo 
  * @param size:图片总元素个数
  * @retval NULL
  * 发送需要写入副内存地址的表头 有效变量size
  */
 /* -----*/
void SViewModeTwo(uint32_t size)
{

  SSendBuf.image_send_buf[0]=0xFF;//标识符
	SSendBuf.image_send_buf[1]=0x18;//标识符
	SSendBuf.image_send_buf[2]=0xA1;//标识符
	SSendBuf.image_send_buf[3]=0xD1;//层数
	SSendBuf.image_send_buf[4]=0x00;//x,y
	SSendBuf.image_send_buf[5]=0x00;
	SSendBuf.image_send_buf[6]=0x00;
	SSendBuf.image_send_buf[7]=0x00;//y
	SSendBuf.image_send_buf[8]=size>>24;//图片总元素个数
	SSendBuf.image_send_buf[9]=size>>16;
	SSendBuf.image_send_buf[10]=size>>8;
	SSendBuf.image_send_buf[11]=size;
	SSendBuf.image_send_buf[12]=size>>24;//wh
	SSendBuf.image_send_buf[13]=size>>16;
	SSendBuf.image_send_buf[14]=size>>8;
	SSendBuf.image_send_buf[15]=size;
	
	Spi_SendData(SSendBuf.image_send_buf,SSendBuf.image_rec_buf, 16);

}
