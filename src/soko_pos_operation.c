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
//地图是否闭合
int isMapClose(struct sokomap *map)
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
		if(x == 0 || x == map->width || y == 0 || y == map ->hight)
		{
			freeMappos(head);
			return 0;
		} 
		if((isWall(map,x,y-1) != 1) && (isInMappos(head,x,y-1) != 1))
		{
			struct mappos * newPointer = (struct mappos *)malloc(sizeof(struct mappos));
			newPointer -> x = x;
			newPointer -> y = y - 1;
			newPointer -> next = NULL;
			tail -> next = newPointer;
			//吧新生成的节点赋值给尾部
			tail = newPointer;
		}
		if((isWall(map,x,y+1)  != 1) && (isInMappos(head,x,y+1) != 1))
		{
			struct mappos * newPointer = (struct mappos *)malloc(sizeof(struct mappos));
			newPointer -> x = x;
			newPointer -> y = y + 1;
			newPointer -> next = NULL;
			tail -> next = newPointer;
			//吧新生成的节点赋值给尾部
			tail = newPointer;
		}
		if((isWall(map,x-1,y) != 1) && (isInMappos(head,x - 1,y) != 1))
		{
			struct mappos * newPointer = (struct mappos *)malloc(sizeof(struct mappos));
			newPointer -> x = x-1;
			newPointer -> y = y;
			newPointer -> next = NULL;
			tail -> next = newPointer;
			//吧新生成的节点赋值给尾部
			tail = newPointer;
		}
		if((isWall(map,x+1,y) != 1) && (isInMappos(head,x + 1,y) != 1))
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
	freeMappos(head);
	return 1;
}
struct mappos * personWalkPath(struct sokomap *beginMap,struct sokomap *endMap)
{
	int begin_person_x = beginMap -> person_x;
	int begin_person_y = beginMap -> person_y;
	int end_x = endMap -> person_x;
	int end_y = endMap -> person_y;
	//左边是箱子，第一个不是箱子，即是向左推，人应该在右边
	if(isBox(endMap,end_x -1,end_y)==1 && isBox(beginMap,end_x - 1,end_y)!=1)
	{
		end_x ++;
	}else if(isBox(endMap,end_x + 1,end_y)==1 && isBox(beginMap,end_x + 1,end_y)!=1)
	{
		end_x --;
	}else if(isBox(endMap,end_x,end_y + 1)==1 && isBox(beginMap,end_x,end_y + 1)!=1)
	{
		end_y --;
	}else
	{
		end_y ++;
	}
	//开始计算人走过的路径
	struct mappos * head = (struct mappos *)malloc(sizeof(struct mappos));
	head->x = begin_person_x;
	head->y = begin_person_y;
	head->next = NULL;
	head->parent = NULL;
	//查看是否能向上走
	struct mappos * pointer = head;
	struct mappos * tail = head;
	while(pointer)
	{
		int x = pointer -> x;
		int y = pointer -> y;
		if(x==end_x && y == end_y)
		{
			break;
		}
		if(isEmpty(beginMap,x,y-1) && (isInMappos(head,x,y-1) != 1))
		{
			struct mappos * newPointer = (struct mappos *)malloc(sizeof(struct mappos));
			newPointer -> parent = pointer;
			newPointer -> x = x;
			newPointer -> y = y - 1;
			newPointer -> next = NULL;
			tail -> next = newPointer;
			//吧新生成的节点赋值给尾部
			tail = newPointer;
		}
		if(isEmpty(beginMap,x,y+1) && (isInMappos(head,x,y+1) != 1))
		{
			struct mappos * newPointer = (struct mappos *)malloc(sizeof(struct mappos));
			newPointer -> parent = pointer;
			newPointer -> x = x;
			newPointer -> y = y + 1;
			newPointer -> next = NULL;
			tail -> next = newPointer;
			//吧新生成的节点赋值给尾部
			tail = newPointer;
		}
		if(isEmpty(beginMap,x-1,y) && (isInMappos(head,x-1,y) != 1))
		{
			struct mappos * newPointer = (struct mappos *)malloc(sizeof(struct mappos));
			newPointer -> parent = pointer;
			newPointer -> x = x-1;
			newPointer -> y = y;
			newPointer -> next = NULL;
			tail -> next = newPointer;
			//吧新生成的节点赋值给尾部
			tail = newPointer;
		}
		if(isEmpty(beginMap,x+1,y) && (isInMappos(head,x+1,y) != 1))
		{
			struct mappos * newPointer = (struct mappos *)malloc(sizeof(struct mappos));
			newPointer -> parent = pointer;
			newPointer -> x = x+1;
			newPointer -> y = y;
			newPointer -> next = NULL;
			tail -> next = newPointer;
			//吧新生成的节点赋值给尾部
			tail = newPointer;
		}
		pointer = pointer -> next;
	}
	//进行反推的操作
	struct mappos * retStr = NULL;
	struct mappos * tmp_pos = NULL;
	while(pointer)
	{
		retStr = (struct mappos *)malloc(sizeof(struct mappos));
		retStr->x = pointer->x;
		retStr->y = pointer->y;
		retStr->next = tmp_pos;

		tmp_pos = retStr;
		pointer = pointer -> parent;
	}
	freeMappos(head);
	return retStr;
}
