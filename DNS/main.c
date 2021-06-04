#ifndef dnsRelayC
#define dnsRelayC
#include"dnsRelay.h"

int main(int argc, char* argv[])
{
	char* DNSServerIP = "10.3.9.44";
	char* filePath = "../res/dnsrelay.txt";
	setLogMod(0, 0);
	if(argc<=1)setLogMod(2, 2);
	for(int i=1;i<argc;i++) {
		printf("%s_",argv[i]);
			if (strcmp("-d", argv[i]) == 0)setLogMod(1,1);
			else if (strcmp("-dd", argv[i]) == 0)setLogMod(2,2);
			else if (inet_addr(argv[i]) != INADDR_NONE) { DNSServerIP = argv[2]; }
			else filePath = argv[i];
	};

	initLog("res/");
	initSock(NULL,53);
	SOCKADDR DNSserver = createSockAddr(DNSServerIP,53);
	DNS* dns = createDNS();
	SOCKADDR client;
	DNShead head;
	memset(&head, 0, sizeof(head));
	idTable* idMap = createIdTable(0);
	initIPFile(filePath);
	system("pause");
	
	time_t lastFreshTime = time(NULL);
	int recvTimes = 0;

	while (1) {
		int len = recvDNS(dns, &client); 
		DNShead head = getHead(dns);
		if (len == -1||len==0) {  log_1("****>>Recv return %d from ip: %s port:%d -id %d [error: %d] ",len, inet_ntoa(((struct sockaddr_in*)&client)->sin_addr), ntohs(((struct sockaddr_in*)&client)->sin_port), head.id, WSAGetLastError()); continue; }
		log_1("==>>Recv [ id:%d ] qr:%d ancount:%d qdcount:%d from IP %s",head.id, head.flag.qr, head.ancount,head.qdcount, inet_ntoa(((struct sockaddr_in*)&client)->sin_addr));
		logMem_2(dns, dns->length);
		if (head.flag.qr == 1) {//response
			///...
			if (sockAddrEqual(&client, &DNSserver)) {
				log_2("Response [ id:%d ] from server : %d answers for query: %s .", head.id,head.ancount,getQueryUrl(dns));
				idInfo info = pollOut(idMap, head.id);
				if (info.deadTime != 0) {
					justChangeId(dns, info.id);
					log_1("<<==Send back it as [ id:%d ] to client IP: %s", info.id, inet_ntoa(((struct sockaddr_in*)&info.addr)->sin_addr));
					sendDNS(dns, &info.addr);
					logMem_2(dns, dns->length);
					addIP(getAnswerIPv4(dns), getQueryUrl(dns));
				}
				else log_1("**Find nothing match this response [ id %d ].",head.id);
			}
			else {
				log_1("**Recv a response from unexpected ip: %s port:%d", inet_ntoa(((struct sockaddr_in*)&client)->sin_addr), ntohs(((struct sockaddr_in*)&client)->sin_port));
			}
		}
		else {//query
			char *url=getQueryUrl(dns);
			dnsInfo info = findIP(url,10);// at least 10s left
			if( /*head.qdcount==1&& getQueryType(dns) == QueryType_A&&*/ info.ipSet.size == 1 && isLocal(info.ipSet)&&info.ipSet.node->ipv4==0) {//find ip 0.0.0.0 in chache
				log_1("Query [ id:%d ] for %s from client.",head.id,dns->buffer+sizeof(DNShead) );
				clearDNS(dns);
				memset(&head, 0, sizeof(head)); 
				addQuery(dns, info.url);
				head.id = ntohs(((DNShead*)dns->buffer)->id);
				head.flag.qr = 1;
				head.flag.aa = 1;//
				head.flag.rd = 1;//
				head.flag.rcode = 3;//name_error
				head.qdcount = 1;
				setHead(dns, head);
				log_1("<<==Send a local refusing visit for %s to ip %s", info.url, inet_ntoa(((struct sockaddr_in*)&client)->sin_addr));
				sendDNS(dns, &client);
				logMem_2(dns, dns->length);
			}
			else if (head.qdcount==1&&(getQueryType(dns)==QueryType_A||isLocal(info.ipSet))&&info.ipSet.size>0) {//find ip in chache
				log_1("IPV4 Query [ id:%d ] for %s from client.", head.id, dns->buffer + sizeof(DNShead));
				clearDNS(dns);
				memset(&head, 0, sizeof(head));
				unsigned char offset= addQuery(dns, info.url);
				addAnswer(dns, &info,offset);
				head.id = ntohs(((DNShead*)dns->buffer)->id);
				head.flag.qr = 1;
				head.flag.rd = 1;
				head.flag.ra = 1;
				head.qdcount = 1;
				head.ancount = info.ipSet.size;
				setHead(dns, head);
				log_1("<<==Send a local or chache info [ id %d ]: url [%s] to ip %s with %d answers",head.id, info.url, inet_ntoa(((struct sockaddr_in*)&client)->sin_addr),head.ancount);
				sendDNS(dns, &client);
				logMem_2(dns, dns->length);
			}
			else {//others
				log_1("[ id:%d ] maybe query for %s from client.", head.id, dns->buffer + sizeof(DNShead));
				uint16_t id = insertIdTable(idMap, head.id, &client, 100);
				justChangeId(dns, id);
				log_1("<<==Change id from %d to %d and send to Server", head.id, id);
				sendDNS(dns, &DNSserver);
				logMem_2(dns, dns->length);
			}
		}
		recvTimes++;
		if (recvTimes % 15 == 0 && time(NULL) - lastFreshTime > 100); {; fixSpaceWaste(idMap, 6); lastFreshTime = time(NULL);  }
	}
	disposeIdTable(idMap);
	disposeDNS(dns);
	closeSocket();
	closeLog();
}


#endif // !dnsRelayC
