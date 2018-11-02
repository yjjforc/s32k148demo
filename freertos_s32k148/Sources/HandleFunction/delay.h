#ifndef __DELAY_H
#define __DELAY_H 
#include "device_registers.h" /* include peripheral declarations S32K144 */

void delay_init(uint8_t SYSCLK);
void delay_us(uint32_t nus);
void delay_xms(uint16_t nms);
void delay_ms(uint16_t nms);
#endif
