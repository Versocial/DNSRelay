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
	uint16_t rd : 1;     /* recursion desired *//*query����ݹ�*/
	uint16_t tc : 1;     /* truncated message *//*�����ض�*/
	uint16_t aa : 1;     /* authoritive answer *//*��Ȩ�ش�*/
	uint16_t opcode : 4; /* purpose of message *//*0 ��ʾ��׼��ѯ��1 ��ʾ�����ѯ��2 ��ʾ������״̬����*/
	uint16_t qr : 1;     /* response flag */ /* query = 0;response=1*/

	uint16_t rcode : 4;  /* response code *//*ֵ0û�д���1��ʽ����2����������3���ִ���4��������֧�֡�5�ܾ�*/
	uint16_t cd : 1;     /* checking disabled by resolver *//*0*/
	uint16_t ad : 1;     /* authentic data from named *//*0*/
	uint16_t z : 1;      /* unused bits, must be ZERO *//*0*/
	uint16_t ra : 1;     /* recursion available *//*response����ݹ�*/
};

typedef  struct
{
	uint16_t id : 16;    /* query identification number *//*�ʴ�id*/
	struct FLAG flag;   /* flag of 16 bits *//*��־λ*/
	uint16_t  qdcount : 16;       /* number of question entries *//*query������*/
	uint16_t  ancount : 16;       /* number of answer entries *//*answer�ش���*/
	uint16_t  nscount : 16;       /* number of authority entries *//*��Ȩ���е���Ȩ��¼��*/
	uint16_t  arcount : 16;       /* number of resource entries *//*���Ӷ��еĸ��Ӽ�¼��*/
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


enum QueryType //��ѯ����Դ��¼���͡�
{
	QueryType_A = 0x01, //ָ������� IP ��ַ��
	QueryType_NS = 0x02, //ָ��������������� DNS ���Ʒ�������
	QueryType_MD = 0x03, //ָ���ʼ�����վ���������Ѿ���ʱ�ˣ�ʹ��MX���棩
	QueryType_MF = 0x04, //ָ���ʼ���תվ���������Ѿ���ʱ�ˣ�ʹ��MX���棩
	QueryType_CNAME = 0x05, //ָ�����ڱ����Ĺ淶���ơ�
	QueryType_SOA = 0x06, //ָ������ DNS ����ġ���ʼ��Ȩ��������
	QueryType_MB = 0x07, //ָ������������
	QueryType_MG = 0x08, //ָ���ʼ����Ա��
	QueryType_MR = 0x09, //ָ���ʼ�������������
	QueryType_NULL = 0x0A, //ָ���յ���Դ��¼
	QueryType_WKS = 0x0B, //������֪����
	QueryType_PTR = 0x0C, //�����ѯ�� IP ��ַ����ָ���������������ָ��ָ��������Ϣ��ָ�롣
	QueryType_HINFO = 0x0D, //ָ������� CPU �Լ�����ϵͳ���͡�
	QueryType_MINFO = 0x0E, //ָ��������ʼ��б���Ϣ��
	QueryType_MX = 0x0F, //ָ���ʼ���������
	QueryType_TXT = 0x10, //ָ���ı���Ϣ��
	QueryType_UINFO = 0x64, //ָ���û���Ϣ��
	QueryType_UID = 0x65, //ָ���û���ʶ����
	QueryType_GID = 0x66, //ָ�����������ʶ����
	QueryType_ANY = 0xFF //ָ�������������͡�
};


//QClass:����Ϊ16λ����ʾ���ࡣ


enum QueryClass //ָ����Ϣ��Э���顣
{
	QueryClass_IN = 0x01, //ָ�� Internet ���
	QueryClass_ICSNET = 0x02, //ָ�� CSNET ��𡣣��ѹ�ʱ��
	QueryClass_ICHAOS = 0x03, //ָ�� Chaos ���
	QueryClass_IHESIOD = 0x04,//ָ�� MIT Athena Hesiod ���
	QueryClass_IANY = 0xFF //ָ���κ���ǰ�г���ͨ�����
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












