#ifndef dnsRelayC
#define dnsRelayC
#include"dnsRelay.h"



int main() {
	initLog("..");
	initSock(NULL,53);
	SOCKADDR DNSserver = createSockAddr("10.3.9.44",53);
	DNS* dns = createDNS();
	SOCKADDR client;
	DNShead head;
	memset(&head, 0, sizeof(head));
	idTable* idMap = createIdTable(0);
	initIPFile("dnsrelay.txt");
	system("pause");
	

	while (1) {
		int len = recvDNS(dns, &client); if (len == -1)continue;
		DNShead head = getHead(dns);
		log("recv id:%d qr:%d",head.id,head.flag.qr);
		
		if (head.flag.qr==1&&sockAddrEqual(&client, &DNSserver)) {//response
			///...
			log("Response info from server."); 
			idInfo info = pollOut(idMap, head.id);
			if (info.deadTime != 0) {
				justChangeId(dns, info.id);
				sendDNS(dns, &info.addr);
				log("send back %d", info.id);
			}
		}
		else if(head.flag.qr==1){
			log("recv a response from unexpected ip: %s port:%d", inet_ntoa(((struct sockaddr_in*)&client)->sin_addr),ntohs(((struct sockaddr_in*)&client)->sin_port));
		}
		else {//query
			log("info from client.");
			dnsInfo info = findIP(getQueryUrl(dns));
			if (info.dataLen!=0) {//find the domain
				clearDNS(dns);
				memset(&head, 0, sizeof(head));
				addAnswer(dns, &info);
				head.id = ((DNShead*)dns->buffer)->id;
				head.flag.qr = 1;
				head.ancount = 1;
				setHead(dns, head);
				log("send a local info %s", info.url);
				sendDNS(dns, &client);
			}
			else {//not find the domain
				uint16_t id = insertIdTable(idMap, head.id, &client, 100);
				justChangeId(dns, id);
				log("change id from %d to %d", head.id, id);
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
