#pragma once

extern void *BackBuffer;

void VID_Init(void);
void VID_SetMode(int);
void VID_Update(void);
void VID_Shutdown(void);