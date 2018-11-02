/**
  ******************************************************************************
  * File Name          : Flash_Handle.h
  * Description        : Code for 3-dragon Flash handle memory the word
  * Author             : YJJ
  * Version            : 0.0.1
  * Date               : 2018.6.12
  ******************************************************************************
  */
 
#ifndef FLASH_HANDLE_H
#define FLASH_HANDLE_H

#include <stdint.h>

void Flash_Handle_Init(void);
void Flash_WordWrite(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void Flash_WordRead(uint8_t *pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);






#endif
