//文件管理 和 表项查询
#ifndef DNSINFOH
#define DNSINFOH

#include<stdio.h>
#include<stdlib.h>
#define maxIpLen 30
#define maxUrlLen 253

struct dnsInfo {
	char ip[maxIpLen+1];
	char url[maxUrlLen+1];
	struct dnsInfo* next;
};
static struct  dnsInfo* theInfo[256];//hash link

int initFile(const char* path);

void add(const char* ip, const char* url);

//void deleteIP(const char* url, const char* ip);

//void deleteURL(const char* url);

char* findIp(const char* url);

int save(const char* path);
#endif // !DNSINFOH
