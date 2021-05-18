#ifndef LogC
#define LogC
#include"log.h"
static FILE* _Log_File;
static time_t _Log_rawTime;
static char _Log_TimeBuffer[50];

FILE* initLog(const char* path) {
	_Log_File= fopen(path, "a+");
	memset(_Log_TimeBuffer, 0, 50);
	time(&_Log_rawTime);
	struct tm* time = localtime(&_Log_rawTime);
	strftime(_Log_TimeBuffer,sizeof(_Log_TimeBuffer), "\nStart Time : %Y/%m/%d %H:%M:%S \n", localtime(&_Log_rawTime));
	fprintf(_Log_File, _Log_TimeBuffer);
	return _Log_File;
}
void log(const char* strFormat, ...) {
	time(&_Log_rawTime);
	struct tm* time = localtime(&_Log_rawTime);
	strftime(_Log_TimeBuffer, sizeof(_Log_TimeBuffer), "\n[ %Y/%m/%d %H:%M:%S ]   ", time);
	printf(_Log_TimeBuffer);
	fprintf(_Log_File, _Log_TimeBuffer);
	va_list ap;
	va_start(ap, strFormat);
	vfprintf(_Log_File, strFormat, ap);
}

#endif // !LogC