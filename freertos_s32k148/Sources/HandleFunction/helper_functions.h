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

#ifndef SOURCES_HELPER_FUNCTIONS_H_
#define SOURCES_HELPER_FUNCTIONS_H_

#include "Cpu.h"
#include "device_registers.h"
#include <stdint.h>

/***********************************
 * @brief: Convert a float to null terminated char array
 * @param srcValue:  pointer to the source float value
 * @param destStr:   pointer to the destination string
 * @param maxLen:    maximum lenght of the string
 ***********************************/
void floatToStr(const float *srcValue, char *destStr, uint8_t maxLen);

/***********************************
 * @brief: Wait for a number of cycles
 * @param nbOfCycles is number of cycles to be waited for
 ***********************************/
void delayCycles(uint32_t nbOfCycles);

/* Method that enables on-device FPU
 * param: 	None
 * return:	None
 */

void enableFPU(void);

#endif /* SOURCES_HELPER_FUNCTIONS_H_ */
