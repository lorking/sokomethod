#include <stdio.h>
#include "debug.h"
#include "sokopush.h"
//是否能向左推
int isCanPushLeft(struct sokomap *map,int x,int y)
{
	if(isValidPosition(map,x,y) != 1)
	{
		return 0;
	}
	//当前位置是否箱子
	if(isBox(map,x,y) != 1)
	{
		return 0;
	}
	//当前的位置,设置临时变量
	if(isValidPosition(map,x -1,y) && isEmpty(map,x-1,y))
	{
		return 1;
	}
	return 0;
}

int pushLeft(struct sokomap *map,int x,int y)
{
	int left_pos = paraPosition2Index(map,x-1,y);
	setBox(map->array,left_pos);
	int current_pos = paraPosition2Index(map,x,y);
	clearBox(map->array,current_pos);
	//清除人的位置
	int person_pos = paraPosition2IndexByXY(map,map->person_x,map->person_y);
	clearPerson(map->array,person_pos);
	setPerson(map->array,current_pos);
	map->person_x = x;
	map->person_y = y;
	return 1;
}

int isCanPushRight(struct sokomap *map,int x,int y)
{
	if(isValidPosition(map,x,y) != 1)
	{
		return 0;
	}
	//当前位置是否箱子
	if(isBox(map,x,y) != 1)
	{
		return 0;
	}
	if(isValidPosition(map,x+1,y) && isEmpty(map,x+1,y))
	{
		return 1;
	}
	return 0;
}

int pushRight(struct sokomap *map,int x,int y)
{
	int left_pos = paraPosition2Index(map,x +1,y);
	setBox(map->array,left_pos);
	int current_pos = paraPosition2Index(map,x,y);
	clearBox(map->array,current_pos);
	//清除人的位置
	int person_pos = paraPosition2IndexByXY(map,map->person_x,map->person_y);
	clearPerson(map->array,person_pos);
	setPerson(map->array,current_pos);
	map->person_x = x;
	map->person_y = y;
	return 1;
}

int isCanPushUp(struct sokomap *map,int x,int y)
{
	if(isValidPosition(map,x,y) != 1)
	{
		return 0;
	}
	//当前位置是否箱子
	if(isBox(map,x,y) != 1)
	{
		return 0;
	}
	if(isValidPosition(map,x,y-1) && isEmpty(map,x,y-1))
	{
		return 1;
	}
	return 0;
}

int pushUp(struct sokomap *map,int x,int y)
{
	int left_pos = paraPosition2Index(map,x,y-1);
	setBox(map->array,left_pos);
	int current_pos = paraPosition2Index(map,x,y);
	clearBox(map->array,current_pos);
	//清除人的位置
	int person_pos = paraPosition2IndexByXY(map,map->person_x,map->person_y);
	clearPerson(map->array,person_pos);
	setPerson(map->array,current_pos);
	map->person_x = x;
	map->person_y = y;
	return 1;
}

int isCanPushDown(struct sokomap *map,int x,int y)
{
	if(isValidPosition(map,x,y) != 1)
	{
		return 0;
	}
	//当前位置是否箱子
	if(isBox(map,x,y) != 1)
	{
		return 0;
	}
	//当前的位置,设置临时变量
	if(isValidPosition(map,x,y+1) && isEmpty(map,x,y+1))
	{
		return 1;
	}
	return 0;
}

int pushDown(struct sokomap *map,int x,int y)
{
	int left_pos = paraPosition2Index(map,x,y+1);
	setBox(map->array,left_pos);
	int current_pos = paraPosition2Index(map,x,y);
	clearBox(map->array,current_pos);
	//清除人的位置
	int person_pos = paraPosition2IndexByXY(map,map->person_x,map->person_y);
	clearPerson(map->array,person_pos);
	setPerson(map->array,current_pos);
	map->person_x = x;
	map->person_y = y;
	return 1;
}

int isValidPosition(struct sokomap *map,int x,int y)
{
	if((x > map->width -1) || x < 0 || (y > map->hight - 1) || y < 0)
	{
		return 0;
	}
	return 1;
}

int paraPosition2Index(struct sokomap *map,int x,int y)
{
	int position = y * map->width + x;
	return position;
}
int paraPosition2IndexByXY(struct sokomap *map,int x,int y)
{
	int position = y * map->width + x;
	return position;
}
int isWall(struct sokomap *map,int x,int y)
{
	if(isValidPosition(map,x,y) != 1)
	{
		return 0;
	}
	int posIndex = paraPosition2Index(map,x,y);
	char posValue = map->array[posIndex];
	return isWall_value(posValue);
}
int isWall_value(char value)
{
	if((value & 0x1) != 0)
	{
		return 1;
	}else
	{
		return 0;
	}
}
int isBox_value(char value)
{
	if((value & 0x2) != 0)
	{
		return 1;
	}else
	{
		return 0;
	}
}

int isBox(struct sokomap *map,int x,int y)
{
	if(isValidPosition(map,x,y) != 1)
	{
		return 0;
	}
	int posIndex = paraPosition2Index(map,x,y);
	char posValue = map->array[posIndex];
	return isBox_value(posValue);
}

int isPerson_value(char value)
{
	if((value & 0x4) != 0)
	{
		return 1;
	}else
	{
		return 0;
	}
}
int isPerson(struct sokomap *map,int x,int y)
{
	if(isValidPosition(map,x,y) != 1)
	{
		return 0;
	}
	int posIndex = paraPosition2Index(map,x,y);
	char posValue = map->array[posIndex];
	return isPerson_value(posValue);
}

int isDest_value(char value)
{
	if((value & 0x8) != 0)
	{
		return 1;
	}else
	{
		return 0;
	}
}

int isDest(struct sokomap *map,int x,int y)
{
	if(isValidPosition(map,x,y) != 1)
	{
		return 0;
	}
	int posIndex = paraPosition2Index(map,x,y);
	char posValue = map->array[posIndex];
	return isDest_value(posValue);
}

int isEmpty(struct sokomap *map,int x,int y)
{
	if((isValidPosition(map,x,y) ==1) && (isWall(map,x,y) !=1) && (isBox(map,x,y) !=1))
	{
		return 1;
	}else
	{
		return 0;
	}
}

//是否为有效的地图
int isValidMap(struct sokomap *map)
{
	return 1;
}

void setValue(char * buf,int position,char markValue)
{
	buf[position] = buf[position] | markValue;
}
void clearValue(char * buf,int position,char markValue)
{
	buf[position] = buf[position] & (~markValue);
}

void setWall(char *buf,int position)
{
	setValue(buf,position,0x1);
}
void clearWall(char *buf,int position)
{
	clearValue(buf,position,0x1);
}

void setBox(char *buf,int position)
{
	setValue(buf,position,0x2);
}
void clearBox(char *buf,int position)
{
	clearValue(buf,position,0x2);
}

void setPerson(char * buf,int position)
{
	setValue(buf,position,0x4);
}
void clearPerson(char *buf,int position)
{
	clearValue(buf,position,0x4);
}

void setDest(char * buf,int position)
{
	setValue(buf,position,0x8);
}

void clearDest(char *buf,int position)
{
	clearValue(buf,position,0x8);
}

void mapProperyCount(struct sokomap *map,int * wallCount,int * boxCount,int * person,int * dest)
{
	int count = (int)map->width * (int)map->hight;
	int wall_count = 0,box_count = 0,person_count = 0,dest_count = 0;
	for(int i = 0;i < count;i++)
	{
		if(isWall_value(map->array[i]))
		{
			wall_count ++;
		}
		if(isBox_value(map->array[i]))
		{
			box_count ++;
		}
		if(isPerson_value(map->array[i]))
		{
			person_count ++;
		}
		if(isDest_value(map->array[i]))
		{
			dest_count ++;
		}
	}
	* wallCount = wall_count;
	* boxCount = box_count;
	* person = person_count;
	* dest = dest_count;
}

int isMapSuccess(struct sokomap *map)
{
	int count = (int)map->width * (int)map->hight;
	//如果有一个位置为目的地址，上面没有箱子就认为还没成功
	for(int i=0;i < count;i ++)
	{	
		if(isDest_value(map->array[i]))
		{
			if(isBox_value(map->array[i]) != 1)
			{
				return 0;
			}
		}
	}
	return 1;
}

int cmpMapBoxIsEqual(struct sokomap *map1,struct sokomap *map2)
{
	int count = (int)map1->width * (int)map1->hight;
	for(int i=0;i < count;i++)
	{
		if(isBox_value(map1->array[i]))
		{
			if(isBox_value(map2->array[i])!=1)
			{
				return 0;
			}
		}
	}
	return 1;
}

void getBoxList(struct sokomap *map,struct mappos *pos)
{
	int i = 0;
	for(int y=0;y < map->hight;y++)
	{
		for(int x=0;x < map->width;x++)
		{
			if(isBox(map,x,y))
			{
				pos[i].x = x;
				pos[i].y = y;
				i++;
			}
		}
	}
}

void memSetSokoMap(struct sokomap * map,char * array,int width,int height)
{
	map->array = array;
	memset(map->array,0,width*height);
	map->width = width;
	map->hight = height;
	map->next = NULL;
	map->parent = NULL;
	map->person_x = -1;
	map->person_y = -1;
}

void setBox_xy(struct sokomap *map,int x,int y)
{
	if(isPerson(map,x,y) || isWall(map,x,y))
		return;
	int position = paraPosition2Index(map,x,y);
	setBox(map->array,position);
}

void setDest_xy(struct sokomap *map,int x,int y)
{
	if(isWall(map,x,y))
		return;
	int position = paraPosition2Index(map,x,y);
	setDest(map->array,position);
}

void setWall_xy(struct sokomap *map,int x,int y)
{
	if(isPerson(map,x,y) || isDest(map,x,y) || isBox(map,x,y))
		return;
	int position = paraPosition2Index(map,x,y);
	setWall(map->array,position);
}

void setPerson_xy(struct sokomap *map,int x,int y)
{
	if(isWall(map,x,y) || isBox(map,x,y))
		return;
	int person_x = map->person_x;
	int person_y = map->person_y;
	if((person_x != -1) && (person_y != -1))
	{
		int position = paraPosition2Index(map,person_x,person_y);
		clearPerson(map->array,position);
	}
	int position = paraPosition2Index(map,x,y);
	setPerson(map->array,position);
	map->person_x = x;
	map->person_y = y;
}

void clearBox_xy(struct sokomap *map,int x,int y)
{
	int position = paraPosition2Index(map,x,y);
	clearBox(map->array,position);
}

void clearDest_xy(struct sokomap *map,int x,int y)
{
	int position = paraPosition2Index(map,x,y);
	clearDest(map->array,position);
}

void clearWall_xy(struct sokomap *map,int x,int y)
{
	int position = paraPosition2Index(map,x,y);
	clearWall(map->array,position);
}

void clearPerson_xy(struct sokomap *map,int x,int y)
{
	int position = paraPosition2Index(map,x,y);
	clearPerson(map->array,position);
	map->person_x = -1;
	map->person_y = -1;
}
