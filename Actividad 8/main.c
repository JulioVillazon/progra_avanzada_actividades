#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define BSIZE 10
#define GCLIENT 100
#define BCLIENT 50

#define AGENT 1
#define ATMG 5
#define ATMB 3

int gqueue[BSIZE];
int bqueue[BSIZE];

int in = 0;
int out = 0;

int total = 0;
int bclients = 0, gclients = 0;

int clientRow = 0, clientServe = 0;
int arriveTimeG = 0, arriveTimeB = 0;

int lowtg = 5, hightg = 22;
int lowtb = 9, hightb = 34;
int lowtatm = 3, hightatm = 5;
int resttime = 3;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t consume_t = PTHREAD_COND_INITIALIZER;
pthread_cond_t produce_t = PTHREAD_COND_INITIALIZER;

void *agent(void *);
void *atmg(void *);
void *atmb(void *);

int main(int argc, const char *argv[])
{

    srand((int)time(NULL));

    int nThreads = ATMG + ATMB + AGENT + 1;

    pthread_t *bank = (pthread_t *)malloc(sizeof(pthread_t) * nThreads);

    int i;

    pthread_t *aux = bank;

    /* Crear los productores */
    printf("(A) THE AGENT IS ARRIVING.\n");
    pthread_create(aux, NULL, agent, NULL);

    /* Crear los consumidores */
    for (i = 0; aux < (bank + ATMG); ++aux)
    {
        printf("(G) OPENING G-ATM %d. \n", ++i);
        pthread_create(aux, NULL, atmg, (void *)i);
    }

    for (aux = bank, i = 0; aux < (bank + ATMB); ++aux)
    {
        printf("(B) OPENING B-ATM %d. \n", ++i);
        pthread_create(aux, NULL, atmb, (void *)i);
    }

    /* Adjuntar los hilos */
    for (aux = bank; aux < (bank + nThreads); ++aux)
    {
        pthread_join(*aux, NULL);
    }

    free(bank);

    return 0;
}

void *agent(void *arg)
{    
    printf("(A) THE AGENT STARTED\n");

    while (clientRow < GCLIENT+BCLIENT)
    {

        /* Asumir que trabajan a diferentes velocidades */

        usleep(rand() % 500);

        pthread_mutex_lock(&mutex);

        if (total < BSIZE && clientRow < GCLIENT + BCLIENT)
        {

            /* Produce un elemento */
            arriveTimeB = (rand() % (hightb - lowtb + 1)) + lowtb;
            arriveTimeG = (rand() % (hightg - lowtg + 1)) + lowtg;
            if((rand() % 3) % 2 == 0){
                if(gclients < GCLIENT){
                    gqueue[in] = clientRow;
                    printf("+++ (Agent) The GENERAL Client %d entered the queue after %ds.\n", gqueue[in], arriveTimeG);
                    ++gclients;
                }else
                {
                    bqueue[in] = clientRow;
                    printf("+++ (Agent) The BUSSINES Client %d entered the queue after %ds.\n", bqueue[in], arriveTimeB);
                    ++bclients;
                }         
            }else
            {
                if(bclients < BCLIENT){
                    bqueue[in] = clientRow;
                    printf("+++ (Agent) The BUSSINES Client %d entered the queue after %ds.\n", bqueue[in], arriveTimeB);
                    ++bclients;
                }else{
                    gqueue[in] = clientRow;
                    printf("+++ (Agent) The GENERAL Client %d entered the queue after %ds.\n", gqueue[in], arriveTimeG);
                    ++gclients;
                }
            } 

            ++clientRow;

            ++in;
            in %= BSIZE;
            total = gclients + bclients;

            if (gclients == 1 || bclients == 1)
            {
                pthread_cond_broadcast(&consume_t);
            }
        }
        else if(clientRow < GCLIENT + BCLIENT)
        {
            /* El buffer está lleno, se va a dormir */

            printf("-------------- AGENT SLEEPING ------------\n");
            pthread_cond_wait(&produce_t, &mutex);
            printf("-------------- AGENT AWAKE ------------\n");
        }

        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

void *atmg(void *arg)
{
    int id = (int)arg;
    int attendedClients = 0;
    printf("(G) G-ATM %d STARTED.\n", id);

    while (clientServe < GCLIENT+BCLIENT)
    {

        /* Asumir que trabajan a diferentes velocidades */

        usleep(rand() % 500);

        pthread_mutex_lock(&mutex);

        if (gclients > 0 && clientServe < GCLIENT+BCLIENT)
        {
            /* Consume un elemento */
            int atentiont = (rand() % (hightatm - lowtatm + 1)) + lowtatm;
            printf("--- (G-ATM %d) Atended a GENERAL Client %d with a time of %ds\n", id, gqueue[out], atentiont);


            ++clientServe;
            ++attendedClients;

            ++out;
            out %= BSIZE;
            --gclients;

            if (gclients == (BSIZE - 1))
            {
                pthread_cond_broadcast(&produce_t);
            }
            if(attendedClients == 5){
                printf("--- (G-ATM %d) RESTING (%ds) AFTER ATTENDING %d CLIENTS...\n", id, resttime, attendedClients);
                sleep(resttime);
                attendedClients = 0;
            }
        }
        else if(clientServe < GCLIENT+BCLIENT)
        {
            /* El buffer está vacío, se va a dormir */
            printf("-------------- G-ATM %d CLOSED ------------\n", id);
            pthread_cond_wait(&consume_t, &mutex);
            printf("-------------- G-ATM %d RE-OPEN ------------\n", id);
        }

        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

void *atmb(void *arg)
{
    int id = (int)arg;
    int attendedClients = 0;
    printf("(B) B-ATM %d STARTED.\n", id);

    while (clientServe < GCLIENT+BCLIENT)
    {

        /* Asumir que trabajan a diferentes velocidades */

        usleep(rand() % 500);

        pthread_mutex_lock(&mutex);

        if (bclients > 0 && clientServe < GCLIENT+BCLIENT)
        {
            /* Consume un elemento */
            int atentiont = (rand() % (hightatm - lowtatm + 1)) + lowtatm;
            printf("--- (B-ATM %d) Atended a BUSSINES Client %d with a time of %ds\n", id, bqueue[out], atentiont);

            ++clientServe;
            ++attendedClients;

            ++out;
            out %= BSIZE;
            --bclients;

            if (bclients == (BSIZE - 1))
            {
                pthread_cond_broadcast(&produce_t);
            }
            if (attendedClients == 5)
            {
                printf("--- (B-ATM %d) RESTING (%ds) AFTER ATTENDING %d CLIENTS...\n", id, resttime, attendedClients);
                sleep(resttime);
                attendedClients = 0;
            }
        }
        else if (bclients == 0 && gclients > 0 && clientServe < GCLIENT + BCLIENT){
            int atentiont = (rand() % (hightatm - lowtatm + 1)) + lowtatm;
            printf("--- (B-ATM %d) Atended a GENERAL Client %d with a time of %ds\n", id, gqueue[out], atentiont);

            ++clientServe;
            ++attendedClients;

            ++out;
            out %= BSIZE;
            --gclients;

            if (gclients == (BSIZE - 1))
            {
                pthread_cond_broadcast(&produce_t);
            }
            if (attendedClients == 5)
            {
                printf("--- (B-ATM %d) RESTING (%ds) AFTER ATTENDING %d CLIENTS...\n", id, resttime, attendedClients);
                sleep(resttime);
                attendedClients = 0;
            }
        }
        else if (clientServe < GCLIENT + BCLIENT)
        {
            /* El buffer está vacío, se va a dormir */
            printf("-------------- B-ATM %d CLOSED ------------\n", id);
            pthread_cond_wait(&consume_t, &mutex);
            printf("-------------- B-ATM %d RE-OPEN ------------\n", id);
        }

        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}
