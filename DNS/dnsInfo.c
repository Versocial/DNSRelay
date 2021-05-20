#include "dnsInfo.h"
#include <string.h>
int initFile(const char* path)
{
	memset(theInfo, 0, 256 * sizeof(struct dnsInfo*));
	FILE *file = fopen(path, "r");
	if (file == NULL)return -1;
	for (;!feof(file);) {
		struct dnsInfo* temp = calloc(1, sizeof(struct dnsInfo));//calloc 自动初始化为0
		fscanf(file, temp->url);
		fscanf(file, temp->ip);
		temp->next = theInfo[*(temp->url)];
		theInfo[*(temp->url)] = temp;
	}
	fclose(file);
	return 0;
}

void add(const char* ip, const char* url)
{
	struct dnsInfo* temp = calloc(1,sizeof(struct dnsInfo));//calloc 自动初始化为0
	temp->next = theInfo[*url];
	theInfo[*url] = temp;
	strncpy(temp->ip, ip,maxIpLen);
	strncpy(temp->url, url, maxUrlLen);
}

char* findIp(const char* url)
{
	for (struct dnsInfo* now = theInfo[*url]; now != NULL; now = now->next) {
		if (strncmp(url, now->url, maxUrlLen) == 0)return now->ip;
	}
}

int save(const char* path)
{
	FILE* file = fopen(path, "w");
	if (file == NULL)return -1;
	for(int i=0;i<256;i++)
		for (struct dnsInfo* now = theInfo[i]; now != NULL; now = now->next) {
			fprintf(file, now->url); fprintf(file, "\t"); fprintf(file, now->ip);
		}
	fclose(file);
	return 0;
}

void deleteURL(const char* url)
{
}
