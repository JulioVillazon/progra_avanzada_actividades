//Julio Villazón | A01370190 -> Trabajo hecho con Jesus Gonzalez
//Jesus Gonzalez | A01370190 -> Trabajo hecho con Julio Villazón

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

typedef struct process
{    
    int p_id; 
} process_t;

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

    char witness = 'T';
    

    while(aux < final)
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
            close(tubo[1]);
            read(tubo[0], &witness, sizeof(char));
            aux->p_id = getpid();
            printf("—->Soy el proceso con PID %d y recibí el testigo %c, el cual tendré por 5 segundos. \n", aux->p_id, witness);
            sleep(5);

            printf("<—- Soy el proceso con PID %d  y acabo de enviar el testigo %c. \n", aux->p_id, witness);
            close(tubo[0]);
            write(tubo[1], &witness, sizeof(char));
                      
            exit(0);
        } 
        else
        {
            close(tubo[1]);
            read(tubo[0], &witness, sizeof(char));  
            wait(&status);               
        }       
        ++aux;
        // if(aux == final){
        //     aux = vector;
        // }
    }          


    free(vector);
    // free(previous);
    return 0;
}