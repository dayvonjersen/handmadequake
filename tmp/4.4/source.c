#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../4.2/file.h"

#define MAX_FILES_IN_PACK 2048

typedef struct {
    char Magic[4];
    int32_t DirectoryOffset;
    int32_t DirectoryLength;
} dpackheader_t;

typedef struct {
    char FileName[56];
    int32_t FilePosition;
    int32_t FileLength;
} dpackfile_t;

typedef struct {
    char FileName[64];
    int32_t FilePosition;
    int32_t FileLength;
} packfile_t;

typedef struct {
    char PackName[128];
    int32_t PackHandle;
    int32_t NumberOfFiles;
    packfile_t *PakFiles;
} pack_t;

typedef struct searchpath_s {
    pack_t *Pack;
    struct searchpath_s *Next;
} searchpath_t;

pack_t *COM_LoadPackFile(char *Path) {
    int PakSize;
    int PakHandle = Sys_FileOpenRead(Path, &PakSize);
    if(PakHandle == -1 || PakSize == 0) return NULL;
    printf("%s: %d bytes\n", Path, PakSize);

    dpackheader_t PakHeader;
    Sys_FileRead(PakHandle, &PakHeader, sizeof(PakHeader));
    printf("PakHeader.Magic: %c%c%c%c\n", PakHeader.Magic[0],PakHeader.Magic[1],PakHeader.Magic[2],PakHeader.Magic[3]);
    if(!(PakHeader.Magic[0]=='P'&&PakHeader.Magic[1]=='A'&&PakHeader.Magic[2]=='C'&&PakHeader.Magic[3]=='K')) return NULL;

    printf("PakHeader.DirectoryOffset: %d\n", PakHeader.DirectoryOffset);
    printf("PakHeader.DirectoryLength: %d\n", PakHeader.DirectoryLength);

    int numFiles = PakHeader.DirectoryLength / sizeof(dpackfile_t);
    packfile_t *FilesInMemory = malloc(numFiles * sizeof(packfile_t));

    Sys_FileSeek(PakHandle, PakHeader.DirectoryOffset);

    dpackfile_t tempPackFile[MAX_FILES_IN_PACK];
    Sys_FileRead(PakHandle, (void *)tempPackFile, PakHeader.DirectoryLength);

    for(int i = 0; i < numFiles; i++) {
        strcpy(FilesInMemory[i].FileName, tempPackFile[i].FileName);
        FilesInMemory[i].FilePosition = tempPackFile[i].FilePosition;
        FilesInMemory[i].FileLength = tempPackFile[i].FileLength;
    }

    pack_t *Pak = (void*)malloc(sizeof(pack_t));
    strcpy(Pak->PackName, Path);
    Pak->PackHandle = PakHandle;
    Pak->NumberOfFiles = numFiles;
    Pak->PakFiles = FilesInMemory;

    return Pak;
}

searchpath_t *COM_SearchPaths = NULL;

void COM_AddGameDirectory(const char *dir) {
    char buf[128];
    pack_t *pakptr;

    for(int i = 0;;i++) {
        sprintf(buf, "%s/PAK%d.PAK", dir, i);
        pakptr = COM_LoadPackFile(buf);
        if(!pakptr) break;
        searchpath_t *newPath = (searchpath_t *)malloc(sizeof(searchpath_t));
        newPath->Pack = pakptr;
        newPath->Next = COM_SearchPaths;
        COM_SearchPaths = newPath;
    }
}

char *COM_FindFile(const char *filename, int *filesize) {
    *filesize = 0;
    if(!filename) return NULL;
    for(searchpath_t *s = COM_SearchPaths; s != NULL; s = s->Next) {
        pack_t *p = s->Pack;
        for(int i = 0; i < p->NumberOfFiles; i++) {
            if(strcmp(filename, p->PakFiles[i].FileName) == 0) {
                *filesize = p->PakFiles[i].FileLength;
                void *filedata = malloc(*filesize+1);
                Sys_FileSeek(p->PackHandle, p->PakFiles[i].FilePosition);
                Sys_FileRead(p->PackHandle, filedata, *filesize);
                return (char *)filedata;
            }
        }
    }
    return NULL;
}

int main(void) {
    COM_AddGameDirectory(".");

    /*
    for(searchpath_t *Pak = COM_SearchPaths; Pak != NULL; Pak = Pak->Next) {
        pack_t *Pak0 = Pak->Pack;
        printf("Number of files: %d\n", Pak0->NumberOfFiles);

        for(int i = 0; i < Pak0->NumberOfFiles; i++) {
            printf("%04d: %- 56s [%d bytes]\n", i+1, Pak0->PakFiles[i].FileName, Pak0->PakFiles[i].FileLength);
        }
    }
    */

    int len;
    char *contents = COM_FindFile("--OMITTED--", &len);
    if(len > 0) {
        printf("size: %d bytes\n", len);
        int idx = Sys_FileOpenWrite("--OMITTED--");
        int out = Sys_FileWrite(idx, contents, len);
        printf("wrote %d bytes\n", out);
        Sys_FileClose(idx);
    }
    return 0;
}
