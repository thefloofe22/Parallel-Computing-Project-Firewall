#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include "lamport.h"
#include "fingerprint.h"
#include "packetsource.h"
#include "TTAS.h"
#include "pthreadmutex.h"
#include "generators.h"


typedef struct pstruct {
	long int time;
	int nsources;
	int work;
	int upacket;
	int expnum;
	int locktype;
	int strategy;
	int dindex;
	lamporthelp_t** lqueuearray;
	volatile int tflag;
	int mynum;
	void* locks;
	int* npackets;
} pstruct_t;


void* timerfunc(void* input);
void seriali(long int time, int nsources, int work, int upacket, int expn, PacketSource_t* source);


void parallellockfree(long int time, int nsources, int work, int upacket, int expn, PacketSource_t* source, volatile Packet_t* p, int locktype);	


void aparallellockfree(long int time, int nsources, int work, int upacket, int expn, PacketSource_t* source, volatile Packet_t* p, int locktype);
