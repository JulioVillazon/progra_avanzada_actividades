#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define THREADS 4
#define MEN 4
#define WOMEN 4

pthread_mutex_t mutex_toilet = PTHREAD_MUTEX_INITIALIZER;

#ifdef __APPLE__
/* Para MacOS */
sem_t *men, *women;
#else
/* Para Linux */
sem_t men, women;
#endif

int toilet_state = 0;
int arrivedwomen = 0;
int arrivedmen = 0;
int exitedmen = 0;
int exitedwoman = 0;

void *mujer_quiere_entrar(void *);
void *hombre_quiere_entrar(void *);
void *mujer_sale(void *);
void *hombre_sale(void *);

int main(int argc, const char *argv[])
{

    srand((unsigned int)time(NULL));

    #ifdef __APPLE__
        /* Para MacOS */
        men = sem_open("men", O_CREAT, 777, MEN/2);
        women = sem_open("women", O_CREAT, 777, WOMEN/2);
    #else
        /* Inicializar semáforos en Linux */
        sem_init(&men, 0, 0);
        sem_init(&women, 0, 0);
    #endif

    /* Crear a los babuinos */
    pthread_t hilos[THREADS];

    pthread_create(&hilos[0], NULL, hombre_quiere_entrar, NULL);
    pthread_create(&hilos[1], NULL, hombre_sale, NULL);
    pthread_create(&hilos[2], NULL, mujer_quiere_entrar, NULL);
    pthread_create(&hilos[3], NULL, mujer_sale, NULL);

       /* Adjuntar los hilos */
    for (int i = 0; i < THREADS; ++i)
    {
        pthread_join(hilos[i], NULL);
    }

    
    #ifdef __APPLE__
        /* En MacOS*/
        sem_close(men);
        sem_close(women);
    #else
        /* Liberar los recursos en Linux */
        sem_destroy(&men);
        sem_destroy(&women);
    #endif

    pthread_exit(NULL);
}

void * mujer_quiere_entrar(void *arg){
    int waiting_women = 0;

    while (arrivedwomen < WOMEN)
    {
        sleep(rand()%3);
        
        ++waiting_women;
        printf("(M) Llega una mujer (%d en espera).\n", waiting_women);

        pthread_mutex_lock(&mutex_toilet);

        if (toilet_state == 0)
        {
            toilet_state = 1;
            printf("(M) Sanitario ocupado por mujeres.\n");
        }
        if (toilet_state == 1)
        {
            --waiting_women;
            printf("(M) Entra una mujer (%d en espera).\n", waiting_women);
            #ifdef __APPLE__
                sem_post(women);
            #else
                sem_post(&women);
            #endif
        }
        pthread_mutex_unlock(&mutex_toilet);

        ++arrivedwomen;
    }

    pthread_exit(NULL);

}

void *mujer_sale(void *arg)
{
    int women_inside = 0;

    while (exitedwoman < WOMEN)
    {
        sleep(rand() % 3);
        #ifdef __APPLE__
            sem_wait(women);
            sem_getvalue(women, &women_inside);
        #else
            sem_wait(&women);
            sem_getvalue(&women, &women_inside);
        #endif
        //Parece que el sem_getvalue al igual que el sem_init y el sem_destroy no funcionan en MACOS
        //Y es por eso que nos regresa el valor de 0 con el el que wome_inside fue declarado siempre
        //Es por eso que solo sale una mujer y se vacia el baño. No encontramos un sustituto para sem_getvalue

        printf("(M) Sale una mujer.\n");

        if(women_inside == 0){
            pthread_mutex_lock(&mutex_toilet);
            toilet_state = 0;
            printf("El baño esta vacío.\n");
            pthread_mutex_unlock(&mutex_toilet);
        }
        ++exitedwoman;
        
    }

 pthread_exit(NULL);

}

void * hombre_quiere_entrar(void *arg){
    int waiting_men = 0;

    while (arrivedmen < MEN)
    {
        sleep(rand() % 3);
        
        ++waiting_men;
        printf("(H) Llega un hombre (%d en espera).\n", waiting_men);

        pthread_mutex_lock(&mutex_toilet);

        if (toilet_state == 0)
        {
            toilet_state = 2;
            printf("(H) Sanitario ocupado por hombres.\n");
        }
        if (toilet_state == 2)
        {
            --waiting_men;
            printf("(H) Entra un hombre (%d en espera).\n", waiting_men);
            #ifdef __APPLE__
                sem_post(men);
            #else
                sem_post(&men);
            #endif
            
        }
        pthread_mutex_unlock(&mutex_toilet);
        ++arrivedmen;
    }

    pthread_exit(NULL);
}


void * hombre_sale(void *arg)
{
    int men_inside = 0;
    while (exitedmen < MEN)
    {
        sleep(rand() % 3);

        #ifdef __APPLE__
            sem_wait(men);
            sem_getvalue(men, &men_inside);
        #else
            sem_wait(&men);
            sem_getvalue(&men, &men_inside);
        #endif

            //Parece que el sem_getvalue al igual que el sem_init y el sem_destroy no funcionan en MACOS
            //Y es por eso que nos regresa el valor de 0 con el el que men_inside fue declarado siempre
            //Es por eso que solo sale un hombre y se vacia el baño. No encontramos un sustituto para sem_getvalue

            //printf("Hay %d hombres adentro\n", men_inside);

            printf("(H) Sale un hombre.\n");

            if (men_inside == 0)
            {
                pthread_mutex_lock(&mutex_toilet);
                toilet_state = 0;
                printf("El baño esta vacío.\n");
                pthread_mutex_unlock(&mutex_toilet);
        }
        ++exitedwoman;
    }

    pthread_exit(NULL);
}