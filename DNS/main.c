#ifndef dnsRelayC
#define dnsRelayC
#include"dnsRelay.h"



int main() {
	initSock("26.82.242.109",1234);
	SOCKADDR DNSserver = createSockAddr("10.3.9.45",53);
	DNS* dns = createDNS();
	SOCKADDR client;
	DNShead head;
	memset(&head, 0, sizeof(head));
	idTable* idMap = createIdTable(0);
	system("pause");
	
	while (1) {
		recvDNS(dns, &client);
		DNShead head = getHead(dns);
		if (sockAddrEqual(&client, &DNSserver)) {
			idInfo info = pollOut(idMap, head.id);
			if (head.flag.qr == 0 && info.deadTime != 0) {
				justChangeId(dns, info.id);
				sendDNS(dns, &info.addr);
			}
		}
		else if(head.flag.qr==0&&1){

		}
		else {
			uint16_t id = insertIdTable(idMap, head.id, &client, 100);
			justChangeId(dns, id);
			sendDNS(dns, &client);
		}
	}
	disposeIdTable(idMap);
	disposeDNS(dns);
	closeSocket();
}


#endif // !dnsRelayC
