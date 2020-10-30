//Julio Villazón | A01370190 -> Trabajo hecho con Jesus Gonzalez
//Jesus Gonzalez | A01422050 -> Trabajo hecho con Julio Villazón

#include <stdio.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define N_SECTIONS 3
#define N_ROBOTS 3
#define R_WEIGHT 1
#define MAX_S_WEIGHT 2

pthread_mutex_t in = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t out = PTHREAD_MUTEX_INITIALIZER;
int sections[N_SECTIONS];
int count = 1;

void *move(void *);

int main(int argc, const char *argv[]) {
    
    pthread_t *robots = (pthread_t *) malloc(sizeof(pthread_t) * N_ROBOTS);
    pthread_t *aux = robots;
    pthread_t *final = robots + N_ROBOTS;

    for(; aux < final; ++aux)
    {
        pthread_create(aux, NULL, move, (void *)&count);
        ++count;
    }
    aux = robots;    
    for(; aux < final; ++aux)
    {
        pthread_join(*aux, NULL); 
    }
    free(robots);
    pthread_exit(NULL);
}

void *move(void *count){
    sleep(rand()%3);
    int robot = (intptr_t) count;
    int currentSec = 0;

    while(currentSec < N_SECTIONS)
    {
        pthread_mutex_lock(&in); //lock
        if(sections[currentSec] + R_WEIGHT < MAX_S_WEIGHT) //weight check
        {
            sections[currentSec] += R_WEIGHT; //new robot in section
            printf("New robot in section %d. New weight: %d\n", currentSec, sections[currentSec]);
            pthread_mutex_unlock(&in); //unlock
            sleep(rand()%3);
            pthread_mutex_lock(&out); //lock
            sections[currentSec] -= R_WEIGHT; //remove robot from previous section
            printf("Robot has left section %d. New weight: %d\n", currentSec, sections[currentSec]);
            pthread_mutex_unlock(&out); //unlock
            currentSec++;
        }
        pthread_mutex_unlock(&in); //unlock
    }
    pthread_exit(NULL);
}
