/**
  ******************************************************************************
  * File Name          : UpperToUnderCanList.h
  * Description        : Code for 3-dragon 上位机 ->下位机 传输CAN解析文件
  * Author             : YJJ
  * Date               : 2018.9.5
  ******************************************************************************
  */
 
#include "UpperToUnderCanList.h"
#include "jl_malloc.h"
#include "JL_Data_Structure.h"
#include "LinkList.h"
#include "Flash_Handle.h"
/* ---------------------------------------------------------------------------*/
/*
*解析上位机下发到MCU的CAN模块的数据，该数据存储在flash中
*功能点：
*       读取存储在Flash中的CAN数据
*       将信息存储在结构体中
*       数据ID与显示解析数据的ID是关联的
*
* 模块结构：
* |模块表头：
*           1.模块总文件大小32u
*           2.子模块个数16u
*           3.自定义数字的种类数 8u(至多支持10种数字)
*           4.自定义汉字的种类数 8u（至多支持10种汉字）
*           5.数字的内容结构体数组 包括地址宽度高度
*           5.汉字的内容结构体数组 包括地址宽度高度
*-------------------------------------------------------------------------------------------------
*解析 图标 显示 数据格式
*
* 
* |         3.子模块数据结构：
*                           1.子模块ID：16u
*                           2.子模块字节总个数：32u
*                           3.子模块数据项个数：16u
*                           3.//数据传输方式//（图标显示 数字显示，汉字显示 1 or 2 or 3）
*                           4.CANID:32u(此数据项在CANID中的位置)
*                           5.数据在CAN帧中的位置 8u
*                           5.子模块有效数据的大小:16u 代表一帧CAN数据帧的数据域长度（缺省值为0)(针对数字显示)
*                           6.子模块数据项内容:
*                                             1.子模块数据项编号：16u(图像显示的特定区域的图片种类 传输的时候由程序赋值，缺省值为0)
*--------------------------------------------------------------------------------------------------
*
*解析 数字 显示 数据格式
*
* 
* |         3.子模块数据结构：
*                           1.子模块ID：16u
*                           2.子模块字节总个数：32u
*                           3.子模块数据项个数：16u
*                           3.//数据传输方式//（图标显示 数字显示，汉字显示 1 or 2 or 3）
*                           4.CANID:32u(此数据项在CANID中的位置)
*                           5.数据在CAN帧中的位置 8u
*                           5.子模块有效数据的大小:16u 代表一帧CAN数据帧的数据域长度（缺省值为0)(针对数字显示)
*                           6.子模块数据项内容:
*                                             1.数据的长度 8u
*                                             2.//数据大小:16u (固定单位字节，例如 8 8个字节的数据)（针对数字显示）
*--------------------------------------------------------------------------------------------------
*解析 汉字显示 数据格式  多帧传输
*
* 
* |         3.子模块数据结构：
*                           1.子模块ID：16u
*                           2.子模块字节总个数：32u
*                           3.子模块数据项个数：16u
*                           4.//数据传输方式//（图标显示 数字显示，汉字显示 1 or 2 or 3）
*                           5.CANID:32u(此数据项在CANID中的位置)
*                           4.数据在CAN帧中的位置 8u
*                           6.子模块有效数据的大小:16u 代表当时传输过来的实际长度（缺省值为0）（最大10个汉字）
*                           5.子模块数据项内容：（数据项只有一个,为CAN传输过来的数据）
*                           				  1.子模块数据项编号：16u                                            
*                                             2.Unicode存储空间 (开辟空间 10个字符）
*                                             
*--------------------------------------------------------------------------------------------------*/

CanModuleHeader canModheader;
CanModuleHeader *PcanModheader;

SubCanModuleHeader subcanmodheader;
SubCanModuleHeader *Psubcanmodheader;

SubModuleCANPicDataContent submodcanpicdcon;
SubModuleCANPicDataContent *Psubmodcanpicdcon;

SubModuleCANNumDataContent  submodcannumdcon;
SubModuleCANNumDataContent  *Psubmodcannumdcon;

SubModuleCANFontDataContent submodcanfondacon;
SubModuleCANFontDataContent *Psubmodcanfondacon;

/* ---------------------------------------------------------------------------*/
/* *解析上位机下发到MCU的CAN模块的数据，该数据存储在flash中-------------*/
/**
  * @brief UpMachine_CANData_Analyses 
  * @param void   
  * @retval NULL
  */
 /* -----*/
void UpMachine_CAN_Analyses(void *Canhead_list)     
{
    uint16_t SubmodCount=0;//遍历读取所有子模块的计数变量
    uint32_t module_pos=0;//记录子模块现在的位置


    Canhead_list = InitLinkList();
    void *candata_list = InitLinkList();
    
    PcanModheader=&canModheader;
    Psubcanmodheader= &subcanmodheader;
    Psubmodcanpicdcon=&submodcanpicdcon;
    Psubmodcannumdcon=&submodcannumdcon;
    Psubmodcanfondacon=&submodcanfondacon;
  //读取表头信息
  //申请内存表头数据
  	module_pos+=CanModule_Position;
    Flash_WordRead((uint8_t*)PcanModheader,CanModule_Position,CanModuleHeader_SIZE);//读取模块表头文件
    module_pos+=CanModuleHeader_SIZE;
	for(SubmodCount=0;SubmodCount<PcanModheader->SubModuleSum;SubmodCount++)//遍历读取所有子模块的数据
	{
	//先获取子模块表头的信息
	Flash_WordRead((uint8_t*)Psubcanmodheader,module_pos,SubCanModuleHeader_Size);module_pos+=SubCanModuleHeader_Size;
	//如果是1 图片显示
	if (Psubcanmodheader->DataMode==1)
	{
	  for (uint8_t i = 0; i < Psubcanmodheader->Data_ItemSize; i++)//遍历写入子模块中的数据项 嵌套链表的形式
	  {
	    Flash_WordRead((uint8_t*)Psubmodcanpicdcon,module_pos,SubModuleCANPicDataContent_Size);//读取到子模块数据内容
	    module_pos+=SubModuleCANPicDataContent_Size;
	    InsertTailLinkList(candata_list,Psubmodcanpicdcon);//将获取到的数据项内容尾插到list中
	  } 
	}
	//如果是1 数字显示
	else if (Psubcanmodheader->DataMode==2)
	{
	  for (uint8_t i = 0; i < Psubcanmodheader->Data_ItemSize; i++)//遍历写入子模块中的数据项 嵌套链表的形式
	  {
	    Flash_WordRead((uint8_t*)Psubmodcannumdcon,module_pos,SubModuleCANFontDataContent_Size);//读取到子模块数据内容
	    module_pos+=SubModuleCANFontDataContent_Size;
	    InsertTailLinkList(candata_list,Psubmodcannumdcon);//将获取到的数据项内容尾插到list中
	  }
	}
	else if (Psubcanmodheader->DataMode==3)
	{
		for (uint8_t i = 0; i < Psubcanmodheader->Data_ItemSize; i++)//遍历写入子模块中的数据项 嵌套链表的形式
	  {
	    Flash_WordRead((uint8_t*)Psubmodcanfondacon,module_pos,SubModuleCANFontDataContent_Size);//读取到子模块数据内容
	    module_pos+=SubModuleCANFontDataContent_Size;
	    InsertTailLinkList(candata_list,Psubmodcanfondacon);//将获取到的数据项内容尾插到list中
	  }
	}
	else
	{
	  printf("have the error for CANDataMode\n");
	}
	Psubcanmodheader->data=candata_list;//将子模块数据项链表放入子模块表头中
	InsertTailLinkList(Canhead_list,Psubcanmodheader);//将获取到的数据链表尾插到list中
	}
}

                         
                           				