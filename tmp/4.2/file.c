#include <stdio.h>

#define MAX_HANDLES 10
static FILE *FileHandles[MAX_HANDLES] = {0};

static int FindHandle(void) {
    for(int i = 0; i < MAX_HANDLES; i++) {
        if(FileHandles[i] == NULL) {
            return i;
        }
    }
    return -1;
}

static int FileLength(FILE *f) {
    int pos, end;
    pos = ftell(f);
    fseek(f, 0, SEEK_END);
    end = ftell(f);
    fseek(f, pos, SEEK_SET);
    return end;
}

int Sys_FileOpenRead(char *Path, int *Size) {
    int HandleIdx = FindHandle();
    FileHandles[HandleIdx] = fopen(Path, "rb");

    if(!FileHandles[HandleIdx]) {
        // Sys_Error(0);
        *Size = -1;
        return -1;
    }

    if(Size != 0) *Size = FileLength(FileHandles[HandleIdx]);
    return HandleIdx;
}

int Sys_FileOpenWrite(char *Path) {
    int HandleIdx = FindHandle();
    FileHandles[HandleIdx] = fopen(Path, "wb");

    if(!FileHandles[HandleIdx]) {
        // Sys_Error(0);
        return -1;
    }

    return HandleIdx;
}

void Sys_FileClose(int HandleIdx) {
    if(HandleIdx < 0 || HandleIdx >= MAX_HANDLES) return;
    if(FileHandles[HandleIdx] == NULL) return;

    fclose(FileHandles[HandleIdx]);
    FileHandles[HandleIdx] = NULL;
}

void Sys_FileSeek(int HandleIdx, int Position) {
    if(HandleIdx < 0 || HandleIdx >= MAX_HANDLES) return;
    if(FileHandles[HandleIdx] == NULL) return;

    fseek(FileHandles[HandleIdx], Position, SEEK_SET);
}

int Sys_FileRead(int HandleIdx, void *Dest, int Count) {
    if(HandleIdx < 0 || HandleIdx >= MAX_HANDLES || Dest == 0) return 0;
    if(FileHandles[HandleIdx] == NULL) return -1;

    return (int)fread(Dest, Count, 1, FileHandles[HandleIdx]);
}

int Sys_FileWrite(int HandleIdx, void *Source, int Count) {
    if(HandleIdx < 0 || HandleIdx >= MAX_HANDLES || Source == 0) return 0;
    if(FileHandles[HandleIdx] == NULL) return -1;

    return (int)fwrite(Source, 1, Count, FileHandles[HandleIdx]);
}
