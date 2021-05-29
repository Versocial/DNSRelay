//#ifndef dnsRelayC
//#define dnsRelayC
//#include"dnsRelay.h"
//
//
//
//int main() {
//
//	initSock(NULL,53);
//	SOCKADDR dest = createSockAddr("10.3.9.45",53);
//	DNS* dns = createDNS();
//	DNShead head;
//	memset(&head, 0, sizeof(head));
//	head.id = 1001;
//	head.flag.rd = 1;
//	char tempUrl[50] = "www.baidu.com";
//	char tempUrl2[50];
//	formalizeURL(tempUrl, tempUrl2);
//	addQuery(dns, tempUrl2);
//	head.qdcount = 1;
//	setHead(dns, head);
//	printDNS(dns, dns->length);
//	sendDNS(dns, &dest);
//	SOCKADDR dest2 = createSockAddr("10.3.9.45",53);
//	int len=recvDNS(dns, &dest2);
//	printf("\n%d\n", len);
//	printDNS(dns, len);
//	system("pause");
//
//
//	closeSocket();
//}
//
//
//#endif // !dnsRelayC
