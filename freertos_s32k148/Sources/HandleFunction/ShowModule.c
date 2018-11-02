/**
  ******************************************************************************
  * File Name          : ShowModule.c
  * Description        : Code for 3-dragon 显示模块 
  * Author             : YJJ
  * Date               : 2018.9.5
  ******************************************************************************
  */
 
#include "ShowModule.h"
#include "LinkList.h"
#include "PicDraw.h"
#include "SendimageData.h"
#include "UnicodeFont.h"
#include "delay.h"
#include "Flash_Handle.h"
#include "LCD_Basic_Elements.h"
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


static uint32_t Mult10(uint32_t n);

extern SPISendBuf SSendBuf;


uint32_t Mult10(uint32_t n)
{
  uint32_t sum=10;
  uint32_t m;
  if(n==0)
  {
    return 1;
  }
  for(m=1;m<n;m++)
  {
    sum*=10;
  }
  return sum;

}

void ViewShow(User_Header * UHead,CanModuleHeader * CHead,uint32_t IDItemValue)
{
   //接收CAN模块接受的数据数据参数（接收方式，队列接收CANlistID号，或则直接set进来）
   //图标显示
   //遍历链表
  
  uint16_t CAN_Rece_ID=0;//CAN接收子模块的ID
  uint16_t CAN_Rece_Item=0;//CAN接收子模块的数据项的数据大小
  
  uint16_t   Tdata=0;
  uint8_t   Tcount=0;
  
  
  CAN_Rece_ID=IDItemValue>>16;
  CAN_Rece_Item=IDItemValue;
  
  
 	
  SubCanModuleHeader *PSCMHead =  (SubCanModuleHeader *)GetPosValue(CHead->Sublist, CAN_Rece_ID);//得到此节点在CAN链表的数据
 	if (PSCMHead->DataMode==1)//图标显示
 	{
  SubShowModuleHeader *PSSMHead=  (SubShowModuleHeader *)GetPosValue(UHead->Sublist, CAN_Rece_ID);//得到此节点在view链表的数据

 		SubModuleShowPicDataContent *PSSPDCont=(SubModuleShowPicDataContent *)GetPosValue(PSSMHead->Contlist,CAN_Rece_Item);
 		SubModuleCANPicDataContent *PSCPDCont=(SubModuleCANPicDataContent *)GetPosValue(PSCMHead->Contlist,0);
      	#ifdef DEBUG
      	Log("show_flag is %d\r\n",PSSPDCont->show_flag);
      	#endif
 		Show_pic_Handle(PSSMHead,PSSPDCont);//显示处理

 	}
 	else if (PSCMHead->DataMode==2)//数字显示
 	{
    SubShowModuleHeader *PSSMHead=  (SubShowModuleHeader *)GetPosValue(UHead->Sublist, CAN_Rece_ID);//得到此节点在view链表的数据
   
    SubModuleCANNumDataContent *PSCNDCont=(SubModuleCANNumDataContent *)GetPosValue(PSCMHead->Contlist,0);
    
    Tdata=PSCNDCont->Data;//计算数据位数
    if(Tdata==0)
    {
      Tcount=1;
    }
    while(Tdata!=0)
    {
      Tdata/=10;
      Tcount++;
    }
    //根据位数来选择，显示的配置数据项
 		SubModuleShowNumFontDataContent *PSSNFDCont=(SubModuleShowNumFontDataContent *)GetPosValue(PSSMHead->Contlist,Tcount);
	    #ifdef DEBUG
	    Log("Num_Kind is %d\r\n",PSSNFDCont->Num_Kind);
	    Log("x is %d\r\n",PSSNFDCont->x);
	    Log("y is %d\r\n",PSSNFDCont->y);         
	    #endif
 		Show_Num_Handle(PSSMHead,PSSNFDCont,PSCNDCont,Tcount,UHead);
 	}
 	else if (PSCMHead->DataMode==3)//汉字显示
 	{
    SubShowModuleHeader *PSSMHead=  (SubShowModuleHeader *)GetPosValue(UHead->Sublist, CAN_Rece_ID);//得到此节点在view链表的数据
     
 		SubModuleShowNumFontDataContent *PSSNFDCont=(SubModuleShowNumFontDataContent *)GetPosValue(PSSMHead->Contlist,CAN_Rece_Item);
 		SubModuleCANFontDataContent *PSCFDCont=(SubModuleCANFontDataContent *)GetPosValue(PSCMHead->Contlist,0);
	    #ifdef DEBUG
	    Log("Unicode_Kind is %d\r\n",PSSNFDCont->Unicode_Kind);
	    Log("x is %d\r\n",PSSNFDCont->x);
	    Log("y is %d\r\n",PSSNFDCont->y);      
	    #endif    
 		Show_Font_Handle(PSSMHead,PSCMHead,PSSNFDCont,PSCFDCont,UHead);
 	}
  else if (PSCMHead->DataMode==4)//动画显示
  {
    SubShowModuleHeader *PSSMHead=  (SubShowModuleHeader *)GetPosValue(UHead->Sublist, CAN_Rece_ID);//得到此节点在view链表的数据
   
    SubModuleShowAnimatedDataContent *PSSMHeadADCont=(SubModuleShowAnimatedDataContent *)GetPosValue(PSSMHead->Contlist,CAN_Rece_Item);
    SubModuleCANNumDataContent *PSCNDCont=(SubModuleCANNumDataContent *)GetPosValue(PSCMHead->Contlist,0);
    #ifdef DEBUG   
    Log("y is %d\r\n",PSSMHeadADCont->X);
    #endif  

    /*动画显示 */   
  }
  else if(PSCMHead->DataMode==5)//进度条显示
  {
    SubShowModuleHeader *PSSMHead=  (SubShowModuleHeader *)GetPosValue(UHead->Sublist, CAN_Rece_ID);//得到此节点在view链表的数据
 
    SubModuleShowBarDataContent *PSSMHeadBDCont=(SubModuleShowBarDataContent *)GetPosValue(PSSMHead->Contlist,0);
    SubModuleCANNumDataContent *PSCNDCont=(SubModuleCANNumDataContent *)GetPosValue(PSCMHead->Contlist,0);

    Show_Bar_Handle(PSSMHead,PSCMHead,PSSMHeadBDCont,PSCNDCont);
  }
  else if(PSCMHead->DataMode==99)//切换屏幕
  {
      
    SubCanModuleHeader *PSCMHead =  (SubCanModuleHeader *)GetPosValue(CHead->Sublist, CAN_Rece_ID);//得到此节点在CAN链表的数据
    SubModuleCANPicDataContent *PSCPDCont=(SubModuleCANPicDataContent *)GetPosValue(PSCMHead->Contlist,0);
    AHead.ShowLayer=PSCPDCont->Data_ItemNumber;
    //清屏函数 然后初始化下一个界面的所有元素
    
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
void Show_pic_Handle(SubShowModuleHeader *PSSMHead,\
					 SubModuleShowPicDataContent *PSSPDCont)//图标显示处理函数
{
//	//擦除显示区域的部分
    Clear_Region(PSSMHead);

	//读取需要显示的子模块数据项的地址 传输
	SendimageHeader sendimageheader;

	sendimageheader.Start_Addr=PSSPDCont->Fpga_Addr;//起始地址
	sendimageheader.pic_X=PSSPDCont->StartX;//起始x
	sendimageheader.pic_Y=PSSPDCont->StartY;//起始y
	sendimageheader.pic_W=PSSPDCont->StartW;//宽度
	sendimageheader.pic_H=PSSPDCont->StartH;//高度
	sendimageheader.Pic_Len=PSSPDCont->StartW*PSSPDCont->StartH;//像素个数

	SViewModeOne(&sendimageheader);//发送需要显示的数据
	
}
/**
  * @brief View_Num_Handle 
  * @param submodhead:Can子模块指针
  * @retval NULL
  */
 /* -----*/
/*---------------------------------------------------------------------------*/
void Show_Num_Handle(SubShowModuleHeader *PSSMHead,\
					 SubModuleShowNumFontDataContent *PSSNFDCont,\
					 SubModuleCANNumDataContent *PSCNDCont,uint8_t Tcount,User_Header *Uhead)//数字显示函数
{
	uint32_t  Number={0};//需要显示的数字
	uint16_t  Num_W=0;
	uint16_t  Num_H=0;
	uint32_t  NumAddr=0;
  uint8_t Num_kind=0;

	//擦除显示区域的部分
    //Clear_Region(PSSMHead);

	//填充数字首先读出要传输的数字的值
	//采用个性数字还是字库中的数字 
     Clear_Region(PSSMHead);
     OSIF_TimeDelay(8);
    Number=PSCNDCont->Data;
      if (PSSNFDCont->Num_Kind>Uhead->NumKindsize)//采用了什么数字种类
	{
		#ifdef DEBUG
		Log("numberkind>Max\r\n");
		#endif
  
	}
  else if(PSSNFDCont->Num_Kind==0)//说明此数字采用unicode汉字的编码来显示
  {
   //确定数字的长度 和信息 用字库中的数字显示则调用unicode值 根据值填充

    //将数字各个位数需要显示的点阵拼接起来发送
    //读取数字的值 数字的unicode是48-57
    //先确定位数
    //计算数据位数 确定显示坐标
    uint32_t  addr=48*128;
    
    SViewModeTwo(32*32*Tcount); //确定需要发送多少数据
    for(uint8_t i=0; i<Tcount; i++)
   {
        SViewModeTwo(32*32*Tcount);
      Flash_WordRead(Font_Buf,addr+(PSCNDCont->Data/(Mult10(i))%10)*128,128);//读取点阵字符解析
      Font_Analyses(Font_Buf);//发送字符数组  
     
      SendimageHeader sendimageheader;
      sendimageheader.Start_Addr=Uhead->FPGA_deputy_addr+(32*32*2)*i;//NumFlash_deputy_addr+Num_W*Num_H*20 为偏移量地址
      sendimageheader.Pic_Len=32*32;//获取所有需要显示汉字的像素点长度
      sendimageheader.pic_X=PSSNFDCont->x+i*32;//其实坐标从起始的解析文件正写入特定的坐标 此坐标是数字在whindow中的显示坐标经过偏移
      sendimageheader.pic_Y=PSSNFDCont->y;
      sendimageheader.pic_W=32;
      sendimageheader.pic_H=32;//横坐标不变 除非超过最大汉字显示范围
      SViewModeOne(&sendimageheader);//发送表头开始搬运
    }
    
   }
   
	else//直接发送搬运命令 通过CAN item数据项ID号已经确定了数字是几位数 直接搬运 
	{
   
		Num_kind=PSSNFDCont->Num_Kind;
		NumberSubKind *NumSK =	(NumberSubKind *)GetPosValue(Uhead->Num_list, Num_kind-1);//得到此节点在CAN链表的数据
		NumAddr=NumSK->NumFPGA_Addr;
		Num_W=NumSK->Num_W;
		Num_H=NumSK->Num_H;
    	//读取需要显示的子模块数据项的地址 传输

    
        for(uint8_t i=0; i<Tcount; i++)
      {
        //获取数字图片地址
       
        SendimageHeader sendimageheader;
        sendimageheader.Start_Addr=(NumAddr+(Num_W*Num_H*4)*(PSCNDCont->Data/(Mult10(i))%10))/2;//NumFlash_deputy_addr+Num_W*Num_H*20 为偏移量地址
        sendimageheader.Pic_Len=Num_W*Num_H;//获取所有需要显示汉字的像素点长度
        sendimageheader.pic_X=PSSNFDCont->x+i*Num_W;//其实坐标从起始的解析文件正写入特定的坐标 此坐标是数字在whindow中的显示坐标经过偏移
        sendimageheader.pic_Y=PSSNFDCont->y;
        sendimageheader.pic_W=Num_W;
        sendimageheader.pic_H=Num_H;//横坐标不变 除非超过最大汉字显示范围
        SViewModeOne(&sendimageheader);//发送表头开始搬运
        OSIF_TimeDelay(8);
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
void Show_Font_Handle(SubShowModuleHeader *PSSMHead,\
					 SubCanModuleHeader *PSCMHead,\
					 SubModuleShowNumFontDataContent *PSSNFDCont,\
					 SubModuleCANFontDataContent *PSCFDCont,User_Header*Uhead)//汉字显示处理函数
{
	
	uint8_t Font_W=0;
	uint8_t Font_H=0;
  uint16_t Nx=0;
  uint16_t Ny=0;
          SendimageHeader sendimageheader;
	//擦除显示区域的部分
    //Clear_Region(PSSMHead);

	//从can接收的链表中读取需要显示Unicode的值
	//选择字库
	//从字库中读取相应的unicode字库的点阵
	//显示方式将数据全部存入副内存 然后开启显示
	//  
	//PSSNFDCont.Unicode_Kind//汉字的种类
    FontSubKind *FontSK =	(FontSubKind *)GetPosValue(Uhead->Font_list, PSSNFDCont->Unicode_Kind);//得到此节点在CAN链表的数据
    Font_W=FontSK->Font_W;	
    Font_H=FontSK->Font_H;		
    Nx=PSSNFDCont->x;
    Ny=PSSNFDCont->y;
    Clear_Region(PSSMHead);
    if(PSCFDCont->RData_len!=0)//显示模块有固定字符显示
    {
      for (uint8_t i = 1; i <PSCFDCont->RData_len+1; i++)
      {
        SViewModeTwo(Font_W*Font_H);
        Flash_WordRead(Font_Buf,FontSK->FontFlash_Addr+PSCFDCont->unicode_reg_pack[i-1]*(Font_W*Font_H)/8,(Font_W*Font_H)/8);//读取点阵字符解析
        Font_Analyses(Font_Buf);//发送字符数组

        sendimageheader.Start_Addr=Uhead->FPGA_deputy_addr;//+(Font_W*Font_H*2)*i;//NumFlash_deputy_addr+Num_W*Num_H*20 为偏移量地址
        sendimageheader.Pic_Len=Font_W*Font_H;//获取所有需要显示汉字的像素点长度
        sendimageheader.pic_X=Nx;//其实坐标从起始的解析文件正写入特定的坐标 此坐标是数字在whindow中的显示坐标经过偏移
        sendimageheader.pic_Y=Ny;
        sendimageheader.pic_W=Font_W;
        sendimageheader.pic_H=Font_H;//横坐标不变 除非超过最大汉字显示范围
        SViewModeOne(&sendimageheader);//发送表头开始搬运
        OSIF_TimeDelay(8);
        
        if((Nx-PSSNFDCont->x)==PSSMHead->MaxW)//换行
        {
          Nx=PSSNFDCont->x;//回到初始x坐标
          Ny=Ny+Font_H;//y换行
        }
        else
        {

           Nx=Nx+Font_W;
          
        }  
        
      }
    }
    
    if(PSCMHead->Effective_Data==255|PSCMHead->Effective_Data==PSCFDCont->NData_len)
    {
    //发送字符数组表头 size大小=像素个数*有效数据长度
    for (uint8_t i=1; i <PSCFDCont->NData_len+1; i++)
    {   SViewModeTwo(Font_W*Font_H);
        Flash_WordRead(Font_Buf,FontSK->FontFlash_Addr+PSCFDCont->unicode_pack[i-1]*(Font_W*Font_H)/8,(Font_W*Font_H)/8);//读取点阵字符解析
        Font_Analyses(Font_Buf);//发送字符数组

       if((Nx-PSSNFDCont->x)==PSSMHead->MaxW)//换行
        {
          Nx=PSSNFDCont->x;//回到初始x坐标
          Ny=Ny+Font_H;//y换行
        }
   		//传输到 副内存地址
        sendimageheader.Start_Addr=Uhead->FPGA_deputy_addr;//+(Font_W*Font_H*2)*i;//NumFlash_deputy_addr+Num_W*Num_H*20 为偏移量地址
        sendimageheader.Pic_Len=Font_W*Font_H;//获取所有需要显示汉字的像素点长度
        sendimageheader.pic_X=Nx;//其实坐标从起始的解析文件正写入特定的坐标 此坐标是数字在whindow中的显示坐标经过偏移
        sendimageheader.pic_Y=Ny;
        sendimageheader.pic_W=Font_W;
        sendimageheader.pic_H=Font_H;//横坐标不变 除非超过最大汉字显示范围
        SViewModeOne(&sendimageheader);//发送表头开始搬运
        OSIF_TimeDelay(8);
        if((Nx-PSSNFDCont->x)==PSSMHead->MaxW)//换行
        {
          Nx=PSSNFDCont->x;//回到初始x坐标
          Ny=Ny+Font_H;//y换行
        }
        else
        {

           Nx=Nx+Font_W;
          
        } 
    }
    
    PSCMHead->Effective_Data=255;
    PSCFDCont->NData_len=0;
    }	
}

void Show_Animated_Handle(SubShowModuleHeader *PSSMHead,\
                       SubCanModuleHeader *PSCMHead,\
                       SubModuleShowAnimatedDataContent *PSSMHeadADCont,\
                       SubModuleCANNumDataContent *PSCNDCont)
{
  //动画显示代码
  //确定需要的帧id发送给任务3 循环显示如何发送 声明一个结构体 结构体包括必须元素 必须元素包括 xy，wh 周期时间 每一幅图片的等待显示周期

}
void Show_Bar_Handle(SubShowModuleHeader *PSSMHead,\
                       SubCanModuleHeader *PSCMHead,\
                       SubModuleShowBarDataContent *PSSMHeadBDCont,\
                       SubModuleCANNumDataContent *PSCNDCont)
{
    float NowValue=1.0,Max_Value=0,CX=0,CY=0,K=0,SX=0,SY=0,FX=0,FY=0,B=0;

    float per=0;
    //uint8_t i = 0;

    NowValue=PSCNDCont->Data;
    Max_Value=PSSMHeadBDCont->Max_Value;
    per=(NowValue/Max_Value);//为了确定显示的长度大小空间


    SX=PSSMHeadBDCont->X_S;
    SY=PSSMHeadBDCont->Y_S;
    FX=PSSMHeadBDCont->X_F;
    FY=PSSMHeadBDCont->Y_F;
    
    CX=FX-SX;
    CX=CX*per;//获取到需要显示的长度
    K=(FY-SY)/(FX-SX);
    B=FY-K*FX;
    CY=CX*K+B;

    LCD_DrawLine( PSSMHeadBDCont->X_S,PSSMHeadBDCont->Y_S, PSSMHeadBDCont->X_F,PSSMHeadBDCont->Y_F,\
                                PSSMHeadBDCont->unit_W,PSSMHeadBDCont->unit_H,0x00000000);//画线
    //确定显示坐标末显示
    LCD_DrawLine( (uint16_t)SX,  (uint16_t)SY, (uint16_t)CX,(uint16_t)CY,\
                                PSSMHeadBDCont->unit_W,PSSMHeadBDCont->unit_H,0x0000ff00);//画线
    



}



//初始化显示显示屏幕
void  show_init(uint8_t Layer)
{
    uint32_t Cvalue=0;

    for(uint16_t i=0;i<5;i++)
    {
      Cvalue=i<<16|0;
      show_Layer(Layer,Cvalue);//初始化屏幕
      OSIF_TimeDelay(5);
    }



}
void show_Layer(uint8_t Layer,uint32_t ulReceivedValue)
{
  switch(Layer)
  {
    case 1:
      {
        ViewShow(AHead.UHead1,AHead.CHead1,ulReceivedValue);
      }break;
    case 2:
      {
        ViewShow(AHead.UHead2,AHead.CHead2,ulReceivedValue);
      }break;
    case 3:
      {
        ViewShow(AHead.UHead3,AHead.CHead3,ulReceivedValue);
      }break;
    default: break;
  }
  
}
