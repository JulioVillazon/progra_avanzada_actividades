//Julio Villazón | A01370190 -> Trabajo hecho con Jesus Gonzalez
//Jesus Gonzalez | A01370190 -> Trabajo hecho con Julio Villazón

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

typedef struct process
{    
    struct process *prev;
    int p_id; 
} process_t;

process_t *previous;

int nProc;

int main(int argc, char *argv[])
{
    char *cmdopt;
    int cmd;

    while ((cmd = getopt(argc, argv, "n:")) != -1)
        switch (cmd)
        {
        case 'n':
            cmdopt = optarg;
            break;

        case '?':
            if (optopt == 'n')
                fprintf(stderr, "", optopt); //Default error  is enough
            else if (isprint(optopt))
                fprintf(stderr, "", optopt); //Default error description is enough
            else
                fprintf(stderr, "", optopt); //Default error description is enough
            return 1;
        default:
            abort();
        }
        
    nProc = atoi(cmdopt);  

    process_t *vector = (process_t *) malloc(nProc * sizeof(process_t));  
    process_t *aux = vector;
    process_t *final = vector + nProc; 

    char witness;

    previous = (process_t *) malloc(sizeof(process_t));
    previous->prev = NULL;
    previous->p_id = getpid();

    for (; aux < final; ++aux)
    {        
        pid_t pId;                        
        int status;
        pId = fork();

        int tubo[2];
        pipe(tubo);

        if (pId == -1)
        {
            printf("Could not create child process\n");
            return -1;
        }
        else if (pId == 0)
        {              
            aux->p_id = getpid();
            aux->prev = previous;          
            process_t *current = aux;  
            // close(tubo[0]);
            write(tubo[1], &current, sizeof(process_t));
            printf("PREV: %d | PID: %d\n", aux->prev->p_id, aux->p_id);            

            exit(0);
        } 
        else
        {
            if (waitpid(pId, &status, 0) != -1)
            {                
                int a =WEXITSTATUS(status);
                if (WIFEXITED(status))
                {
                    close(tubo[1]);
                    read(tubo[0], &previous, sizeof(process_t));                    
                }
            }
        }       
    }              


    free(vector);
    // free(previous);
    return 0;
}