#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/aldagai_generalak.h"
#include "../include/Scheduler.h"

long tok = 0;
long tid;
pthread_t threads[];

int Prozesuak_egin_behar_duena()
{

        return 1;
}

void *Loader(long *lag)
{
        int quantu = 0;
        int i = 0;

        while (1)
        {
                sem_wait(&semaforo_pg);

                if (i < 50)
                {

                        pthread_create(&threads[i], NULL, (void *)Prozesuak_egin_behar_duena, NULL);

                        tid = (long)threads[i];

                        printf("LOADER: %ld \n", tid);
                        fflush(stdout);

                        if (estrategi_zenb == 1)
                        {
                                sartu_Pcb_eta_processqueuean(tid);
                        }
                        else
                        {
                                quantu = rand() % (10 + 1);
                                sartu_Pcb_eta_processqueuean_roundrobin(tid, quantu);
                        }

                        i++;
                }
                else
                {
                        i = 0;
                }
        }
}
