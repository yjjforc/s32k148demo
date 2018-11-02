/**
******************************************************************************
* File Name          : SPI_Register_Drive.h
* Description        : Code for 3-dragon S32K 
* Author             : YJJ
* Version            : 0.0.1
* Date               : 2018.7.9
******************************************************************************
*/
#ifndef SPI_REGISTER_DRIVE_H
#define SPI_REGISTER_DRIVE_H

#include "device_registers.h"           // Device header

void LPSPI1_init_master (void);
void LPSPI1_transmit_16bits (uint8_t send);
uint8_t LPSPI1_receive_16bits (void);

uint16_t LPSI1_WriteRead_16bits(uint16_t TxData);

uint8_t LPSI1_WriteRead_8bits(uint8_t TxData);
uint16_t LPSI1_WriteRead_16bits_BufTrans(uint16_t *pbuf,uint16_t len);

uint8_t LPSI1_WriteRead_8bits_BufTrans(uint8_t *pbuf,uint16_t len);


void SPI_PORT_init (void) ;

#endif

