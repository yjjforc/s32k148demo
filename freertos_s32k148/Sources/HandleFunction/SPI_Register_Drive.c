/**
******************************************************************************
* File Name          : SPI_Register_Drive.c
* Description        : Code for 3-dragon S32K
* Author             : YJJ
* Version            : 0.0.1
* Date               : 2018.7.9
******************************************************************************
*/
/* Include -----------------------------------------------------------------*/
#include "SPI_Register_Drive.h"
#include "flashread.h"
#include "JL_Data_Structure.h"


/* LPSPI.c              (c) 2016 NXP
 * Descriptions: S32K144 FlexCAN example functions.
 * May 31 2016 S. Mihalik: Initial version.
 * Oct 31 2016 SM - adjust PRESCALE for 40 MHz SPLLDIV2_CLK
 * Nov 02 2016 SM - cleared flags in transmit, receive functions
 */



void LPSPI1_init_master(void) {
  PCC->PCCn[PCC_LPSPI1_INDEX] = 0;          /* Disable clocks to modify PCS ( default) */
  PCC->PCCn[PCC_LPSPI1_INDEX] =0x81000000 ; /* Enable PCS=SPLL_DIV2 (40 MHz func'l clock) */

  LPSPI1->CR    = 0x00000000;   /* Disable module for configuration */
  LPSPI1->IER   = 0x00000000;   /* Interrupts not used */
  LPSPI1->DER   = 0x00000000;   /* DMA not used */
  LPSPI1->CFGR0 = 0x00000000;   /* Defaults: */
                                /* RDM0=0: rec'd data to FIFO as normal */
                                /* CIRFIFO=0; Circular FIFO is disabled */
                                /* HRSEL, HRPOL, HREN=0: Host request disabled */
  LPSPI1->CFGR1 = 0x00000001;   /* Configurations: master mode*/
                                /* PCSCFG=0: PCS[3:2] are enabled */
                                /* OUTCFG=0: Output data retains last value when CS negated */
                                /* PINCFG=0: SIN is input, SOUT is output */
                                /* MATCFG=0: Match disabled */
                                /* PCSPOL=0: PCS is active low */
                                /* NOSTALL=0: Stall if Tx FIFO empty or Rx FIFO full */
                                /* AUTOPCS=0: does not apply for master mode */
                                /* SAMPLE=0: input data sampled on SCK edge */
                                /* MASTER=1: Master mode */
  LPSPI1->TCR   = 0x1300000F;/*0x5300000F*/  /* Transmit cmd: PCS3, 16 bits, prescale func'l clk by 4, etc*/
                                /* CPOL=0: SCK inactive state is low */
                                /* CPHA=1: Change data on SCK lead'g, capture on trail'g edge*/
                                /* PRESCALE=2: Functional clock divided by 2**2 = 4 */
                                /* PCS=3: Transfer using PCS3 */
                                /* LSBF=0: Data is transfered MSB first */
                                /* BYSW=0: Byte swap disabled */
                                /* CONT, CONTC=0: Continuous transfer disabled */
                                /* RXMSK=0: Normal transfer: rx data stored in rx FIFO */
                                /* TXMSK=0: Normal transfer: data loaded from tx FIFO */
                                /* WIDTH=0: Single bit transfer */
                                /* FRAMESZ=15: # bits in frame = 15+1=16 */
  LPSPI1->CCR   = 0x04090808;   /* Clock dividers based on prescaled func'l clk of 100 nsec */
                                /* SCKPCS=4: SCK to PCS delay = 4+1 = 5 (500 nsec) */
                                /* PCSSCK=4: PCS to SCK delay = 9+1 = 10 (1 usec) */
                                /* DBT=8: Delay between Transfers = 8+2 = 10 (1 usec) */
                                /* SCKDIV=8: SCK divider =8+2 = 10 (1 usec: 1 MHz baud rate) */
  LPSPI1->FCR   = 0x00000003;   /* RXWATER=0: Rx flags set when Rx FIFO >0 */
                                /* TXWATER=3: Tx flags set when Tx FIFO <= 3 */
  LPSPI1->CR    = 0x00000009;   /* Enable module for operation */
                                /* DBGEN=1: module enabled in debug mode */
                                /* DOZEN=0: module enabled in Doze mode */
                                /* RST=0: Master logic not reset */
                                /* MEN=1: Module is enabled */
}

void LPSPI1_transmit_16bits (uint8_t send) {
  while((LPSPI1->SR & LPSPI_SR_TDF_MASK)>>LPSPI_SR_TDF_SHIFT==0);
                                   /* Wait for Tx FIFO available */
  LPSPI1->TDR = send;              /* Transmit data */
  LPSPI1->SR |= LPSPI_SR_TDF_MASK; /* Clear TDF flag */
}

uint8_t LPSPI1_receive_16bits (void) {
	uint8_t receive = 0;

  while((LPSPI1->SR & LPSPI_SR_RDF_MASK)>>LPSPI_SR_RDF_SHIFT==0);
                                   /* Wait at least one RxFIFO entry */
  receive = LPSPI1->RDR;            /* Read received data */
  LPSPI1->SR |= LPSPI_SR_RDF_MASK; /* Clear RDF flag */
  return receive;                  /* Return received data */
}
void SPI_PORT_init (void) 
{
  PCC->PCCn[PCC_PORTB_INDEX ]|=PCC_PCCn_CGC_MASK; /* Enable clock for PORTB */
  PORTB->PCR[14]|=PORT_PCR_MUX(3); /* Port B14: MUX = ALT3, LPSPI1_SCK */
  PORTB->PCR[15]|=PORT_PCR_MUX(3); /* Port B15: MUX = ALT3, LPSPI1_SIN */
  PORTB->PCR[16]|=PORT_PCR_MUX(3); /* Port B16: MUX = ALT3, LPSPI1_SOUT */
  PORTB->PCR[17]|=PORT_PCR_MUX(3); /* Port B17: MUX = ALT3, LPSPI1_PCS3 */
}
/* SPI 鍙戦�佷竴涓瓧鑺傝鍙栦竴涓瓧鑺� ------------------------------------------*/
/* SPI WriteAndRead  ------------------------------*/
/**
  * @brief LPSI1_WriteRead_16bits 
  * @param TxData: 鍙戦�佺殑瀛楄妭
  * @retval 鎺ユ敹鐨勫瓧鑺�
  */  

uint16_t LPSI1_WriteRead_16bits(uint16_t TxData)
{          
  uint8_t RTxData;
  LPSPI1_transmit_16bits(TxData);//浼犺緭涓�涓瓧鑺�16
  RTxData=LPSPI1_receive_16bits();
  return RTxData;//鎺ュ彈涓�涓瓧鑺�
        
}
//鍙戦�佹暟缁勬暟鎹�
uint16_t LPSI1_WriteRead_16bits_BufTrans(uint16_t *pbuf,uint16_t len)
{
  for(uint16_t i_count=0;i_count<len;i_count++)
  {
    LPSI1_WriteRead_16bits(pbuf[i_count]);
  }
  return 0;
}

uint8_t LPSI1_WriteRead_8bits_BufTrans(uint8_t *pbuf,uint16_t len)
{
	  for(uint16_t i_count=0;i_count<len;i_count++)
	  {
		  LPSI1_WriteRead_8bits(pbuf[i_count]);
	  }
	  return 0;
}


uint8_t LPSI1_WriteRead_8bits(uint8_t TxData)
{
  uint8_t RTxData;
  //LPSPI_DRV_MasterTransfer(FLASHREAD, &TxData,&RTxData, 1);
  
  LPSPI1_transmit_16bits(TxData);
  RTxData=LPSPI1_receive_16bits();


	  return RTxData;
}

void Spi_SendData(const uint8_t * send_buf,uint8_t * rec_buf,uint16_t size)
{
  LPSPI_DRV_MasterTransferBlocking(SEND,send_buf,rec_buf, size,50U);
}
