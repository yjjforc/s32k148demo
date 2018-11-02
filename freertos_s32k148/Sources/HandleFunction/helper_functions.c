/*
 * Copyright (c) 2015 - 2016 , Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "helper_functions.h"
#include <stdint.h>
#include <stdbool.h>

/***********************************
 * @brief: Convert a float to null terminated char array
 * @param srcValue:  pointer to the source float value
 * @param destStr:   pointer to the destination string
 * @param maxLen:    maximum lenght of the string
 ***********************************/
void
floatToStr (const float *srcValue, char *destStr, uint8_t maxLen)
{
  uint8_t i, lessThanOne = 0;
  float tempVal = (*srcValue);
  uint8_t currentVal;

  if (tempVal < 0)
    {
      tempVal *= -1;
      *destStr = '-';
      destStr++;
    }
  for (i = 0; i < maxLen; i++)
    {
      currentVal = (uint8_t) (tempVal);
      *destStr = currentVal + 48;
      destStr++;
      tempVal -= currentVal;
      if ((tempVal < 1) && (lessThanOne == 0))
	{
	  *destStr = '.';
	  destStr++;
	  lessThanOne = 1;
	}
      tempVal *= 10;
    }
  *destStr = 0;
}

/***********************************
 * @brief: Wait for a number of cycles
 * @param nbOfCycles is number of cycles to be waited for
 ***********************************/
void
delayCycles (uint32_t nbOfCycles)
{
  volatile uint32_t i = nbOfCycles;
  while (i--)
    ;
}

/* Method that enables on-device FPU
 * param: 	None
 * return:	None
 */

void enableFPU(void)
{
	/* Enable FPU � set both CPACR[CP11] and CPACR[CP10] to Full Access - 0b11 */
	S32_SCB->CPACR |= (S32_SCB_CPACR_CP10_MASK | S32_SCB_CPACR_CP11_MASK);
}
