#include <stdlib.h>
#include <stdio.h>
#include "soko_map_operation.h"
#include "sokopush.h"
#include "soko_pos_operation.h"
enum direction{UP,DOWN,LEFT,RIGHT};
//拷贝一个地图
struct sokomap * copyMap(struct sokomap map)
{
	struct sokomap * currentHeader;
	currentHeader = (struct sokomap *)malloc(sizeof(struct sokomap));
	currentHeader->width = map.width;
	currentHeader->hight = map.hight;
	currentHeader -> next = NULL;
	currentHeader -> parent = NULL;
	currentHeader -> pos = NULL;
	currentHeader -> person_x = map.person_x;
	currentHeader -> person_y = map.person_y;
	currentHeader -> array = (char *)malloc(sizeof(char) * map.width * map.hight);
	memcpy(currentHeader -> array,map.array,map.width * map.hight);
	return currentHeader;
}
//判断地图是否为死图
int isMapDead(struct sokomap *header,int x,int y,enum direction direct)
{
	switch(direct)
	{
		case UP:
			if(isWall(*header,x,y-1) != 1 )//上方为墙
			{
				if(isDest(*header,x,y) != 1)//箱子不在目标地址上
				{
					//任意两边为墙地图肯定棍了
					if(isWall(*header,x-1,y) || isWall(*header,x+1,y))
					{
						return 1;
					}
					//左上角为墙，同时左边为箱子，也同样的棍
					if(isWall(*header,x-1,y-1) && isBox(*header,x-1,y))
					{
						return 1;
					}
					//右上角为墙,右边为箱子，也棍
					if(isWall(*header,x+1,y-1) && isBox(*header,x+1,y))
					{
						return 1;
					}
				}
				//上边一排为墙，此时查看这一排的箱子数量，目标数量，是否相符
				int flag = 0;//标志位
				int boxNum = 0,destNum = 0;
				//先向左查询
				int count = x;
				for(;count >=0;count--)
				{
					if(isWall(*header,count,y-1) != 1)//当不为墙时查询它下边的是否为墙
					{
						if(isWall(*header,count,y) != 1)//此时下边也不为墙，则死图的条件不成立,跳出次条件判断
						{
							flag = 1;
							break;
						}else
						{
							break;
						}
					}else//当下边为墙时，跳出循环
					{
						if(isWall(*header,count,y) == 1)
						{
							break;
						}else
						{
							if(isDest(*header,count,y))
							{
								destNum ++;
							}
							if(isBox(*header,count,y))
							{
								boxNum ++;
							}
						}
					}
				}
				if(flag)
				{
					break;
				}
				//再向右查寻
				flag = 0;
				count = x + 1;
				for(;count < header -> width;count ++)
				{
					if(isWall(*header,count,y-1) != 1)//当不为墙时查询它下边的是否为墙
					{
						if(isWall(*header,count,y) != 1)//此时下边也不为墙，则死图的条件不成立,跳出次条件判断
						{
							flag = 1;
							break;
						}else
						{
							break;
						}
					}else//当下边为墙时，跳出循环
					{
						if(isWall(*header,count,y) == 1)
						{
							break;
						}else
						{
							if(isDest(*header,count,y))
							{
								destNum ++;
							}
							if(isBox(*header,count,y))
							{
								boxNum ++;
							}
						}
					}
				}
				if(flag)
				{
					break;
				}
				if(boxNum > destNum)//箱子数量大于目的数量时，证明此地图已经棍了
				{
					return 1;
				}		
			}
			break;	
		case DOWN:
			if(isWall(*header,x,y+1) != 1 )//下方为墙
			{
				if(isDest(*header,x,y) != 1)//箱子不在目标地址上
				{
					//任意两边为墙地图肯定棍了
					if(isWall(*header,x-1,y) || isWall(*header,x+1,y))
					{
						return 1;
					}
					//左下角为墙，同时左边为箱子，也同样的棍
					if(isWall(*header,x-1,y+1) && isBox(*header,x-1,y))
					{
						return 1;
					}
					//右下角为墙,右边为箱子，也棍
					if(isWall(*header,x+1,y+1) && isBox(*header,x+1,y))
					{
						return 1;
					}
				}
				//上边一排为墙，此时查看这一排的箱子数量，目标数量，是否相符
				int flag = 0;//标志位
				int boxNum = 0,destNum = 0;
				//先向左查询
				int count = x;
				for(;count >=0;count--)
				{
					if(isWall(*header,count,y+1) != 1)//当不为墙时查询它下边的是否为墙
					{
						if(isWall(*header,count,y) != 1)//此时下边也不为墙，则死图的条件不成立,跳出次条件判断
						{
							flag = 1;
							break;
						}else
						{
							break;
						}
					}else//当下边为墙时，跳出循环
					{
						if(isWall(*header,count,y) == 1)
						{
							break;
						}else
						{
							if(isDest(*header,count,y))
							{
								destNum ++;
							}
							if(isBox(*header,count,y))
							{
								boxNum ++;
							}
						}
					}
				}
				if(flag)
				{
					break;
				}
				//再向右查寻
				flag = 0;
				count = x + 1;
				for(;count < header -> width;count ++)
				{
					if(isWall(*header,count,y+1) != 1)//当不为墙时查询它下边的是否为墙
					{
						if(isWall(*header,count,y) != 1)//此时下边也不为墙，则死图的条件不成立,跳出次条件判断
						{
							flag = 1;
							break;
						}else
						{
							break;
						}
					}else//当下边为墙时，跳出循环
					{
						if(isWall(*header,count,y) == 1)
						{
							break;
						}else
						{
							if(isDest(*header,count,y))
							{
								destNum ++;
							}
							if(isBox(*header,count,y))
							{
								boxNum ++;
							}
						}
					}
				}
				if(flag)
				{
					break;
				}
				if(boxNum > destNum)//箱子数量大于目的数量时，证明此地图已经棍了
				{
					return 1;
				}		
			}
			break;
		case LEFT:
			if(isWall(*header,x-1,y) != 1 )//左方为墙
			{
				if(isDest(*header,x,y) != 1)//箱子不在目标地址上
				{
					//任意两边为墙地图肯定棍了
					if(isWall(*header,x,y - 1) || isWall(*header,x,y+1))
					{
						return 1;
					}
					if(isWall(*header,x-1,y -1) && isBox(*header,x,y-1))
					{
						return 1;
					}
					if(isWall(*header,x -1,y+1) && isBox(*header,x,y + 1))
					{
						return 1;
					}
				}
				//上边一排为墙，此时查看这一排的箱子数量，目标数量，是否相符
				int flag = 0;//标志位
				int boxNum = 0,destNum = 0;
				//先向上查询
				int count = y;
				for(;count >=0;count--)
				{
					if(isWall(*header,x - 1,count) != 1)//当不为墙时查询它右边的是否为墙
					{
						if(isWall(*header,x,count) != 1)//此时右边也不为墙，则死图的条件不成立,跳出次条件判断
						{
							flag = 1;
							break;
						}else
						{
							break;
						}
					}else//当右边为墙时，跳出循环
					{
						if(isWall(*header,x,count) == 1)
						{
							break;
						}else
						{
							if(isDest(*header,x,count))
							{
								destNum ++;
							}
							if(isBox(*header,x,count))
							{
								boxNum ++;
							}
						}
					}
				}
				if(flag)
				{
					break;
				}
				//再向下查寻
				flag = 0;
				count = y + 1;
				for(;count < header -> hight;count ++)
				{
					if(isWall(*header,x - 1,count) != 1)//当不为墙时查询它下边的是否为墙
					{
						if(isWall(*header,x,count) != 1)//此时下边也不为墙，则死图的条件不成立,跳出次条件判断
						{
							flag = 1;
							break;
						}else
						{
							break;
						}
					}else//当下边为墙时，跳出循环
					{
						if(isWall(*header,x,count) == 1)
						{
							break;
						}else
						{
							if(isDest(*header,x,count))
							{
								destNum ++;
							}
							if(isBox(*header,x,count))
							{
								boxNum ++;
							}
						}
					}
				}
				if(flag)
				{
					break;
				}
				if(boxNum > destNum)//箱子数量大于目的数量时，证明此地图已经棍了
				{
					return 1;
				}		
			}
			break;
		case RIGHT:
			if(isWall(*header,x+1,y) != 1 )//右方为墙
			{
				if(isDest(*header,x,y) != 1)//箱子不在目标地址上
				{
					//任意两边为墙地图肯定棍了
					if(isWall(*header,x,y - 1) || isWall(*header,x,y+1))
					{
						return 1;
					}
					if(isWall(*header,x+1,y -1) && isBox(*header,x,y-1))
					{
						return 1;
					}
					if(isWall(*header,x + 1,y+1) && isBox(*header,x,y + 1))
					{
						return 1;
					}
				}
				//上边一排为墙，此时查看这一排的箱子数量，目标数量，是否相符
				int flag = 0;//标志位
				int boxNum = 0,destNum = 0;
				//先向上查询
				int count = y;
				for(;count >=0;count--)
				{
					if(isWall(*header,x + 1,count) != 1)//当不为墙时查询它右边的是否为墙
					{
						if(isWall(*header,x,count) != 1)//此时右边也不为墙，则死图的条件不成立,跳出次条件判断
						{
							flag = 1;
							break;
						}else
						{
							break;
						}
					}else//当右边为墙时，跳出循环
					{
						if(isWall(*header,x,count) == 1)
						{
							break;
						}else
						{
							if(isDest(*header,x,count))
							{
								destNum ++;
							}
							if(isBox(*header,x,count))
							{
								boxNum ++;
							}
						}
					}
				}
				if(flag)
				{
					break;
				}
				//再向下查寻
				flag = 0;
				count = y + 1;
				for(;count < header -> hight;count ++)
				{
					if(isWall(*header,x + 1,count) != 1)//当不为墙时查询它下边的是否为墙
					{
						if(isWall(*header,x,count) != 1)//此时下边也不为墙，则死图的条件不成立,跳出次条件判断
						{
							flag = 1;
							break;
						}else
						{
							break;
						}
					}else//当左边为墙时，跳出循环
					{
						if(isWall(*header,x,count) == 1)
						{
							break;
						}else
						{
							if(isDest(*header,x,count))
							{
								destNum ++;
							}
							if(isBox(*header,x,count))
							{
								boxNum ++;
							}
						}
					}
				}
				if(flag)
				{
					break;
				}
				if(boxNum > destNum)//箱子数量大于目的数量时，证明此地图已经棍了
				{
					return 1;
				}		
			}
			break;
	}
	return 0;
}
/*
int isMapDead_allderiction(struct sokomap *header,int x,int y)
{
	if(isMapDead(header,x,y,))
}
*/
//增加下一个地图的步骤
void addNxtStep(struct sokomap *header,struct sokomap **currentTail,struct sokomap *current,struct mappos * boxPos,int boxCount)
{
	//计算人所在的列表
	if(current->pos==NULL)
	{
		current->pos = personCanWalkPoint(*current);
	}
	getBoxList(*current,boxPos);
	//进行循环判断箱子可以移动的方向
	for(int i=0;i < boxCount;i++)
	{
		int x = boxPos[i].x;
		int y = boxPos[i].y;
		//向上推的操作
		if(isCanPushUp(*current,x,y))
		{	
			if(isInMappos(current->pos,boxPos[i].x,boxPos[i].y+1))
			{
				struct sokomap * tmpMap = copyMap(*current);
				tmpMap ->parent = current;
				pushUp(*tmpMap,x,y);
				//判断地图是否已经棍了,没棍的话就加入到尾步
				if(isMapDead(tmpMap,x,y+1,UP))
				{
					freeMap(tmpMap);
				}else if(isInSokoMap(header,tmpMap))//已经在列表中，就把这个新地图删除
				{
					freeMap(tmpMap);
				}else
				{
					(*currentTail)->next = tmpMap;
					*currentTail = tmpMap;
				}
			}
		}
		//向下推的操作
		if(isCanPushDown(*current,boxPos[i].x,boxPos[i].y))
		{
			if(isInMappos(current->pos,boxPos[i].x,boxPos[i].y - 1))
			{
				struct sokomap * tmpMap = copyMap(*current);
				tmpMap ->parent = current;
				pushDown(*tmpMap,x,y);
				//判断地图是否已经棍了,没棍的话加入到尾部
				if(isMapDead(tmpMap,x,y-1,DOWN))
				{
					freeMap(tmpMap);
				}else if(isInSokoMap(header,tmpMap))//已经在列表中，就把这个新地图删除
				{
					freeMap(tmpMap);
				}else
				{
					(*currentTail)->next = tmpMap;
					*currentTail = tmpMap;
				}
			}
		}
		//向左推的操作
		if(isCanPushLeft(*current,boxPos[i].x,boxPos[i].y))
		{
			if(isInMappos(current->pos,boxPos[i].x + 1,boxPos[i].y))
			{
				struct sokomap * tmpMap = copyMap(*current);
				tmpMap ->parent = current;
				pushLeft(*tmpMap,x,y);
				//判断地图是否已经棍了,没棍的话加入到尾部
				if(isMapDead(tmpMap,x - 1,y,LEFT))
				{
					freeMap(tmpMap);
				}else if(isInSokoMap(header,tmpMap))//已经在列表中，就把这个新地图删除
				{
					freeMap(tmpMap);
				}else
				{
					(*currentTail)->next = tmpMap;
					*currentTail = tmpMap;
				}
			}
		}
		//向右推的操作
		if(isCanPushRight(*current,boxPos[i].x,boxPos[i].y))
		{
			if(isInMappos(current->pos,boxPos[i].x - 1,boxPos[i].y))
			{
				struct sokomap * tmpMap = copyMap(*current);
				tmpMap ->parent = current;
				pushRight(*tmpMap,x,y);
				//判断地图是否已经棍了,没棍的话加入到尾部
				if(isMapDead(tmpMap,x + 1,y,RIGHT))
				{
					freeMap(tmpMap);
				}else if(isInSokoMap(header,tmpMap))//已经在列表中，就把这个新地图删除
				{
					freeMap(tmpMap);
				}else
				{
					(*currentTail)->next = tmpMap;
					*currentTail = tmpMap;
				}
			}
		}
	}
}
//计算地图的列表
struct sokomap * caculatePath(struct sokomap map)
{
	struct sokomap *current,*currentHeader;//当前各个地图列表,failHeader失败的列表
	struct sokomap *retSokomap = NULL;//要返回的列表
	struct sokomap **currentTail;
	int box_count,wall_count,person_count,dest_count;
	mapProperyCount(map,&wall_count,&box_count,&person_count,&dest_count);//计算箱子的个数
	struct mappos * boxPos = (struct mappos*) malloc(sizeof(struct mappos)*box_count);
	//生成头节点
	currentHeader = copyMap(map);
	//进行循环的操作
	current = currentHeader;
	*currentTail = current;
	//循环的操作
	int flag = 0;
	while(current)
	{
		if(isMapSuccess(*current))
		{
			flag = 1;
			break;
		}
		addNxtStep(currentHeader,currentTail,current,boxPos,box_count);
		current = current -> next;
	}
	if(flag)
	{
		//成功的地图，进行一些必要的保存步骤的操作
		struct mappos * tmpSokoPtr = NULL;
		while(current)
		{
			retSokomap = copyMap(*current);
			retSokomap->next = tmpSokoPtr;
			tmpSokoPtr = retSokomap;
			current = current -> parent;
		}
	}
	//清除内存的操作
	freeMap(currentHeader);
	return retSokomap;
}
//回收列表
void freeMap(struct sokomap * map)
{
	struct sokomap * pointer;
	while(map)
	{
		pointer = map ->next;
		freeMappos(map->pos);//释放地图的列表
		free(map->array);
		free(map);
		map = pointer;
	}
}
//判断地图是否在列表中
int isInSokoMap(struct sokomap * header,struct sokomap * map)
{
	while(header)
	{
		//先判断箱子所在的位置相等,并且人在的位置在可以走到的列表中，认为相等
		if(cmpMapBoxIsEqual(*header,*map))
		{
			// 计算人可以走到的位置列表
			if(header->pos==NULL)
			{
				header->pos = personCanWalkPoint(*header);
			}
			if(isInMappos(header->pos,map->person_x,map->person_y))
			{
				return 1;
			}
		}
		header = header -> next;
	}
	return 0;
}
