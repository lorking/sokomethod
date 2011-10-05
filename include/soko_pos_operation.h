#ifndef _SOKO_POS_OPERATION_H_
#define _SOKO_POS_OPERATION_H_
#include "sokopush.h"
/*人能走到的空闲位置的列表*/
struct mappos * personCanWalkPoint(struct sokomap *map);
/*回收所能走到的箱子的列表*/
void freeMappos(struct mappos * pos);
/*判断x,y是否在列表中*/
int isInMappos(struct mappos * header,int x,int y);
#endif
