#include <stdio.h>

int main(void) {
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
}
