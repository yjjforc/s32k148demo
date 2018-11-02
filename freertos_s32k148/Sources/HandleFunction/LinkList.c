#include "LinkList.h"
#include "FreeRTOS.h"
#include "semphr.h"




//链表初始化
void *InitLinkList(void)
{
	LinkList *list = (LinkList *)pvPortMalloc(sizeof(LinkList));
	if (list == NULL)
	{
		return NULL;
	}
	list->head.data = NULL;
	list->head.next = NULL;
	list->size = 0;
	return list;
}

//指定位置插入
int InsertLinkList(void *list, int pos, void *data)
{
	if (list == NULL)
	{
		return -1;
	}
	if (data == NULL)
	{
		return -2;
	}
	LinkList *mylist = (LinkList *)list;
	//如果pos不对就插在末尾
	if (pos<0 || pos>mylist->size - 1)
	{
		pos = mylist->size;
	}
	Node *pro = &(mylist->head);	//头节点
	Node *pnew = NULL;				//新节点
	int i = 0;
	for (i = 0; i < pos; i++)
	{
		pro = pro->next;
	}

	pnew = (Node *)pvPortMalloc(sizeof(Node));
	if (pnew == NULL)
	{
		return -3;
	}
	//给数据赋值
	pnew->data = data;
	//新节点指向原来节点指向的地方
	pnew->next = pro->next;
	//原来节点指向新节点
	pro->next = pnew;
	//入链完成 元素个数+1
	mylist->size++;

	
	return 0;
}

//头插
int InsertHeadLinkList(void *list, void *data)
{
	InsertLinkList(list, 0, data);
	return 0;
}

//尾插
int InsertTailLinkList(void *list, void *data)
{
	LinkList *mylist = (LinkList *)list;
	InsertLinkList(list, mylist->size, data);
	return 0;
}

//指定位置删除
int RemoveLinkList(void *list, int pos)
{
	if (list == NULL)
	{
		return -1;
	}
	LinkList *mylist = (LinkList *)list;
	if (pos<0 || pos>mylist->size - 1)
	{
		return -2;
	}
	Node *pro = &(mylist->head);	//头节点
	Node *tmp = NULL;				//辅助节点
	int i = 0;
	for (i = 0; i < pos; i++)
	{
		pro = pro->next;
	}
	//tmp指向原来pro指向的地方
	tmp = pro->next;
	//pro指向tmp的下一个节点
	pro->next = tmp->next;
	//释放节点
	vPortFree(tmp);
	//删除完成 元素个数-1
	mylist->size--;

	return 0;
}

//头删
int RemoveHead(void *list)
{
	RemoveLinkList(list, 0);
	return 0;
}

//尾删
int RemoveTail(void *list)
{
	LinkList *mylist = (LinkList *)list;
	RemoveLinkList(list, mylist->size - 1);
	return 0;
}

//指定值前面插入  在值为x的数据前面插入data   func是比较函数
int ValueLeftInsert(void *list, void *x, void *data, int (*func)(void *, void *))
{
	if (list == NULL)
	{
		return -1;
	}
	if (x == NULL)
	{
		return -2;
	}
	if (data == NULL)
	{
		return -3;
	}
	if (func == NULL)
	{
		return -4;
	}
	int flag = 0;//标志位
	LinkList *mylist = (LinkList *)list;
	Node *pro = &(mylist->head);	//头节点
	Node *tmp = pro->next;			//pro的下一个  此处是首节点
	Node *pnew = NULL;				//新节点
	while (tmp != NULL)
	{
		if (func(tmp->data, x) == 0)//用户的比较函数 相等就返回0
		{
			pnew = (Node *)pvPortMalloc(sizeof(Node));
			if (pnew == NULL)
			{
				return -5;
			}
			//新节点赋值
			pnew->data = data;
			//新节点指向tmp
			pnew->next = tmp;
			//pro节点指向新节点 完成入链
			pro->next = pnew;
			//元素个数++
			mylist->size++;
			flag = 1;
			break;
		}
		pro = pro->next;
		tmp = tmp->next;
	}
	if (flag == 0)
	{
		printf("\n指定值前面插入失败，未找到数值\n");
		return -6;
	}

	return 0;
}

//指定值后面插入  在值为x的数据后面插入data   func是比较函数
int ValueRightInsert(void *list, void *x, void *data, int(*func)(void *, void *))
{
	if (list == NULL)
	{
		return -1;
	}
	if (x == NULL)
	{
		return -2;
	}
	if (data == NULL)
	{
		return -3;
	}
	if (func == NULL)
	{
		return -4;
	}
	int flag = 0;//标志位
	LinkList *mylist = (LinkList *)list;
	Node *pro = mylist->head.next;	//首节点
	Node *pnew = NULL;				//新节点
	while (pro != NULL)
	{
		if (func(pro->data, x) == 0)
		{
			pnew = (Node *)pvPortMalloc(sizeof(Node));
			if (pnew == NULL)
			{
				return -5;
			}
			//新节点赋值
			pnew->data = data;
			//新节点指向原来pro指向的地方
			pnew->next = pro->next;
			//pro指向新节点   完成入链
			pro->next = pnew;
			//元素个数+1
			mylist->size++;
			flag = 1;
			break;
		}
		pro = pro->next;
	}
	if (flag == 0)
	{
		printf("\n指定值后面插入失败，未找到数值\n");
		return -6;
	}
	return 0;
}

//指定值删除    删除数据为x的节点		  func是比较函数
int ValueDel(void *list, int *x, int(*func)(void *, void *))
{
	if (list == NULL)
	{
		return -1;
	}
	if (x == NULL)
	{
		return -2;
	}
	if (func == NULL)
	{
		return -3;
	}
	int flag = 0;//标志位
	LinkList *mylist = (LinkList *)list;
	Node *pro = &(mylist->head);	//头节点
	Node *tmp = pro->next;			//pro的下一个节点 此处是首节点
	while (tmp != NULL)
	{
		if (func(tmp->data, x) == 0)
		{
			//断链 pro指向tmp下一个节点
			pro->next = tmp->next;
			//释放掉数值相同的节点
			vPortFree(tmp);
			//元素个数-1
			mylist->size--;
			flag = 1;
			break;
		}
		pro = pro->next;
		tmp = tmp->next;
	}
	if (flag == 0)
	{
		printf("\n指定值删除失败，未找到数值\n");
		return -4;
	}

	return 0;
}

//遍历打印
int PrintList(void *list, void(*PrintFunc)(void *))
{
	if (list == NULL)
	{
		return -1;
	}
	LinkList *mylist = (LinkList *)list;
	Node *tmp = mylist->head.next;	//首节点
	while (tmp != NULL)
	{
		PrintFunc(tmp->data);
		tmp = tmp->next;
	}


	return 0;
}

//获取指定位置元素
void *GetPosValue(void *list, int pos)
{
	if (list == NULL)
	{
		return NULL;
	}
	LinkList *mylist = (LinkList *)list;
	if (pos<0 || pos>mylist->size - 1)
	{
		return NULL;
	}
	Node *pro = &(mylist->head);	//头节点
	int i = 0;
	for (i = 0; i < pos; i++)
	{
		pro = pro->next;
	}
	return pro->next->data;
}

//获取头元素
void *GetHeadValue(void *list)
{
	return GetPosValue(list, 0);
}

//获取尾元素
void *GetTailValue(void *list)
{
	LinkList *mylist = (LinkList *)list;
	return GetPosValue(list, mylist->size - 1);
}

//销毁1   外面list不置空
int FreeList(void *list)
{
	//销毁注意:  &(mylist->head)与list的地址是一样的 所以只能释放一次
	LinkList *mylist = (LinkList *)list;
	Node *pro = mylist->head.next;	//首节点
	Node *tmp = NULL;				//辅助节点
	while (pro != NULL)
	{
		tmp = pro->next;//保存pro的下一个节点
		vPortFree(pro);		//释放掉pro
		pro = tmp;		//让pro等于之前保存的节点
	}
	vPortFree(list);
	list = NULL;
	return 0;
}

//销毁2  外面函数的list也置空
int FreeList2(void **list1)
{
	void *list = *list1;
	//销毁注意:  &(mylist->head)与list的地址是一样的 所以只能释放一次
	LinkList *mylist = (LinkList *)list;
	Node *pro = mylist->head.next;	//首节点
	Node *tmp = NULL;				//辅助节点
	while (pro != NULL)
	{
		tmp = pro->next;//保存pro的下一个节点
		vPortFree(pro);		//释放掉pro
		pro = tmp;		//让pro等于之前保存的下一个节点
	}
	vPortFree(list);
	*list1 = NULL;//外面的list置空
	return 0;
}