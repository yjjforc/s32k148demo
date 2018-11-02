/**
  ******************************************************************************
  * File Name          : UnicodeFont.h
  * Description        : Code for 3-dragon Unicode 汉字解析处理
  * Author             : YJJ
  * Date               : 2018.9.5
  ******************************************************************************
  */
#include "UnicodeFont.h"
#include "SendimageData.h"

/* ---------------------------------------------------------------------------*/
/*
*Unicode 汉字解析处理
*功能点：
*		1.根据Unicode值解析数据 边解析边传输 一次发送4096个字节
*		
*------------------------------------------------------------------------------*/
/**
  * @brief Font_Analyses 
  * @param buf:点阵字体数组 
  * @retval NULL
  */
 /* -----*/
/*---------------------------------------------------------------------------*/

uint8_t Font_Buf[128];
extern SPISendBuf SSendBuf;

void Font_Analyses(uint8_t *buf)
{

	uint8_t temp,t,t1;
	uint16_t i=0;
	uint8_t csize=128;			//得到字体一个字符对应点阵集所占的字节数	 

	for(t=0;t<csize;t++)
	{   												   
	  temp=buf[t];			//得到点阵数据                          
	  for(t1=0;t1<8;t1++)
	  {
	    if(temp&0x80)
	    {
	      SSendBuf.masterDataSend[i++]=0xFF;
	      SSendBuf.masterDataSend[i++]=0xFF;
	      SSendBuf.masterDataSend[i++]=0xFF;
	      SSendBuf.masterDataSend[i++]=0xFF;
	    }
	    else
	    {
	      SSendBuf.masterDataSend[i++]=0x00;
	      SSendBuf.masterDataSend[i++]=0x00;
	      SSendBuf.masterDataSend[i++]=0x00;
	      SSendBuf.masterDataSend[i++]=0x00;
	    }
	    temp<<=1;
	    if(i==4096)
	    {
	      Spi_SendData(SSendBuf.masterDataSend,SSendBuf.masterDataReceive, 4096);
	      i=0;
	    }
	  }  
    
	}  
}

/* ---------------------------------------------------------------------------*/
/*
*Unicode 汉字读取
*功能点：
*		1.根据Unicode值从flash中读取点阵数组存入数组缓冲中
*		
*------------------------------------------------------------------------------*/
/**
  * @brief Font_Analyses 
  * @param buf:点阵字体数组 
  * @retval NULL
  */
 /* -----*/
/*---------------------------------------------------------------------------*/
#include "Flash_Handle.h"
#include "LinkList.h"
void Unicode_GetArray(uint32_t Unicode,uint16_t Font_W,uint16_t Font_H,SubModuleShowNumFontDataContent *PSSNFDCont)
{
	uint32_t addr;
	//读取字体返回一个数组指针
  FontSubKind *FontSK =	(FontSubKind *)GetPosValue(AHead.UHead1->Font_list, PSSNFDCont->Unicode_Kind);//得到此节点在CAN链表的数据
	addr=FontSK->FontFlash_Addr+Unicode*((Font_W*Font_H)/8);//字库unicode字符起始地址
	Flash_WordRead(Font_Buf,addr,((Font_W*Font_H)/8));
}