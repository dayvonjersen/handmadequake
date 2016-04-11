#define _CRT_SECURE_NO_WARNINGS 1

#include "winquake.h"
#include "quakedef.h"

#include <stdio.h>

BOOL IsRunning = TRUE;

int WindowWidth = 640;
int WindowHeight = 480;
int BufferWidth = 640;
int BufferHeight = 480;
int BytesPerPixel = 4;
void *BackBuffer;

typedef struct dibinfo_s {
	BITMAPINFOHEADER bmiHeader;
	RGBQUAD			 acolors[256];
} dibinfo_t;

dibinfo_t BitMapInfo = { 0 };

BOOL Fullscreen = FALSE;

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

	case WM_KEYUP:
	case WM_DESTROY:
		Sys_Shutdown();
		break;

	default:
		Result = DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return Result;
}

void DrawPic8(int x, int y, int w, int h, uint8 *src, uint8 *dest) {
	dest += (BufferWidth * BytesPerPixel * y) + (x * BytesPerPixel);
	uint8 *bufferWalker = dest;
	for (int height = 0; height < h; height++) {
		for (int width = 0; width < w; width++) {
			*bufferWalker++ = *src++;
		}
		dest += BufferWidth * BytesPerPixel;
		bufferWalker = dest;
	}
}

void DrawPic32(int x, int y, int w, int h, uint8 *src, uint8 *dest) {
	dest += (BufferWidth * BytesPerPixel * y) + (x * BytesPerPixel);
	int *bufferWalker = (int*)dest;
	for (int height = 0; height < h; height++) {
		for (int width = 0; width < w; width++) {

			//RGBQUAD tempColor = BitMapInfo.acolors[*src];
			//uint32 myColor  = (tempColor.rgbRed << 16) | (tempColor.rgbGreen << 8) | tempColor.rgbBlue;
			//*bufferWalker++ = myColor;
			//src++;

			*bufferWalker++ = *(int*)&BitMapInfo.acolors[*src++];
		}
		dest += BufferWidth * BytesPerPixel;
		bufferWalker = (int *)dest;
	}
}

void DrawRect8(int x, int y, int w, int h, uint8 color, uint8* buffer) {
	if (x < 0) x = 0;
	if (y < 0) y = 0;
	if (x + w > BufferWidth)  w = BufferWidth - x;
	if (y + h > BufferHeight) h = BufferHeight - y;

	buffer += (BufferWidth * BytesPerPixel * y) + (x * BytesPerPixel);
	uint8 *bufferWalker = buffer;

	//uint32 color = (r << 16) | (g << 8) | b;

	for (int height = 0; height < h; height++) {
		for (int width = 0; width < w; width++) {
			*bufferWalker++ = color;
		}
		buffer += BufferWidth * BytesPerPixel;
		bufferWalker = (uint8 *)buffer;
	}
}

void DrawRect32(int x, int y, int w, int h, int r, int g, int b, uint8* buffer) {
	if (x < 0) x = 0;
	if (y < 0) y = 0;
	if (x + w > BufferWidth)  w = BufferWidth - x;
	if (y + h > BufferHeight) h = BufferHeight - y;

	buffer += (BufferWidth * BytesPerPixel * y) + (x * BytesPerPixel);
	int *bufferWalker = (int *)buffer;

	uint32 color = (r << 16) | (g << 8) | b;

	for (int height = 0; height < h; height++) {
		for (int width = 0; width < w; width++) {
			*bufferWalker++ = color;
		}
		buffer += BufferWidth * BytesPerPixel;
		bufferWalker = (int *)buffer;
	}
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

	HWND mainwindow = CreateWindowEx(dwExStyle, "Module 3", "Lesson 3.4", dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, r.right - r.left, r.bottom - r.top, NULL, NULL, hInstance, 0);

	if (Fullscreen) {
		SetWindowLong(mainwindow, GWL_STYLE, 0);
	}

	ShowWindow(mainwindow, SW_SHOWDEFAULT);
	
	BitMapInfo.bmiHeader.biSize = sizeof(BitMapInfo.bmiHeader);
	BitMapInfo.bmiHeader.biWidth = BufferWidth;
	BitMapInfo.bmiHeader.biHeight = -BufferHeight;
	BitMapInfo.bmiHeader.biPlanes = 1;
	BitMapInfo.bmiHeader.biBitCount = 8 * BytesPerPixel;
	BitMapInfo.bmiHeader.biCompression = BI_RGB;

	BackBuffer = malloc(BufferWidth * BufferHeight * BytesPerPixel);

	uint8 *paletteData = malloc(768);
	FILE *palette = fopen("PALETTE.LMP", "rb");
	size_t ret = fread(paletteData, 1, 768, palette);
	fclose(palette);

	for (int i = 0; i < 256; i++) {
		BitMapInfo.acolors[i].rgbRed = *paletteData++;
		BitMapInfo.acolors[i].rgbGreen = *paletteData++;
		BitMapInfo.acolors[i].rgbBlue = *paletteData++;
	}
	
	//HDC DeviceContext = GetDC(mainwindow);
	//PatBlt(DeviceContext, 0, 0, BufferWidth, BufferHeight, BLACKNESS);
	//ReleaseDC(mainwindow, DeviceContext);
	
	float oldtime = Sys_InitFloatTime();
	
	int discWidth, discHeight;
	size_t retval;
	FILE *disc = fopen("DISC.lmp", "rb");
	retval = fread(&discWidth, 4, 1, disc);
	retval = fread(&discHeight, 4, 1, disc);
	void *discData = malloc(discWidth*discHeight);
	retval = fread(discData, 1, discWidth*discHeight, disc);
	fclose(disc);

	int pauseWidth, pauseHeight;
	FILE *pause = fopen("PAUSE.lmp", "rb");
	retval = fread(&pauseWidth, 4, 1, pause);
	retval = fread(&pauseHeight, 4, 1, pause);
	void *pauseData = malloc(pauseWidth*pauseHeight);
	retval = fread(pauseData, 1, pauseWidth*pauseHeight, pause);
	fclose(pause);	

	MSG msg;
	while (IsRunning) {
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		float newtime = Sys_FloatTime();
		Host_Frame(newtime - oldtime);
		oldtime = newtime;

		switch (BytesPerPixel) {
			// 8-bit mode
		case 1: {
			uint8 *MemoryWalker = (uint8 *)BackBuffer;
			for (int y = 0; y < BufferHeight; y++) {
				for (int x = 0; x < BufferWidth; x++) {
					*MemoryWalker++ = rand() % 256;
				}
			}
			//DrawRect8(10, 10, 300, 150, 1, BackBuffer);
			DrawPic8(10, 10, discWidth, discHeight, discData, BackBuffer);
			DrawPic8(100, 100, pauseWidth, pauseHeight, pauseData, BackBuffer);
		}; break;
			// 32-bit mode
		case 4: {
			uint32 *MemoryWalker = (uint32 *)BackBuffer;
			for (int y = 0; y < BufferHeight; y++) {
				for (int x = 0; x < BufferWidth; x++) {
					uint8 r = rand() % 256;
					uint8 g = rand() % 256;
					uint8 b = rand() % 256;

					*MemoryWalker++ = (r << 16) | (g << 8) | b;
				}
			}
			//DrawRect32(10, 10, 300, 150, 0, 0, 0xff, BackBuffer);
			DrawPic32(10, 10, discWidth, discHeight, discData, BackBuffer);
			DrawPic32(100, 100, pauseWidth, pauseHeight, pauseData, BackBuffer);
		}; break;
		}


		HDC dc = GetDC(mainwindow);
		StretchDIBits(dc, 0, 0, WindowWidth, WindowHeight, 0, 0, BufferWidth, BufferHeight, BackBuffer, (BITMAPINFO*)&BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
		ReleaseDC(mainwindow, dc);
	}

	Host_Shutdown();

	free(BackBuffer);
	free(discData);
	free(pauseData);

	return 0;
}