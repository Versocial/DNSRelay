#ifndef dnsRelayH
#define dnsRelayH
#define  _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment (lib, "ws2_32.lib")  //加载 ws2_32.dll
#define WINDOWS
//#define LINUX



#ifdef WINDOWS
#include <winsock2.h>
#endif

#ifdef LINUX
#include<sys/socket.h>
#endif // LINUX



//sock.c
struct sockaddr_in createSockAddr(const char* str);
int recvStringFrom(char* buf, int len, SOCKADDR* clientAddr);
int sendStringTo(char* buf, int len, SOCKADDR* clientAddr);

#endif // dnsRelayH
