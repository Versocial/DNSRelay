#ifndef LOGH
#define LOGH

#define  _CRT_SECURE_NO_WARNINGS
#include<time.h>
#include<stdio.h>
#include <stdlib.h>
#include<stdarg.h>
#include<string.h>

//log.c
FILE* initLog(const char* path);
void log_1(const char* strFormat, ...);
void log_2(const char* strFormat, ...);
void logMem_2(void* mem, int len);
void setLogMod(int fileMod_, int stdoutMod_);
void closeLog();

#endif // !LOGH