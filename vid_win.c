#include "quakedef.h"
#include "winquake.h"

int WindowWidth = 640;
int WindowHeight = 480;
int BufferWidth = 640;
int BufferHeight = 480;
int BytesPerPixel = 4;

BOOL Fullscreen = FALSE;

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT Result = 0;
	switch (uMsg) {
	case WM_ACTIVATE:
		break;

	case WM_CREATE:
		break;

	case WM_KEYUP:
	case WM_DESTROY:
		Sys_Shutdown();
		break;

	default:
		Result = DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return Result;
}

void VID_Init(void) {
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = MainWndProc;
	wc.hInstance = GlobalInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = "Module 3";

	if (!RegisterClass(&wc)) {
		exit(1);
	}

	RECT r;
	r.top = r.left = 0;
	r.right = WindowWidth;
	r.bottom = WindowHeight;

	DWORD dwExStyle = 0;
	DWORD dwStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

	if (Fullscreen) {
		DEVMODE dmScreenSettings = { 0 };
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = BufferWidth;
		dmScreenSettings.dmPelsHeight = BufferHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
			dwExStyle = WS_EX_APPWINDOW;
			dwStyle = WS_POPUP;
		}
		else {
			Fullscreen = FALSE;
		}
	}

	AdjustWindowRectEx(&r, dwExStyle, 0, dwStyle);

	HWND mainwindow = CreateWindowEx(
		dwExStyle, 
		"Module 3", 
		"Lesson 3.5", 
		dwStyle, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		r.right - r.left, 
		r.bottom - r.top, 
		NULL, 
		NULL, 
		GlobalInstance, 
		0
	);

	if (Fullscreen) {
		SetWindowLong(mainwindow, GWL_STYLE, 0);
	}

	ShowWindow(mainwindow, SW_SHOWDEFAULT);

	//HDC DeviceContext = GetDC(mainwindow);
	//PatBlt(DeviceContext, 0, 0, BufferWidth, BufferHeight, BLACKNESS);
	//ReleaseDC(mainwindow, DeviceContext);
}

void VID_Shutdown(void) {

}