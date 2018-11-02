#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Node{
	void *data;
	struct Node *next;
}Node;

typedef struct LinkList{
	Node head;
	int size;
}LinkList;

//链表初始化
void *InitLinkList(void);

//指定位置插入
int InsertLinkList(void *list, int pos, void *data);

//头插
int InsertHeadLinkList(void *list, void *data);

//尾插
int InsertTailLinkList(void *list, void *data);

//指定位置删除
int RemoveLinkList(void *list, int pos);

//头删
int RemoveHead(void *list);

//尾删
int RemoveTail(void *list);

//指定值前面插入  在值为x的数据前面插入data   func是比较函数
int ValueLeftInsert(void *list, void *x, void *data, int(*func)(void *, void *));

//指定值后面插入  在值为x的数据后面插入data   func是比较函数
int ValueRightInsert(void *list, void *x, void *data, int(*func)(void *, void *));

//指定值删除    删除数据为data的节点
int ValueDel(void *list, int *x, int(*func)(void *, void *));

//遍历打印
int PrintList(void *list, void(*PrintFunc)(void *));

//获取指定位置元素
void *GetPosValue(void *list, int pos);

//获取头元素
void *GetHeadValue(void *list);

//获取尾元素
void *GetTailValue(void *list);

//销毁1   外面list不置空
int FreeList(void *list);

//销毁2  外面函数的list也置空
int FreeList2(void **list);
