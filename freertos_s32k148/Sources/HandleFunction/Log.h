/**
  ******************************************************************************
  * File Name          : Log.h
  * Description        : Code for 3-dragon Log¥¶¿Ì
  * Author             : YJJ
  * Version            : 0.0.1
  * Date               : 2018.6.12
  ******************************************************************************
  */
#ifndef DEBUG_LOG_H
#define DEBUG_LOG_H



#define DEBUG      1 
#define __LOG__   0
#ifdef __LOG__  
#define Log  cli_echo 
#else  
#endif 

#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
/* USER CODE BEGIN 1 */

#include "Cpu.h"


void cli_echo(const char* fmt, ...);


#endif