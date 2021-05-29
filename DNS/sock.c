#ifndef SockC //解决windows linux 差异
#define SockC
#include "sock.h"

#ifdef WINDOWS
#define LengthType int
static SOCKET servSock;
#endif // WINDOWS

#ifdef LINUX
#define LengthType ssize_t
static int servSock;
#endif // LINUX
static struct sockaddr_in servAddr;
#define DEFAULTPORT 53
#define DEFAULTLOCALPORT 1234

void initSock(const char* ip,u_short port) 
{
#ifdef WINDOWS
    WSADATA originWsaData;
    WSAStartup(MAKEWORD(2, 2), &originWsaData);
#endif
    //套接字初始化
    servSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    //套接地址初始化
    memset(&servAddr, 0, sizeof(servAddr));  //每个字节都用0填充
    servAddr.sin_family = PF_INET;  //使用IPv4地址
    servAddr.sin_addr.s_addr = ip==NULL?htonl(INADDR_ANY): inet_addr(ip); //htonl(INADDR_ANY); //自动获取IP地址
    servAddr.sin_port = htons(port);  //端口
    //绑定套接字和套接地址
    bind(servSock, (SOCKADDR*)&servAddr, sizeof(SOCKADDR));
}


LengthType sendInfoTo(char*buf,int len, SOCKADDR* clientAddr)
{
  return   sendto(servSock, buf,len, 0, clientAddr, sizeof(SOCKADDR));
}

LengthType recvInfoFrom(char* buf, int maxLen, SOCKADDR* clientAddr)
{
    LengthType length=sizeof(struct sockaddr_in);
   return  recvfrom(servSock, buf, maxLen, 0, clientAddr, &length);
}

void closeSocket()
{
    closesocket(servSock);
#ifdef WINDOWS
    WSACleanup();
#endif // WINDOWS

}

//或许仅供测试使用
 SOCKADDR createSockAddr(const char *ip,u_short port) {
    struct sockaddr_in ans;
    memset(&ans, 0, sizeof(ans));  //每个字节都用0填充
    ans.sin_family = PF_INET;  //使用IPv4地址
    ans.sin_addr.s_addr = inet_addr(ip); //自动获取IP地址
    ans.sin_port = htons(port);  //端口
    return *((SOCKADDR*)&ans);
}


 int sockAddrEqual(const SOCKADDR* a, const SOCKADDR* b) {
     return strcmp(inet_ntoa(((struct sockaddr_in*)a)->sin_addr), inet_ntoa(((struct sockaddr_in*)b)->sin_addr)) == 0 && ((struct sockaddr_in*)a)->sin_port== ((struct sockaddr_in*)b)->sin_port;
 }

#endif // !SockC