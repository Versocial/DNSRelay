#include "dnsInfo.h"
#include<time.h>
#include <string.h>
#include "log.h"

int formalizeURL(char url[], char* dest) {
	char* dotPos = dest; dest++;
	for (int i = 0; i < maxUrlLen; i++, dest++) {
		if (url[i] == '.') { *dotPos = dest - dotPos - 1; dotPos = dest; }
		else *dest = url[i];
		if (url[i] == 0) { *dotPos = dest - dotPos - 1; return i + 2; }
	}return 0;
}

int initIPFile(const char* path)
{
	memset(theInfo, 0, 256 * sizeof(struct DNSINFO*));
	FILE *file = fopen(path, "r");
	if (file == NULL) { log("IPfile init fopen %s error.",path); return -1; }
	char tempUrl[maxUrlLen + 1];
	char tempIp[maxIpLen + 1];
	for (;!feof(file);) {
		struct DNSINFO* now = calloc(1, sizeof(struct DNSINFO));//calloc 自动初始化为0
		fscanf(file, tempUrl);
		formalizeURL(now->url, tempUrl);
		unsigned int a, b, c, d;
		fscanf(file, "%d.%d.%d.%d",&a,&b,&c,&d);
		now->ipv4 = a*256*256*256+b*256*256+c*256+d;
		now->endTime = 0;
		now->next = theInfo[*(now->url)];
		now->dataLen = 4;
		theInfo[*(now->url)] = now;
	}
	fclose(file);
	return 0;
}

void addIP(const unsigned int ipv4, const char* url,time_t ttl)
{
	struct DNSINFO* temp = calloc(1,sizeof(struct DNSINFO));//calloc 自动初始化为0
	temp->next = theInfo[*url];
	theInfo[*url] = temp;
	 temp->endTime=time(NULL)+ttl;
	 temp->ipv4 = ipv4;
	strncpy(temp->url, url, maxUrlLen);
}

dnsInfo findIP(const char* url)
{
	time_t t = time(NULL); struct DNSINFO* prev = NULL;
	for (struct DNSINFO* now = theInfo[*url]; now != NULL; prev = now, now = now->next) {
		if (now->endTime != 0 && now->endTime < t) { 
			if (prev == NULL) { theInfo[*url] = NULL; }
			else { prev->next = now->next; }
			free(now);
		}
		if (strncmp(url, now->url, maxUrlLen) == 0)return *now;
	}
	struct DNSINFO temp;
	temp.dataLen = 0;
	return temp;
}
//
//int save(const char* path)
//{
//	FILE* file = fopen(path, "w");
//	if (file == NULL)return -1;
//	for(int i=0;i<256;i++)
//		for (struct DNSINFO* now = theInfo[i]; now != NULL; now = now->next) {
//			fprintf(file, now->url); fprintf(file, "\t"); fprintf(file, now->ip);
//		}
//	fclose(file);
//	return 0;
//}

void deleteURL(const char* url)
{
}
