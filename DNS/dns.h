#ifndef DNSH
#define DNSH
#define maxIpLen 30
#define maxUrlLen 253
struct FLAG
{
    unsigned qr : 1;     /* response flag */ /* query = 0;response=1*/
    unsigned opcode : 4; /* purpose of message *//*0 表示标准查询；1 表示反向查询；2 表示服务器状态请求*/
    unsigned aa : 1;     /* authoritive answer *//*授权回答*/
    unsigned tc : 1;     /* truncated message *//*过长截断*/
    unsigned rd : 1;     /* recursion desired *//*query请求递归*/
    unsigned ra : 1;     /* recursion available *//*response允许递归*/
    unsigned z : 1;      /* unused bits, must be ZERO *//*0*/
    unsigned cd : 1;     /* checking disabled by resolver *//*0*/
    unsigned ad : 1;     /* authentic data from named *//*0*/
    unsigned rcode : 4;  /* response code *//*值0没有错误、1格式错误、2服务器错误、3名字错误、4服务器不支持、5拒绝*/
};

struct HEADER 
{
    unsigned id : 16;    /* query identification number *//*问答id*/
    struct FLAG flag;   /* flag of 16 bits *//*标志位*/
    unsigned  qdcount : 16;       /* number of question entries *//*query问题数*/
    unsigned  ancount : 16;       /* number of answer entries *//*answer回答数*/
    unsigned  nscount : 16;       /* number of authority entries *//*授权段中的授权记录数*/
    unsigned  arcount : 16;       /* number of resource entries *//*附加段中的附加记录数*/
};

struct DNS
{
    struct HEADER head;


};

enum QueryType //查询的资源记录类型。
{
    A = 0x01, //指定计算机 IP 地址。
    NS = 0x02, //指定用于命名区域的 DNS 名称服务器。
    MD = 0x03, //指定邮件接收站（此类型已经过时了，使用MX代替）
    MF = 0x04, //指定邮件中转站（此类型已经过时了，使用MX代替）
    CNAME = 0x05, //指定用于别名的规范名称。
    SOA = 0x06, //指定用于 DNS 区域的“起始授权机构”。
    MB = 0x07, //指定邮箱域名。
    MG = 0x08, //指定邮件组成员。
    MR = 0x09, //指定邮件重命名域名。
    NULL = 0x0A, //指定空的资源记录
    WKS = 0x0B, //描述已知服务。
    PTR = 0x0C, //如果查询是 IP 地址，则指定计算机名；否则指定指向其它信息的指针。
    HINFO = 0x0D, //指定计算机 CPU 以及操作系统类型。
    MINFO = 0x0E, //指定邮箱或邮件列表信息。
    MX = 0x0F, //指定邮件交换器。
    TXT = 0x10, //指定文本信息。
    UINFO = 0x64, //指定用户信息。
    UID = 0x65, //指定用户标识符。
    GID = 0x66, //指定组名的组标识符。
    ANY = 0xFF //指定所有数据类型。
};

//QClass:长度为16位，表示分类。

enum QueryClass //指定信息的协议组。
{
    IN = 0x01, //指定 Internet 类别。
    CSNET = 0x02, //指定 CSNET 类别。（已过时）
    CHAOS = 0x03, //指定 Chaos 类别。
    HESIOD = 0x04,//指定 MIT Athena Hesiod 类别。
    ANYN = 0xFF //指定任何以前列出的通配符。
};

int formalizeURL(char url[],char*dest) {
    char* dotPos = dest; dest++;
    for (int i = 0; i < maxUrlLen; i++, dest++) {
        if (url[i] == '.') { *dotPos = dest - dotPos - 1; dotPos = dest; }
        else *dest = url[i];
        if (url[i] == 0)return i + 2;
    }return 0;
}



#endif // !DNSH