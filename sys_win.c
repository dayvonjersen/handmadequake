#include <stdint.h>
#include <Windows.h>

typedef int8_t int8;
typedef uint8_t uint8;
typedef int32_t int32;
typedef uint32_t uint32;

#define MAX_NUM_ARGVS 50

int32 argc = 0;
uint8 *largv[MAX_NUM_ARGVS+1];

int32 COM_CheckParm(uint8 *parm) {
	for (int32 i = 0; i < argc; i++) {
		if (!strcmp(parm, largv[i])) {
			return i;
		}
	}
	return 0;
}

int32 CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int32 nCmdShow) {
	largv[0] = "";

	while (*lpCmdLine && argc < MAX_NUM_ARGVS+1) {
		while (*lpCmdLine && ( *lpCmdLine <= 32 || *lpCmdLine > 126 ) ) {
			lpCmdLine++;
		}
		if (*lpCmdLine) {
			largv[argc] = lpCmdLine;
			argc++;
			while (*lpCmdLine && *lpCmdLine > 32 && *lpCmdLine <= 126) {
				lpCmdLine++;
			}
			if (*lpCmdLine) {
				*lpCmdLine = 0;
				lpCmdLine++;
			}
		}
	}
	
	int32 test = COM_CheckParm("-setalpha");
	int32 value = atoi(largv[test + 1]);
		
	return 0;
}