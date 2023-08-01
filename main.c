#include "serial.h"




int main(int argc, char **argv) {
	int M = 0;
	int N = 0;
	int W = 0;
	int U = 0;
	int E = 0;
	// int R = 0;
	int L = 0;
	int s = 0;
	for (unsigned int i = 1; i < argc; i++) {
		if (strcmp("-M", argv[i]) == 0) {
			M = atoi(argv[i + 1]);
		} else if (strcmp("-N", argv[i]) == 0) {
			N = atoi(argv[i + 1]);
		 } else if (strcmp("-W", argv[i]) == 0) {
			W = atoi(argv[i + 1]);
		 } else if (strcmp("-U", argv[i]) == 0) {
            U = atoi(argv[i + 1]);
		 } else if (strcmp("-E", argv[i]) == 0) {
            E = atoi(argv[i + 1]);
		 } else if (strcmp("-L", argv[i]) == 0) {
			L = atoi(argv[i + 1]);
		}  else if (strcmp("-S", argv[i]) == 0) {
            s = atoi(argv[i + 1]);
		}
	}
		PacketSource_t* source = createPacketSource(W, N, E);
		volatile Packet_t* p = NULL;
		switch (L) {

				case 0:
					// printf("reached here\n");
					seriali(M, N, W, U, E, source);
					break;
				case 1:
					//TTAS Lock
					if (s == 0) {
						// lock free
						//printf("lock free\n");
						parallellockfree(M, N, W, U, E, source, p, 0);
					} else if (s == 1) {
						// lock queues
						parallellockfree(M, N, W, U, E, source, p, 1);
					} else if (s == 2) {
						//aweomse and TTAS
						aparallellockfree(M, N, W, U, E, source, p, 1);
					}
					break;
				case 2:
					// pthread_mutex Lock
					 if (s == 0) {
                        // lock free
                        parallellockfree(M, N, W, U, E, source, p, 0);
                    } else if (s == 1) {
                        // lock queues
                        parallellockfree(M, N, W, U, E, source, p, 2);
                    } else if (s == 2) {
						//aweomse and pthread_mutex
						aparallellockfree(M, N, W, U, E, source, p, 2);
					}
					
		}
	return 0;
}

// void parallellockfree(long int time, int nsources, int work, int upacket, int expn, PacketSource_t* source, volatile Packet_t* p, int locktype);
//void parallellockfree(long int time, int nsources, int work, int upacket, int expn, PacketSource_t* source, volatile Packet_t* p, int locktype)
