#include "IP.h"


IPLink newIPlink() { IPLink link; link.node = NULL; link.size = 0; return link; }

void  addIPNode(IPLink* link, unsigned int ipv4, unsigned int ttl) {
	struct IPNode* now = calloc(1, sizeof(struct IPNode));
	now->ipv4 = ipv4;
	now->killTime = ttl;
	now->next = link->node;
	link->node = now;
	link->size++;
}

void refresh(IPLink* link, time_t lowestLeft) {
	time_t t = time(NULL);
	struct IPNode* now = link->node;
	struct IPNode* prev = NULL;
	while (now != NULL) {
		if (now->killTime - lowestLeft < t) {
			if (prev == NULL) { link->node = now->next; free(now); }
			else { prev->next = now->next; free(now); }
			link->size--;
		}
	}
}

int isLocal(IPLink link) {
	return link.node && link.node->killTime == 0;
}
