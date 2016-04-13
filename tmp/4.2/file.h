int Sys_FileOpenRead(char *Path, int *Size);
int Sys_FileOpenWrite(char *Path);
void Sys_FileClose(int HandleIdx);
void Sys_FileSeek(int HandleIdx, int Position);
int Sys_FileRead(int HandleIdx, void *Dest, int Count);
int Sys_FileWrite(int HandleIdx, void *Source, int Count);
