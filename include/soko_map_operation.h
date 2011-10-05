#ifndef _SOKO_MAP_OPERATION_H_
#define _SOKO_MAP_OPERATION_H_
#include "sokopush.h"
/*回收内存*/
void freeMap(struct sokomap * map);
enum direction{UP,DOWN,LEFT,RIGHT};
/*计算到达的列表*/
struct sokomap * caculatePath(struct sokomap *map); 
/*是否在地图列表中*/
int isInSokoMap(struct sokomap * header,struct sokomap * map); 
#endif
