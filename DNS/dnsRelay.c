#ifndef dnsRelayC
#define dnsRelayC
#include"dnsRelay.h"

int main() {
	DNS* dns = createDNS();
	SOCKADDR* addrRecv;




}


#endif // !dnsRelayC
//
//#if 1
//int main() {
//	if (!initLog("log.txt")) {
//		printf("Log died!"); 
//	};
//	log("%s%d","stupid",3);
//	char str[3000] = "0000000000000000000";
//	char str2[3000];
//	struct  sockaddr_in temp=createSockAddr("10.21.204.191");
//	sendStringTo(str, sizeof(str),(SOCKADDR*)&temp);
//	//recvStringFrom(str2, sizeof(str2), (SOCKADDR*)&temp);
//	printf(str);
//	//printf(str2);
//	system("pause");
//	return 0;
//};
//#else
//int main() {
//	if (!initLog("log.txt")) {
//		printf("Log died!");
//	};
//	log("%s%d", "stupid", 3);
//	char str[3000] = "0000000000000000000";
//	char str2[3000];
//	struct  sockaddr_in temp = createSockAddr("10.21.204.191");
//	//sendStringTo(str, sizeof(str), (SOCKADDR*)&temp);
//	recvStringFrom(str2, sizeof(str2), (SOCKADDR*)&temp);
//	//printf(str);
//	printf(str2);
//	system("pause");
//	return 0;
//};
//#endif