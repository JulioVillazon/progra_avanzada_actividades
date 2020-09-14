#include <stdio.h>
#include <stdlib.h>

typedef struct{
    char * nombre;
    char * apellido;
    int edad;
} paciente;

typedef struct {
    paciente * pacientes;
    int ** cama;
} hospital;

int main(int argc, const char * argv[])
{

    paciente * pacientes;
    paciente * aux;
    int numpacientes = 0;
    int numcamas = 0;
    int ** camas = malloc(numcamas);
    

    printf("Numero de pacientes: ");
    scanf("%d", &numpacientes);

    pacientes = (paciente *) malloc(sizeof(paciente) * numpacientes);
    paciente * final = pacientes + numpacientes;
    do{
        numcamas += 5;
        int ** temp = realloc(camas,numcamas);
        camas = temp;

        for (size_t i = 0; i < numcamas; i++)
        {
            camas[i] = i + 1;
            printf("Cama n°: %d \n", camas[i]);
        }

    } while (numcamas < numpacientes);
    //printf("Hola?");
    
    
    for (aux = pacientes; aux < final; ++aux)
    {
        aux->nombre = (char *) malloc(sizeof(char) * 30);
        printf("Ingrese el nombre: ");
        scanf("%s", aux-> nombre);

        aux->apellido = (char *) malloc(sizeof(char) * 30);
        printf("Ingrese el apellido: ");
        scanf("%s", aux->apellido);

        printf("Ingrese la edad: ");
        scanf("%d", &aux->edad);
    }

    printf("\n\n-------Hospital--------\n");
    printf("La cantidad de pacientes es: %d\n", numpacientes);
    for(aux = pacientes; aux < final; ++aux)
    {
        printf("Nombre: %30s \t Apellido: %60s \t Edad: %3d \n", aux->nombre, aux->apellido, aux->edad);
    }

    for (aux = pacientes; aux < final; ++aux) {
        free(aux->nombre);
        free(aux->apellido);
    }
    
    free(pacientes);

    return 0;
    
}