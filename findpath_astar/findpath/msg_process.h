/************************************************************************/
/*
消息处理
*/
/************************************************************************/
#ifndef _MSG_PROCESS_H_
#define _MSG_PROCESS_H_
#include "findpath/mapblock.h"

#include <Windows.h>
#include "map.h"

//主窗口消息处理
LPARAM APIENTRY MainWndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

//设置路径点半径窗口的消息处理
BOOL CALLBACK WayPointDistanceWndProc(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);



static char path[512]="map\\test1.bmp";
static char path1[512]="map\\test1.path";
class MsgProcess
{
public:
	MsgProcess();
	static MsgProcess* getInstance();

	//菜单命令
	void onMenuMsg(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

	//重绘
	void onPaint(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

	//鼠标左键按下
	void onMouseDownL(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

	//鼠标右键按下
	void onMouseDownR(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

	//鼠标移动
	void onMouseMove(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);


	//自动生成网格
	void onGenMapBlock(int distance);
	void redraw(HWND hwnd);
	void findpath();
private:
	//导入
	void onLoad();

	//保存路径点
	void onSaveWayPoints();

	//设置寻路起点
	void onSetBegin();

	//设置寻路终点
	void onSetEnd();

	//开始寻路
	void onBeginFindPath();

	bool canWalk(int top, int left, int right, int bottom);
private:
	int m_distance;
	Map		m_map;
	MapBlockData m_mb_data;

	std::string m_map_path;		//黑白图路径
	std::vector<MapBlock> m_findpath_result;

	MapBlock m_findpath_begin;				//寻路起点
	MapBlock m_findpath_end;				//寻路终点
	MapBlock m_focus_mapblock;				//鼠标当前所在的网格
};

#endif