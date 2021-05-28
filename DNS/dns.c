#ifndef DNSC
#define DNSC
#include "dns.h"
#define DNS_Buffer_Size 65536

int formalizeURL(char url[], char* dest) {
    char* dotPos = dest; dest++;
    for (int i = 0; i < maxUrlLen; i++, dest++) {
        if (url[i] == '.') { *dotPos = dest - dotPos - 1; dotPos = dest; }
        else *dest = url[i];
        if (url[i] == 0)return i + 2;
    }return 0;
}

DNS* createDNS()
{
    DNS* dns = calloc(1,sizeof(DNS));
    if (dns == NULL) { log("DNS calloc error.\n"); }
    dns->buffer = calloc(DNS_Buffer_Size, sizeof(char));
    if (dns->buffer == NULL) { log("DNS buffer calloc error.\n "); }
    dns->bufferLen = DNS_Buffer_Size;
    dns->length = sizeof(DNShead);
    //....
    return dns;
}

void disposeDNS(DNS* dns) {
    if (dns&&dns->buffer != NULL)free(dns->buffer);
    if (dns)free(dns);
}


void addQuery(DNS* dns, const char* queryUrl)
{
    //memccpy();
    memcpy(dns->buffer + dns->length, queryUrl, strlen(queryUrl) + 1);
    struct QueryInfo* query = dns->buffer+dns->length+strlen(queryUrl)+1;
    query->type = htons(QueryType_A);
    query->class = htons(QueryClass_IN);
    dns->length += strlen(queryUrl) + 1+sizeof(struct QueryInfo);
    return;
}

void addAnswer(DNS* dns,const dnsInfo* info)
{
   //memcpy();
    memcpy(dns->buffer + dns->length, info->url, strlen(info->url)+1);
    struct AnswerInfo* answer = dns->buffer + dns->length + strlen(info->url) + 1;
    answer->type = htons(QueryType_A);
    answer->class = htons(QueryClass_IN);
    answer->ttl = htonl(info->endTime-time(NULL));
    answer->dataLength = htons(info->dataLen);
    *(uint32_t*)((char*)answer + sizeof(struct AnswerInfo) )= info->ipv4; 
    dns->length += strlen(info->url) + 1+sizeof(struct AnswerInfo)+ info->dataLen;
    return;
}

int sendDNS(DNS* dns, SOCKADDR* dest)
{
   return  sendInfoTo(dns->buffer, dns->length, dest); 
}

int recvDNS(DNS* dns, SOCKADDR* source)
{
    return recvInfoFrom(dns->buffer,dns->bufferLen,source);
}

 DNShead getHead(DNS* dns)
{
    DNShead head;
    DNShead* netHead =(DNShead*) dns->buffer;
    head.id = ntohs(netHead->id);
    head.ancount = ntohs(netHead->ancount);
    head.arcount = ntohs(netHead->arcount);
    head.qdcount = ntohs(netHead->qdcount);
    head.nscount = ntohs(netHead->nscount);
    head.flag = netHead->flag;
    return head;
}
 void clearDNS(DNS* dns)
 {
     dns->length = sizeof(DNShead);
 }

 void setHead(DNS* dns,DNShead head)
 {
     DNShead *netHead=(DNShead*) dns->buffer;
     netHead->id = htons(head.id);
     netHead->ancount = htons(head.ancount);
     netHead->arcount = htons(head.arcount);
     netHead->qdcount = htons(head.qdcount);
     netHead->nscount = htons(head.nscount);
     netHead->flag = head.flag;
 }

 void printDNS(DNS* dns, int len)
 {
     int k = 0;
     while (k <len) {
         printf("%2x ", dns->buffer[k]);
         k++;
         if (k % 8== 0)printf("\n");
     }
 }

 void justChangeId(DNS*dns, unsigned short id)
 {
     DNShead* netHead = (DNShead*)dns->buffer;
     netHead->id = htons(id);
 }

#endif //DNSC