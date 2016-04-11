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
}

void Host_Shutdown(void) {
	VID_Shutdown();
}