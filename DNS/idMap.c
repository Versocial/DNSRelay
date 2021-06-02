#include "idMap.h"

idTable* createIdTable(uint16_t minId) {
	idTable* table = (idTable*)calloc(1, sizeof(idTable));
	if (table == NULL) { log("createIdTable calloc error."); return NULL; };
	table->minID = minId;
	return table;
}

void disposeIdTable(idTable* table) {
	idTable* temp;
	while (table->next != NULL) {
		temp = table;
		table = table->next;
		free(temp);
	}free(table);
}

uint16_t insertIdTable(idTable* table, uint16_t id, const SOCKADDR* addr, time_t lifeTime) {
	if (table == NULL) { log("insert table null."); return 65535; }
	int num = TableSize;
	time_t t = time(NULL);
	while (table->used == TableSize) {
		time_t minDeadT = 0;//*
		if (t > table->minDeadTime) {
			for (int i = 0; i < TableSize; i++) {
				if (table->info[i].deadTime == 0)continue;
				if (minDeadT == 0)minDeadT = table->info[i].deadTime;//*
				else if (minDeadT > table->info[i].deadTime)minDeadT = table->info[i].deadTime;//*
				if (t > table->info[i].deadTime) {
					if (num == TableSize)num = i;
					table->info[i].deadTime = 0;
					table->used--;
				}
			}
		}
		table->minDeadTime = minDeadT;//*
		if (table->used == TableSize) {
			if (table->next != NULL)table = table->next;
			else break;
		}
	}
	if (table->used == TableSize) {
		table->next = createIdTable(table->minDeadTime + TableSize);
		table = table->next;
		num = 0;
	}
	else if (num == TableSize) {
		num--;
		while (num >= 0) {
			if (table->info[num].deadTime == 0)break;
			num--;
		}
	}

	table->info[num].id = id;
	table->info[num].deadTime = t + lifeTime;
	table->info[num].addr = *addr;
	table->used++;
	if (t + lifeTime < table->minDeadTime || table->used == 1)table->minDeadTime = t + lifeTime;

	return num + (table->minID);
}

idInfo pollOut(idTable* table, uint16_t id) {
	idInfo ans;
	uint16_t minNum = id - TableSize + 1;
	while (table != NULL && (table->minID<minNum || table->minID>id)) table = table->next;
	if (table == NULL) { ans.deadTime = 0; return ans; }
	ans = table->info[id - table->minID];
	if (1) {
		table->info[id - table->minID].deadTime = 0;
		table->used--;
	}
	return ans;
}

void fixSpaceWaste(idTable*table) {

}
