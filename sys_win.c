
#include "winquake.h"
#include "quakedef.h"

BOOL IsRunning = TRUE;

int BufferWidth = 640;
int BufferHeight = 480;
void *BackBuffer;
BITMAPINFO BitMapInfo = { 0 };

static double GTimePassed = 0.0;
static double SecondsPerTick = 0.0;
static __int64 GTimeCount = 0;

float Sys_InitFloatTime(void) {
	LARGE_INTEGER Frequency;
	QueryPerformanceFrequency(&Frequency);

	SecondsPerTick = 1.0 / (double)Frequency.QuadPart;

	LARGE_INTEGER Counter;
	QueryPerformanceCounter(&Counter);
	GTimeCount = Counter.QuadPart;

	return 0.f;
}

float Sys_FloatTime(void) {
	LARGE_INTEGER Counter;
	QueryPerformanceCounter(&Counter);

	__int64 Interval = Counter.QuadPart - GTimeCount;
	GTimeCount = Counter.QuadPart;

	double SecondsGoneBy = (double)Interval * SecondsPerTick;
	GTimePassed += SecondsGoneBy;

	return (float)GTimePassed;
}

void Sys_Shutdown(void) {
	IsRunning = FALSE;
}

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam , LPARAM lParam) {
	LRESULT Result = 0;
	switch (uMsg) {
	case WM_ACTIVATE:
		break;

	case WM_CREATE:
		break;

	case WM_DESTROY:
		Sys_Shutdown();
		break;

	default:
		Result = DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return Result;
}

int32 CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int32 nCmdShow) {
	COM_ParseCmdLine(lpCmdLine);
	//int32 test = COM_CheckParm("-setalpha");
	//int32 value = Q_atoi(com_argv[test + 1]);

	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = MainWndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = "Module 3";

	if (!RegisterClass(&wc)) {
		exit(1);
	}

	RECT r;
	r.top = r.left = 0;
	r.right = BufferWidth;
	r.bottom = BufferHeight;

	DWORD dwExStyle = 0;
	DWORD dwStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

	AdjustWindowRectEx(&r, dwExStyle, 0, dwStyle);

	HWND mainwindow = CreateWindowEx(dwExStyle, "Module 3", "Lesson 3.2", dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, r.right - r.left, r.bottom - r.top, NULL, NULL, hInstance, 0);
	ShowWindow(mainwindow, SW_SHOWDEFAULT);
	
	BitMapInfo.bmiHeader.biSize = sizeof(BitMapInfo.bmiHeader);
	BitMapInfo.bmiHeader.biWidth = BufferWidth;
	BitMapInfo.bmiHeader.biHeight = BufferHeight;
	BitMapInfo.bmiHeader.biPlanes = 1;
	BitMapInfo.bmiHeader.biBitCount = 32;
	BitMapInfo.bmiHeader.biCompression = BI_RGB;

	BackBuffer = malloc(BufferWidth * BufferHeight * 4);

	//HDC DeviceContext = GetDC(mainwindow);
	//PatBlt(DeviceContext, 0, 0, BufferWidth, BufferHeight, BLACKNESS);
	//ReleaseDC(mainwindow, DeviceContext);
	
	float oldtime = Sys_InitFloatTime();
	
	MSG msg;
	while (IsRunning) {
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		float newtime = Sys_FloatTime();
		Host_Frame(newtime - oldtime);
		oldtime = newtime;

		int *MemoryWalker = (int *)BackBuffer;
		for (int y = 0; y < BufferHeight; y++) {
			for (int x = 0; x < BufferWidth; x++) {
				char r = rand() % 256;
				char g = rand() % 256;
				char b = rand() % 256;

				*MemoryWalker++ = (r << 16) | (g << 8) | b;
			}
		}

		HDC dc = GetDC(mainwindow);
		StretchDIBits(dc, 0, 0, BufferWidth, BufferHeight, 0, 0, BufferWidth, BufferHeight, BackBuffer, &BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
		ReleaseDC(mainwindow, dc);
	}

	Host_Shutdown();

	return 0;
}