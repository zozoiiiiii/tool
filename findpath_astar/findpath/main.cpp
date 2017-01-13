/************************************************************************/
/*
Ѱ·�༭���������ǵ���ڰ�ͼ������·���㲢���浽path�ļ���·������Գ����Զ�����
Ҳ�����ֶ����ɣ�Ҫ��ÿ�����ߵ��ĵط�����Ҫ��һ��·���㡣
*/
/************************************************************************/
#include "resource.h"
#include <windows.h>
#include "msg_process.h"

//HINSTANCE		g_hInst = NULL;				//�Ի���ʵ��
char			szAppName[20]="findpath";	//�Ի������


BOOL InitApplication(HANDLE hInstance)
{
	WNDCLASS  wc;

	wc.style            = CS_GLOBALCLASS|CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc      = (WNDPROC)MainWndProc;					//��Ϣ������
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
	//������
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

	//��Ϣѭ��
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
