#include "winquake.h"
#include "quakedef.h"

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam , LPARAM lParam) {
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int32 CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int32 nCmdShow) {
	//COM_ParseCmdLine(lpCmdLine);
	//int32 test = COM_CheckParm("-setalpha");
	//int32 value = Q_atoi(com_argv[test + 1]);

	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = MainWndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = "Module 2";

	if (!RegisterClass(&wc)) {
		exit(1);
	}

	RECT r;
	r.top = r.left = 0;
	r.right = 800;
	r.bottom = 600;

	DWORD WindowStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

	AdjustWindowRect(&r, WindowStyle, FALSE);

	HWND mainwindow = CreateWindowEx(0, "Module 2", "Lesson 2.1", WindowStyle, 200, 200, r.right - r.left, r.bottom - r.top, NULL, NULL, hInstance, 0);
	ShowWindow(mainwindow, SW_SHOWDEFAULT);


	HDC DeviceContext = GetDC(mainwindow);
	PatBlt(DeviceContext, 0, 0, 800, 600, BLACKNESS);
	ReleaseDC(mainwindow, DeviceContext);

	return 0;
}