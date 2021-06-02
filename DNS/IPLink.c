#include "IP.h"


IPLink newIPlink() { IPLink link; link.node = NULL; link.size = 0; return link; }

void  addIPNode(IPLink* link, unsigned int ipv4, unsigned int ttl) {
	struct IPNode* now = calloc(1, sizeof(struct IPNode));
	if (now == NULL) {
		log("IPNode calloc error."); return;
	}
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
	log("bad@1");
	while (now != NULL) {
		if (now->killTime  < t+ lowestLeft) {
			if (prev == NULL) { link->node = now->next; free(now); now = link->node; }
			else { prev->next = now->next; free(now); now = prev->next; }
			link->size--;
		}
		else {
			prev = now;
			now = now->next;
		}
	}
	log("bad@2");
}

int isLocal(IPLink link) {
	return link.node && link.node->killTime == 0;
}
