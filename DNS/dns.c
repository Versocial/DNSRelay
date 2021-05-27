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
    memcpy(dns->buffer + dns->length, queryUrl, strlen(queryUrl) + 2);
    dns->length += strlen(queryUrl) + 2;
    return;
}

void addAnswer(DNS* dns,dnsInfo* info)
{
   //memcpy()
}

 DNShead getHead(DNS* dns)
{
    DNShead head;
    DNShead* netHead = dns->buffer;
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
     dns->length = 20;
 }
 void setHead(DNS* dns,DNShead head)
 {
     DNShead *netHead= dns->buffer;
     netHead->id = htons(head.id);
     netHead->ancount = htons(head.ancount);
     netHead->arcount = htons(head.arcount);
     netHead->qdcount = htons(head.qdcount);
     netHead->nscount = htons(head.nscount);
     netHead->flag = head.flag;
 }
