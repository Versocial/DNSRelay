#include "idMap.h"

idTable* createIdTable(uint16_t minId) {
	idTable* table = (idTable*)calloc(1, sizeof(idTable));
	if (table == NULL) { log_1("createIdTable calloc error."); return NULL; };
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
	if (table == NULL) { log_1("insert table null."); return 65535; }
	int num = TableSize;
	time_t t = time(NULL);
	while (table->used == TableSize) {// find a not used out table or the final table
		time_t minDeadT = 0;//*
		if (t > table->minDeadTime) {
			for (int i = 0; i < TableSize; i++) {
				if (table->info[i].deadTime == 0)continue;//***not use
				if (t > table->info[i].deadTime) {//***find dated one so it is  not used out,find the table and num
					log_1("id %d time out", table->info->id);
					if (num == TableSize)num = i;
					table->info[i].deadTime = 0;
					table->used--;
				}
				if (minDeadT == 0)minDeadT = table->info[i].deadTime;//*
				else if (minDeadT > table->info[i].deadTime)minDeadT = table->info[i].deadTime;//*
			}
		}
		table->minDeadTime = minDeadT;//*
		if (table->used == TableSize) {//used out
			if (table->next != NULL)table = table->next;
			else break;
		}
	}
	if (table->used == TableSize&&table->next==NULL) {//final table
		table->next = createIdTable(table->minID + TableSize);
		table = table->next;
		num = 0;
	}
	else if (num == TableSize) {//not  used out table but not a dated one
		num=0;
		while (num < TableSize) {
			if (table->info[num].deadTime == 0)break;
			num++;
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
	long long  minNum = id - TableSize + 1;
	while (table != NULL && (table->minID<minNum || table->minID>id)) table = table->next;
	if (table == NULL) { ans.deadTime = 0; return ans; }
	ans = table->info[id - table->minID];
	if (1) {
		table->info[id - table->minID].deadTime = 0;
		table->used--;
	}
	return ans;
}

void fixSpaceWaste(idTable*now,time_t lowestLeft) {
	if (now == NULL)return;
	time_t t = time(NULL)+lowestLeft;
	idTable* prev = now;
	while (now != NULL) {
		if (now->used == 0) { now = now->next; continue; }
		for (int k=0, left=now->used;left>0&&k<TableSize; k++) {
			if (now->info[k].deadTime != 0) {
				left--; 
				if (now->info[k].deadTime < t) { log_1("idMap : delete dated id %d",now->info[k].id); now->info[k].deadTime = 0; now->used--; }
			}
		}
		now = now->next;
	}
	now = prev;
	prev = NULL; 
	while (now->next != NULL) {
		if (now->next->used == 0) { if (prev == NULL)prev = now; }
		else prev = NULL;
		now = now->next;
	}

	if(prev!=NULL){
		now = prev->next;
		while (now != NULL) {
			prev->next = now->next;
			free(now);
			now = prev->next;
		}
	}
	return;
}
