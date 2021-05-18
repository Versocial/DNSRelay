#include "dnsRelay.h"
#include "log.h"

#define LOGPATH "../res/log.txt"
int main() {
	initLog(LOGPATH);
	log("傻逼-----%d----stupid", 100);
}