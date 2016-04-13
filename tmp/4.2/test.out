#include <stdio.h>
#include "file.h"

int main(void) {
    int s;
    int idx = Sys_FileOpenRead("test.c", &s);
    printf("got size: %d\n", s);

    char d[s];
    Sys_FileRead(idx, d, s);
    Sys_FileClose(idx);
    printf("got data: \n%s\n", d);

    idx = Sys_FileOpenWrite("test.out");
    Sys_FileWrite(idx, d, s);
    Sys_FileClose(idx);
    printf("wrote to test.out.\n");

    return 0;
    /*
    FILE *f = fopen("Source2.c", "rb");

    int pos, end;

    pos = ftell(f);
    fseek(f, 0, SEEK_END);
    end = ftell(f);
    fseek(f, pos, SEEK_SET);

    char FileData[end - pos];
    fread(FileData, 1, end, f);

    fclose(f);

    printf(FileData);

    FILE *WriteFile = fopen("Source.out", "wb");
    fwrite(FileData, 1, end - pos, WriteFile);
    fclose(WriteFile);

    return 0;
    */
}
