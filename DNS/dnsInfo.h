//文件管理 和 表项查询
#ifndef DNSINFOH
#define DNSINFOH
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#define maxIpLen 30
#define maxUrlLen 253

typedef struct DNSINFO {
	//char ip[maxIpLen+1];
	char url[maxUrlLen+1];
	unsigned dataLen : 16;
	unsigned ipv4:32;
	time_t endTime;
	struct DNSINFO* next;
} dnsInfo;

static  dnsInfo* theInfo[256];//hash link

int initIPFile(const char* path);

void addIP(const unsigned int ipv4, const char* url, time_t ttl);

//void deleteIP(const char* url, const char* ip);

//void deleteURL(const char* url);

dnsInfo findIP(const char* url);

int formalizeURL(char dest[], const char* src);
//int save(const char* path);
#endif // !DNSINFOH
