#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>   // mkdir for Windows
#else
#include <unistd.h> // usleep
#include <sys/stat.h> // mkdir
#endif
int main() {
    return 0;
}