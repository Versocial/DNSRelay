#ifndef LogC
#define LogC

#include"log.h"
static int mod = 0;
static FILE* _Log_File=NULL;
static time_t _Log_rawTime;
static char _Log_TimeBuffer[50];

FILE* initLog(const char* path) {
    char tempPath[100]; memcpy(tempPath, path, 100);
    memset(_Log_TimeBuffer, 0, 50);
    time(&_Log_rawTime);
    struct tm* time = localtime(&_Log_rawTime);
    strftime(_Log_TimeBuffer, sizeof(_Log_TimeBuffer), "%Y_%m_%d_%H_%M_%S", localtime(&_Log_rawTime));
    strcat(tempPath, "/log_"); strcat(tempPath, _Log_TimeBuffer); strcat(tempPath, ".log");
    _Log_File = fopen(tempPath, "a+");
    if (_Log_File == NULL) { printf("log init error.\n"); return NULL; }
    strftime(_Log_TimeBuffer, sizeof(_Log_TimeBuffer), "\nStart Time : %Y/%m/%d %H:%M:%S \n", localtime(&_Log_rawTime));
    fprintf(_Log_File, _Log_TimeBuffer);
    return _Log_File;
}
void log(const char* strFormat, ...) {
    if (mod == 3)return;
#define fileOK mod!=1
#define stdoutOK (mod == 0 || _Log_File == NULL)
    time(&_Log_rawTime);
    struct tm* time = localtime(&_Log_rawTime);
    strftime(_Log_TimeBuffer, sizeof(_Log_TimeBuffer), "\n[ %Y/%m/%d %H:%M:%S ]   ", time);
    if (_Log_File == NULL) { printf("[LOG ]: NOT INIT AT "); mod = 1; }
    va_list ap;
    va_start(ap, strFormat);
    if (fileOK) {
        fprintf(_Log_File, _Log_TimeBuffer);
        vfprintf(_Log_File, strFormat, ap);
        fprintf(_Log_File, "\n");
    }if (stdoutOK) { 
        printf(_Log_TimeBuffer); 
        vprintf(strFormat, ap);
        printf("\n");
    }
}

void setLogMod(int Mod) {
    mod = Mod;
}

void closeLog() {
    free(_Log_File);
}

#endif // !LogC