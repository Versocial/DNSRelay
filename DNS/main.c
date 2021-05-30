#ifndef dnsRelayC
#define dnsRelayC
#include"dnsRelay.h"



int main() {
	initLog("../res");
	initSock(NULL,53);
	SOCKADDR DNSserver = createSockAddr("10.3.9.44",53);
	DNS* dns = createDNS();
	SOCKADDR client;
	DNShead head;
	memset(&head, 0, sizeof(head));
	idTable* idMap = createIdTable(0);
	initIPFile("../res/dnsrelay.txt");
	system("pause");
	

	while (1) {
		int len = recvDNS(dns, &client); if (len == -1)continue;
		DNShead head = getHead(dns);
		log(">>recv id:%d qr:%d from IP %s",head.id,head.flag.qr, inet_ntoa(((struct sockaddr_in*)&client)->sin_addr));
		
		if (head.flag.qr == 1) {//response
			///...
			if (sockAddrEqual(&client, &DNSserver)) {
				log("Response info from server.");
				idInfo info = pollOut(idMap, head.id);
				if (info.deadTime != 0) {
					justChangeId(dns, info.id);
					sendDNS(dns, &info.addr);
					log("send back %d to client IP: %s", info.id, inet_ntoa(((struct sockaddr_in*)&info.addr)->sin_addr));
				}
				else log("find nothing match this id.");
			}
			else {
				log("recv a response from unexpected ip: %s port:%d", inet_ntoa(((struct sockaddr_in*)&client)->sin_addr), ntohs(((struct sockaddr_in*)&client)->sin_port));
			}
		}
		else {//query
			log("info from client.");
			dnsInfo info = findIP(getQueryUrl(dns));
			if (info.dataLen != 0 && info.ipv4 == 0) {
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
			if (info.dataLen!=0&&info.ipv4!=0) {//find the domain
				clearDNS(dns);
				memset(&head, 0, sizeof(head));
				unsigned char offset= addQuery(dns, info.url);
				addAnswer(dns, &info,offset);
				head.id = ntohs(((DNShead*)dns->buffer)->id);
				head.flag.qr = 1;
				head.flag.rd = 1;
				head.flag.ra = 1;
				head.qdcount = 1;
				head.ancount = 1;
				setHead(dns, head);
				log("send a local info : url [%s] to ip %s", info.url, inet_ntoa(((struct sockaddr_in*)&client)->sin_addr));
				//printDNS(dns, dns->length);
				sendDNS(dns, &client);
			}
			else {//not find the domain
				uint16_t id = insertIdTable(idMap, head.id, &client, 100);
				justChangeId(dns, id);
				log("change id from %d to %d and send to Server", head.id, id);
				sendDNS(dns, &DNSserver);
			}
		}
		
	}
	disposeIdTable(idMap);
	disposeDNS(dns);
	closeSocket();
	closeLog();
}


#endif // !dnsRelayC
