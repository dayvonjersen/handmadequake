#include <Windows.h>

#define MAX_NUM_ARGVS 50

int argc = 0;
char *largv[MAX_NUM_ARGVS];

int COM_CheckParm(char *parm) {
	for (int i = 0; i < argc; i++) {
		if (!strcmp(parm, largv[i])) {
			return i;
		}
	}
	return 0;
}

int	CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	largv[0] = "";

	while (*lpCmdLine && argc < MAX_NUM_ARGVS) {
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

	largv[argc] = "";

	int test = COM_CheckParm("-setalpha");
		
	return 0;
}