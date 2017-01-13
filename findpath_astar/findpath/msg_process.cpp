#include "msg_process.h"
#include <math.h>
#include <vector>
#include <map>
#include "resource.h"
#include "findpath/AStar.h"
#include <process.h>


LPARAM APIENTRY MainWndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	static int size=SearchNode::getInstance()->nodes.size();
	switch (nMsg)
	{
	case WM_CREATE:
		SetTimer(hwnd, 999, 100, NULL);
		break;

	case WM_TIMER:
		{
			int new_size=SearchNode::getInstance()->nodes.size();
			if(size != new_size)
			{
				RECT WinRect;
				GetClientRect(hwnd, &WinRect);//获取窗口的大小，从而为下面的方块坐标设定范围  
				InvalidateRect(hwnd, &WinRect, TRUE);
				size=new_size;
			}

		}
		break;
	case WM_COMMAND:
		MsgProcess::getInstance()->onMenuMsg(hwnd, nMsg, wParam, lParam);
		break;
	case WM_PAINT:
		MsgProcess::getInstance()->onPaint(hwnd, nMsg, wParam, lParam);
		break;

		//鼠标按下消息
	case WM_LBUTTONDOWN:
		MsgProcess::getInstance()->onMouseDownL(hwnd, nMsg, wParam, lParam);
		break;

		//鼠标右击
	case WM_CONTEXTMENU:
		MsgProcess::getInstance()->onMouseDownR(hwnd, nMsg, wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		MsgProcess::getInstance()->onMouseMove(hwnd, nMsg, wParam, lParam);
		break;

	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	case WM_DESTROY:
		PostQuitMessage(NULL);
		break;

	default:
		break;
	}

	return DefWindowProc(hwnd,nMsg,wParam,lParam);
}



BOOL CALLBACK WayPointDistanceWndProc(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		SetDlgItemInt(hWndDlg, IDC_WAYPOINT_DISTANCE, 10, true);
		break;
	case WM_COMMAND:
		{
			switch (wParam)
			{
			case IDOK:
				{
					BOOL translate;
					int distance=GetDlgItemInt(hWndDlg, IDC_WAYPOINT_DISTANCE, &translate, true);
					MsgProcess::getInstance()->onGenMapBlock(distance);
					HWND hwnd=GetParent(hWndDlg);
					MsgProcess::getInstance()->redraw(hwnd);
					return EndDialog(hWndDlg, IDCANCEL);
				}
				break;

			case IDCANCEL:
				return EndDialog(hWndDlg, IDCANCEL);
			default:
				break;
			}
		}
	}

	return FALSE;
}




/////////////////////////////////////////////////////////////////////////////////////////
MsgProcess::MsgProcess()
{
	m_distance=0;
}

MsgProcess* MsgProcess::getInstance()
{
	static MsgProcess s_msg_process;
	return &s_msg_process;
}

void MsgProcess::onLoad()
{
	m_map_path=path;

	m_map.loadMap(path);
}

void MsgProcess::onSaveWayPoints()
{
	//m_waypoint.saveToFile(path1);
}

void MsgProcess::onSetBegin()
{
	m_findpath_begin=m_focus_mapblock;
}

void MsgProcess::onSetEnd()
{
	m_findpath_end=m_focus_mapblock;
}

unsigned int __stdcall ThreadFunc(void* param)
{
	MsgProcess* t=(MsgProcess*)param;
	t->findpath();
	return 0;
}

void MsgProcess::onBeginFindPath()
{
	unsigned int threadID;
	HANDLE m_handle=(HANDLE)_beginthreadex(NULL,				//安全属性
		0,				//栈空间
		ThreadFunc,		//线程入口函数
		this,			//传递的参数
		0,				//标志位
		&threadID);		//线程ID

	if(0==m_handle)
		return;
}

void MsgProcess::findpath()
{

	SearchNode::getInstance()->nodes.clear();

	AStar astar(&m_mb_data);
	if(m_findpath_begin.x==-1 || m_findpath_end.x==-1)
		return;

	m_findpath_result.clear();
	astar.findpath(m_findpath_begin, m_findpath_end, m_findpath_result);
	if(m_findpath_result.size()==0)
		MessageBox(NULL, "寻路失败", "hello", MB_OK);

	redraw(GetActiveWindow());
	//m_finder.loadFile(path, path1);
	//m_finder.findpath(m_findpath_begin, m_findpath_end, m_findpath_result);

}

bool MsgProcess::canWalk(int top, int left, int right, int bottom)
{
	for(int i=left; i<right; i++)
	for(int j=top; j<bottom; j++)
	{
		if(!m_map.canWalk(Point(i,j)))
			return false;
	}

	return true;
}

void MsgProcess::onMenuMsg(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case MENU_LOADMAP:
		onLoad();
		//设置对话框大小
		MoveWindow(hwnd, 100, 100, m_map.getWidth(), m_map.getHeight(), true);
		redraw(hwnd);
		break;

	case MENU_GEN_POINTS:
		{
			HINSTANCE instance=(HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE);
			DialogBox(instance, MAKEINTRESOURCE(IDD_DLG_WAYPOING_DISTANCE), NULL, (DLGPROC)WayPointDistanceWndProc);
		}
		break;
	case MENU_SAVE_POINTS:
		onSaveWayPoints();
		break;
	case MENU_BEGIN:
		onSetBegin();
		redraw(hwnd);
		break;
	case MENU_END:
		onSetEnd();
		redraw(hwnd);
		break;
	case MENU_FINDPATH:
		onBeginFindPath();
		redraw(hwnd);
		break;
	default:
		break;
	}
}

void MsgProcess::onPaint(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	if(m_map_path.empty())
		return;

	HDC hDC;
	PAINTSTRUCT ps;
	hDC=BeginPaint(hwnd,&ps);


	HPEN find_result_pen = CreatePen(PS_SOLID,5,0x9B30FF);

	HPEN old_pen = CreatePen(PS_SOLID,1,0xA52A2A);
	HPEN pen=CreatePen(PS_SOLID,5,0x00FF00);

	HPEN canwalk_pen=CreatePen(PS_SOLID,1,0x0FFF00);

	HPEN begin_pen=CreatePen(PS_SOLID,2,0x9F79EE);
	HPEN end_pen=CreatePen(PS_SOLID,2,0x9F79EE);

	SelectObject(hDC, old_pen);

	//显示黑白图
	HDC DCMemory = CreateCompatibleDC(NULL);//建立内存兼容DC
	HBITMAP bitmap;//定义bitmap对象
	BITMAP bmp;//定义bmp结构
	bitmap =(HBITMAP) LoadImageA(NULL,m_map_path.c_str(),IMAGE_BITMAP,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE);
	GetObject(bitmap,sizeof(bmp),&bmp);//获得bitmap信息到bmp结构中;
	SelectObject(DCMemory,bitmap);//将BMP选入到内存DC中
	DeleteObject(bitmap);//删除对象
	BitBlt(hDC,0,0,bmp.bmWidth,bmp.bmHeight,DCMemory,0,0,SRCCOPY);//将内存DC中的内容拷贝到当前窗口DC中,并显示
	DeleteDC(DCMemory);//删除DC；

	//路径点
	/*for(int i=0; i<m_points.size(); i++)
	{
		Point p=m_points[i];
		Ellipse(hDC,p.x-3, p.y-3, p.x+3, p.y+3);
	}*/

	for(int i=0; i<m_mb_data.row; i++)
	for(int j=0; j<m_mb_data.column; j++)
	{
		int top=i*m_distance;
		int left=j*m_distance;
		int bottom=top+m_distance;
		int right=left+m_distance;

		int index=i*m_mb_data.column + j;
		if(CANWALK==m_mb_data.m_data[index])
		{
			SelectObject(hDC, canwalk_pen);

			MoveToEx(hDC, left, top, NULL);
			LineTo(hDC, right, top);
			LineTo(hDC, right, bottom);
			LineTo(hDC, left, bottom);
			LineTo(hDC, left, top);

			SelectObject(hDC, old_pen);
			//DeleteObject(canwalk_pen);
		}
	}

	if(m_focus_mapblock.x!=0)
	{

		int top=m_focus_mapblock.y*m_distance;
		int left=m_focus_mapblock.x*m_distance;
		int bottom=top+m_distance;
		int right=left+m_distance;


		SelectObject(hDC, pen);
		Rectangle(hDC, left, top, right, bottom);

		SelectObject(hDC, old_pen);
		//DeleteObject(pen);
	}

	if(m_findpath_begin.x!=0)
	{
		int top=m_findpath_begin.y*m_distance;
		int left=m_findpath_begin.x*m_distance;
		int bottom=top+m_distance;
		int right=left+m_distance;


		SelectObject(hDC, begin_pen);
		Rectangle(hDC, left, top, right, bottom);

		SelectObject(hDC, old_pen);
		//DeleteObject(begin_pen);
	}
	if(m_findpath_end.x!=0)
	{
		int top=m_findpath_end.y*m_distance;
		int left=m_findpath_end.x*m_distance;
		int bottom=top+m_distance;
		int right=left+m_distance;


		SelectObject(hDC, end_pen);
		Rectangle(hDC, left, top, right, bottom);

		SelectObject(hDC, old_pen);
		//DeleteObject(end_pen);
	}

	for(int i=0; i<SearchNode::getInstance()->nodes.size(); i++)
	{
		MapBlock mb=SearchNode::getInstance()->nodes[i];
		int top=mb.y*m_distance;
		int left=mb.x*m_distance;
		int bottom=top+m_distance;
		int right=left+m_distance;

		SelectObject(hDC, old_pen);
		Rectangle(hDC, left, top, right, bottom);
	}

	//导航路径
	for(int i=0; i<m_findpath_result.size(); i++)
	{
		int top=m_findpath_result[i].y*m_distance;
		int left=m_findpath_result[i].x*m_distance;
		int bottom=top+m_distance;
		int right=left+m_distance;

		SelectObject(hDC, find_result_pen);
		Rectangle(hDC, left, top, right, bottom);

		SelectObject(hDC, old_pen);
	}


	EndPaint(hwnd,&ps);
}

void MsgProcess::onMouseDownL(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
}

void MsgProcess::onMouseDownR(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hwndContext;
	hwndContext = (HWND)wParam;

	WORD cur_x=LOWORD(lParam);
	WORD cur_y=HIWORD(lParam);


	// 来自 hListBox 的消息
	if(hwndContext == hwnd) {
		// 非右键点击
		if (cur_x == -1U || cur_y == -1U) {
			return;
		}

		HINSTANCE instance=(HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE);
		HMENU hMenu = LoadMenu(instance, MAKEINTRESOURCE(IDR_MENU2));
		HMENU hPopup = GetSubMenu(hMenu, 0);
		TrackPopupMenu(hPopup, TPM_LEFTALIGN, cur_x, cur_y, 0, hwnd, NULL);
	}
}

void MsgProcess::onMouseMove(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	if(m_distance==0)
		return;

	WORD cur_x=LOWORD(lParam);
	WORD cur_y=HIWORD(lParam);

	int old_x=m_focus_mapblock.x;
	int old_y=m_focus_mapblock.y;
	m_focus_mapblock.x = cur_x/m_distance;
	m_focus_mapblock.y=cur_y/m_distance;

	if(old_x!=m_focus_mapblock.x || old_y!=m_focus_mapblock.y)
	{
		//redraw(hwnd);
	}

	//在标题栏显示鼠标坐标
	char str[50];
	sprintf(str, "x = %d,y = %d",cur_x,cur_y);
	SetWindowTextA(hwnd, str);
}


void MsgProcess::redraw(HWND hwnd)
{
	RECT WinRect;
	GetClientRect(hwnd, &WinRect);//获取窗口的大小，从而为下面的方块坐标设定范围
	InvalidateRect(hwnd, &WinRect, true);
}

void MsgProcess::onGenMapBlock(int distance)
{
	int i=0;
	int j=0;
	m_distance=distance;
	m_mb_data.row=m_map.getHeight()/distance;
	m_mb_data.column=m_map.getWidth()/distance;
	int size=m_mb_data.row*m_mb_data.column;
	m_mb_data.m_data=new int[size];
	memset(m_mb_data.m_data, 0, sizeof(int)*size);


	for(int i=0; i<m_mb_data.row; i++)
	for(int j=0; j<m_mb_data.column; j++)
	{
		int top=i*m_distance;
		int left=j*m_distance;
		int bottom=top+m_distance;
		int right=left+m_distance;

		int index=i*m_mb_data.column + j;
		if(canWalk(top, left, right, bottom))
			m_mb_data.m_data[index]=CANWALK;
	}
	//路径点半径
	/*for (int y=0; y < m_map.getHeight() ;  y += distance)
	{
		for (int x = 0; x < m_map.getWidth() ; x+=distance)
		{
			int left=x;
			int top=y;
			int right=distance+x;
			int bottom=distance+y;
			if(canWalk(top, left, right, bottom))
				m_mb_data.m_data[i*m_mb_data.column+j]=CANWALK;

			j++;
		}
		i++;
	}*/
}