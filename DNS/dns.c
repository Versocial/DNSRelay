#ifndef DNSC
#define DNSC
#include "dns.h"
#define DNS_Buffer_Size 65536
#define DEFALUTTTL 7200

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


unsigned char addQuery(DNS* dns, const char* queryUrl)
{
    //memccpy();
    unsigned char offset = dns->length;
    memcpy(dns->buffer + dns->length, queryUrl, strlen(queryUrl) + 1);
    struct QueryInfo* query = dns->buffer+dns->length+strlen(queryUrl)+1;
    query->type = htons(QueryType_A);
    query->class = htons(QueryClass_IN);
    dns->length += strlen(queryUrl) + 1+sizeof(struct QueryInfo);
    return offset;// return offset of url
}

unsigned char addAnswer(DNS* dns,const dnsInfo* info, unsigned char urlOffset)
{
    int number = 0;
    unsigned char offset=urlOffset;
    for (struct IPNode* ip = info->ipSet.node; ip != NULL; ip = ip->next) {
        if (urlOffset == 0) {//0 means no offset can be use.
            memcpy(dns->buffer + dns->length, info->url, strlen(info->url) + 1);
            dns->length += strlen(info->url) + 1;
            offset = dns->length;
        }
        else {
            *(dns->buffer + dns->length) = 0xc0; *(dns->buffer + dns->length + 1) = offset;
            dns->length += 2;
        }

        struct AnswerInfo* answer = (struct AnswerInfo*)(dns->buffer + dns->length);
        answer->type = htons(QueryType_A);
        answer->class = htons(QueryClass_IN);
        answer->ttl = (ip->killTime == 0) ? htonl(DEFALUTTTL) : htonl(ip->killTime - time(NULL));
        answer->dataLength = htons(4);
        *(uint32_t*)((char*)answer + sizeof(struct AnswerInfo)) = ip->ipv4;
        dns->length += sizeof(struct AnswerInfo) + ntohs(answer->dataLength);
        number++;
    }
    log("add %d answers",number);
    return offset;
}

int sendDNS(DNS* dns, SOCKADDR* dest)
{
  int flag= sendInfoTo(dns->buffer, dns->length, dest); 
  if (flag == -1)log("sendDNS error: dns id %d",((DNShead*)dns->buffer)->id);
  return flag;
}

int recvDNS(DNS* dns, SOCKADDR* source)
{
    int len= recvInfoFrom(dns->buffer,dns->bufferLen,source);
    dns->length = len == -1 ? 0 : len;
    return len;
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
     }printf("\n\n");
 }

 void justChangeId(DNS*dns, unsigned short id)
 {
     DNShead* netHead = (DNShead*)dns->buffer;
     netHead->id = htons(id);
 }


 char* getQueryUrl(DNS* dns) {
     //if(dns->buffer)
     return dns->buffer + sizeof(DNShead);
 }

IPLink getAnswerIPv4(DNS* dns) {
    int number = 0;
    time_t timeNow = time(NULL);
    IPLink link=newIPlink();
     struct IPNode* start=NULL;
     DNShead head = getHead(dns);
     int queryNum=head.qdcount;
     int answerNum=head.ancount; 
     int offset = sizeof(head);
     for (int i = 0; i < queryNum; i++) {
         if (dns->buffer[offset] == 0xc0)offset += 2;
         else { while (dns->buffer[offset] != 0)offset++; offset++; }
         offset += sizeof(struct QueryInfo);//
     }
     for (int i = 0; i < answerNum; i++) {

         if (dns->buffer[offset] == 0xc0)offset += 2;
         else { while (dns->buffer[offset] != 0)offset++; offset++; }
         struct AnswerInfo* answerInfo = (struct AnswerInfo*)(dns->buffer + offset);
         int len= ntohs((answerInfo)->dataLength) ;
         unsigned short temp1 = ntohl((answerInfo)->ttl);

         time_t t = ntohl((answerInfo)->ttl) + timeNow;
         offset += sizeof(struct AnswerInfo);
         if (ntohs(answerInfo->type)==QueryType_A&&len == 4) {
             addIPNode(&link, (*(uint32_t*)(dns->buffer + offset)), t);
             number++;
         }
         offset += len;
     }
     log("find %d IP", number);
     return link;
 }


#endif //DNSC