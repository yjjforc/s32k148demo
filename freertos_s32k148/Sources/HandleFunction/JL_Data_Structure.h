/**
  ******************************************************************************
  * File Name          : JL_Data_Structure.h
  * Description        : Code for 3-dragon 京龙数据类型
  * Author             : YJJ
  * Date               : 2018.9.5
  ******************************************************************************
  */
 
#ifndef JL_DATAT_STRUCTURE_H
#define JL_DATAT_STRUCTURE_H

#include <stdint.h>

#include "Log.h"
/* 显示模块------------------------------------------------------------------------------*/




typedef struct 
{
  uint32_t NumFPGA_Addr;//自定义数字的在flash中的存储地址：32u (图标显示此项未用到) 8
  uint16_t Num_W;//自定义数字的宽度
  uint16_t Num_H;//自定义数字的高度
}NumberSubKind;

typedef struct 
{
  uint32_t FontFlash_Addr;//Unicode汉字在flash中的存储地址：32u  8
  uint16_t Font_W;//Unicode汉字的宽度 16u
  uint16_t Font_H;//Unicode汉字的高度 16u
}FontSubKind;

typedef struct 
{
	uint32_t AllFileSize;//模块总文件大小32u              12+4+4+4
	uint16_t SubModuleSum;//子模块个数 16u
  uint32_t FPGA_deputy_addr;//FPGA副内存地址
  uint8_t  NumKindsize;//自定义数字的种类数
  uint8_t  FontKindsize;//汉字的种类数
  void *    Num_list;
  void *    Font_list;
  void *   Sublist;
}User_Header;//模块文件表头;

typedef struct 
{
  uint16_t ID;//子模块ID：16u                   13+4
  uint8_t DataMode;//数据存储方式：（图标显示，数字汉字显示）1 or 2  0表示出错返回error
  uint16_t Data_ItemSize;//子模块数据项个数： 16u
  uint16_t MaxX;//子模块显示最大横坐标x：16u
  uint16_t MaxY;//子模块显示最大纵坐标y:16u
  uint16_t MaxW;//子模块显示最大宽度w：16u
  uint16_t MaxH;//子模块显示最大高度h：16u
  void * Contlist;
}SubShowModuleHeader;//子模块表头

typedef struct 
{
  uint8_t  show_flag;//是否显示(1 or 2  1正常显示 2，不显示 0xFF 读取Flash错误  其他提示地址错误)  15
  uint16_t Data_ItemNumber;//子模块数据项编号：16u
  uint32_t Fpga_Addr;//图片在FPGA内存的地址：32u
  uint16_t StartX;//图片开始横坐标x:16u
  uint16_t StartY;//图片开始纵坐标y：16u
  uint16_t StartW;//图片宽度w：16u
  uint16_t StartH;//图片高度h:16u
}SubModuleShowPicDataContent;//子模块数据内容


typedef struct 
{
  uint16_t Data_ItemNumber;//子模块数据项编号：16u   8     
  uint8_t Num_Kind;//数字字体的种类
  uint8_t Unicode_Kind;//汉字字体的种类
  uint16_t x;
  uint16_t y;

}SubModuleShowNumFontDataContent;//xywh


typedef struct 
{
  uint16_t Data_ItemNumber;
  uint16_t X;
  uint16_t Y;
  uint16_t W;
  uint16_t H; 
}SubModuleShowAnimatedDataContent;//动画显示

typedef struct 
{
  uint16_t Data_ItemNumber;
  uint8_t  show_flag;//是否显示(1 or 2  1正常显示 2，不显示 0xFF 读取Flash错误  其他提示地址错误)  15
  uint32_t Max_Value;//进度条百分之百对应的值
  uint32_t color;//进度条颜色值
  uint8_t  unit_W;//单元大小  4 16 64
  uint8_t  unit_H;//单元列高 单位是单元大小 
  uint16_t X_S;//起始x坐标
  uint16_t Y_S;//起始Y坐标
  uint16_t  X_F;//结束坐标
  uint16_t  Y_F;//结束坐标
  uint16_t BarMoreArea;//进度条多出部分长度直线部分
}SubModuleShowBarDataContent;//进度条显示

 
/* 显示模块------------------------------------------------------------------------------*/

/* CAN显示-------------------------------------------------------------------------------*/

typedef struct 
{
  uint16_t SubModuleSum;//子模块个数 16u    2+4
  void *Sublist;
}CanModuleHeader;

typedef struct 
{
  uint8_t ID;//子模块ID：16u    11+4
  uint8_t  Data_Pos;//数据在CANID中的位置  
  uint8_t DataMode;//数据传输方式//（图标显示 数字显示，汉字显示 1 or 2 or 3）
  uint8_t Effective_Data;//子模块有效数据的大小:8u 代表在CANID中的数据长度(缺省值为1）
  uint16_t Data_ItemSize;//子模块数据项个数： 16u
  uint32_t CANID;//CANID:32u(此数据项在CANID中的位置) 
  void *Contlist;
}SubCanModuleHeader;

typedef struct 
{
  uint16_t Data_ItemNumber;//子模块数据项编号：16u   2

}SubModuleCANPicDataContent;

typedef struct 
{
  uint32_t Data; //数据值:16u (固定单位字节，例如 8 8个字节的数据)   4
}SubModuleCANNumDataContent;



typedef struct 
{
  uint16_t NData_len;//存储空间的有效区域长度  42
  uint16_t RData_len;//reg数组的个数
  uint16_t unicode_pack[20];//Unicode存储空间 (开辟空间)
  uint16_t unicode_reg_pack[20];//固定显示的字符
}SubModuleCANFontDataContent;


//管理所有配置结构
typedef struct     
{
  User_Header *UHead1;
  User_Header *UHead2;
  User_Header *UHead3;
  CanModuleHeader *CHead1;
  CanModuleHeader *CHead2;  
  CanModuleHeader *CHead3;
  uint8_t ShowLayer;
}Head;


extern Head AHead;


typedef struct
{
  flexcan_msgbuff_t recemsg1;
  flexcan_msgbuff_t recemsg2;
  flexcan_msgbuff_t recemsg3;
//  flexcan_msgbuff_t recemsg4;
//  flexcan_msgbuff_t recemsg5;
//  flexcan_msgbuff_t recemsg6;
//  flexcan_msgbuff_t recemsg7;  

}CRece_BUFF;

extern CRece_BUFF CRBuffer;








//发送函数 共用
void Spi_SendData(const uint8_t * send_buf,uint8_t * rec_buf,uint16_t size);



 #endif