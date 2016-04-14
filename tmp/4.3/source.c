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

int main(void) {
    int PakSize;
    int PakHandle = Sys_FileOpenRead("PAK0.PAK", &PakSize);
    Sys_FileClose(PakHandle);
    
    printf("PAK0.PAK: %d bytes\n", PakSize);
}
