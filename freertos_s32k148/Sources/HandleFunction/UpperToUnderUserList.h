/**
  ******************************************************************************
  * File Name          : UpperToUnderUserList.h
  * Description        : Code for 3-dragon 上位机 ->下位机 传输解析数据
  * Author             : YJJ
  * Date               : 2018.9.5
  ******************************************************************************
  */
 
#ifndef __UPPERTOUNDERUSERLIST_H
#define __UPPERTOUNDERUSERLIST_H

#include <stdint.h>
/*
速度解决方案：

上位机发送模块A（下位机需要将模块A显示为速度信息）

例如：上位机下发编号A 为速度显示模块 包含信息（最大值，最小值，显示区域 各位的显示位置，在FPGA的DDR地址）

车辆设备CAN主机发送到下位机 发送 速度CAN帧 下位机解析 解析速度 

速度解析
例如：上位机发送CAN解析文件 （CAN帧的STD或者EXD，速度所在的位置，所占字节大小） 
Vehicle_Data->Speed_Infor.FrameName
Vehicle_Data->Speed_Infor.position
Vehicle_Date->Speed_Infor.ByteSize
流程：主机发送过来CAN帧 STD=0x00001,数据长度为8,第一次遍历查表，找到STD的个数，第二次遍历，获取各元素的位置信息，根据解析模块
     将获取到的数据填入速度显示模块，CPU发送速度的地址到FPGA 一次显示完毕


所需的各部分为：
1.显示模块文件
2.CAN解析文件
3.各逻辑函数：
	包括：
		1.解析显示模块 放入RAM    View
		2.解析CAN解析文件，放入RAM 
		3.解析CAN帧 各显示模块的解析方式不同细分为各子函数 例如：接收到左转向标志，则填写到方向模块显示函数，
		4.数据传入显示模块函数 各显示模块函数不同 速度显示模块 道路信息显示
 */


/* -----
*解析上位机下发到MCU的显示模块的数据，该数据存储在flash中
*功能点：
*       读取存储在Flash中的数据
*       将信息存储在结构体中
*/

void UpMachine_View_Analyses(void *viewhead_list);
#endif