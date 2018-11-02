/**
  ******************************************************************************
  * File Name          : cJCAN.c
  * Description        : Code for 3-dragon 使用cjson解析 CAN
  * Author             : YJJ
  * Date               : 2018.9.20
  ******************************************************************************
  */
 
 #include "cJCAN.h"
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



static uint8_t cJCAN_Analysis(uint8_t layer,uint8_t *A_Buf);

/*读取Flash中的canJSON数据并将其解析成特定数据结构*/
/*
 * 功能点：
 * 		  1.读取flash中的数据
 * 		  2.解析
 *
 */


//打印函数
void printSubModuleCANPicDataContent(void *data)
{
	SubModuleCANPicDataContent *tmp = (SubModuleCANPicDataContent *)data;
	Log("Data_ItemNumber:%d\n" , tmp->Data_ItemNumber);
}
//打印函数
void printSubModuleCANNumDataContent(void *data)
{
	SubModuleCANNumDataContent *tmp = (SubModuleCANNumDataContent *)data;
	Log("Data:%d\n" , tmp->Data);
}

//打印函数
void printSubModuleCANFontDataContent(void *data)
{
	SubModuleCANFontDataContent *tmp = (SubModuleCANFontDataContent *)data;
	Log("Data_ItemNumber:%d\n" , tmp->RData_len);

	Log("unicode_pack[0]:%d\n" , tmp->unicode_pack[0]);
	Log("unicode_pack[1]:%d\n" , tmp->unicode_pack[1]);
	Log("unicode_pack[2]:%d\n" , tmp->unicode_pack[2]);
	Log("unicode_pack[3]:%d\n" , tmp->unicode_pack[3]);
	Log("unicode_pack[4]:%d\n" , tmp->unicode_pack[4]);
	Log("unicode_pack[5]:%d\n" , tmp->unicode_pack[5]);
	Log("unicode_pack[6]:%d\n" , tmp->unicode_pack[6]);
	Log("unicode_pack[7]:%d\n" , tmp->unicode_pack[7]);

}
//打印函数
void printSubCanModuleHeader(void *data)
{
	SubCanModuleHeader *tmp = (SubCanModuleHeader *)data;
	Log("ID:%d  Data_ItemSize:%d\n" , tmp->ID,tmp->Data_ItemSize);
  Log("ID:%d  CANID:%d\n" , tmp->ID,tmp->CANID);
}


uint8_t canjson_buf[]={"{\"CANModule\":{\"SubModuleSum\":6,\"SubModule\":[{\"ID\":0,\"DataMode\":2,\"CANID\":800,\"Data_Pos\":0,\"Effective_Data\":2,\"Content\":[{\"Data\":777}]},{\"ID\":1,\"DataMode\":5,\"CANID\":801,\"Data_Pos\":0,\"Effective_Data\":4,\"Content\":[{\"Data\":50}]},{\"ID\":2,\"DataMode\":1,\"CANID\":802,\"Data_Pos\":0,\"Effective_Data\":3,\"Content\":[{\"Data_ItemNumber\":0}]},{\"ID\":3,\"DataMode\":99,\"CANID\":803,\"Data_Pos\":0,\"Effective_Data\":1,\"Content\":[{\"Data_ItemNumber\":1}]},{\"ID\":4,\"DataMode\":1,\"CANID\":804,\"Data_Pos\":0,\"Effective_Data\":1,\"Content\":[{\"Data_ItemNumber\":0}]},{\"ID\":5,\"DataMode\":3,\"CANID\":805,\"Data_Pos\":0,\"Effective_Data\":255,\"Content\":[{\"NData_len\":4,\"RData_len\":3,\"Unicode_Pack\":[26472,26471,25470,26473],\"Unicode_Reg_Pack\":[26472,26460,26450]}]}]}}"};
uint8_t cJCAN_Analysis(uint8_t layer,uint8_t *A_Buf)
{	
	cJSON *root = cJSON_Parse((char const*)A_Buf);
  
    //创建链表


  	void *CanSub_list= InitLinkList();// 子模块链表
 


	if(!root)
	{

		Log("get root faild !\n");
		return -1;
	}
	cJSON *PCANModule = cJSON_GetObjectItem(root, "CANModule");
	if(!PCANModule) 
	{
		Log("no CANModule!\n");
		return -1;
	}
	#ifdef DEBUG
	Log("PCANModule type is %d\n",PCANModule->type);
	#endif
	cJSON *PSubModuleSum = cJSON_GetObjectItem(PCANModule, "SubModuleSum");
	if(!PSubModuleSum) 
	{
		Log("no SubModuleSum!\n");		
		return -1;
	}
	#ifdef DEBUG
	Log("PCANModule size is %d\n",PCANModule->valueint);
	#endif
	cJSON *PSubModule = cJSON_GetObjectItem(PCANModule, "SubModule");
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

   	//申请内存
	CanModuleHeader *Chead=(CanModuleHeader*)pvPortMalloc(sizeof(CanModuleHeader));

	Chead->SubModuleSum= PSubModule_size; 


	for (int i = 0; i <PSubModule_size ; i++)
	{
		cJSON *PID= cJSON_GetObjectItem(PSubModulechild, "ID");	
		cJSON *PDataMode= cJSON_GetObjectItem(PSubModulechild, "DataMode");
		cJSON *PCANID= cJSON_GetObjectItem(PSubModulechild, "CANID");
		cJSON *PData_Pos= cJSON_GetObjectItem(PSubModulechild, "Data_Pos");
		cJSON *PEffective_Data= cJSON_GetObjectItem(PSubModulechild, "Effective_Data");	
		#ifdef DEBUG			
		Log("PID%d\n",PID->valueint);
		Log("PDataMode%d\n",PDataMode->valueint);
		Log("PCANID%d\n",PCANID->valueint);
		Log("PData_Pos%d\n",PData_Pos->valueint);
		Log("PEffective_Data%d\n",PEffective_Data->valueint);
    	#endif


	    //创建链表
	    void *CanCon_list= InitLinkList();//数据项链表



		cJSON *PContent = cJSON_GetObjectItem(PSubModulechild, "Content");
		if(!PContent) 
		{
			Log("no PContent!\n");		
			return -1;
		}
		uint8_t PContent_size = cJSON_GetArraySize(PContent);
		#ifdef DEBUG
		Log("PContent size is %d\n",PContent_size);		
    	#endif
    
    	SubCanModuleHeader *CSubModHead=(SubCanModuleHeader*)pvPortMalloc(sizeof(SubCanModuleHeader));
    Log("SubCanModuleHeader size is %d\n",sizeof(SubCanModuleHeader));	
		CSubModHead->ID=PID->valueint;
		CSubModHead->Data_Pos=PData_Pos->valueint;
		CSubModHead->DataMode=PDataMode->valueint;
		CSubModHead->Effective_Data=PEffective_Data->valueint;
		CSubModHead->Data_ItemSize=PContent_size;
		CSubModHead->CANID=PCANID->valueint;

		cJSON* PContentchild = PContent->child;//子对象

		for (uint8_t j = 0; j <PContent_size ; j++)
		{
			if (PDataMode->valueint==1||PDataMode->valueint==99)
			{
				cJSON *PData_ItemNumber= cJSON_GetObjectItem(PContentchild, "Data_ItemNumber");
				#ifdef DEBUG	
				Log("PData_ItemNumber%d\n",PData_ItemNumber->valueint);	
				#endif
				SubModuleCANPicDataContent *CSMPCont=(SubModuleCANPicDataContent*)pvPortMalloc(sizeof(SubModuleCANPicDataContent));
				CSMPCont->Data_ItemNumber=PData_ItemNumber->valueint;

				InsertTailLinkList(CanCon_list,CSMPCont);//将获取到的数据项内容尾插到list中
        		//vPortFree(CSMPCont);
        		#ifdef DEBUG
        		printSubModuleCANPicDataContent(GetPosValue(CanCon_list, j));
            #endif
        
        
				
			}
			else if(PDataMode->valueint==2||PDataMode->valueint==4||PDataMode->valueint==5)
			{
				cJSON *PData= cJSON_GetObjectItem(PContentchild, "Data");		
				// cJSON *PData_len= cJSON_GetObjectItem(PContentchild, "Data_len");
				#ifdef DEBUG
				Log("PData%d\n",PData->valueint);	
				//Log("%d\n",PData_len->valueint);
				#endif
				SubModuleCANNumDataContent *CSMNCont=(SubModuleCANNumDataContent*)pvPortMalloc(sizeof(SubModuleCANNumDataContent));
				CSMNCont->Data=PData->valueint;
				//CSMNCont->Data_len=PData->valueint;	

            	InsertTailLinkList(CanCon_list,CSMNCont);//将获取到的数据项内容尾插到list中
        		//vPortFree(CSMNCont);
        		#ifdef DEBUG
        		printSubModuleCANNumDataContent(GetPosValue(CanCon_list, j));	
            	#endif

			}		
			else if(PDataMode->valueint==3)
			{
	
				cJSON *PNData_len= cJSON_GetObjectItem(PContentchild, "NData_len");
        cJSON *PRData_len= cJSON_GetObjectItem(PContentchild, "RData_len");
				#ifdef DEBUG

				Log("PNData_len%d\n",PNData_len->valueint);		
        Log("PRData_len%d\n",PRData_len->valueint);	
				#endif
				cJSON *PUnicode_Pack= cJSON_GetObjectItem(PContentchild, "Unicode_Pack");
				cJSON *PUnicode_reg_pack= cJSON_GetObjectItem(PContentchild, "Unicode_Reg_Pack");
				uint8_t PUnicode_Pack_size = cJSON_GetArraySize(PUnicode_Pack);
				uint8_t PUnicode_reg_Pack_size = cJSON_GetArraySize(PUnicode_reg_pack);
				#ifdef DEBUG
				Log("PUnicode_Pack_size size is %d\n",PUnicode_Pack_size);
				#endif
        		SubModuleCANFontDataContent *CSMFCont=(SubModuleCANFontDataContent*)pvPortMalloc(sizeof(SubModuleCANFontDataContent));
        Log("SubModuleCANFontDataContent size is %d",sizeof(SubModuleCANFontDataContent));
				CSMFCont->NData_len=PNData_len->valueint;
        CSMFCont->RData_len=PRData_len->valueint;
				      
				//cJSON* PUnicode_Packchild = PUnicode_Pack->child;//子对象
				for (uint8_t k = 0; k < PUnicode_Pack_size; k++)
				{
					cJSON * pSub = cJSON_GetArrayItem(PUnicode_Pack, k);
					//uint8_t * ivalue = (uint8_t*)pSub->valueint ;
					#ifdef DEBUG
					Log("Unicode_Pack[%d] : %d",k,pSub->valueint);
          			CSMFCont->unicode_pack[k]=pSub->valueint;
          			#endif      
				}
				for (uint8_t k = 0; k < PUnicode_reg_Pack_size; k++)
				{
					cJSON * pSub = cJSON_GetArrayItem(PUnicode_reg_pack, k);
					//uint8_t * ivalue = (uint8_t*)pSub->valueint ;
					#ifdef DEBUG
					Log("Unicode_reg_pack[%d] : %d",k,pSub->valueint);
          			CSMFCont->unicode_reg_pack[k]=pSub->valueint;
          			#endif      
				}				
				
				InsertTailLinkList(CanCon_list,CSMFCont);//将获取到的数据项内容尾插到list中
        		//vPortFree(CSMFCont);
        		#ifdef DEBUG
        		printSubModuleCANFontDataContent(GetPosValue(CanCon_list, j));
        		#endif	
			}
			//将数据项链表导入子模块
      CSubModHead->Contlist=CanCon_list;

			if(PContent_size>1)
			{
				PContentchild=PContentchild->next;
			}
		}
    

    
    
		InsertTailLinkList(CanSub_list,CSubModHead);//将获取到的数据项内容尾插到list中
    	//vPortFree(CSubModHead);	
    	#ifdef DEBUG
    	printSubCanModuleHeader(GetPosValue(CanSub_list, i));
    	#endif

		if(PSubModule_size>1)
		{
			PSubModulechild=PSubModulechild->next;
		}		
	}
Log("csub address %d",CanSub_list);
	Chead->Sublist=CanSub_list;

  if(layer==1)
  {
    AHead.CHead1=Chead;
  }
  else if(layer==2)
  {
    AHead.CHead2=Chead;
  }
  else if(layer==3)
  {
    AHead.CHead3=Chead;
  }
  SubCanModuleHeader *PSCM1Head =	(SubCanModuleHeader *)GetPosValue(CanSub_list, 2);//得到此节点在CAN链表的数据
  Log("PSCMHead addresss%d",PSCM1Head);
  Log("data_pos is %d\r\n",PSCM1Head->Data_Pos);
  Log("CANID is %d\r\n",PSCM1Head->CANID);
	cJSON_Delete(root);

 	return 1;
}


void Can_Config(void)
{
  
	if(cJCAN_Analysis(1,canjson_buf)==1)
  {
    Log("CAN config is Success\r\n");
  }
}




