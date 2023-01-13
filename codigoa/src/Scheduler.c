#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/aldagai_generalak.h"
struct mm /*memory magement*/
{
    int pgb; /*Orri taularen helbide fisikoa*/ 
    int code; /*Kodearen segmentuaren helbide birtuala*/
    int data; /*Datuen segmentuaren helbide birtula*/
};

struct PCB /*(Proces Control Block) Prozesu bat sortzen den bakoitzean sortzen den datu blokea*/
{
    long tid; /*Prozesuaren identifikadorea*/
    int momentuko_exekuzio_denbora; /*RoundRobin-erako: Momenturarte egindako exekuzio denbora*/
    int quantum; /*RoundRobin-erako: Hurrengo prozesua exekutatzera sartu baino lehen egin beharreko exekuzio denbora*/
    int exekuzioa_bukatzeko_denbora;/*RoundRobin-erako: Prozesua guztiz bukatzeko behar duen exekuzio denbora*/
    struct mm mm;
    struct PCB *hurrengoa; /*PCB lista dinamikoan osagai hau eta gero dagoen osagaia*/
};


struct PCB *rootpcb = NULL; 
struct PCB *azkenapcb = NULL;
int processqueue_tam = 0;
long tam = sizeof(struct PCB);
struct PCB processqueue[1000]; /*Exekutatzeko dauden prozesuen array-a*/

int hutsik()
{
    if (rootpcb == NULL)

        return 1;
    else
        return 0;
}

void sartu_Pcb_eta_processqueuean(long x)
{

    struct PCB berria;

    berria.tid = x;

    berria.hurrengoa = NULL;

    if (hutsik())
    {

        rootpcb = &berria;
        azkenapcb = &berria;
    }
    else if (&rootpcb == &azkenapcb)
    {

        rootpcb->hurrengoa = &berria;
        azkenapcb = &berria;
    }
    else
    {

        azkenapcb->hurrengoa = &berria;
        azkenapcb = &berria;
    }

    if (processqueue_tam == 0)
    {
        processqueue[0] = berria;
        processqueue_tam++;
    }
    else if (processqueue_tam == 1)
    {
        processqueue_tam++;
        processqueue[1] = berria;
    }
    else if (processqueue_tam >= 2 && processqueue_tam < 1000)
    {

        processqueue_tam++;
        processqueue[processqueue_tam - 1] = berria;
    }
    else
    {
        printf("procesqueuea beteta itxaron libratu arte");
    }
}

void sartu_Pcb_eta_processqueuean_roundrobin(long x, int quantu)
{

    struct PCB berria;
    berria.exekuzioa_bukatzeko_denbora = rand() % (50 - 11 + 1) + 11;
    berria.quantum = quantu;
    berria.tid = x;
    berria.momentuko_exekuzio_denbora = 0;
    berria.hurrengoa = NULL;

    if (hutsik())
    {

        rootpcb = &berria;
        azkenapcb = &berria;
    }
    else if (&rootpcb == &azkenapcb)
    {

        rootpcb->hurrengoa = &berria;
        azkenapcb = &berria;
    }
    else
    {

        azkenapcb->hurrengoa = &berria;
        azkenapcb = &berria;
    }

    if (processqueue_tam == 0)
    {
        processqueue[0] = berria;
        processqueue_tam++;
    }
    else if (processqueue_tam == 1)
    {
        processqueue_tam++;
        processqueue[1] = berria;
    }
    else if (processqueue_tam >= 2 && processqueue_tam < 1000)
    {

        processqueue_tam++;
        processqueue[processqueue_tam - 1] = berria;
    }
    else
    {
        printf("procesqueuea beteta itxaron libratu arte \n");
    }
}

void kendu_Processqueuetik()
{

    for (int i = 0; i < processqueue_tam - 2; i++)
    {
        processqueue[i] = processqueue[i + 1];
    }

    processqueue_tam--;
}

void kendu_Processqueuetik_roundrobin(long ttid)
{
    int aurkitua = 0;
    int ez_aurkitua = 0;
    int i = 0;
    int lag = 0;

    while (aurkitua != 1)
    {
        if (i < processqueue_tam)
        {

            if (processqueue[i].tid == ttid)
            {
                aurkitua++;
                lag = i;
            }
            else
            {
                i++;
            }
        }
        else
        {
            printf("ez dago procesqueuean");
            ez_aurkitua++;
            aurkitua++;
        }
    }
    if (ez_aurkitua == 0)
    {
        for (int a = lag; a < processqueue_tam - 2; a++)

        {
            processqueue[a] = processqueue[a + 1];
        }
        processqueue_tam--;
    }
}

void printqueue()
{
    printf("Process Queueko elemtu guztiak:\n");

    if (processqueue_tam > 2)
    {
        for (int i = 0; i <= processqueue_tam - 1; i++)
        {
            printf("%ld  ", processqueue[i].tid);
        }
    }
    else
    {
        printf("PROZESU GUTXIEGI");
    }

    printf("\n");
}

void *Scheduler(long *freq_sc)
{
    int bukatuta = 0;
    int guztiz_bukatua = 0;
    int pasatako_segunduak = 0;
    int i = 0;
    while (1)
    {
        sem_wait(&semaforo_sc);

        if (processqueue_tam > 0)
        {
            if (estrategi_zenb == 1)
            {

                printf("%ld-a duen prozesua sartuko da exekuziora\n", processqueue[processqueue_tam - 1].tid);
                fflush(stdout);
                printf("%ld-a duen prozesua guztiz bukatu da\n", processqueue[processqueue_tam - 1].tid);
                fflush(stdout);
                kendu_Processqueuetik();
            }
            else
            {
                printf("%ld-a duen prozesua sartuko da exekuziora\n", processqueue[i].tid);

                while (bukatuta != 1)
                {
                    processqueue[i].momentuko_exekuzio_denbora++;
                    pasatako_segunduak++;

                    if (processqueue[i].momentuko_exekuzio_denbora >= processqueue[i].exekuzioa_bukatzeko_denbora)
                    {
                        bukatuta++;
                        guztiz_bukatua++;
                    }
                    else if (pasatako_segunduak == processqueue[i].quantum || processqueue[i].quantum == 0)
                    {

                        bukatuta++;
                    }
                }
                if (guztiz_bukatua == 1)
                {
                    printf("%ld-a tid-a duen prozeusa guztiz bukatu da\n",processqueue[i].tid);
                    fflush(stdout);
                    kendu_Processqueuetik_roundrobin(processqueue[i].tid);
                }
                else
                {
                    i++;
                }

                if (i == processqueue_tam)
                {
                    i = 0;
                }

                bukatuta = 0;
                guztiz_bukatua = 0;
                pasatako_segunduak = 0;
            }
        }
    }
}
