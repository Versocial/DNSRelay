//文件管理 和 表项查询
#ifndef DNSINFOH
#define DNSINFOH
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include "IP.h"
#include<time.h>
#include <string.h>
#include "log.h"
#define maxIpLen 30
#define maxUrlLen 253


typedef struct DNSINFO {
	char url[maxUrlLen+1];
	IPLink ipSet;
	struct DNSINFO* next;
} dnsInfo;

static  dnsInfo* theInfo[256];//hash link

dnsInfo* createDnsInfo();

int initIPFile(const char* path);

void addIP(IPLink ip, const char* url);

dnsInfo findIP(const char* url, time_t lowestLeft);

int formalizeURL(char dest[], const char* src);

#endif // !DNSINFOH
