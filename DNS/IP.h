#ifndef IPH
#define IPH
#include<stdlib.h>
#include<time.h>

typedef struct {
	int size ;
	struct IPNode* node;
} IPLink;

 struct IPNode {
	unsigned ipv4 : 32;
	time_t killTime;
	struct IPNode* next;
};

IPLink newIPlink();
void  addIPNode(IPLink* link, unsigned int ipv4, unsigned int ttl);
void refresh(IPLink* link, time_t lowestLeft);
int isLocal(IPLink link);

#endif // !IPH
