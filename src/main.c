#include <stdio.h>
#include <gtk/gtk.h>
#include "debug.h"
#include "sokopush.h"
#include "soko_map_operation.h"
#include "test.h"
#include "soko_pos_operation.h"
#define WIDTH  500
#define HEIGHT 550
#define DRAWING_WIDTH 421
#define DRAWING_HEIGHT 421
enum BigStatus{
	BEFORE,DOING,END
};
enum DrawStatus{
	BOX=0,PERSON,DEST,WALL,CLEAR,OTHER
};
enum ResultStatus{
	RUNING,STOP
};

static GtkWidget *window;//总窗口
static GtkWidget *hbox;//水平的窗口
static GtkWidget *hbuttonbox1;//水平的按钮的窗口1,
static GtkWidget *hbuttonbox2,*hbuttonbox3;//水平按钮的窗口2
static GtkWidget *vbox;//垂直的窗口
static GtkWidget *drawingArea;//绘制图片的
static int bigstatus;//大状态
static int drawStatus;//画图时所处的状态
static gulong drawButtonStatusHandlerId[5];//存放连接按钮的工具
//按钮
static GtkWidget *sizeButton,*wallButton,*boxButton,*destButton,*personButton,*clearButton,*startCaculateButton;//按钮
static GtkWidget *startShowButton,*nxtButton,*preButton,*stopButton;//按钮
static GdkPixbuf *boxPixbuf=NULL,*wallPixbuf=NULL,*destPixbuf=NULL,*personPixbuf=NULL,*boxInDestPixbuf=NULL;//图像的缓冲区
static struct sokomap *map=NULL,*resultMap=NULL;//地图的指针和答案的指针
//清除资源的操作
void clearResource()
{
	g_object_unref(boxPixbuf);
	g_object_unref(wallPixbuf);
	g_object_unref(destPixbuf);
	g_object_unref(personPixbuf);
	g_object_unref(boxInDestPixbuf);
	clearMap();
}
//清除地图的操作
void clearMap()
{
	freeMap(map);
	freeMap(resultMap);
}
//初始化地图
struct sokomap * initMap(int width,int height)
{
	struct sokomap * retMap = (struct sokomap *)malloc(sizeof(struct sokomap));
	char * buf = (char *)malloc(sizeof(char) * width * height);
	retMap -> array = buf;
	memSetSokoMap(retMap,buf,width,height);
	return retMap;
}
//读取图片缓冲区的操作
void readPixbuf()
{
	GError *error = NULL;
	gchar *filename = g_strdup ("res/box_green.png");
	boxPixbuf = gdk_pixbuf_new_from_file (filename, &error);
	g_free(filename);
	
	filename = g_strdup("res/wall.png");
	wallPixbuf = gdk_pixbuf_new_from_file (filename, &error);
	g_free(filename);
	
	filename = g_strdup("res/target.png");
	destPixbuf = gdk_pixbuf_new_from_file (filename, &error);
	g_free(filename);

	filename = g_strdup("res/man_up.png");
	personPixbuf = gdk_pixbuf_new_from_file (filename, &error);
	g_free(filename);

	filename = g_strdup("res/box_red.png");
	boxInDestPixbuf = gdk_pixbuf_new_from_file (filename, &error);
	g_free(filename);
}
//程序退出的方法
void closeApp(GtkWidget *window, gpointer data)
{
	gtk_main_quit();
}
//设计地图按钮单击的事件
void taggleSetMapButtonClick(GtkWidget *widget,gpointer data){
	if(bigstatus != BEFORE){//没处于设计地图的状态时，直接返回
		return;
	}
	char * dataPtr = (char *)data;
	if(strcmp(dataPtr,"set_wall")==0)
	{
		if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(wallButton)))
		{
			drawStatus = WALL;
		}else
		{
			drawStatus = OTHER;
		}
	}else if(strcmp(dataPtr,"set_box")==0)
	{
		if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(boxButton)))
		{
			drawStatus = BOX;
		}else
		{
			drawStatus = OTHER;
		}
	}else if(strcmp(dataPtr,"set_dest")==0)
	{
		if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(destButton)))
		{
			drawStatus = DEST;
		}else
		{
			drawStatus = OTHER;
		}
	}else if(strcmp(dataPtr,"set_person")==0)
	{
		if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(personButton)))
		{
			drawStatus = PERSON;
		}else
		{
			drawStatus = OTHER;
		}
	}else if(strcmp(dataPtr,"clear")==0)
	{
		if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(clearButton)))
		{
			drawStatus = CLEAR;
		}else
		{
			drawStatus = OTHER;
		}
	}
	setSetMapButtonStatus();
}
//设置按钮的状态
void setSetMapButtonStatus()
{
	//先屏蔽事件
	g_signal_handler_block(wallButton, drawButtonStatusHandlerId[WALL]);
	g_signal_handler_block(boxButton, drawButtonStatusHandlerId[BOX]);
	g_signal_handler_block(destButton,drawButtonStatusHandlerId[DEST]);
	g_signal_handler_block(personButton, drawButtonStatusHandlerId[PERSON]);
	g_signal_handler_block(clearButton, drawButtonStatusHandlerId[CLEAR]);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wallButton),FALSE);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(boxButton),FALSE);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(destButton),FALSE);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(personButton),FALSE);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(clearButton),FALSE);
	switch(drawStatus){
		case BOX:
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(boxButton),TRUE);
			break;
		case PERSON:
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(personButton),TRUE);
			break;
		case DEST:
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(destButton),TRUE);
			break;
		case WALL:
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wallButton),TRUE);
			break;
		case CLEAR:
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(clearButton),TRUE);
			break;
		case OTHER:
			break;
	}
	//恢复事件
	g_signal_handler_unblock(wallButton, drawButtonStatusHandlerId[WALL]);
	g_signal_handler_unblock(boxButton, drawButtonStatusHandlerId[BOX]);
	g_signal_handler_unblock(destButton,drawButtonStatusHandlerId[DEST]);
	g_signal_handler_unblock(personButton, drawButtonStatusHandlerId[PERSON]);
	g_signal_handler_unblock(clearButton, drawButtonStatusHandlerId[CLEAR]);
}
//画图区域鼠标单击的事件
gboolean mouseDrawablePress(GtkWidget *widget,GdkEventButton *event,gpointer data)
{
	//当没有处在要计算的状态时，返回
	if(bigstatus != BEFORE)
	{
		return FALSE;
	}
	int x =(int)event -> x;
	int y = (int)event -> y;
	int unit_x = (DRAWING_WIDTH-1)/map->width;
	int unit_y = (DRAWING_HEIGHT-1)/map->hight;
	int map_x = x / unit_x;
	int map_y = y / unit_y;	
	switch(drawStatus)
	{
		case BOX:
			setBox_xy(map,map_x,map_y);
			break;
		case PERSON:
			setPerson_xy(map,map_x,map_y);
			break;
		case DEST:
			setDest_xy(map,map_x,map_y);
			break;
		case WALL:
			setWall_xy(map,map_x,map_y);
			break;
		case CLEAR:
			if(isPerson(map,map_x,map_y))
			{
				clearPerson_xy(map,map_x,map_y);
				break;
			}else if(isBox(map,map_x,map_y))
			{
				clearBox_xy(map,map_x,map_y);
				break;
			}else if(isDest(map,map_x,map_y))
			{
				clearDest_xy(map,map_x,map_y);
				break;
			}else if(isWall(map,map_x,map_y))
			{
				clearWall_xy(map,map_x,map_y);
				break;
			}
		case OTHER:
			break;
	}
	GdkRectangle rect;
	rect.x = 0;
	rect.y = 0;
	rect.width = DRAWING_WIDTH;
	rect.height = DRAWING_HEIGHT;
	gdk_window_invalidate_rect(widget->window,&rect,FALSE);
	return FALSE;
}
//设置背景的操作
void drawBackgroud(GdkDrawable *drawable,GdkGC *gc)
{
	//设置边框
	gdk_draw_line(drawable,gc,0,0,DRAWING_WIDTH,0);
	gdk_draw_line(drawable,gc,0,0,0,DRAWING_HEIGHT);
	gdk_draw_line(drawable,gc,DRAWING_WIDTH-1,0,DRAWING_WIDTH-1,DRAWING_HEIGHT);
	gdk_draw_line(drawable,gc,0,DRAWING_HEIGHT-1,DRAWING_WIDTH,DRAWING_HEIGHT-1);
	//画竖条
	int unit_x = (DRAWING_WIDTH-1)/map->width;
	int unit_y = (DRAWING_HEIGHT-1)/map->hight;
	for(int i = 1;i < map->width;i++)
	{
		gdk_draw_line(drawable,gc,i * unit_x,0,unit_x * i,DRAWING_HEIGHT);
	}
	for(int i = 1; i < map -> hight;i++)
	{
		gdk_draw_line(drawable,gc,0,i * unit_x,DRAWING_HEIGHT,unit_x * i);
	}
	GdkPixbuf *dest_boxPixbuf = NULL,*dest_wallPixbuf = NULL,*dest_destPixbuf = NULL,* dest_personPixbuf = NULL, *dest_boxInDestPixbuf = NULL;
	dest_boxPixbuf = gdk_pixbuf_scale_simple(boxPixbuf,unit_x,unit_y, GDK_INTERP_BILINEAR);
	dest_wallPixbuf = gdk_pixbuf_scale_simple(wallPixbuf,unit_x,unit_y, GDK_INTERP_BILINEAR);
	dest_destPixbuf = gdk_pixbuf_scale_simple(destPixbuf,unit_x,unit_y, GDK_INTERP_BILINEAR);
	dest_personPixbuf = gdk_pixbuf_scale_simple(personPixbuf,unit_x,unit_y, GDK_INTERP_BILINEAR);
	dest_boxInDestPixbuf = gdk_pixbuf_scale_simple(boxInDestPixbuf,unit_x,unit_y, GDK_INTERP_BILINEAR);
	for(int y = 0; y < map->hight; y++)
	{
		for(int x = 0; x < map->width;x++)
		{
			if(isWall(map,x,y)){
				gdk_draw_pixbuf(drawable,gc,dest_wallPixbuf, 0, 0,x * unit_x,y * unit_y,-1,-1,GDK_RGB_DITHER_NORMAL,0,0);
			}else if(isDest(map,x,y)){
				if(isBox(map,x,y)){
					gdk_draw_pixbuf(drawable,gc,dest_boxInDestPixbuf, 0, 0,x * unit_x,y * unit_y,-1,-1,GDK_RGB_DITHER_NORMAL,0,0);
				}else if(isPerson(map,x,y)){
					gdk_draw_pixbuf(drawable,gc,dest_destPixbuf, 0, 0,x * unit_x,y * unit_y,-1,-1,GDK_RGB_DITHER_NORMAL,0,0);
					gdk_draw_pixbuf(drawable,gc,dest_personPixbuf, 0, 0,x * unit_x,y * unit_y,-1,-1,GDK_RGB_DITHER_NORMAL,0,0);
				}else{
					gdk_draw_pixbuf(drawable,gc,dest_destPixbuf, 0, 0,x * unit_x,y * unit_y,-1,-1,GDK_RGB_DITHER_NORMAL,0,0);
				}
			}else if(isBox(map,x,y)){
				gdk_draw_pixbuf(drawable,gc,dest_boxPixbuf, 0, 0,x * unit_x,y * unit_y,-1,-1,GDK_RGB_DITHER_NORMAL,0,0);
			}else if(isPerson(map,x,y)){
				gdk_draw_pixbuf(drawable,gc,dest_personPixbuf, 0, 0,x * unit_x,y * unit_y,-1,-1,GDK_RGB_DITHER_NORMAL,0,0);
			}
		}
	}
	g_object_unref(dest_boxPixbuf);
	g_object_unref(dest_wallPixbuf);
	g_object_unref(dest_destPixbuf);
	g_object_unref(dest_personPixbuf);
	g_object_unref(dest_boxInDestPixbuf);
}
//重新设置状态的操作
void resetStatus()
{
	bigstatus = BEFORE;
	drawStatus = OTHER;
}
//暴露时绘图的按钮
gboolean drawingAreaExposeEventCallback(GtkWidget *widget,GdkEventExpose *event, gpointer data)
{
	if(widget -> window)
	{
		GdkGC * gdkGC = gdk_gc_new(widget->window);
		GdkColormap * colorMap = gdk_gc_get_colormap(gdkGC);//获得colormap
		GdkColor  foreColor;
		foreColor.red = 65535;
		foreColor.green = 0;
		foreColor.blue = 0;
		gdk_colormap_alloc_color(colorMap,&foreColor,FALSE,TRUE);
		gdk_gc_set_foreground(gdkGC,&foreColor);
		drawBackgroud(widget->window,gdkGC);
		//把图像从colormap中删除
		gdk_colormap_free_colors(colorMap,&foreColor,1);
	}
	return TRUE;
} 

//初始化绘图区
void initDrawingArea(GtkWidget *drawingArea)
{
	//设置事件
	g_signal_connect (G_OBJECT(drawingArea),"expose_event",G_CALLBACK(drawingAreaExposeEventCallback), NULL);
	g_signal_connect (G_OBJECT(drawingArea),"button_press_event",G_CALLBACK(mouseDrawablePress), NULL);
}
//初始化widget的操作
void initWidget()
{
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	vbox = gtk_vbox_new(FALSE,8);
	hbox = gtk_hbox_new(FALSE,8);
	hbuttonbox1 = gtk_hbutton_box_new();//水平按钮的box1
	hbuttonbox2 = gtk_hbutton_box_new();//水平按钮的box2
	hbuttonbox3 = gtk_hbutton_box_new();//水平按钮的box3
	//初始化按钮
	sizeButton = gtk_button_new_with_label("new soko");
	wallButton = gtk_toggle_button_new_with_label("set wall");
	boxButton = gtk_toggle_button_new_with_label("set box");
	destButton = gtk_toggle_button_new_with_label("set dest");
	personButton = gtk_toggle_button_new_with_label("set person");
	clearButton = gtk_toggle_button_new_with_label("clear");
	startCaculateButton = gtk_button_new_with_label("start caculate");
	//加入按钮的单击事件
	drawButtonStatusHandlerId[WALL] = g_signal_connect(GTK_OBJECT(wallButton),"clicked",G_CALLBACK(taggleSetMapButtonClick),"set_wall");
	drawButtonStatusHandlerId[BOX] = g_signal_connect(GTK_OBJECT(boxButton),"clicked",G_CALLBACK(taggleSetMapButtonClick),"set_box");
	drawButtonStatusHandlerId[DEST] = g_signal_connect(GTK_OBJECT(destButton),"clicked",G_CALLBACK(taggleSetMapButtonClick),"set_dest");
	drawButtonStatusHandlerId[PERSON] = g_signal_connect(GTK_OBJECT(personButton),"clicked",G_CALLBACK(taggleSetMapButtonClick),"set_person");
	drawButtonStatusHandlerId[CLEAR] = g_signal_connect(GTK_OBJECT(clearButton),"clicked",G_CALLBACK(taggleSetMapButtonClick),"clear");

	startShowButton = gtk_toggle_button_new_with_label(">");
	nxtButton = gtk_button_new_with_label(">>");
	preButton = gtk_button_new_with_label("<<");
	stopButton = gtk_button_new_with_label("||");
	//初始化画图的操作
	drawingArea = gtk_drawing_area_new();
	gtk_widget_set_events(drawingArea,GDK_BUTTON_PRESS_MASK);
	
	gtk_widget_set_size_request (drawingArea, DRAWING_WIDTH, DRAWING_HEIGHT);
	initDrawingArea(drawingArea);
	//设置hbox的大小
	gtk_widget_set_size_request(GTK_WIDGET(hbox),WIDTH,DRAWING_HEIGHT);
	//加入画图的操作
	gtk_box_pack_start(GTK_BOX(hbox), drawingArea, TRUE, FALSE, 5);
	//加入按钮的操作
	//设置第一行
	gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox1), GTK_BUTTONBOX_SPREAD);
	gtk_box_set_spacing (GTK_BOX (hbuttonbox1), 20);
	gtk_container_add (GTK_CONTAINER (hbuttonbox1), sizeButton);
	gtk_container_add (GTK_CONTAINER (hbuttonbox1), wallButton);
	gtk_container_add (GTK_CONTAINER (hbuttonbox1), boxButton);
	gtk_container_add (GTK_CONTAINER (hbuttonbox1), destButton);
	//设置第二行
	gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox2), GTK_BUTTONBOX_SPREAD);
	gtk_box_set_spacing (GTK_BOX (hbuttonbox2), 20);
	gtk_container_add (GTK_CONTAINER (hbuttonbox2), personButton);
	gtk_container_add (GTK_CONTAINER (hbuttonbox2), clearButton);
	gtk_container_add (GTK_CONTAINER (hbuttonbox2), startCaculateButton);
	//设置第三行
	gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox3), GTK_BUTTONBOX_SPREAD);
	gtk_box_set_spacing (GTK_BOX (hbuttonbox3), 20);
	gtk_container_add (GTK_CONTAINER (hbuttonbox3), startShowButton);
	gtk_container_add (GTK_CONTAINER (hbuttonbox3), nxtButton);
	gtk_container_add (GTK_CONTAINER (hbuttonbox3), preButton);
	gtk_container_add (GTK_CONTAINER (hbuttonbox3), stopButton);

	gtk_window_set_title(GTK_WINDOW(window), "sokomethod");
	gtk_widget_set_size_request(GTK_WIDGET(window),WIDTH,HEIGHT);
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_window_set_resizable(GTK_WINDOW(window),FALSE);
	//设置hbox
	gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox), hbuttonbox1, TRUE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox), hbuttonbox2, TRUE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox), hbuttonbox3, TRUE, FALSE, 5);
	gtk_container_add(GTK_CONTAINER(window), vbox);
	//画图的操作
	g_signal_connect(GTK_OBJECT(window),"destroy",GTK_SIGNAL_FUNC(closeApp),NULL);
}
int main(int argc,char * argv[])
{
	gtk_init(&argc, &argv);
	map = initMap(10,10);//初始化地图
	bigstatus = BEFORE;
	drawStatus = OTHER;
	initWidget();
	readPixbuf();//读取图片buf
	gtk_widget_show_all(window);
	gtk_main ();
	return 0;
}
