#include <stdio.h>
#include <stdarg.h>

char* f(char *format, ...) {
    va_list va;
    va_start(va, format);
    
    int a = va_arg(va, int);
    static char b[1024];
    vsprintf(b, format, va);

    va_end(va);
    return b;
}

int main() {
    char * res = f("%d\n", 5, 10);
    printf(res);

    return 0;
}
