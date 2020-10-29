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
sem_t *chair, *food;
#else
/* Para Linux */
sem_t chair, food;
#endif

int dwarfs_waiting = 0;
//Blancanieves tiene 2 estados 0 = dando una vuelta, 1 = sirviendo
int snowwhite_state = 0;

void *snowhite(void *);
void *dwarf(void *);

int main(int argc, const char *argv[])
{

    srand((unsigned int)time(NULL));

#ifdef __APPLE__
    /* Para MacOS */
    chair = sem_open("chair", O_CREAT, 777, CHAIRS);
    food = sem_open("food", O_CREAT, 777, 0);
#else
    /* Inicializar semáforos en Linux */
    sem_init(&chair, 0, CHAIRS);
    sem_init(&food, 0, 0);
#endif

    /* Crear a los babuinos */
    pthread_t threads[THREADS];

    //Creando hilo de blancanieves
    pthread_create(&threads[0], NULL, snowhite, NULL);

    //Creando hilo de enanitos
    for (int i = 1; i <= DWARFS; ++i)
    {
        pthread_create(&threads[i], NULL, dwarf, NULL);
    }

    /* Adjuntar los hilos */
    for (int i = 0; i < THREADS; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    #ifdef __APPLE__
        /* En MacOS*/
        sem_close(chair);
        sem_close(food);
    #else
        /* Liberar los recursos en Linux */
        sem_destroy(&chair);
        sem_destroy(&food);
    #endif

    pthread_exit(NULL);
}

void *dwarf(void *arg)
{
    int available_chairs;
    int total_dwarfs = 0;
    int eating;

    #ifdef __APPLE__
        /* Para MacOS */
        sem_getvalue(chair, &available_chairs);
        sem_getvalue(food, &eating);
    #else
        /* Inicializar semáforos en Linux */
        sem_getvalue(&chair, &available_chairs);
        sem_getvalue(&food, &eating);
    #endif

    while (total_dwarfs < DWARFS)
    {
        sleep(rand() % 5);

        printf("(E) Regreso un enanito.\n");
        ++dwarfs_waiting;
        ++total_dwarfs;


        if(available_chairs > 0){
            #ifdef __APPLE__
                /* Para MacOS */
                sem_wait(chair);
                sem_getvalue(chair, &available_chairs);
            #else
                /* Inicializar semáforos en Linux */
                sem_wait(&chair);
                sem_getvalue(&chair, &available_chairs);
            #endif

            printf("(E) Se sento un enanito (%d sillas disponibles) \n", available_chairs);
            if (dwarfs_waiting > 0)
            {
                --dwarfs_waiting;
            }
            snowwhite_state = 1;
 
        }
        else{
            printf("(E) Las sillas estan ocupadas. (%d enanitos esperando) \n", dwarfs_waiting);
        }

        // pthread_mutex_unlock (&mutex);

        if (eating > 0)
        {
            #ifdef __APPLE__
                /* Para MacOS */
                sem_wait(food);
                sem_getvalue(food, &eating);
            #else
                /* Inicializar semáforos en Linux */
                sem_wait(&food);
                sem_getvalue(&food, &eating);
            #endif
        }
    }

    pthread_exit(NULL);
}

void *snowhite(void *arg)
{
    
}

