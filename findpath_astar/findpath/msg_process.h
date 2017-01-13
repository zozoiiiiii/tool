/************************************************************************/
/*
��Ϣ����
*/
/************************************************************************/
#ifndef _MSG_PROCESS_H_
#define _MSG_PROCESS_H_
#include "findpath/mapblock.h"

#include <Windows.h>
#include "map.h"

//��������Ϣ����
LPARAM APIENTRY MainWndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

//����·����뾶���ڵ���Ϣ����
BOOL CALLBACK WayPointDistanceWndProc(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);



static char path[512]="map\\test1.bmp";
static char path1[512]="map\\test1.path";
class MsgProcess
{
public:
	MsgProcess();
	static MsgProcess* getInstance();

	//�˵�����
	void onMenuMsg(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

	//�ػ�
	void onPaint(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

	//����������
	void onMouseDownL(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

	//����Ҽ�����
	void onMouseDownR(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

	//����ƶ�
	void onMouseMove(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);


	//�Զ���������
	void onGenMapBlock(int distance);
	void redraw(HWND hwnd);
	void findpath();
private:
	//����
	void onLoad();

	//����·����
	void onSaveWayPoints();

	//����Ѱ·���
	void onSetBegin();

	//����Ѱ·�յ�
	void onSetEnd();

	//��ʼѰ·
	void onBeginFindPath();

	bool canWalk(int top, int left, int right, int bottom);
private:
	int m_distance;
	Map		m_map;
	MapBlockData m_mb_data;

	std::string m_map_path;		//�ڰ�ͼ·��
	std::vector<MapBlock> m_findpath_result;

	MapBlock m_findpath_begin;				//Ѱ·���
	MapBlock m_findpath_end;				//Ѱ·�յ�
	MapBlock m_focus_mapblock;				//��굱ǰ���ڵ�����
};

#endif