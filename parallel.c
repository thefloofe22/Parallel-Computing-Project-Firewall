#include "serial.h"




void* parallellock(void* input) {
	pstruct_t* pstruct = (pstruct_t*) input;
	lamporthelp_t** lqueues = pstruct->lqueuearray;
	int nindex = pstruct->mynum;
	//int tflag = pstruct->tflag;
	// int npackets = pstruct->npackets;
	// int nsources = pstruct->nsources;
	int locktype = pstruct->locktype;
	TTASlock_t** lockarray = NULL;
	pthread_mutex_t* lockarray1 = NULL;
	int* npackets = pstruct->npackets;
	if (locktype == 1) {
		// TTAS lock
		 lockarray = (TTASlock_t**)pstruct->locks;
	} else if (locktype == 2) {
		//printf("parallel reached\n");
		 lockarray1 = (pthread_mutex_t*)pstruct->locks;
	}
	volatile Packet_t* p = NULL;
	while (true) {
		if (pstruct->tflag) {
			return NULL;
		}
		// aquire lock if necessary
		if (locktype == 1) {
			//aquire TTAS lock
			trylockTTAS(lockarray[nindex]);
		} else if (locktype == 2) {
			//aquire pthread_mutex_lock
			trypthreadmutex(&lockarray1[nindex]);
		}
		//printf("here\n");
		while ((p = dequeue(lqueues[nindex])) == NULL) {
			if (pstruct->tflag) {
			return NULL;
			}
		}
		//printf("got past dequeue\n");
		 // aquire lock if necessary
        if (locktype == 1) {
            //aquire TTAS lock
            TTASunlock(lockarray[nindex]);
        } else if (locktype == 2) {
            //aquire pthread_mutex_lock
            unlockpthreadmutex(&lockarray1[nindex]);
        }
		getFingerprint(p->iterations, p->seed);
		__sync_fetch_and_add(npackets, 1);
		free((Packet_t*) p);
	}
	lockarray = NULL;
	return NULL;
}
	


void parallellockfree(long int time, int nsources, int work, int upacket, int expn, PacketSource_t* source, volatile Packet_t* p, int locktype) {
	pstruct_t holder;
	pthread_t timerthread;
	//volatile int tflag = 0;
	holder.time = time;
	holder.tflag = 0;
	pthread_create(&(timerthread), NULL, timerfunc, (void*)&(holder));
	pstruct_t parallelstruct[nsources];
	pthread_t listofthreads[nsources];
	lamporthelp_t* listofqueues[nsources];
	int npackets = 0;
	for (unsigned int count = 0; count < nsources; count++) {
		listofqueues[count] = initializelamport(32);
	}
	//parallelstruct.lqueuearray = listofqueues;
	// parallelstruct.nsources = nsources;
	//volatile int tflag = 0;
	// choose lock type
	 TTASlock_t** lockarray = NULL;
    pthread_mutex_t* lockarray1 = NULL;
	if (locktype == 0) {
		// no lock lock free
	} else if (locktype == 1) {
		//TTAS Lock
		// TTASlock_t* lockarray[nsources];
		//int *list = malloc(n*sizeof(int))
		lockarray = (TTASlock_t**)malloc(nsources * sizeof(TTASlock_t*));
		for (unsigned int TTAS = 0; TTAS < nsources; TTAS++) {
			lockarray[TTAS] = TTASinit(NULL);
		}
		// lockarray[nsources] = NULL;
		//(void*) lockarray;
		
	} else if (locktype == 2) {
		// pthread_mutex lock
		//pthread_mutex_t lockarray1[nsources];
		lockarray1 = (pthread_mutex_t*)malloc(nsources * sizeof(pthread_mutex_t));
		for (unsigned int pthread_mutex = 0; pthread_mutex < nsources; pthread_mutex++) {
			initpthreadmutex(&lockarray1[pthread_mutex]);
		}
		//(void*) lockarray;
	}
	//printf("reached here?\n");	
	for (unsigned int n = 0; n < nsources; n++) {
		//printf("queue start process\n");
		parallelstruct[n].lqueuearray = listofqueues;
		parallelstruct[n].nsources = nsources;
		parallelstruct[n].tflag = holder.tflag;
		parallelstruct[n].mynum = n;
		parallelstruct[n].npackets = &npackets;
		parallelstruct[n].locktype = 0;
		if (locktype == 1) {
			//TTAS lock
			parallelstruct[n].locks = (void*)lockarray;
			parallelstruct[n].locktype = 1;
		} else if (locktype == 2) {
			// pthread lock
			//printf("pthread lock setup\n");
			parallelstruct[n].locks = (void*)lockarray1;
			parallelstruct[n].locktype = 2;
		}
		//printf("reached prior to creating thread\n");
		pthread_create(&(listofthreads[n]), NULL, parallellock, (void*)&(parallelstruct[n]));
	}
	//printf("thread create\n");
	// volatile Packet_t p;
	// enqueue packets bb
	//printf("reached\n");
	while (true) {
	//printf("nsources %u\n", nsources);
    for (unsigned int s = 0; s < nsources; s++) {
	//printf("holder.tflag%u\n", holder.tflag);
          switch (upacket) {
                case 0:
                    // exp packets
                    p = getExponentialPacket(source, s);
                    break;
                case 1:
                    // uniform packets
                    p = getUniformPacket(source, s);
                        break;
          }
         // printf("before enqueue\n");
		 if (holder.tflag) {
			printf("%u\n", npackets);
			return;
		}
         while (enqueue(listofqueues[s], p) == 0) 
			{
				if (holder.tflag) {
					printf("%u\n", npackets);
            		return;
				}
			}
         // printf("after enqueue\n");
    }
	}
	// free(lockarray);
	// lockarray1 = NULL;	
	printf("%u\n", npackets);
}
			
