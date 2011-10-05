#include <stdlib.h>
#include "soko_pos_operation.h"
#include "sokopush.h"
//用递归的方式求出人能走到的列表
struct mappos * personCanWalkPoint(struct sokomap *map)
{
	int x = map->person_x;
	int y = map->person_y;
	struct mappos * head = (struct mappos *)malloc(sizeof(struct mappos));
	head->x = x;
	head->y = y;
	head->next = NULL;
	//查看是否能向上走
	struct mappos * pointer = head;
	struct mappos * tail = head;
	while(pointer)
	{
		int x = pointer -> x;
		int y = pointer -> y;
		if(isEmpty(map,x,y-1) && (isInMappos(head,x,y-1) != 1))
		{
			struct mappos * newPointer = (struct mappos *)malloc(sizeof(struct mappos));
			newPointer -> x = x;
			newPointer -> y = y - 1;
			newPointer -> next = NULL;
			tail -> next = newPointer;
			//吧新生成的节点赋值给尾部
			tail = newPointer;
		}
		if(isEmpty(map,x,y+1) && (isInMappos(head,x,y+1) != 1))
		{
			struct mappos * newPointer = (struct mappos *)malloc(sizeof(struct mappos));
			newPointer -> x = x;
			newPointer -> y = y + 1;
			newPointer -> next = NULL;
			tail -> next = newPointer;
			//吧新生成的节点赋值给尾部
			tail = newPointer;
		}
		if(isEmpty(map,x-1,y) && (isInMappos(head,x-1,y) != 1))
		{
			struct mappos * newPointer = (struct mappos *)malloc(sizeof(struct mappos));
			newPointer -> x = x-1;
			newPointer -> y = y;
			newPointer -> next = NULL;
			tail -> next = newPointer;
			//吧新生成的节点赋值给尾部
			tail = newPointer;
		}
		if(isEmpty(map,x+1,y) && (isInMappos(head,x+1,y) != 1))
		{
			struct mappos * newPointer = (struct mappos *)malloc(sizeof(struct mappos));
			newPointer -> x = x+1;
			newPointer -> y = y;
			newPointer -> next = NULL;
			tail -> next = newPointer;
			//吧新生成的节点赋值给尾部
			tail = newPointer;
		}
		pointer = pointer -> next;
	}
	return head;
}
//判断是否在在列表中
int isInMappos(struct mappos * header,int x,int y)
{
	while(header)
	{
		if((header->x==x) && (header-> y == y))
		{
			return 1;
		}
		header = header -> next;
	}
	return 0;
}
//回收所有的节点
void freeMappos(struct mappos * pos)
{
	struct mappos * pointer;
	while(pos)
	{
		pointer = pos -> next;
		free(pos);
		pos = pointer;
	}
}
