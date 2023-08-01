#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "packetsource.h"
#include "generators.h"
#include "math.h"
#include "fingerprint.h"
#include "stopwatch.h"


typedef struct lamporthelp {
    volatile int head;
    volatile int tail;
    int depth;
    volatile Packet_t** packet;
} lamporthelp_t;





lamporthelp_t* initializelamport(int depth);


int enqueue(lamporthelp_t* lamp, volatile Packet_t* pack);

volatile Packet_t* dequeue(lamporthelp_t* lamp);
