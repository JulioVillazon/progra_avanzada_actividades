#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define CHAIRS 4
#define DWARFS 7
#define SNOWWHITE 1
#define THREADS DWARFS + SNOWWHITE

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

#ifdef __APPLE__
/* Para MacOS */
sem_t *chair, turn;
#else
/* Para Linux */
sem_t chair, turn;
#endif

int dwarfs_waiting = 0;

void *snowhite(void *);
void *dwarf(void *);

int main(int argc, const char *argv[])
{

    srand((unsigned int)time(NULL));

    #ifdef __APPLE__
        /* Para MacOS */
        chair = sem_open("chair", O_CREAT, 777, CHAIRS);
        turn = sem_open("turn", O_CREAT, 777, 0);
    #else
        /* Inicializar semáforos en Linux */
        sem_init(&chair, 0, CHAIRS);
        sem_init(&turn, 0, 0);
    #endif

    /* Crear a los babuinos */
    pthread_t threads[THREADS];

    //Creando hilo de enanitos
    for (int i = 0; i < DWARFS; ++i)
    {
        pthread_create(&threads[i], NULL, dwarf, (void *)&i);
    }

     //Creando hilo de blancanieves
    pthread_create(&threads[DWARFS+1], NULL, snowhite, NULL);

    /* Adjuntar los hilos */
    for (int i = 0; i < THREADS; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    #ifdef __APPLE__
        /* En MacOS*/
        sem_close(chair);
        sem_close(turn);
    #else
        /* Liberar los recursos en Linux */
        sem_destroy(&chair);
        sem_destroy(&turn);
    #endif

    pthread_exit(NULL);
}

void *dwarf(void *arg)
{
    int ndwarf = *(int *)arg;

    while (1)
    {     
        sleep(rand()%3);

        printf("-->(Enanito %d) Regreso el enanito y este esperando una silla. \n", ndwarf);

        #ifdef __APPLE__
            sem__wait(chair);
        #else
            sem_wait(&chair);
        #endif

        printf("(Enanito %d) Ya esta sentado. \n", ndwarf);

        pthread_mutex_lock(&mutex);
        ++dwarfs_waiting;
        pthread_mutex_unlock(&mutex);

        
        #ifdef __APPLE__
            sem__wait(turn);
        #else
            sem_wait(&turn);
        #endif

        pthread_mutex_lock(&mutex);
        --dwarfs_waiting;
        pthread_mutex_unlock(&mutex);

        printf("(Enanito %d) Ya esta comiendo. \n", ndwarf);
        printf("(Enanito %d) Ya acabo de comer y va a regresar a la mina --> \n", ndwarf);

        
        #ifdef __APPLE__
            sem__post(chair);
        #else
            sem_post(&chair);
        #endif
    }

    pthread_exit(NULL);
}

void *snowhite(void *arg)
{
    while(1){

        sleep(rand()%3);

        pthread_mutex_lock(&mutex);

        if(dwarfs_waiting == 0){
            pthread_mutex_unlock(&mutex);
            printf("Blancanieves se va dar una vuelta \n");
        }else{
            pthread_mutex_unlock(&mutex);
            printf("Blancanieves le sirve la comida a un enanito\n");

            #ifdef __APPLE__
                sem__post(turn);
            #else
                sem_post(&turn);
            #endif
        }
    }

    pthread_exit(NULL);

}

