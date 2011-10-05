#ifndef _SOKO_PUSH_H_
#define _SOKO_PUSH_H_
/*存储位置的结构体*/
struct mappos
{
	unsigned char x;//横坐标
	unsigned char y;//纵坐标
	struct mappos * next;//下一个节点
};
/*存储推箱子地图的存储结构,char 从低bit到高bit为1依次为墙,箱子,人,目标*/
struct sokomap
{
	unsigned char width;//地图的宽度
	unsigned char hight;//地图的高度
	char * array;//用来存储地图的列表
	struct sokomap * next;//下一个地图的指针
	struct sokomap * parent;//父节点的指针
	unsigned char person_x,person_y;//人所在的坐标
	struct mappos * pos;
};
/*能否向左推*/
int isCanPushLeft(struct sokomap map,int x,int y);
/*向左推动指定位置的操作,0推动成功,-1操作失败*/
int pushLeft(struct sokomap map,int x,int y);
/*是否能向右推*/
int isCanPushRight(struct sokomap map,int x,int y);
/*向右推箱子*/
int pushRight(struct sokomap map,int x,int y);
/*是否能向上推*/
int isCanPushUp(struct sokomap map,int x,int y);
/*向上推箱子*/
int pushUp(struct sokomap map,int x,int y);
/*是否能向下推*/
int isCanPushDown(struct sokomap map,int x,int y);
/*向下推箱子*/
int pushDown(struct sokomap map,int x,int y);
/*所传入的位置是否为墙*/
int isWall(struct sokomap map,int x,int y);
int isWall_value(char value);
int isDest_value(char value);
int isBox_value(char value);
int isPerson_value(char value);
/*所传入的位置是否为目的地址*/
int isDest(struct sokomap map,int x,int y);
/*所传入的位置是否为箱子*/
int isBox(struct sokomap map,int x,int y);
/*所传入的位置是否为人*/
int isPerson(struct sokomap map,int x,int y);
/*所处的位置是否为空,即非石头，非箱子*/
int isEmpty(struct sokomap map,int x,int y);
/*是否为有效的地图*/
int isValidMap(struct sokomap map);
/*获得各个属性的数量*/
void mapProperyCount(struct sokomap map,int * wallCount,int * boxCount,int * person,int * dest);
/*是否为有效的位置,有效时返回0，无效返回-1*/
int isValidPosition(struct sokomap map,int x,int y);
/*把位置转化为index*/
int paraPosition2Index(struct sokomap map,int x,int y);
int paraPosition2IndexByXY(struct sokomap map,int x,int y);
/*设置某位置为箱子*/
void setBox(char *buf,int position);
/*设置某位置为Dest*/
void setDest(char *buf,int position);
/*设置某处位置为Wall*/
void setWall(char *buf,int position);
/*设置某处置为Person*/
void setPerson(char *buf,int position);
/*清除箱子*/
void clearBox(char *buf,int position);
/*清除Dest*/
void clearDest(char *buf,int position);
/*清除Wall*/
void clearWall(char *buf,int position);
/*清除Person*/
void clearPerson(char *buf,int position);
/*地图是否已经成功*/
int isMapSuccess(struct sokomap map);
/*比较两个地图是否相同*/
int cmpMapBoxIsEqual(struct sokomap map1,struct sokomap map2);
/*获得所有箱子的坐标*/
void getBoxList(struct sokomap map,struct mappos *pos);
#endif
