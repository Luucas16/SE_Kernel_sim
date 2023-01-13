#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/aldagai_generalak.h"
#include "../include/Scheduler.h"

struct CPU
{
    int CPU_Numb; /*CPU-aren indetifikadorea*/
    struct CORE *core; 
    struct CPU *hurrengoa; /*CPU lista dinamikoaren osagai hau eta gero dagoen osagaia*/
};

struct CORE
{
    int core_Numb; /*Core-aren identifikadorea*/
    struct HARI *hari;
    struct CORE *hurrengoa; /*CPU baten CORE lista dinamikoaren osagai hau eta gero dagoen osagaia*/
};

struct MMU
{
    int TLB[100]; /* HARI baten MMU-aren Translation lookaside buffer-a*/
};

struct HARI
{
    int tid;/*Exekutatzen ari den prozeuaren tid-a(thread indetificatord)*/
    int PC; /*Hariaren Program Counter-a*/
    int IR; /*Hariaren Instruction Register-a*/
    struct MMU MMU; 
    struct HARI *hurrengoa; /*CPU baten CORE baten HARI-en lista dinamikoaren osagai hau eta gero dagoen osagaia*/
};
        
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

                if (i < 50) /*threads[] arrayarentzako da, prozesu berriak sortzeko pthread motako aldagaiak behar baitira*/
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
