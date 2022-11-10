#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

struct PCB
{
    int pid;
    struct PCB *hurrengoa;
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
struct PROCESINFO{
   long freq ;
   long procmax;

};




struct PCB *root = NULL;
struct PCB *azkena = NULL;

long done,tik ,tok, p,proces_num, tid= 0;
pthread_mutex_t mutex_cl /*Erlojuaren eta timerraren mutexa*/, 
                mutex_pg/*Timerraren eta Prozesu sortzailearen mutexa*/, 
                mutex_sc/*Timerra eta Planifikatzaileraren mutexa*/; 
pthread_cond_t cond, 
               cond2,
               cond3,
               cond4;
 pthread_t threads[20];


int hutsik()
{
    if (root == NULL)
        return 1;
    else
        return 0;
}

void sartu(int x)
{
    struct PCB *berria;
    berria = malloc(sizeof(struct PCB));
    berria->pid = x;
    berria->hurrengoa = NULL;
    if (hutsik())
    {
        root = berria;
        azkena = berria;
    }
    else
    {
        azkena->hurrengoa = berria;
        azkena = berria;
    }
}

int kendu()
{
    if (!hutsik())
    {
        int informazioa;
        int proces_id = root->pid;
        struct PCB *bor = root;
        if (root == azkena)
        {
            informazioa = root->pid;
            root = NULL;
            azkena = NULL;
        }
        else
        {
            informazioa = root->pid;
            root = root->hurrengoa;
        }
        free(bor);
        return informazioa;
    }
    else
        return -1;
}

void imprimitudena()
{
    struct PCB *reco = root;
    printf("Listako PCB elemtu guztiak:\n");
    while (reco != NULL)
    {
        printf("%i - ", reco->pid);
        reco = reco->hurrengoa;
    }
    printf("\n");
}

void dena_borratu()
{
    struct PCB *reco = root;
    struct PCB *bor;
    while (reco != NULL)
    {
        bor = reco;
        reco = reco->hurrengoa;
        free(bor);
    }
}
pthread_t Prozesuak_egin_behar_duena(){
    //printf("prozesu bat naiz \n");
    return pthread_self();
}
void *Erlojua()
{
    done = 0;
    while (1)
    {
        pthread_mutex_lock(&mutex_cl);
        while (done < 2)
        {
            pthread_cond_wait(&cond, &mutex_cl);
          //printf("Tenporizadoreari seinalea bidalita \n");
        }
        done = 0;
        pthread_cond_broadcast(&cond2);
        //printf("timer guztiak bukatuta");
        pthread_mutex_unlock(&mutex_cl);
    }
}

void *Tenporizadorea_pg(long *frekuentzia) //100000
{

    pthread_mutex_lock(&mutex_cl);
    while (1)
    {
        done++;
       // printf("Seinalea sortuta Prozesu sortzaileari %ld \n",p);
        if (tik == *frekuentzia)
        {
           // printf("Seinalea sortuta Prozesu sortzaileari %ld \n",p);
            pthread_mutex_lock(&mutex_pg);
            pthread_cond_wait(&cond3, &mutex_pg);

          // printf("Seinalea sortuta Prozesu sortzaileari %ld \n",p);
            tik=0;
            p++;
        }else{tik++;}
        
        pthread_cond_signal(&cond);
        pthread_cond_wait(&cond2, &mutex_cl);
    }
    
        
    
    
}

void *Tenporizadorea_sch(long *frekuentzia) //100000
{

    pthread_mutex_lock(&mutex_cl);
    while (1)
    {
        done++;
       // printf("Seinalea sortuta Prozesu sortzaileari %ld \n",p);
        if (tik == *frekuentzia)
        {
           // printf("Seinalea sortuta Prozesu sortzaileari %ld \n",p);
           // pthread_mutex_lock(&mutex_sc);
           // pthread_cond_wait(&cond4, &mutex_sc);

          // printf("Seinalea sortuta Prozesu sortzaileari %ld \n",p);
            tik=0;
            p++;
        }else{tik++;}
        
        pthread_cond_signal(&cond);
        pthread_cond_wait(&cond2, &mutex_cl);
    }
    
        
    
    
}
void *Prozesuak_Sortu(long *lag)
{  
  dena_borratu();
   //pthread_t h;
   
    pthread_mutex_lock(&mutex_pg);
  printf("zenbatero %ld \n",*lag);
  
   while (1)
   {
    for (int i = 0; i < 20; i++)
    {
    //printf("Hemen nago");
   if (tok == *lag)
   {
   // printf("Hemen nago");
    
    pthread_create(&threads[i],NULL,(void *)Prozesuak_egin_behar_duena,NULL);
    tid = (long )&threads[i];
   // printf("%d",tid);
    sartu(tid);
    imprimitudena();
    
    tok=0;
   }else{tok++;}
   pthread_cond_signal(&cond3);
   pthread_mutex_unlock(&mutex_pg);
    }
   }
}
void *Scheduler(){
pthread_mutex_lock(&mutex_pg);
pthread_cond_signal(&cond3);
pthread_mutex_unlock(&mutex_pg);
}

int main(int argc, char *argv[])
{
   // struct  PROCESINFO a;
    
    /*Erlojuaren zenbat seinalero egin +1 Timerran*/
    long freq = strtol(argv[1], NULL, 10);
    /*Timerraren zenbat seinalero egin +1 Prozesu sortzailean*/
    long freq_pg = strtol(argv[2], NULL, 10);
    //a.freq = freq_pg;
    /*Prozesu sortzaileak sortu ditzakeen prozesu kopuru maximoa*/
    //long procesmax = strtol(argv[3], NULL, 10);
   // a.procmax = procesmax;
    /*Hariak erazagutu*/
    pthread_t h1, h2, h3, h4;
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

    /*Hariek leno zuten harira itzuli*/
    pthread_join(h1,NULL);
    pthread_join(h2,NULL);
    pthread_join(h3,NULL);
    pthread_join(h4,NULL);
    /*Mutex-ak bukarazi*/
    pthread_mutex_destroy(&mutex_cl);
    pthread_mutex_destroy(&mutex_pg);
    pthread_mutex_destroy(&mutex_sc);
}