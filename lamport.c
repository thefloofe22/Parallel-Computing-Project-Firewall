#include "lamport.h"


lamporthelp_t* initializelamport(int depth) {
	lamporthelp_t* lamp = calloc(1, sizeof(lamporthelp_t));
	lamp->depth = depth;
	volatile int head = 0;
	volatile int tail = 0;
	lamp->head = head;
	lamp->tail = tail;
	lamp->packet = (volatile Packet_t**) calloc(depth, sizeof(volatile Packet_t*));
	return lamp; 
}

int enqueue(lamporthelp_t* lamp, volatile Packet_t* pack) {
	if (lamp->tail - lamp->head == lamp->depth) {
		return 0;
	}
	lamp->packet[lamp->tail % lamp->depth] = pack;
	// memory barrier
	__sync_synchronize();
	lamp->tail++; 
	return 1;
}

volatile Packet_t* dequeue(lamporthelp_t* lamp) {
	if ((lamp->tail - lamp->head) == 0) {
		return NULL;
	} else {
		volatile Packet_t* deqpack = lamp->packet[lamp->head % lamp->depth];
     	__sync_synchronize();
     	lamp->head++;
		
    	return deqpack;
	}
}
			

