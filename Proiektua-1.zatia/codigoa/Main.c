#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

struct PCB
{
    int tid;
};
struct PCB_LIST
{
    struct PCB pcb;
    struct PCB_LIST *hurrengoa;
};
struct PROCESSQUEUE
{
    struct PCB_LIST *prozesua;
    struct PROCESSQUEUE *hurrengoa;
};
struct CPU
{
    int CPU_Numb;
    struct CORE *core;
    struct CPU *hurrengoa;
};

struct CORE
{
    int core_Numb;
    struct HARI *hari;
    struct CORE *hurrengoa;
};
struct HARI
{
    int tid;
    struct HARI *hurrengoa;
};

struct PCB_LIST *rootpcb = NULL;
struct PCB_LIST *azkenapcb = NULL;
struct PROCESSQUEUE *root_queue = NULL;
struct PROCESSQUEUE *azkena_queue = NULL;

long done, tik, tok, tak, tuk, proces_num, tid, max, lag = 0;
pthread_mutex_t mutex_cl /*Erlojuaren eta timerraren mutexa*/,
    mutex_pg /*Timerraren eta Prozesu sortzailearen mutexa*/,
    mutex_sc /*Timerra eta Planifikatzaileraren mutexa*/;
pthread_cond_t cond,
    cond2,
    cond3,
    cond4;
pthread_t threads[20];

int hutsik()
{
    if (rootpcb == NULL)
        return 1;
    else
        return 0;
}

void sartu(int x)
{
    struct PCB_LIST *berria;
    struct PROCESSQUEUE *proces;
    berria = malloc(sizeof(struct PCB_LIST));
    berria->pcb.tid = x;
    berria->hurrengoa = NULL;
    proces->prozesua = berria;
    if (hutsik())
    {
        rootpcb = berria;
        root_queue = proces;
        azkenapcb = berria;
        azkena_queue = proces;
    }
    else
    {
        azkenapcb->hurrengoa = berria;
        azkena_queue->hurrengoa = proces;
        azkenapcb = berria;
        azkena_queue = proces;
    }

}

void kendu(long x)
{

    struct PCB_LIST *bor = rootpcb;
    struct PCB_LIST *aurrekoa = NULL;
    if (rootpcb == azkenapcb)
    {

        rootpcb = NULL;
        azkenapcb = NULL;
    }
    else
    {
        while ((bor->pcb.tid != x) || (bor != NULL))
        {
            aurrekoa = bor;
            bor = bor->hurrengoa;
        }
        aurrekoa->hurrengoa = bor->hurrengoa;
    }
    free(bor);
}

void imprimitudena()
{
    struct PCB_LIST *reco = rootpcb;
    printf("Listako PCB_LIST elemtu guztiak:\n");
    while (reco != NULL)
    {
        printf("%i - ", reco->pcb.tid);
        reco = reco->hurrengoa;
    }
    printf("\n");
}

void dena_borratu()
{
    struct PCB_LIST *reco = rootpcb;
    struct PCB_LIST *bor;
    while (reco != NULL)
    {
        bor = reco;
        reco = reco->hurrengoa;
        free(bor);
    }
}
pthread_t Prozesuak_egin_behar_duena()
{
    return pthread_self();
}
void printqueue()
{
    struct PROCESSQUEUE *reco = root_queue;
    printf("Process Queueko elemtu guztiak:\n");
    while (reco != NULL)
    {
        printf("%d - ", reco->prozesua->pcb.tid);
        reco = reco->hurrengoa;
    }
    printf("\n");
}
// void sartu_queuean(struct PCB_LIST *proces)
// {
//     struct PROCESSQUEUE *berria;
//     berria = malloc(sizeof(struct PROCESSQUEUE));
//     berria->prozesua = proces;
//     berria->hurrengoa = NULL;
//     if (root_queue == NULL)
//     {
//         root_queue = berria;
//         azkena_queue = berria;
//     }
//     else
//     {
//         azkena_queue->hurrengoa = berria;
//         azkena_queue = berria;
//     }
// }
// void kenduqueuetik(int x)
// {
//     struct PROCESSQUEUE *bor = root_queue;
//     struct PROCESSQUEUE *aurrekoa = NULL;
//     if ((root_queue->prozesua->pcb.tid == x) && (root_queue == azkena_queue))
//     {

//         root_queue = NULL;
//         azkena_queue = NULL;
//     }
//     else
//     {
//         while ((bor->prozesua->pcb.tid != x) || (bor != NULL))
//         {
//             aurrekoa = bor;
//             bor = bor->hurrengoa;
//         }
//         aurrekoa->hurrengoa = bor->hurrengoa;
//     }
//     free(bor);
// }
void queue_guztia_borratu()
{
    struct PROCESSQUEUE *reco = root_queue;
    struct PROCESSQUEUE *bor;
    while (reco != NULL)
    {
        bor = reco;
        reco = reco->hurrengoa;
        free(bor);
    }
}
// struct PCB_LIST *prozesua_bilatu_pcb_listean(int x)
// {

//     struct PCB_LIST *bor = rootpcb;
//     while ((bor->pcb.tid != x) || (bor->hurrengoa != NULL))
//     {
//         bor = bor->hurrengoa;
//     }
//     return bor;
// }
void *Erlojua()
{
    done = 0;
    while (1)
    {
        pthread_mutex_lock(&mutex_cl);
        while (done < 2)
        {
            pthread_cond_wait(&cond, &mutex_cl);
        }
        done = 0;
        pthread_cond_broadcast(&cond2);
        pthread_mutex_unlock(&mutex_cl);
    }
}

void *Tenporizadorea_pg(long *frekuentzia) // 100000
{

    pthread_mutex_lock(&mutex_cl);
    while (1)
    {
        done++;

        if (tik == *frekuentzia)
        {
            pthread_mutex_lock(&mutex_pg);
            pthread_cond_wait(&cond3, &mutex_pg);
            tik = 0;
        }
        else
        {
            tik++;
        }

        pthread_cond_signal(&cond);
        pthread_cond_wait(&cond2, &mutex_cl);
    }
}

void *Tenporizadorea_sch(long *frekuentzia) // 100000
{

    pthread_mutex_lock(&mutex_cl);
    while (1)
    {

        done++;
    

        if (tuk == *frekuentzia)
        {
            
            pthread_mutex_lock(&mutex_sc);
            pthread_cond_wait(&cond4, &mutex_sc);
            tuk = 0;
        }
        else
        {
            tuk++;
        }

        pthread_cond_signal(&cond);
        pthread_cond_wait(&cond2, &mutex_cl);
    }
}

void *Prozesuak_Sortu(long *lag)
{
    dena_borratu();
    pthread_mutex_lock(&mutex_pg);
    while (1)
    {
        for (int i = 0; i < 100; i++)
        {
            if (tok == *lag)
            {
                pthread_create(&threads[i], NULL, (void *)Prozesuak_egin_behar_duena, NULL);
                tid = (long)threads[i];
                sartu(tid);
                // struct PCB_LIST *a = prozesua_bilatu_pcb_listean(tid);
                //sartu_queuean(a);
                //imprimitudena();

                tok = 0;
            }
            else
            {
                tok++;
            }
            pthread_cond_signal(&cond3);
            pthread_mutex_unlock(&mutex_pg);
        }
    }
}

void *Scheduler(long *freq_sc)
{
    struct PROCESSQUEUE *bor;
    queue_guztia_borratu();
    pthread_mutex_lock(&mutex_sc);
    while (1)
    {
        if (tak == *freq_sc)
        {
            bor = root_queue;
            if (bor != NULL)
            {
                if (bor->hurrengoa == NULL)
                {
                    max = bor->prozesua->pcb.tid;
                }
                else
                {

                    while (bor != NULL)
                    {
                        if (bor->prozesua->pcb.tid > max)
                        {
                            max = bor->prozesua->pcb.tid;
                        }

                        bor = bor->hurrengoa;
                    }
                }
                // kenduqueuetik(max);
                kendu(max);
                printf("%ld-a duen prozesua sartuko da exekuziora\n", max);
                //printqueue();
            }
            tak = 0;
        }
        else
        {
            tak++;
        }

        pthread_cond_signal(&cond4);
        pthread_mutex_unlock(&mutex_sc);
    }
}

int main(int argc, char *argv[])
{
    /*Erlojuaren zenbat seinalero egin +1 Timerran*/
    long freq = strtol(argv[1], NULL, 10);
    /*Timerraren zenbat seinalero egin +1 Prozesu sortzailean*/
    long freq_pg = strtol(argv[2], NULL, 10);
    /*Timerraren zenbat seinalero egin +1 Schedulerran*/
    long freq_sc = strtol(argv[3], NULL, 10);

    /*Hariak erazagutu*/
    pthread_t h1, h2, h3, h4, h5;

    /*Kondizioak eta Mutex inizializatu*/
    pthread_mutex_init(&mutex_cl, NULL);
    pthread_mutex_init(&mutex_pg, NULL);
    pthread_mutex_init(&mutex_sc, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_cond_init(&cond2, NULL);
    pthread_cond_init(&cond3, NULL);
    pthread_cond_init(&cond4, NULL);

    /*Erlojua hasi*/
    pthread_create(&h1, NULL, (void *)Erlojua, NULL);
    /*Prozesu sortzailearen Timerraren haria hasi*/
    pthread_create(&h2, NULL, (void *)Tenporizadorea_pg, &freq);
    /*Schedulerraren Timerraren haria hasi*/
    pthread_create(&h3, NULL, (void *)Tenporizadorea_sch, &freq);
    /*Prozesu sortzailearen haria hasi*/
    pthread_create(&h4, NULL, (void *)Prozesuak_Sortu, &freq_pg);
    /*Schedulerraren haria hasi*/
    pthread_create(&h5, NULL, (void *)Scheduler, &freq_sc);
    /*Hariek leno zuten harira itzuli*/
    pthread_join(h1, NULL);
    pthread_join(h2, NULL);
    pthread_join(h3, NULL);
    pthread_join(h4, NULL);
    pthread_join(h5, NULL);

    /*Mutex-ak bukarazi*/
    pthread_mutex_destroy(&mutex_cl);
    pthread_mutex_destroy(&mutex_pg);
    pthread_mutex_destroy(&mutex_sc);
}