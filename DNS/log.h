#ifndef LOGH
#define LOGH

#define  _CRT_SECURE_NO_WARNINGS
#include<time.h>
#include<stdio.h>
#include <stdlib.h>
#include<stdarg.h>

//log.c
FILE* initLog(const char* path);
void log(const char* strFormat, ...);

#endif // !LOGH