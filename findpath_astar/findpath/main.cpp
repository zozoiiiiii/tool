/************************************************************************/
/*
寻路编辑器，功能是导入黑白图后，生成路径点并保存到path文件，路径点可以程序自动生成
也可以手动生成，要求每个能走到的地方至少要有一个路径点。
*/
/************************************************************************/
#include "resource.h"
#include <windows.h>
#include "msg_process.h"

//HINSTANCE		g_hInst = NULL;				//对话框实例
char			szAppName[20]="findpath";	//对话框标题


BOOL InitApplication(HANDLE hInstance)
{
	WNDCLASS  wc;

	wc.style            = CS_GLOBALCLASS|CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc      = (WNDPROC)MainWndProc;					//消息处理函数
	wc.cbClsExtra       = 0;
	wc.cbWndExtra       = 0;
	wc.hIcon            = NULL;
	wc.hInstance        = (HINSTANCE)hInstance;
	wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground    = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName     = MAKEINTRESOURCE(IDR_MENU1);
	wc.lpszClassName    = szAppName;

	return RegisterClass(&wc);
}

BOOL InitInstance(HANDLE hInstance, int nCmdShow)
{
	//主窗口
	HWND menu_hwnd = CreateWindowEx(0, szAppName, szAppName, 
		WS_OVERLAPPEDWINDOW|WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, (HINSTANCE)hInstance, NULL);

	ShowWindow(menu_hwnd, SW_SHOW);
	UpdateWindow(menu_hwnd);
	return TRUE;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;

	if (!InitApplication(hInstance))
		return 0;

	if (!InitInstance(hInstance, nCmdShow))
		return 0;

	//消息循环
	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (msg.wParam);
}
