#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Wsock32.lib")
#include <Winsock2.h>
#include <stdio.h>
void CheckIP(void) //CheckIP函数，用于获取本机IP地址
{
	WORD wVersionRequested;//WORD类型变量，用于存放Winsock版本的值
	WSADATA wsaData;

	char name[255];//用于存放主机名
	PHOSTENT hostinfo;
	wVersionRequested = MAKEWORD(2, 0);
	//调用MAKEWORD()函数获得Winsock的版本，用于加载Winsock库
	if (WSAStartup(wVersionRequested, &wsaData) == 0)
	{
		//加载Winsock库，如果WSAStartup()函数的返回值为0，说明加载成功
		if (gethostname(name, sizeof(name)) == 0)
		{
			//判断是否成功的将本地主机名存放入由name参数指定的缓冲区中
			if ((hostinfo = gethostbyname(name)) != NULL)
			{
				//如果获得主机名成功的话，调用inet_ntoa()函数取得IP地址
				LPCSTR ip = inet_ntoa(*(struct in_addr*)*hostinfo->h_addr_list);
				printf("本机的IP地址是   <<%s>>\n", ip);//输出IP地址 
				printf("本机的名称是   <<%s>>\n", name);
			}
		} 
		WSACleanup();//卸载Winsock库，并释放所有资源
	}
}

int main()
{
	CheckIP();//调用CheckIP()函数获得并输出IP地址
	system("pause");
	return 0;
}
