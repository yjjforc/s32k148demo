/**
  ******************************************************************************
  * File Name          : UpperToUnderUserList.h
  * Description        : Code for 3-dragon 上位机 ->下位机 传输解析数据
  * Author             : YJJ
  * Date               : 2018.9.5
  ******************************************************************************
  */
#include "UpperToUnderUserList.h"
#include "jl_malloc.h"
#include "JL_Data_Structure.h"
#include "LinkList.h"
#include "Flash_Handle.h"
/* ---------------------------------------------------------------------------*/
/*
*解析上位机下发到MCU的显示模块的数据，该数据存储在flash中
*功能点：
*       读取存储在Flash中的数据
*       将信息存储在结构体中
*
* 模块结构：
* |模块表头：
*           1.模块总文件大小32u
*           2.子模块个数16u
*           3.自定义数字的种类数 8u(至多支持10种数字)
*           4.自定义汉字的种类数 8u（至多支持10种汉字）
*           5.数字的内容结构体数组 包括地址宽度高度
*           5.汉字的内容结构体数组 包括地址宽度高度
*--------------------------------------------------------------------------------------------
*解析 图标 显示 数据格式
*
* 
* |         3.子模块数据结构：
*                           1.子模块ID：16u
*                           2.子模块字节总个数：32u
*                           3.数据存储方式：数据存储方式：（图标显示，数字显示，汉字显示）1，2，3     0表示出错返回error
*                           4.子模块数据项个数： 16u
*                           5.子模块显示最大横坐标x：16u
*                           6.子模块显示最大纵坐标y:16u
*                           7.子模块显示最大宽度w：16u
*                           8.子模块显示最大高度h：16u                         
*                           9.子模块数据项内容：
*                                             1.是否显示(1 or 2  1正常显示 2，不显示 0xFF 读取Flash错误  其他提示地址错误)
*                                             2.子模块数据项编号：16u
*                                             3.图片在FPGA内存的地址：32u
*                                             4.图片开始横坐标x:16u
*                                             5.图片开始纵坐标y：16u
*                                             6.图片宽度w：16u
*                                             7.图片高度h:16u
*                                            
*
*---------------------------------------------------------------------------------------------
*解析 汉字数字显示 数据格式
*
* 数字显示子模块数据项存储的内容是第一个字符显示的位置
*  
* 
* |         3.子模块数据结构：
*                           1.子模块ID：16u
*                           2.子模块字节总个数：32u
*                           3.数据存储方式：数据存储方式：（图标显示，数字显示，汉字显示）1，2，3     0表示出错返回error
*                           4.子模块数据项个数： 16u
*                           5.子模块显示最大横坐标x：16u
*                           6.子模块显示最大纵坐标y:16u
*                           7.子模块显示最大宽度w：16u
*                           8.子模块显示最大高度h：16u                         
*                           9.子模块数据项内容：根据数据项个数增加 x1,x2,x3,x4, y1,y2,y3,y4,w1,h1,w2,h2,w3,h3,w4,h4 
*                                              1.子模块数据项编号：16u             
*                                              2.数字字体的种类:8u//初始化配置的时候已经定义好
*                                              3.汉字字体的种类:8u//初始化配置的时候已经定义好
*                                              4.左侧数据横坐标X：16u 
*                                              5.左侧数据纵坐标Y：16u
*                                              
*                                              
*            
*---------------------------------------------------------------------------------------------
*/


User_Header mfile_header;
User_Header *Pmfile_header;

SubViewModuleHeader submoduleheader;
SubViewModuleHeader *Psubmoduleheader;

SubModulePicDataContent submodpicdacon;
SubModulePicDataContent *Psubmodpicdacon;

SubModuleNumFontDataContent submodnumdacon;
SubModuleNumFontDataContent *Psubmodnumdacon;

NumberSubKind numberkind[10];
FontSubKind   fontkind[10];

//MFile_Header mfile_header;
//MFile_Header *Pmfile_header;
//
//SubViewModuleHeader submoduleheader;
//SubViewModuleHeader *Psubmoduleheader;
//
//
//SubModulePicDataContent submodpicdacon;
//SubModulePicDataContent *Psubmodpicdacon;
//
//SubModuleNumFontDataContent submodnumdacon;
//SubModuleNumFontDataContent *Psubmodnumdacon;
//
//NumberKind numberkind[10];
//FontKind   fontkind[10];

/* ---------------------------------------------------------------------------*/
/* *解析上位机下发到MCU的显示模块的数据，该数据存储在flash中-------------*/
/**
  * @brief UpMachine_Display_Analyses 
  * @param void   
  * @retval NULL
  */
 /* -----*/
    
 uint8_t testr_buf[4096]={0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0xA0,
                          0X00,0X00,0x20>>8,0x20,0x20>>8,0x20,0x00,0xA0,0X00,
                          0X00,0x20>>8,0x20,0x20>>8,0x20,0x00,0x01,0x00,0x00,
                          0x00,0x00,1,0x00,0x02,0x00,0x00,0x00,0x00,0x00,32,0x00,
                          25,0x01,0x00,0x00,0X00,0X17,0X76,0X40,0x00,0x00,0x00,
                          0x00,0x00,0x32,0x00,0x25,2,0x01,0X00,0X17,0X83,0X40,
                          0x00,0x00,0x00,0x00,0x00,32,0x00,25};

void UpMachine_View_Analyses(void *viewhead_list)
{ 
  uint16_t SubmodCount=0;//遍历读取所有子模块的计数变量
  uint32_t module_pos=0;//记录子模块现在的位置

  viewhead_list = InitLinkList();
  void *viewdata_list= InitLinkList();
  
  Pmfile_header=&mfile_header;//实例化指针
  Psubmoduleheader=&submoduleheader;
  Psubmodpicdacon=&submodpicdacon;
  Psubmodnumdacon=&submodnumdacon;
  

  //读取表头信息
  //申请内存表头数据
  module_pos+=ViewModule_Position;
  
  memcpy((uint8_t*)Pmfile_header,testr_buf+module_pos,12);
  module_pos+=12;
  //Flash_WordRead((uint8_t*)Pmfile_header,module_pos,12);module_pos+=12;//读取模块表头文件前12个字节

//  for (uint8_t i = 0; i <Pmfile_header->NumKindsize; i++)
//  {
//    Pmfile_header->Pnumkindbuf=&numberkind[i];
//    Flash_WordRead(((uint8_t*)Pmfile_header->Pnumkindbuf),module_pos,8);//读取模块一个数字种类的具体参数放入结构体数组
//
//    module_pos+=8;
//  }
//  for (uint8_t j = 0; j < Pmfile_header->FontKindsize; j++)
//  {
//    Pmfile_header->Pfontkindbuf=&fontkind[j];
//    Flash_WordRead((uint8_t*)Pmfile_header->Pfontkindbuf,module_pos,8);//读取模块一个数字种类的具体参数放入结构体数组
//    module_pos+=8;
//  }
  for(SubmodCount=0;SubmodCount<Pmfile_header->SubModuleSum;SubmodCount++)//遍历读取所有子模块的数据
  {
    //先获取子模块表头的信息
    Flash_WordRead((uint8_t*)Psubmoduleheader,module_pos,SubViewModuleHeader_SIZE);module_pos+=SubViewModuleHeader_SIZE;
    //如果是1 图片显示
    if (Psubmoduleheader->DataMode==1)
    {

      for (uint8_t i = 0; i < Psubmoduleheader->Data_ItemSize; i++)//遍历写入子模块中的数据项 嵌套链表的形式
      {
        Flash_WordRead((uint8_t*)Psubmodpicdacon,module_pos,SubModulePicDataContent_SIZE);//读取到子模块数据内容
        module_pos+=SubModulePicDataContent_SIZE;
        InsertTailLinkList(viewdata_list,Psubmodpicdacon);//将获取到的数据项内容尾插到list中
      }
    }
    //如果是1 数字汉字显示
    else if (Psubmoduleheader->DataMode==2)
    {
      for (uint8_t i = 0; i < Psubmoduleheader->Data_ItemSize; i++)//遍历写入子模块中的数据项 嵌套链表的形式
      {
        Flash_WordRead((uint8_t*)Psubmodnumdacon,module_pos,SubModuleNumDataContent_SIZE);//读取到子模块数据内容
        module_pos+=SubModuleNumDataContent_SIZE;
        InsertTailLinkList(viewdata_list,Psubmodnumdacon);//将获取到的数据项内容尾插到list中
      }
    }
    else
    {
      printf("have the error for VIEWDataMode\n");
    }
     Psubmoduleheader->data=viewdata_list;//将子模块数据项链表放入子模块表头中
    InsertTailLinkList(viewhead_list,Psubmoduleheader);//将获取到的数据链表尾插到list中
  }
}



