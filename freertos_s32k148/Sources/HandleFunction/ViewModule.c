/**
  ******************************************************************************
  * File Name          : ViewModule.c
  * Description        : Code for 3-dragon 显示模块 
  * Author             : YJJ
  * Date               : 2018.9.5
  ******************************************************************************
  */
 
#include "ViewModule.h"
#include "LinkList.h"
#include "PicAreaHandle.h"
#include "SendimageData.h"
#include "UnicodeFont.h"
/* ---------------------------------------------------------------------------*/
/*
*数据显示流模块 结合Viewlist 与CanList 显示数据 
*功能点：
*		2.接收到CAN数据读出ID号(图标只需要ID号子模块的数据项编号，数字显示 需要传入具体的值 汉字显示则需要传入具体的unicode空间)
*		3.发送读出的数据 图标数字 汉字多显 
*------------------------------------------------------------------------------*/
/**
  * @brief UpMachine_CANData_Analyses 
  * @param Viewlist:显示模块嵌套链表
  * @param Canlist: Can模块嵌套链表
  * @retval NULL
  */
 /* -----*/

void ViewShow(void *Viewlist,void *CanList)
{
   //接收CAN模块接受的数据数据参数（接收方式，队列接收CANlistID号，或则直接set进来）
   //图标显示
   //遍历链表
  uint16_t CAN_Rece_ID=0;//CAN接收子模块的ID
  uint16_t CAN_Rece_Item=0;//CAN接收子模块的数据项的数据大小
   
 	SubCanModuleHeader *Psubcanmodhead =	(SubCanModuleHeader *)GetPosValue(CanList, CAN_Rece_ID);//得到此节点在CAN链表的数据
 	SubViewModuleHeader *Psubviemodhead=	(SubViewModuleHeader *)GetPosValue(Viewlist, CAN_Rece_ID);//得到此节点在view链表的数据

 	if (Psubcanmodhead->DataMode==1)//图标显示
 	{
 		SubModulePicDataContent *viewpicdatacon=(SubModulePicDataContent *)GetPosValue(Psubviemodhead->Contlist,CAN_Rece_Item);
 		SubModuleCANPicDataContent *canpicnumdacon=(SubModuleCANPicDataContent *)GetPosValue(Psubcanmodhead->Contlist,CAN_Rece_Item);

 		View_pic_Handle(Psubviemodhead,Psubcanmodhead,viewpicdatacon,canpicnumdacon);//显示处理

 	}
 	else if (Psubcanmodhead->DataMode==2)//数字显示
 	{
 		SubModuleNumFontDataContent *Pviewnumdatacon=(SubModuleNumFontDataContent *)GetPosValue(Psubviemodhead->Contlist,CAN_Rece_Item);
 		SubModuleCANNumDataContent *Pcannumdacon=(SubModuleCANNumDataContent *)GetPosValue(Psubcanmodhead->Contlist,CAN_Rece_Item);

 		View_Num_Handle(Psubviemodhead,Psubcanmodhead,Pviewnumdatacon,Pcannumdacon);
 	}
 	else if (Psubcanmodhead->DataMode==3)//汉字显示
 	{
 		SubModuleNumFontDataContent *Pviewnumdatacon=(SubModuleNumFontDataContent *)GetPosValue(Psubviemodhead->Contlist,CAN_Rece_Item);
 		SubModuleCANFontDataContent *Pcanfontdatacon=(SubModuleCANFontDataContent *)GetPosValue(Psubcanmodhead->Contlist,CAN_Rece_Item);

 		View_Font_Handle(Psubviemodhead,Psubcanmodhead,Pviewnumdatacon,Pcanfontdatacon);
 	}
 	else
 	{
 		Log("ViewShow have no DataMode  error\r\n");
 	}

   	 
  
   //数字显示
   //汉字显示
   

}

/* ---------------------------------------------------------------------------*/
/*
*图标 数字 汉字显示
*功能点：
*		1.依据子模块数据结构 做处理
*		
*------------------------------------------------------------------------------*/
/**
  * @brief View_pic_Handle 
  * @param submodhead:Can子模块指针
  * @retval NULL
  */
 /* -----*/
/*---------------------------------------------------------------------------*/
void View_pic_Handle(SubViewModuleHeader *subviemodhead,\
					 SubCanModuleHeader *subcanmodhead,\
					 SubModulePicDataContent *viewpicdatacon,\
					 SubModuleCANPicDataContent *cannpicdacon)//图标显示处理函数
{
	//擦除显示区域的部分
	PixelRGB888alp Picpixel;
	AreaSize areasize;
	Picpixel.color1=0x00;
	Picpixel.color2=0x00;
	Picpixel.color3=0x00;
	Picpixel.alpha_channel=0x00;
    areasize.MaxX=subviemodhead->MaxX;
    areasize.MaxY=subviemodhead->MaxY;
    areasize.MaxW=subviemodhead->MaxW;
    areasize.MaxH=subviemodhead->MaxH;

	PicAreaFill(&Picpixel,&areasize);//全色填充 清空区域


	//读取需要显示的子模块数据项的地址 传输
	SendimageHeader sendimageheader;

	sendimageheader.Start_Addr=viewpicdatacon->Fpga_Addr;//起始地址
	sendimageheader.pic_X=viewpicdatacon->StartX;//起始x
	sendimageheader.pic_Y=viewpicdatacon->StartY;//起始y
	sendimageheader.pic_W=viewpicdatacon->StartW;//宽度
	sendimageheader.pic_H=viewpicdatacon->StartH;//高度
	sendimageheader.Pic_Len=viewpicdatacon->StartW*viewpicdatacon->StartH;//像素个数

	SViewModeOne(&sendimageheader);//发送需要显示的数据
	
}
/**
  * @brief View_Num_Handle 
  * @param submodhead:Can子模块指针
  * @retval NULL
  */
 /* -----*/
/*---------------------------------------------------------------------------*/
void View_Num_Handle(SubViewModuleHeader *subviemodhead,\
					 SubCanModuleHeader *subcanmodhead,\
					 SubModuleNumFontDataContent *viewnumdatacon,\
					 SubModuleCANNumDataContent *cannumdacon)//数字显示函数
{
	uint32_t  Number={0};//需要显示的数字
	uint16_t  Num_W=0;
	uint16_t  Num_H=0;
	uint32_t  NumAddr=0;
  uint8_t Num_kind=0;
	SendimageHeader sendimageheader;//发送数据参数结构体
	//擦除显示区域的部分
	PixelRGB888alp Picpixel;
	AreaSize areasize;
	Picpixel.color1=0x00;
	Picpixel.color2=0x00;
	Picpixel.color3=0x00;
	Picpixel.alpha_channel=0x00;
    areasize.MaxX=subviemodhead->MaxX;
    areasize.MaxY=subviemodhead->MaxY;
    areasize.MaxW=subviemodhead->MaxW;
    areasize.MaxH=subviemodhead->MaxH;

	PicAreaFill(&Picpixel,&areasize);//全色填充 清空区域

	//填充数字首先读出要传输的数字的值
	//采用个性数字还是字库中的数字 
    Number=cannumdacon->Data;
      if (viewnumdatacon->Num_Kind)//采用了什么数字种类
	{
		//mfile_header.numkindbuf[viewnumdatacon.Num_Kind]
  //载入相应的数字库  
    
  
	}
	else//直接发送搬运命令 通过CAN item数据项ID号已经确定了数字是几位数 直接搬运 
	{
    Num_kind=viewnumdatacon->Num_Kind;
		NumAddr=numberkind[Num_kind].NumFPGA_Addr;
		Num_W=numberkind[Num_kind].Num_W;
		Num_H=numberkind[Num_kind].Num_H;
    //读取需要显示的子模块数据项的地址 传输
		switch(subcanmodhead->Effective_Data)//数据项标号就是数据的位数 选择直接发送
		{
			case 1:
			{

			  //需要选择对应数字图片的内存地址的位置
				sendimageheader.Start_Addr=NumAddr;//起始地址:副内存地址 数字的位置
				sendimageheader.pic_X=viewnumdatacon->x;//起始x
				sendimageheader.pic_Y=viewnumdatacon->y;//起始y
				sendimageheader.pic_W=Num_W;//宽度
				sendimageheader.pic_H=Num_H;//高度
				sendimageheader.Pic_Len=Num_W*Num_H;//像素个数i

				SViewModeOne(&sendimageheader);//发送需要显示的数据
			}break;
			case 2:
			{
				//需要选择对应数字图片的内存地址的位置
				sendimageheader.Start_Addr=Num_Addr_Choose(Number/10,Num_W,Num_H,Num_kind);//起始地址:副内存地址
				sendimageheader.pic_X=viewnumdatacon->x;//起始x
				sendimageheader.pic_Y=viewnumdatacon->y;//起始y
				sendimageheader.pic_W=Num_W;//宽度
				sendimageheader.pic_H=Num_H;//高度
				sendimageheader.Pic_Len=Num_W*Num_H;//像素个数

				SViewModeOne(&sendimageheader);//发送需要显示的数据
				//需要选择对应数字图片的内存地址的位置
				sendimageheader.Start_Addr=Num_Addr_Choose(Number%10,Num_W,Num_H,Num_kind);//起始地址:副内存地址
				sendimageheader.pic_X=viewnumdatacon->x+Num_W;//起始x
				sendimageheader.pic_Y=viewnumdatacon->y;//起始y
				sendimageheader.pic_W=Num_W;//宽度
				sendimageheader.pic_H=Num_H;//高度
				sendimageheader.Pic_Len=Num_W*Num_H;//像素个数

				SViewModeOne(&sendimageheader);//发送需要显示的数据
			}break;
			case 3:
			{
				//需要选择对应数字图片的内存地址的位置
				sendimageheader.Start_Addr=Num_Addr_Choose(Number/100,Num_W,Num_H,Num_kind);//起始地址:副内存地址
				sendimageheader.pic_X=viewnumdatacon->x;//起始x
				sendimageheader.pic_Y=viewnumdatacon->y;//起始y
				sendimageheader.pic_W=Num_W;//宽度
				sendimageheader.pic_H=Num_H;//高度
				sendimageheader.Pic_Len=Num_W*Num_H;//像素个数

				SViewModeOne(&sendimageheader);//发送需要显示的数据
				//需要选择对应数字图片的内存地址的位置
				sendimageheader.Start_Addr=Num_Addr_Choose(Number%100/10,Num_W,Num_H,Num_kind);//起始地址:副内存地址
				sendimageheader.pic_X=viewnumdatacon->x+Num_W;//起始x
				sendimageheader.pic_Y=viewnumdatacon->y;//起始y
				sendimageheader.pic_W=Num_W;//宽度
				sendimageheader.pic_H=Num_H;//高度
				sendimageheader.Pic_Len=Num_W*Num_H;//像素个数

				SViewModeOne(&sendimageheader);//发送需要显示的数据
				//需要选择对应数字图片的内存地址的位置
				sendimageheader.Start_Addr=Num_Addr_Choose(Number%100/10%10,Num_W,Num_H,Num_kind);//起始地址:副内存地址
				sendimageheader.pic_X=viewnumdatacon->x+(2*Num_W);//起始x
				sendimageheader.pic_Y=viewnumdatacon->y;//起始y
				sendimageheader.pic_W=Num_W;//宽度
				sendimageheader.pic_H=Num_H;//高度
				sendimageheader.Pic_Len=Num_W*Num_H;//像素个数

				SViewModeOne(&sendimageheader);//发送需要显示的数据
			}break;
			case 4:
			{
				//需要选择对应数字图片的内存地址的位置
				sendimageheader.Start_Addr=Num_Addr_Choose(Number/1000,Num_W,Num_H,Num_kind);//起始地址:副内存地址
				sendimageheader.pic_X=viewnumdatacon->x;//起始x
				sendimageheader.pic_Y=viewnumdatacon->y;//起始y
				sendimageheader.pic_W=Num_W;//宽度
				sendimageheader.pic_H=Num_H;//高度
				sendimageheader.Pic_Len=Num_W*Num_H;//像素个数

				SViewModeOne(&sendimageheader);//发送需要显示的数据
				//需要选择对应数字图片的内存地址的位置
				sendimageheader.Start_Addr=Num_Addr_Choose(Number%1000/100,Num_W,Num_H,Num_kind);//起始地址:副内存地址
				sendimageheader.pic_X=viewnumdatacon->x+Num_W;//起始x
				sendimageheader.pic_Y=viewnumdatacon->y;//起始y
				sendimageheader.pic_W=Num_W;//宽度
				sendimageheader.pic_H=Num_H;//高度
				sendimageheader.Pic_Len=Num_W*Num_H;//像素个数

				SViewModeOne(&sendimageheader);//发送需要显示的数据
				//需要选择对应数字图片的内存地址的位置
				sendimageheader.Start_Addr=Num_Addr_Choose(Number%1000%100/10,Num_W,Num_H,Num_kind);//起始地址:副内存地址
				sendimageheader.pic_X=viewnumdatacon->x+(2*Num_W);//起始x
				sendimageheader.pic_Y=viewnumdatacon->y;//起始y
				sendimageheader.pic_W=Num_W;//宽度
				sendimageheader.pic_H=Num_H;//高度
				sendimageheader.Pic_Len=Num_W*Num_H;//像素个数

				SViewModeOne(&sendimageheader);//发送需要显示的数据
				//需要选择对应数字图片的内存地址的位置
				sendimageheader.Start_Addr=Num_Addr_Choose(Number%1000%100/10%10,Num_W,Num_H,Num_kind);//起始地址:副内存地址
				sendimageheader.pic_X=viewnumdatacon->x+(3*Num_W);//起始x
				sendimageheader.pic_Y=viewnumdatacon->y;//起始y
				sendimageheader.pic_W=Num_W;//宽度
				sendimageheader.pic_H=Num_H;//高度
				sendimageheader.Pic_Len=Num_W*Num_H;//像素个数

				SViewModeOne(&sendimageheader);//发送需要显示的数据
			}break;
			case 5:
			{
				//需要选择对应数字图片的内存地址的位置
				sendimageheader.Start_Addr=Num_Addr_Choose(Number/10000,Num_W,Num_H,Num_kind);//起始地址:副内存地址
				sendimageheader.pic_X=viewnumdatacon->x;//起始x
				sendimageheader.pic_Y=viewnumdatacon->y;//起始y
				sendimageheader.pic_W=Num_W;//宽度
				sendimageheader.pic_H=Num_H;//高度
				sendimageheader.Pic_Len=Num_W*Num_H;//像素个数

				SViewModeOne(&sendimageheader);//发送需要显示的数据
				//需要选择对应数字图片的内存地址的位置
				sendimageheader.Start_Addr=Num_Addr_Choose(Number%10000/1000,Num_W,Num_H,Num_kind);//起始地址:副内存地址
				sendimageheader.pic_X=viewnumdatacon->x+Num_W;//起始x
				sendimageheader.pic_Y=viewnumdatacon->y;//起始y
				sendimageheader.pic_W=Num_W;//宽度
				sendimageheader.pic_H=Num_H;//高度
				sendimageheader.Pic_Len=Num_W*Num_H;//像素个数

				SViewModeOne(&sendimageheader);//发送需要显示的数据
				//需要选择对应数字图片的内存地址的位置
				sendimageheader.Start_Addr=Num_Addr_Choose(Number%10000%1000/100,Num_W,Num_H,Num_kind);//起始地址:副内存地址
				sendimageheader.pic_X=viewnumdatacon->x+(2*Num_W);//起始x
				sendimageheader.pic_Y=viewnumdatacon->y;//起始y
				sendimageheader.pic_W=Num_W;//宽度
				sendimageheader.pic_H=Num_H;//高度
				sendimageheader.Pic_Len=Num_W*Num_H;//像素个数

				SViewModeOne(&sendimageheader);//发送需要显示的数据
				//需要选择对应数字图片的内存地址的位置
				sendimageheader.Start_Addr=Num_Addr_Choose(Number%10000%1000%100/10,Num_W,Num_H,Num_kind);//起始地址:副内存地址
				sendimageheader.pic_X=viewnumdatacon->x+(3*Num_W);//起始x
				sendimageheader.pic_Y=viewnumdatacon->y;//起始y
				sendimageheader.pic_W=Num_W;//宽度
				sendimageheader.pic_H=Num_H;//高度
				sendimageheader.Pic_Len=Num_W*Num_H;//像素个数

				SViewModeOne(&sendimageheader);//发送需要显示的数据
				//需要选择对应数字图片的内存地址的位置
				sendimageheader.Start_Addr=Num_Addr_Choose(Number%10000%1000%100/10%10,Num_W,Num_H,Num_kind);//起始地址:副内存地址
				sendimageheader.pic_X=viewnumdatacon->x+(4*Num_W);//起始x
				sendimageheader.pic_Y=viewnumdatacon->y;//起始y
				sendimageheader.pic_W=Num_W;//宽度
				sendimageheader.pic_H=Num_H;//高度
				sendimageheader.Pic_Len=Num_W*Num_H;//像素个数

				SViewModeOne(&sendimageheader);//发送需要显示的数据
			}break;
			default:{}break;
		}
  	}
}


/**
  * @brief View_Font_Handle 
  * @param submodhead:Can子模块指针
  * @retval NULL
  */
 /* -----*/
/*---------------------------------------------------------------------------*/
void View_Font_Handle(SubViewModuleHeader *subviemodhead,\
					 SubCanModuleHeader *subcanmodhead,\
					 SubModuleNumFontDataContent *viewfontdatacon,\
					 SubModuleCANFontDataContent *canfontdatacon)//汉字显示处理函数
{
	
	uint8_t Font_W=0;
	uint8_t Font_H=0;


		//擦除显示区域的部分
	PixelRGB888alp Picpixel;
	AreaSize areasize;
	Picpixel.color1=0x00;
	Picpixel.color2=0x00;
	Picpixel.color3=0x00;
	Picpixel.alpha_channel=0x00;
    areasize.MaxX=subviemodhead->MaxX;
    areasize.MaxY=subviemodhead->MaxY;
    areasize.MaxW=subviemodhead->MaxW;
    areasize.MaxH=subviemodhead->MaxH;

	PicAreaFill(&Picpixel,&areasize);//全色填充 清空区域

	//从can接收的链表中读取需要显示Unicode的值
	//选择字库
	//从字库中读取相应的unicode字库的点阵
	//显示方式将数据全部存入副内存 然后开启显示
	//  
	//viewfontdatacon.Unicode_Kind//汉字的种类

	Font_W=fontkind[viewfontdatacon->Unicode_Kind].Font_W;	
	Font_H=fontkind[viewfontdatacon->Unicode_Kind].Font_H;		

	SViewModeTwo(Font_W*Font_W*subcanmodhead->Effective_Data);//发送字符数组表头 size大小=像素个数*有效数据长度
    for (uint8_t i = 0; i <subcanmodhead->Effective_Data; i++)
    {   
   		Unicode_GetArray(canfontdatacon->unicode_pack[i],Font_W,Font_H,viewfontdatacon);//读取字节数目
   		Font_Analyses(Font_Buf);//发送字符数组
   		//传输到 副内存地址
    }
  SendimageHeader sendimageheader;
	sendimageheader.Start_Addr=Pmfile_header->FPGA_deputy_addr;//NumFlash_deputy_addr+Num_W*Num_H*20 为偏移量地址
	sendimageheader.Pic_Len=subcanmodhead->Effective_Data*Font_W*Font_H;//获取所有需要显示汉字的像素点长度
	sendimageheader.pic_X=viewfontdatacon->x;//其实坐标从起始的解析文件正写入特定的坐标 此坐标是数字在whindow中的显示坐标经过偏移
	sendimageheader.pic_Y=viewfontdatacon->y;
	sendimageheader.pic_W=Font_W*subcanmodhead->Effective_Data;
	sendimageheader.pic_H=Font_H;//横坐标不变 除非超过最大汉字显示范围
	SViewModeOne(&sendimageheader);//发送表头开始搬运
	
}



uint32_t Num_Addr_Choose(uint8_t number,uint16_t w,uint16_t h,uint8_t Num_kind)
{
	uint32_t addr=0;

	switch(number)
	{
		case 0:
		{
		    addr=addr;
		}break;
    case 1:
		{
			addr+=w*h*2+numberkind[Num_kind].NumFPGA_Addr;//数字1的地址
		}break;
    case 2:
		{
			addr+=w*h*4+numberkind[Num_kind].NumFPGA_Addr;//数字1的地址
		}break;
    case 3:
		{
			addr+=w*h*6+numberkind[Num_kind].NumFPGA_Addr;//数字1的地址
		}break;
    case 4:
		{
			addr+=w*h*8+numberkind[Num_kind].NumFPGA_Addr;//数字1的地址
		}break;
    case 5:
		{
			addr+=w*h*10+numberkind[Num_kind].NumFPGA_Addr;//数字1的地址
		}break;
    case 6:
		{
			addr+=w*h*12+numberkind[Num_kind].NumFPGA_Addr;//数字1的地址
		}break;
    case 7:
		{
			addr+=w*h*14+numberkind[Num_kind].NumFPGA_Addr;//数字1的地址
		}break;
    case 8:
		{
			addr+=w*h*16+numberkind[Num_kind].NumFPGA_Addr;//数字1的地址
		}break;
    case 9:
		{
			addr+=w*h*18+numberkind[Num_kind].NumFPGA_Addr;//数字1的地址
		}break;
		default:break;
	}

  	return addr;
}