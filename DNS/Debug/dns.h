#ifndef DNSH
#define DNSH
#include "sock.h"
#include"log.h"
#include "dnsInfo.h"
#include <stdint.h>
#define maxIpLen 30
#define maxUrlLen 253

#pragma pack(1)
struct FLAG  //order matters
{
	uint16_t rd : 1;     /* recursion desired *//*query请求递归*/
	uint16_t tc : 1;     /* truncated message *//*过长截断*/
	uint16_t aa : 1;     /* authoritive answer *//*授权回答*/
	uint16_t opcode : 4; /* purpose of message *//*0 表示标准查询；1 表示反向查询；2 表示服务器状态请求*/
	uint16_t qr : 1;     /* response flag */ /* query = 0;response=1*/

	uint16_t rcode : 4;  /* response code *//*值0没有错误、1格式错误、2服务器错误、3名字错误、4服务器不支持、5拒绝*/
	uint16_t cd : 1;     /* checking disabled by resolver *//*0*/
	uint16_t ad : 1;     /* authentic data from named *//*0*/
	uint16_t z : 1;      /* unused bits, must be ZERO *//*0*/
	uint16_t ra : 1;     /* recursion available *//*response允许递归*/
};

typedef  struct
{
	uint16_t id : 16;    /* query identification number *//*问答id*/
	struct FLAG flag;   /* flag of 16 bits *//*标志位*/
	uint16_t  qdcount : 16;       /* number of question entries *//*query问题数*/
	uint16_t  ancount : 16;       /* number of answer entries *//*answer回答数*/
	uint16_t  nscount : 16;       /* number of authority entries *//*授权段中的授权记录数*/
	uint16_t  arcount : 16;       /* number of resource entries *//*附加段中的附加记录数*/
} DNShead;

struct QueryInfo {
	uint32_t type : 16;
	uint32_t class :16;
};

struct AnswerInfo {
	unsigned type : 16;
	unsigned class :16;
	unsigned int ttl;
	unsigned short dataLength;
};

#pragma pack()
typedef struct
{
	unsigned int bufferLen;
	 int length;
	unsigned char* buffer;

}DNS;


enum QueryType //查询的资源记录类型。
{
	QueryType_A = 0x01, //指定计算机 IP 地址。
	QueryType_NS = 0x02, //指定用于命名区域的 DNS 名称服务器。
	QueryType_MD = 0x03, //指定邮件接收站（此类型已经过时了，使用MX代替）
	QueryType_MF = 0x04, //指定邮件中转站（此类型已经过时了，使用MX代替）
	QueryType_CNAME = 0x05, //指定用于别名的规范名称。
	QueryType_SOA = 0x06, //指定用于 DNS 区域的“起始授权机构”。
	QueryType_MB = 0x07, //指定邮箱域名。
	QueryType_MG = 0x08, //指定邮件组成员。
	QueryType_MR = 0x09, //指定邮件重命名域名。
	QueryType_NULL = 0x0A, //指定空的资源记录
	QueryType_WKS = 0x0B, //描述已知服务。
	QueryType_PTR = 0x0C, //如果查询是 IP 地址，则指定计算机名；否则指定指向其它信息的指针。
	QueryType_HINFO = 0x0D, //指定计算机 CPU 以及操作系统类型。
	QueryType_MINFO = 0x0E, //指定邮箱或邮件列表信息。
	QueryType_MX = 0x0F, //指定邮件交换器。
	QueryType_TXT = 0x10, //指定文本信息。
	QueryType_UINFO = 0x64, //指定用户信息。
	QueryType_UID = 0x65, //指定用户标识符。
	QueryType_GID = 0x66, //指定组名的组标识符。
	QueryType_ANY = 0xFF //指定所有数据类型。
};


//QClass:长度为16位，表示分类。


enum QueryClass //指定信息的协议组。
{
	QueryClass_IN = 0x01, //指定 Internet 类别。
	QueryClass_ICSNET = 0x02, //指定 CSNET 类别。（已过时）
	QueryClass_ICHAOS = 0x03, //指定 Chaos 类别。
	QueryClass_IHESIOD = 0x04,//指定 MIT Athena Hesiod 类别。
	QueryClass_IANY = 0xFF //指定任何以前列出的通配符。
};




char* getQueryUrl(DNS* dns);

DNS* createDNS();

void disposeDNS(DNS* dns);

void addQuery(DNS* dns, const char* queryUrl);

void addAnswer(DNS* dns, const dnsInfo* info);

int sendDNS(DNS* dns, SOCKADDR* dest);

int recvDNS(DNS* dns, SOCKADDR* source);

DNShead getHead(DNS* dns);

void clearDNS(DNS* dns);

void setHead(DNS* dns, DNShead head);

void printDNS(DNS* dns, int len);

void justChangeId(DNS* dns, unsigned short id);

#endif // !DNSH












