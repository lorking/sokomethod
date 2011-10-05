#include <stdio.h>
#include "debug.h"
#include "sokopush.h"
#include "soko_map_operation.h"
#include "test.h"
#include "soko_pos_operation.h"
int main()
{
	struct sokomap map;
	char mapBuffer[8][8];
	map.width = 10;
	memSetSokoMap(&map,mapBuffer,8,8);
	for(int i=0;i < 8;i++)
	{
		setWall_xy(&map,i,0);
		setWall_xy(&map,i,7);
		setWall_xy(&map,0,i);
		setWall_xy(&map,7,i);
	}
	setPerson_xy(&map,2,2);
	setBox_xy(&map,3,3);
	setDest_xy(&map,6,6);
	struct sokomap * retMap = caculatePath(&map);
	while(retMap)
	{
		printf("success!\nx:%d,y:%d",retMap->person_x,retMap->person_y);
		retMap = retMap->next;
	}
	return 0;
}
