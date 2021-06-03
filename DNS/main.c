#ifndef dnsRelayC
#define dnsRelayC
#include"dnsRelay.h"



int main(int argc, char* argv[])
{
	char* DNSServerIP = "10.3.9.44";
	char* filePath = "../res/dnsrelay.txt";
	if (argc > 1) {
		if (argc >= 2) {
			if (strcmp("-d", argv[1]) == 0)setLogMod(1);
			else if (strcmp("-dd", argv[1]) == 0)setLogMod(2);
		}
		if (argc >= 3) {
			if (inet_addr(argv[2]) != INADDR_NONE) { DNSServerIP = argv[2]; }
			else log("The ip %s is invalid!",argv[2]);
		}
		if (argc >= 4) {
			filePath = argv[3];
		}
	};

	initLog("../res");
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
		int len = recvDNS(dns, &client); //log("recv %d", len);
		DNShead head = getHead(dns);
		if (len == -1) { WSAGetLastError(); log(">>recv -1 from ip: %s port:%d -id %d", inet_ntoa(((struct sockaddr_in*)&client)->sin_addr), ntohs(((struct sockaddr_in*)&client)->sin_port), head.id); continue; }
		log(">>recv id:%d qr:%d ancount:%d qdcount:%d from IP %s",head.id, head.flag.qr, head.ancount,head.qdcount, inet_ntoa(((struct sockaddr_in*)&client)->sin_addr));
		
		if (head.flag.qr == 1) {//response
			///...
			if (sockAddrEqual(&client, &DNSserver)) {
				log("Response id %d info from server url %s .",head.id,getQueryUrl(dns));
				idInfo info = pollOut(idMap, head.id);
				if (info.deadTime != 0) {
					//printDNS(dns, dns->length);
					justChangeId(dns, info.id);
					log(" xxxxxx"); printDNS(dns, dns->length);
					//system("pause");
					sendDNS(dns, &info.addr);
					addIP(getAnswerIPv4(dns), getQueryUrl(dns));
					log("send back id %d to client IP: %s", info.id, inet_ntoa(((struct sockaddr_in*)&info.addr)->sin_addr));
				}
				else log("find nothing match this id %d.",head.id);
			}
			else {
				log("recv a response from unexpected ip: %s port:%d", inet_ntoa(((struct sockaddr_in*)&client)->sin_addr), ntohs(((struct sockaddr_in*)&client)->sin_port));
			}
		}
		else {//query
			log("info from client.");
			char *url=getQueryUrl(dns);
			dnsInfo info = findIP(url,10);// at least 10s left
			if(head.qdcount==1 && getQueryType(dns) == QueryType_A &&info.ipSet.size == 1 && isLocal(info.ipSet)&&info.ipSet.node->ipv4==0) {//fliter--local--0
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
				log("send a local refusing visit : url [%s] to ip %s", info.url, inet_ntoa(((struct sockaddr_in*)&client)->sin_addr));
				sendDNS(dns, &client);
			}
			else if (head.qdcount==1&&getQueryType(dns)==QueryType_A&&info.ipSet.size>0) {//find the domain-not-local0
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
				log("send a local or chache infoid %d : url [%s] to ip %s",head.id, info.url, inet_ntoa(((struct sockaddr_in*)&client)->sin_addr));
				log(" yyyyyyyy"); printDNS(dns, dns->length);
				//system("pause");
				sendDNS(dns, &client);
			}
			else {//not find the domain
				uint16_t id = insertIdTable(idMap, head.id, &client, 100);
				justChangeId(dns, id);
				log("change id from %d to %d and send to Server", head.id, id);
				sendDNS(dns, &DNSserver);
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
