#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>


int hutsik();
void sartu_Pcb_eta_processqueuean(long x);
void sartu_Pcb_eta_processqueuean_roundrobin(long x, int quantu);
void kendu_Processqueuetik();
void kendu_Processqueuetik_roundrobin(long ttid);
void imprimitudena();
void printqueue();
void *Scheduler(long *freq_sc);
