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
sem_t men, women;
#endif

int dwarfs_waiting = 0;
//Blsncanieves tiene 2 estados 0 = dando una vuelta, 1 = sirviendo
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
    sem_init(&men, 0, 0);
    sem_init(&women, 0, 0);
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
        sem_destroy(&men);
        sem_destroy(&women);
    #endif

    pthread_exit(NULL);
}

void *dwarf(void *arg)
{
    int dwarfs_seated = 0;

    while (1)
    {
        sleep(rand() % 3);

        printf("(D) Regreso un enanito.(%d enanitos esperando) \n", ++dwarfs_waiting);

        pthread_mutex_lock(&mutex);

        if(dwarfs_seated < CHAIRS){
            sem_post(chair);
            
            printf("(D) Se sento un enanito (%d enanitos en la mesa) \n", ++dwarfs_seated);
            if(dwarfs_waiting > 0){
                --dwarfs_waiting;
            }
        }else{
            printf("(D) Las sillas estan ocupadas. \n");
        }

        pthread_mutex_unlock(&mutex);

        

        // if (toilet_state == 0)
        // {
        //     toilet_state = 1;
        //     printf("(M) Sanitario ocupado por mujeres.\n");
        // }
        // if (toilet_state == 1)
        // {
        //     --waiting_women;
        //     printf("(M) Entra una mujer (%d en espera).\n", waiting_women);

        //     #ifdef __APPLE__
        //                 sem_post(women);
        //     #else
        //                 sem_post(&women);
        //     #endif
        // }
        // pthread_mutex_unlock(&mutex_toilet);

        // ++arrivedwomen;
    }

    pthread_exit(NULL);
}

void *snowhite(void *arg)
{
    
}

