/**
  ******************************************************************************
  * File Name          : Flash_Handle.c
  * Description        : Code for 3-dragon Flash handle memory the word
  * Author             : YJJ
  * Date               : 2018.6.12
  ******************************************************************************
  */
 

 /* Includes -------------------------------------------------------------------*/
#include "Flash_Handle.h"
#include "w25qxx.h"
/* Includes -------------------------------------------------------------------*/



/* 将接收到的表格信息存储到fash中 ---------------------------------------------*/
/* receive data to the flash  -----------------------------*/
/**
  * @brief Flash_WordWrite 
  * @param PData: 数组指针   
    
  * @retval NULL
  */  
void Flash_WordWrite(uint8_t* pBuffer,\
																		uint32_t WriteAddr,\
																				uint16_t NumByteToWrite)  
{
	 W25QXX_Write(pBuffer,WriteAddr,NumByteToWrite) ; 
}

/* 将存储到flash中的表格信息读取出来 ------------------------------------------*/
/* Read the data in flash  ------------------------------*/
/**
  * @brief Flash_WordRead 
  * @param PData: 接收数组指针  
  * @param ReadAddr：读取的地址信息
  * @param NumByteToRead：读取的长度
  * @retval NULL
  */  
void Flash_WordRead(uint8_t *pBuffer,\
																	 uint32_t ReadAddr,\
																		    uint16_t NumByteToRead)
{
	W25QXX_Read(pBuffer,ReadAddr,NumByteToRead);
	
}


void Flash_Handle_Init(void)
{
  W25QXX_Init();//wq25qxx初始化
}
