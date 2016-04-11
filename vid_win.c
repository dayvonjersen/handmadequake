#include "quakedef.h"
#include "winquake.h"

static int WindowWidth = 640;
static int WindowHeight = 480;
static int BufferWidth = 640;
static int BufferHeight = 480;
static int BytesPerPixel = 4;

void *BackBuffer = NULL;

BOOL Fullscreen = FALSE;
BITMAPINFO BitMapInfo = { 0 };

WNDCLASS wc = { 0 };
HWND MainWindow;

typedef enum {
	MS_WINDOWED,
	MS_FULLSCREEN
} modestate_t;

typedef struct {
	modestate_t type;
	int width;
	int height;
} vmode_t;

vmode_t modelist[30];
int modenums = 0;

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT Result = 0;
	switch (uMsg) {
	case WM_ACTIVATE:
		break;

	case WM_CREATE:
		break;

	case WM_CLOSE:
		Sys_Shutdown();
		break;

	case WM_KEYDOWN:
		switch (wParam) {
		case 'W': 
			VID_SetMode(3); break;
		case 'A':
			VID_SetMode(0); break;
		case 'S':
			VID_SetMode(1); break;
		case 'D':
			VID_SetMode(2); break;
		case 'Q':
			Sys_Shutdown(); break;
		}
		break;

	default:
		Result = DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return Result;
}

void VID_InitWindowedMode(void) {
	modelist[modenums].type = MS_WINDOWED;
	modelist[modenums].width = 320;
	modelist[modenums].height = 240;
	modenums++;
	modelist[modenums].type = MS_WINDOWED;
	modelist[modenums].width = 640;
	modelist[modenums].height = 480;
	modenums++;
	modelist[modenums].type = MS_WINDOWED;
	modelist[modenums].width = 800;
	modelist[modenums].height = 600;
	modenums++;
	modelist[modenums].type = MS_WINDOWED;
	modelist[modenums].width = 1024;
	modelist[modenums].height = 768;
	modenums++;
}

void VID_InitFullscreenMode(void) {

}

void VID_SetMode(int ModeValue) {
	if (BackBuffer) {
		VID_Shutdown();
	}

	WindowWidth = modelist[ModeValue].width;
	WindowHeight = modelist[ModeValue].height;

	RECT r;
	r.top = r.left = 0;
	r.right = WindowWidth;
	r.bottom = WindowHeight;

	DWORD dwExStyle = 0;
	DWORD dwStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

	if (modelist[ModeValue].type == MS_FULLSCREEN) {
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

	MainWindow = CreateWindowEx(
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
		SetWindowLong(MainWindow, GWL_STYLE, 0);
	}

	ShowWindow(MainWindow, SW_SHOWDEFAULT);

	//HDC DeviceContext = GetDC(mainwindow);
	//PatBlt(DeviceContext, 0, 0, BufferWidth, BufferHeight, BLACKNESS);
	//ReleaseDC(mainwindow, DeviceContext);
	BitMapInfo.bmiHeader.biSize = sizeof(BitMapInfo.bmiHeader);
	BitMapInfo.bmiHeader.biWidth = BufferWidth;
	BitMapInfo.bmiHeader.biHeight = -BufferHeight;
	BitMapInfo.bmiHeader.biPlanes = 1;
	BitMapInfo.bmiHeader.biBitCount = 8 * BytesPerPixel;
	BitMapInfo.bmiHeader.biCompression = BI_RGB;

	BackBuffer = malloc(BufferWidth * BufferHeight * BytesPerPixel);
}

void VID_Init(void) {
	wc.lpfnWndProc = MainWndProc;
	wc.hInstance = GlobalInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = "Module 3";

	if (!RegisterClass(&wc)) {
		exit(1);
	}

	VID_InitWindowedMode();
	VID_InitFullscreenMode();

	VID_SetMode(0);	
}

void VID_Update(void) {
	HDC dc = GetDC(MainWindow);
	StretchDIBits(dc, 
		0, 0, WindowWidth, WindowHeight, 
		0, 0, BufferWidth, BufferHeight, 
		BackBuffer, 
		&BitMapInfo, 
		DIB_RGB_COLORS, SRCCOPY);
	ReleaseDC(MainWindow, dc);
}

void VID_Shutdown(void) {
	DestroyWindow(MainWindow);
	free(BackBuffer);
	BackBuffer = NULL;
}