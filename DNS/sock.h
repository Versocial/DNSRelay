#ifndef SOCKH
#define SOCKH

#define WINDOWS
//#define LINUX

#ifdef WINDOWS
#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib")	//加载 ws2_32.dll
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma warning(disable:4996) 
#endif

#ifdef LINUX
#include<sys/socket.h>
#endif // LINUX

//sock.c
void initSock(const char* ip, u_short port);
int recvInfoFrom(char* buf, int len, SOCKADDR* clientAddr);
int sendInfoTo(char* buf, int len, SOCKADDR* clientAddr);
void closeSocket();
SOCKADDR createSockAddr(const char* ip, u_short port);
int sockAddrEqual(const SOCKADDR* a,const SOCKADDR* b);
#endif // !SOCKH