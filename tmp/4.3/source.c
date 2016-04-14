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

int main(void) {
    int PakSize;
    int PakHandle = Sys_FileOpenRead("PAK0.PAK", &PakSize);

    dpackheader_t PakHeader;
    Sys_FileRead(PakHandle, &PakHeader, sizeof(PakHeader));

    Sys_FileClose(PakHandle);
    
    printf("PAK0.PAK: %d bytes\n", PakSize);
    printf("PakHeader.Magic: %c%c%c%c\n", PakHeader.Magic[0],PakHeader.Magic[1],PakHeader.Magic[2],PakHeader.Magic[3]);
    printf("PakHeader.DirectoryOffset: %d\n", PakHeader.DirectoryOffset);
    printf("PakHeader.DirectoryLength: %d\n", PakHeader.DirectoryLength);
    printf("Number of files: %d\n", PakHeader.DirectoryLength / sizeof(dpackfile_t));
}
