/**
  ******************************************************************************
  * File Name          : cJUser.c
  * Description        : Code for 3-dragon 使用cjson解析 USER
  * Author             : YJJ
  * Date               : 2018.9.5
  ******************************************************************************
  */
 
 #include "cJUser.h"
#include "cJSON.h"
#include "Log.h"
#include "LinkList.h"
#include "stdint.h"

#include "JL_Data_Structure.h"

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

 /*读取Flash中的数据并将其解析成特定数据结构*/
/*
 * 功能点：
 * 		  1.读取flash中的数据
 * 		  2.解析
 *
 */
static uint8_t cJUser_Analysis(uint8_t layer,uint8_t *A_Buf);

//打印函数
void printNumberSubKind(void *data)
{
	NumberSubKind *tmp = (NumberSubKind *)data;
	Log("NumFPGA_Addr:%d Num_W:%d  Num_H:%d\n", tmp->NumFPGA_Addr, tmp->Num_W,tmp->Num_H);
}
//打印函数
void printFontSubKind(void *data)
{
	FontSubKind *tmp = (FontSubKind *)data;
	Log("FontFlash_Addr:%d  Font_W:%d  Font_H:%d\n", tmp->FontFlash_Addr, tmp->Font_W,tmp->Font_H);
}
//打印函数
void printSubViewModuleHeader(void *data)
{
	SubShowModuleHeader * tmp = (SubShowModuleHeader *)data;
	Log("ID:%d  DataMode:%d\n", tmp->ID, tmp->DataMode);
}

void printSubModulePicDataContent(void *data)
{
	SubModuleShowPicDataContent *tmp = (SubModuleShowPicDataContent *)data;
	Log("show_flag:%d  Data_ItemNumber:%d\n", tmp->show_flag, tmp->Data_ItemNumber);
}

void printSubModuleNumFontDataContent(void *data)
{
	SubModuleShowNumFontDataContent *tmp = (SubModuleShowNumFontDataContent *)data;
	Log("Data_ItemNumber:%d Num_Kind:%d\n", tmp->Data_ItemNumber, tmp->Num_Kind);
  Log("x:%d y:%d\n", tmp->x, tmp->y);
}
void printSubModuleShowBarDataContent(void *data)
{
  SubModuleShowBarDataContent *tmp=(SubModuleShowBarDataContent*)data;
  	Log("BarMoreArea:%d Max_Value:%d\n", tmp->BarMoreArea, tmp->Max_Value);

}


uint8_t test_buf[]={"{\"UserModule\":{\"SubModuleSum\":20,\"FPGA_deputy_addr\":10485760,\"Number\":[{\"NumFPGA_Addr\":0,\"Num_W\":108,\"Num_H\":146},{\"NumFPGA_Addr\":3072000,\"Num_W\":24,\"Num_H\":24}],\"Font\":[{\"FontFlash_Addr\":0,\"Font_W\":32,\"Font_H\":32},{\"FontFlash_Addr\":3088512,\"Font_W\":32,\"Font_H\":32},{\"FontFlash_Addr\":3088512,\"Font_W\":16,\"Font_H\":16}],\"SubModule\":[{\"ID\":0,\"DataMode\":2,\"MaxX\":400,\"MaxY\":140,\"MaxW\":324,\"MaxH\":146,\"ShowLayer\":0,\"Content\":[{\"Num_Kind\":0,\"Unicode_Kind\":0,\"x\":500,\"y\":140},{\"Num_Kind\":1,\"Unicode_Kind\":0,\"x\":450,\"y\":140},{\"Num_Kind\":1,\"Unicode_Kind\":0,\"x\":400,\"y\":140},{\"Num_Kind\":1,\"Unicode_Kind\":0,\"x\":400,\"y\":140},{\"Num_Kind\":1,\"Unicode_Kind\":0,\"x\":400,\"y\":140}]},{\"ID\":1,\"DataMode\":5,\"MaxX\":100,\"MaxY\":100,\"MaxW\":18,\"MaxH\":24,\"ShowLayer\":1,\"Content\":[{\"show_flag\":1,\"Max_Value\":100,\"color\":16711680,\"unit_W\":16,\"unit_H\":32,\"X_S\":60,\"Y_S\":300,\"X_F\":160,\"Y_F\":400,\"BarMoreArea\":50}]},{\"ID\":2,\"DataMode\":1,\"MaxX\":50,\"MaxY\":80,\"MaxW\":18,\"MaxH\":24,\"ShowLayer\":1,\"Content\":[{\"show_flag\":1,\"Fpga_Addr\":366468,\"StartX\":50,\"StartY\":80,\"StartW\":60,\"StartH\":64},{\"show_flag\":1,\"Fpga_Addr\":351108,\"StartX\":300,\"StartY\":96,\"StartW\":60,\"StartH\":64}]},{\"ID\":3,\"DataMode\":1,\"MaxX\":50,\"MaxY\":80,\"MaxW\":18,\"MaxH\":24,\"ShowLayer\":1,\"Content\":[{\"show_flag\":1,\"Fpga_Addr\":366468,\"StartX\":50,\"StartY\":80,\"StartW\":60,\"StartH\":64},{\"show_flag\":1,\"Fpga_Addr\":351108,\"StartX\":300,\"StartY\":96,\"StartW\":60,\"StartH\":64}]},{\"ID\":4,\"DataMode\":1,\"MaxX\":100,\"MaxY\":150,\"MaxW\":64,\"MaxH\":60,\"ShowLayer\":1,\"Content\":[{\"show_flag\":1,\"Fpga_Addr\":358788,\"StartX\":100,\"StartY\":150,\"StartW\":64,\"StartH\":60}]},{\"ID\":5,\"DataMode\":3,\"MaxX\":100,\"MaxY\":360,\"MaxW\":128,\"MaxH\":128,\"ShowLayer\":0,\"Content\":[{\"Num_Kind\":0,\"Unicode_Kind\":0,\"x\":100,\"y\":360},{\"Num_Kind\":0,\"Unicode_Kind\":0,\"x\":132,\"y\":360}]}]}}"};
uint8_t cJUser_Analysis(uint8_t layer,uint8_t *A_Buf)
{
	cJSON *root = cJSON_Parse((char const*)A_Buf);
  
  //创建链表

  void *Num_list= InitLinkList(); //数字链表
  void *Fon_list= InitLinkList();//汉字链表
  void *UserSub_list= InitLinkList();// 子模块链表

	if(!root)
	{

		Log("get root faild !\n");
		return -1;
	}
	cJSON *PUserModule = cJSON_GetObjectItem(root, "UserModule");
	if(!PUserModule) 
	{
		Log("no UserModule!\n");
		return -1;
	}
	#ifdef DEBUG
	Log("PUserModule type is %d\n",PUserModule->type);
	#endif
	cJSON *PSubModuleSum = cJSON_GetObjectItem(PUserModule, "SubModuleSum");
	if(!PSubModuleSum) 
	{
		Log("no SubModuleSum!\n");		
		return -1;
	}
	cJSON *PFPGA_deputy_addr = cJSON_GetObjectItem(PUserModule, "FPGA_deputy_addr");
	if(!PFPGA_deputy_addr) 
	{
		Log("no FPGA_deputy_addr!\n");		
		return -1;
	}






	cJSON *PNumber = cJSON_GetObjectItem(PUserModule, "Number");
	if(!PNumber) 
	{
		Log("no PNumber!\n");		
		return -1;
	}
  
	uint8_t PNumber_size = cJSON_GetArraySize(PNumber);
	#ifdef DEBUG
	Log("PNumber size is %d\n",PNumber_size);
	#endif

	cJSON* PNumchild = PNumber->child;//子对象
	for (uint8_t i = 0; i < PNumber_size; i++)
	{
		cJSON *PNumFPGA_Addr= cJSON_GetObjectItem(PNumchild, "NumFPGA_Addr");	
		cJSON *PNum_W= cJSON_GetObjectItem(PNumchild, "Num_W");
		cJSON *PNum_H= cJSON_GetObjectItem(PNumchild, "Num_H");	
		#ifdef DEBUG	
		Log("PNumFPGA_Addr%d\n",PNumFPGA_Addr->valueint);
		Log("PNum_W%d\n",PNum_W->valueint);
		Log("PNum_H%d\n",PNum_H->valueint);
		#endif
		//为结构体数组指针申请内存放置数据
		NumberSubKind *NumSKind=(NumberSubKind*)pvPortMalloc(sizeof(NumberSubKind));
		NumSKind->NumFPGA_Addr=PNumFPGA_Addr->valueint;
		NumSKind->Num_W=PNum_W->valueint;
		NumSKind->Num_H=PNum_H->valueint;

	    InsertTailLinkList(Num_list,NumSKind);//将获取到的数据项内容尾插到list中
	    //vPortFree(NumSKind); 
	    #ifdef DEBUG
	    printNumberSubKind(GetPosValue(Num_list, i));
	    #endif
	    PNumchild=PNumchild->next;
	}

	cJSON *PFont = cJSON_GetObjectItem(PUserModule, "Font");
	if(!PNumber) 
	{
		Log("no Font!\n");		
		return -1;
	}
	uint8_t PFont_size = cJSON_GetArraySize(PFont);
	#ifdef DEBUG
	Log("PFont size is %d\n",PFont_size);
	#endif

	cJSON* PFontchild = PFont->child;//子对象
	for (uint8_t i = 0; i < PFont_size; i++)
	{
		cJSON *PFontFlash_Addr= cJSON_GetObjectItem(PFontchild, "FontFlash_Addr");	
		cJSON *PFont_W= cJSON_GetObjectItem(PFontchild, "Font_W");
		cJSON *PFont_H= cJSON_GetObjectItem(PFontchild, "Font_H");
		#ifdef DEBUG		
		Log("PFontFlash_Addr%d\n",PFontFlash_Addr->valueint);
		Log("PFont_W%d\n",PFont_W->valueint);
		Log("PFont_H%d\n",PFont_H->valueint);
		#endif
    
		FontSubKind * FonSKind=(FontSubKind*)pvPortMalloc(sizeof(FontSubKind));	
		FonSKind->FontFlash_Addr=PFontFlash_Addr->valueint;
		FonSKind->Font_W=PFont_W->valueint;
		FonSKind->Font_H=PFont_H->valueint;
    
    	InsertTailLinkList(Fon_list,FonSKind);//将获取到的数据项内容尾插到list中
    	//vPortFree(FonSKind);
    	#ifdef DEBUG
    	printFontSubKind(GetPosValue(Fon_list, i));
    	#endif
		if(PFont_size>1)
		{
			PFontchild=PFontchild->next;
		}	
	}
  
  
  	//申请主模块内存
	User_Header *Uhead=(User_Header*)pvPortMalloc(sizeof(User_Header));
	Uhead->SubModuleSum= PSubModuleSum->valueint;
	Uhead->FPGA_deputy_addr=PFPGA_deputy_addr->valueint;
	Uhead->NumKindsize= PNumber_size;
	Uhead->FontKindsize= PFont_size;
  

	cJSON *PSubModule = cJSON_GetObjectItem(PUserModule, "SubModule");
	if(!PSubModule) 
	{
		Log("no SubModule!\n");		
		return -1;
	}	
	uint8_t PSubModule_size = cJSON_GetArraySize(PSubModule);
	#ifdef DEBUG
	Log("PSubModule size is %d\n",PSubModule_size);
	#endif
	cJSON* PSubModulechild = PSubModule->child;//子对象
	for (uint8_t i = 0; i < PSubModule_size; i++)
	{
		cJSON *PID= cJSON_GetObjectItem(PSubModulechild, "ID");	
		cJSON *PDataMode= cJSON_GetObjectItem(PSubModulechild, "DataMode");
		cJSON *PMaxX= cJSON_GetObjectItem(PSubModulechild, "MaxX");
		cJSON *PMaxY= cJSON_GetObjectItem(PSubModulechild, "MaxY");
		cJSON *PMaxW= cJSON_GetObjectItem(PSubModulechild, "MaxW");
		cJSON *PMaxH= cJSON_GetObjectItem(PSubModulechild, "MaxH");
		#ifdef DEBUG			
		Log("PID%d\n",PID->valueint);
		Log("PDataMode%d\n",PDataMode->valueint);
		Log("PMaxX%d\n",PMaxX->valueint);
		Log("PMaxY%d\n",PMaxY->valueint);
		Log("PMaxW%d\n",PMaxW->valueint);
		Log("PMaxH%d\n",PMaxH->valueint);
		#endif
    	//创建数据项链表
      	void *UserCon_list= InitLinkList();//数据项链表
              
		//申请内存
		SubShowModuleHeader *USubModHead=(SubShowModuleHeader*)pvPortMalloc(sizeof(SubShowModuleHeader));

		USubModHead->ID=PID->valueint;
		USubModHead->DataMode=PDataMode->valueint;
		USubModHead->Data_ItemSize=PSubModule_size;
		USubModHead->MaxX=PMaxX->valueint;
		USubModHead->MaxY=PMaxY->valueint;
		USubModHead->MaxW=PMaxW->valueint;
		USubModHead->MaxH=PMaxH->valueint;
     
    
		cJSON *PContent = cJSON_GetObjectItem(PSubModulechild, "Content");
		if(!PContent) 
		{
			Log("no Content!\n");		
			return -1;
		}

		uint8_t PContent_size = cJSON_GetArraySize(PContent);
		#ifdef DEBUG
		Log("PContent size is %d\n",PContent_size);
		#endif
		cJSON* PContentchild = PContent->child;//子对象

		for (int j = 0; j <PContent_size; j++)
		{

			if (PDataMode->valueint==1)
			{
				cJSON *Pshow_flag= cJSON_GetObjectItem(PContentchild, "show_flag");	
				cJSON *PFpga_Addr= cJSON_GetObjectItem(PContentchild, "Fpga_Addr");
				cJSON *PStartX= cJSON_GetObjectItem(PContentchild, "StartX");
				cJSON *PStartY= cJSON_GetObjectItem(PContentchild, "StartY");
				cJSON *PStartW= cJSON_GetObjectItem(PContentchild, "StartW");
				cJSON *PStartH= cJSON_GetObjectItem(PContentchild, "StartH");
				#ifdef DEBUG			
				Log("Pshow_flag%d\n",Pshow_flag->valueint);
				Log("PFpga_Addr%d\n",PFpga_Addr->valueint);
				Log("PStartX%d\n",PStartX->valueint);
				Log("PStartY%d\n",PStartY->valueint);
				Log("PStartW%d\n",PStartW->valueint);
				Log("PStartH%d\n",PStartH->valueint);
    			#endif
    			//修改内存 
				SubModuleShowPicDataContent *USMPCont=(SubModuleShowPicDataContent*)pvPortMalloc(sizeof(SubModuleShowPicDataContent));

				USMPCont->show_flag=Pshow_flag->valueint;
				USMPCont->Data_ItemNumber=j;
				USMPCont->Fpga_Addr=PFpga_Addr->valueint;
				USMPCont->StartX=PStartX->valueint;
				USMPCont->StartY=PStartY->valueint;
				USMPCont->StartW=PStartW->valueint;
				USMPCont->StartH=PStartH->valueint;
        
		        InsertTailLinkList(UserCon_list,USMPCont);//将获取到的数据项内容尾插到list中
		        //vPortFree(USMPCont);
		        #ifdef DEBUG
		        printSubModulePicDataContent(GetPosValue(UserCon_list, j));
		        #endif
		        USubModHead->Contlist=UserCon_list;//将数据项链表放入子模块中
			}
			else if(PDataMode->valueint==2||PDataMode->valueint==3)
			{
				cJSON *PNum_Kind= cJSON_GetObjectItem(PContentchild, "Num_Kind");	
				cJSON *PUnicode_Kind= cJSON_GetObjectItem(PContentchild, "Unicode_Kind");
				cJSON *Px= cJSON_GetObjectItem(PContentchild, "x");
				cJSON *Py= cJSON_GetObjectItem(PContentchild, "y");	
				#ifdef DEBUG	
				Log("PNum_Kind%d\n",PNum_Kind->valueint);
				Log("PUnicode_Kind%d\n",PUnicode_Kind->valueint);
				Log("Px%d\n",Px->valueint);
				Log("Py%d\n",Py->valueint);
				#endif
				//修改内存
				SubModuleShowNumFontDataContent *USMNFCont=(SubModuleShowNumFontDataContent*)pvPortMalloc(sizeof(SubModuleShowNumFontDataContent));

				USMNFCont->Data_ItemNumber=j;
				USMNFCont->Num_Kind=PNum_Kind->valueint;
				USMNFCont->Unicode_Kind=PUnicode_Kind->valueint;
				USMNFCont->x=Px->valueint;
				USMNFCont->y=Py->valueint;
        
		        InsertTailLinkList(UserCon_list,USMNFCont);//将获取到的数据项内容尾插到list中
		        //vPortFree(USMNFCont);
		        #ifdef DEBUG
		        Log("j=%d\n",j);
		        printSubModuleNumFontDataContent(GetPosValue(UserCon_list, j));		
		        #endif
		        USubModHead->Contlist=UserCon_list;//将数据项链表放入子模块中
    		}
			else if(PDataMode->valueint==4)//动画显示
			{


				cJSON *PX= cJSON_GetObjectItem(PContentchild, "X");	
				cJSON *PY= cJSON_GetObjectItem(PContentchild, "Y");
				cJSON *PW= cJSON_GetObjectItem(PContentchild, "W");
				cJSON *PH= cJSON_GetObjectItem(PContentchild, "H");	
				#ifdef DEBUG	
				Log("PX%d\n",PX->valueint);
				Log("PY%d\n",PY->valueint);
				Log("PW%d\n",PW->valueint);
				Log("PH%d\n",PH->valueint);
				#endif				
				SubModuleShowAnimatedDataContent *PSSMHeadADCont=(SubModuleShowAnimatedDataContent*)pvPortMalloc(sizeof(SubModuleShowAnimatedDataContent));

				PSSMHeadADCont->Data_ItemNumber=j;
				PSSMHeadADCont->X=PX->valueint;
				PSSMHeadADCont->Y=PY->valueint;
				PSSMHeadADCont->W=PW->valueint;
				PSSMHeadADCont->H=PH->valueint;

		        InsertTailLinkList(UserCon_list,PSSMHeadADCont);//将获取到的数据项内容尾插到list中
		       	#ifdef DEBUG
		        Log("j=%d\n",j);
		        //printSubModuleNumFontDataContent(GetPosValue(UserCon_list, j));		
		        #endif
		        USubModHead->Contlist=UserCon_list;//将数据项链表放入子模块中
			}
			else if(PDataMode->valueint==5)//进度条显示
			{


				cJSON *Pshow_flag= cJSON_GetObjectItem(PContentchild, "show_flag");	
				cJSON *PMax_Value= cJSON_GetObjectItem(PContentchild, "Max_Value");
				cJSON *Pcolor= cJSON_GetObjectItem(PContentchild, "color");
				cJSON *Punit_W= cJSON_GetObjectItem(PContentchild, "unit_W");
				cJSON *Punit_H= cJSON_GetObjectItem(PContentchild, "unit_H");	
				cJSON *PX_S= cJSON_GetObjectItem(PContentchild, "X_S");	
				cJSON *PY_S= cJSON_GetObjectItem(PContentchild, "Y_S");	
				cJSON *PX_F= cJSON_GetObjectItem(PContentchild, "X_F");
				cJSON *PY_F= cJSON_GetObjectItem(PContentchild, "Y_F");	
				cJSON *PBarMoreArea= cJSON_GetObjectItem(PContentchild, "BarMoreArea");
				#ifdef DEBUG	
				Log("Pshow_flag%d\n",Pshow_flag->valueint);
				Log("PMax_Value%d\n",PMax_Value->valueint);
				Log("Pcolor%d\n",Pcolor->valueint);
				Log("Punit_W%d\n",Punit_W->valueint);
				Log("Punit_H%d\n",Punit_H->valueint);
				Log("PX_S%d\n",PX_S->valueint);
				Log("PY_S%d\n",PY_S->valueint);
				Log("PX_F%d\n",PX_F->valueint);
				Log("PY_F%d\n",PY_F->valueint);
				Log("PBarMoreArea%d\n",PBarMoreArea->valueint);
				#endif	
				SubModuleShowBarDataContent *PSSMHeadBDCont=(SubModuleShowBarDataContent*)pvPortMalloc(sizeof(SubModuleShowBarDataContent));					

				PSSMHeadBDCont->Data_ItemNumber=j;
				PSSMHeadBDCont->show_flag=Pshow_flag->valueint;
				PSSMHeadBDCont->Max_Value=PMax_Value->valueint;
				PSSMHeadBDCont->color=Pcolor->valueint;
				PSSMHeadBDCont->unit_W=Punit_W->valueint;				
				PSSMHeadBDCont->unit_H=Punit_H->valueint;
				PSSMHeadBDCont->X_S=PX_S->valueint;
				PSSMHeadBDCont->Y_S=PY_S->valueint;
				PSSMHeadBDCont->X_F=PX_F->valueint;
				PSSMHeadBDCont->Y_F=PY_F->valueint;
				PSSMHeadBDCont->BarMoreArea=PBarMoreArea->valueint;

		        InsertTailLinkList(UserCon_list,PSSMHeadBDCont);//将获取到的数据项内容尾插到list中
		       	#ifdef DEBUG
		        Log("j=%d\n",j);
		        printSubModuleShowBarDataContent(GetPosValue(UserCon_list, j));		
		        #endif
		        USubModHead->Contlist=UserCon_list;//将数据项链表放入子模块中
			}

			if(PContent_size>1)
			{
				PContentchild=PContentchild->next;
			}			
		}
    
		InsertTailLinkList(UserSub_list,USubModHead);//将获取到的数据项内容尾插到list中
	    //vPortFree(USubModHead);	
	    #ifdef DEBUG
	    printSubViewModuleHeader(GetPosValue(UserSub_list, i));
	    #endif
		PSubModulechild=PSubModulechild->next;
	}

    Uhead->Sublist=UserSub_list;
    Uhead->Num_list=Num_list;
    Uhead->Font_list=Fon_list;
      if(layer==1)
  {
    AHead.UHead1=Uhead;
  }
  else if(layer==2)
  {
    AHead.UHead2=Uhead;
  }
  else if(layer==3)
  {
    AHead.UHead3=Uhead;
  }
    cJSON_Delete(root);
  	return 1;
}




void User_Config(void)
{
  if(cJUser_Analysis(1,test_buf)==1)
  {
    Log("User config is Success\r\n");
  } 
}


