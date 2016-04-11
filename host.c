#include "quakedef.h"

double realtime = 0.0;
double oldrealtime = 0.0;
double host_frametime = 0.0;

qboolean Host_FilterTime(float time) {
	realtime += time;
	if (realtime - oldrealtime < 1.0 / 72.0) {
		return false;
	}

	host_frametime = realtime - oldrealtime;
	oldrealtime = realtime;

	return true;
}

void Host_Init(void) {
	VID_Init();
}

void Host_Frame(float timestep) {
	if (!Host_FilterTime(timestep)) {
		return;
	}

	Sys_SendKeyEvents();
	
	VID_Update();
	//DrawRect32(10, 10, 300, 150, 0, 0, 0xff, BackBuffer);
	//DrawPic32(10, 10, discWidth, discHeight, discData, BackBuffer);
	//DrawPic32(100, 100, pauseWidth, pauseHeight, pauseData, BackBuffer);
}

void Host_Shutdown(void) {
	VID_Shutdown();
}