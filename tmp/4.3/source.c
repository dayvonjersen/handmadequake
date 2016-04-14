#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>

#include "../4.2/file.h"

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

#define MAX_FILES_IN_PACK 2048
int main(void) {
    int PakSize;
    int PakHandle = Sys_FileOpenRead("PAK0.PAK", &PakSize);

    dpackheader_t PakHeader;
    Sys_FileRead(PakHandle, &PakHeader, sizeof(PakHeader));
    
    int numFiles = PakHeader.DirectoryLength / sizeof(dpackfile_t);
    packfile_t *FilesInMemory = malloc(numFiles * sizeof(packfile_t));

    Sys_FileSeek(PakHandle, PakHeader.DirectoryOffset);

    dpackfile_t tempPackFile[MAX_FILES_IN_PACK];
    Sys_FileRead(PakHandle, (void *)tempPackFile, PakHeader.DirectoryLength);

    Sys_FileClose(PakHandle);

    printf("PAK0.PAK: %d bytes\n", PakSize);
    printf("PakHeader.Magic: %c%c%c%c\n", PakHeader.Magic[0],PakHeader.Magic[1],PakHeader.Magic[2],PakHeader.Magic[3]);
    printf("PakHeader.DirectoryOffset: %d\n", PakHeader.DirectoryOffset);
    printf("PakHeader.DirectoryLength: %d\n", PakHeader.DirectoryLength);
    printf("Number of files: %d\n", numFiles);

    for(int i = 0; i < numFiles; i++) {
        printf("%04d: %- 56s [%d bytes]\n", i+1, tempPackFile[i].FileName, tempPackFile[i].FileLength);
    }
}
