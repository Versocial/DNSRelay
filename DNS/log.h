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
void log(const char* strFormat, ...);
void setLogMod(int Mod);
void closeLog();

#endif // !LOGH