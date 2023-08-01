#include "serial.h"




void* timerfunc(void* input) {
	//printf("reached\n");
	pstruct_t* input1 = (pstruct_t*) input;
	int time = input1->time;
	//printf("%u\n", time);
	// int timerflag = input1->tflag;
	usleep((useconds_t) time);
	//printf("woken up\n");
	input1->tflag = 1;
	return NULL;
}

void seriali(long int time, int nsources, int work, int upacket, int expn, PacketSource_t* source) {
	pstruct_t holder;
	int npackets = 0;
	holder.time = time;
	holder.tflag = 0;
	pthread_t timerthread;
	pthread_create(&(timerthread), NULL, timerfunc, (void*)&(holder));
	volatile Packet_t* p = NULL;
	while (true) {
	for (unsigned int s = 0; s < nsources; s++) {
		switch (upacket) {
			case 0:
				// exponential packet
				p = getExponentialPacket(source, s);
				break;
			case 1:
				// uniform packet
				p = getUniformPacket(source, s);
				break;
		}
		if (holder.tflag) {
			// time to return
			//free((Packet_t*) p);
			printf("%u\n", npackets);
			return;
		} else {
			getFingerprint(p->iterations, p->seed);
			__sync_fetch_and_add(&npackets, 1);
			free((Packet_t*) p);
		}
	}
	}
	return;
}
	
	 
