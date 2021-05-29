//#ifndef dnsRelayC
//#define dnsRelayC
//#include"dnsRelay.h"
//
//
//
//int main() {
//	printf("%d %d %d\n", sizeof(struct FLAG),sizeof(struct AnswerInfo));
//
//
//	initSock(NULL);
//	SOCKADDR dest = createSockAddr("26.82.242.109");
//	DNS* dns = createDNS();
//	DNShead head;
//	memset(&head, 0, sizeof(head));
//	head.id = 1001;
//	head.flag.opcode =15;
//	head.flag.qr = 1;
//	head.flag.aa = 1;
//	head.qdcount = 1;
//	addQuery(dns, "www.nice.com");
//	head.ancount = 1;
//	dnsInfo* info=calloc(1,sizeof(info));
//	memcpy(info->url , "www.kitty.com",14);
//	info->endTime = time(NULL) + 127;
//	info->dataLen = 4;
//	info->ipv4 = inet_addr("10.1.3.127");
//	addAnswer(dns, info);
//	setHead(dns, head);
//	printDNS(dns, dns->length);
//	sendDNS(dns, &dest);
//	system("pause");
//
//
//	closeSocket();
//}
//
//
//#endif // !dnsRelayC
