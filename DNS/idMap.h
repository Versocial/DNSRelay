#ifndef IDMAPH
#define IDMAPH
#include "dns.h"
#include "log.h"
#define TableSize 32

typedef struct {
	time_t deadTime;
	unsigned id : 16;
	 SOCKADDR addr;
}idInfo;

typedef struct IdTable {
	time_t minDeadTime;
	unsigned used:8;
	unsigned minID:16;
	idInfo info[TableSize];
	struct IdTable* next;
}idTable;
idTable* createIdTable(uint16_t minId);

void disposeIdTable(idTable* table);

uint16_t insertIdTable(idTable* table, uint16_t id, const SOCKADDR* addr, time_t lifeTime);

idInfo pollOut(idTable* table, uint16_t id);

void fixSpaceWaste(idTable* table);

#endif // !IDMAPH
