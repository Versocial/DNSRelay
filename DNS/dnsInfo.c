#ifndef DNSINFOC
#define DNSINFOC
#include "dnsInfo.h"

int formalizeURL(char dest[], const char* src)
{
	char* dotPos = dest; dest++;
	for (int i = 0; i < maxUrlLen; i++, dest++) {
		if (src[i] == '.') { *dotPos = dest - dotPos - 1; dotPos = dest; }
		else *dest = src[i];
		if (src[i] == 0) { *dotPos = dest - dotPos - 1; return i + 2; }
	}return 0;
}

dnsInfo* createDnsInfo()
{
	struct DNSINFO* now = calloc(1, sizeof(struct DNSINFO));//calloc 自动初始化为0
	now->ipSet = newIPlink();
	return now;
}

int initIPFile(const char* path)
{
	memset(theInfo, 0, 256 * sizeof(struct DNSINFO*));
	FILE *file = fopen(path, "r");
	if (file == NULL) { log_1("IPfile init fopen %s error.",path); return -1; }
	char tempUrl[maxUrlLen + 1];
	int number = 0;
	for (;!feof(file);) {
		unsigned int a, b, c, d;
		fscanf(file, "%d.%d.%d.%d",&a,&b,&c,&d);
		dnsInfo* now = createDnsInfo();
		addIPNode( &(now->ipSet), (d<<24)+(c<<16)+(b<<8)+a,0);//net order
		fscanf(file, "%s",tempUrl);
		formalizeURL(now->url, tempUrl);
		now->next = theInfo[*(now->url)];
		theInfo[*(now->url)] = now;
		number++;
	}
	log_1("IPfile init %d ip in %s",number,path);
	fclose(file);
	return 0;
}




void addIP(IPLink ip, const char* url)
{
	struct DNSINFO* temp = calloc(1,sizeof(struct DNSINFO));//calloc 自动初始化为0
	temp->next = theInfo[*url];
	theInfo[*url] = temp;
	temp->ipSet = ip;
	strncpy(temp->url, url, maxUrlLen);
}

dnsInfo findIP(const char* url,time_t lowestLeft)
{
	 struct DNSINFO* prev = NULL;
	for (struct DNSINFO* now = theInfo[*url]; now != NULL; prev = now, now = now->next) {
		if (strncmp(url, now->url, maxUrlLen) == 0) {
			//log("", now->url);
			refresh(&(now->ipSet),lowestLeft);
			if (now->ipSet.size > 0) {
				if (prev != NULL) {//ti qian
					prev->next = now->next;
					now->next = theInfo[*url];
					theInfo[*url] = now;
				}
				log_2("Find url : %s", now->url);
				return *now;
			}
			else {
				log_1("Dated url : %s", now->url);
				if (prev == NULL)theInfo[*url] = now->next;
				else { prev->next=now->next; }
				free(now);
			}
			break;
		}
	}
	struct DNSINFO temp;
	memcpy(temp.url, url, strlen(url) + 1);
	temp.ipSet.size = 0;
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

#endif // !DNSINFOC
