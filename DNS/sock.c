#ifndef SockC //解决windows linux 差异
#define SockC
#include "dnsRelay.h"
#ifdef WINDOWS
#define LengthType int
static SOCKET servSock;
#endif // WINDOWS
#ifdef LINUX
#define LengthType ssize_t
static int servSock;
#endif // LINUX
static struct sockaddr_in servAddr;
#define DEFAULTPORT 1234

void initSock() 
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
    servAddr.sin_addr.s_addr = inet_addr("10.21.204.191"); //htonl(INADDR_ANY); //自动获取IP地址
    servAddr.sin_port = htons(DEFAULTPORT);  //端口
    //绑定套接字和套接地址
    bind(servSock, (SOCKADDR*)&servAddr, sizeof(SOCKADDR));
}


inline LengthType sendStringTo(char*buf,int len, SOCKADDR* clientAddr)
{
  return   sendto(servSock, buf,len, 0, (struct sockaddr*)clientAddr, sizeof(SOCKADDR));
}

inline LengthType recvStringFrom(char* buf, int len, SOCKADDR* clientAddr)
{
    LengthType length;
   return  recvfrom(servSock, buf, len, 0, clientAddr, &length);
}

inline void closeSocket()
{
    closesocket(servSock);
#ifdef WINDOWS
    WSACleanup();
#endif // WINDOWS

}

//或许仅供测试使用
struct sockaddr_in createSockAddr(const char *str) {
    struct sockaddr_in ans;
    memset(&ans, 0, sizeof(ans));  //每个字节都用0填充
    ans.sin_family = PF_INET;  //使用IPv4地址
    ans.sin_addr.s_addr = inet_addr(str); //自动获取IP地址
    ans.sin_port = htons(DEFAULTPORT);  //端口
    return ans;
}

#endif // !SockC