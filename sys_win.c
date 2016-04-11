#define _CRT_SECURE_NO_WARNINGS 1

#include "winquake.h"
#include "quakedef.h"

#include <stdio.h>

static BOOL IsRunning = TRUE;

HINSTANCE GlobalInstance;

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

void Sys_SendKeyEvents(void) {
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Sys_Shutdown(void) {
	IsRunning = FALSE;
}

/*
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
*/
int32 CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int32 nCmdShow) {
	GlobalInstance = hInstance;

	COM_ParseCmdLine(lpCmdLine);
	//int32 test = COM_CheckParm("-setalpha");
	//int32 value = Q_atoi(com_argv[test + 1]);
	
	Host_Init();

	float oldtime = Sys_InitFloatTime();
	
	/*
	uint8 *paletteData = malloc(768);
	FILE *palette = fopen("PALETTE.LMP", "rb");
	size_t ret = fread(paletteData, 1, 768, palette);
	fclose(palette);

	for (int i = 0; i < 256; i++) {
		BitMapInfo.acolors[i].rgbRed = *paletteData++;
		BitMapInfo.acolors[i].rgbGreen = *paletteData++;
		BitMapInfo.acolors[i].rgbBlue = *paletteData++;
	}

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
	*/
	while (IsRunning) {
		float newtime = Sys_FloatTime();
		Host_Frame(newtime - oldtime);
		oldtime = newtime;
	}

	Host_Shutdown();

	//free(discData);
	//free(pauseData);

	return 0;
}