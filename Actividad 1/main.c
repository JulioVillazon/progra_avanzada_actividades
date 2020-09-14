//Julio Villazón | A01370190 -> trabajado en equipo con Jesus Gonzalez (para esta tarea si tuvimos que aprender en conjunto y no se pudo solo.)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    int bed;
    char *name;
    char *lastname;
    int age;
    char *phone;
}room;

int main (int argc, const char * argv[]){
    int option = 0;
    int beds = 0;
    int count;
    int newPatients = 0;
    int available;
    int bedNumber = 0;
    int moreBeds = 0;
    room *hospital;
    room *aux;
    room *start;
    room *final;
    
    do 
    {
        printf("\n");
        printf("Numero de camas (multiplo de 5): ");
        scanf("%d", &beds);
        printf("\n");
    }
    while(beds % 5 != 0);

    hospital = (room *) malloc(sizeof(room) * beds);
    final = hospital + beds;
    start = hospital;

    do
    {
        printf("\nIngrese el numero de la accion solicitada.\n");
        printf("1. Listar pacientes.\n2. Ingresar paciente.\n3. Dar de alta paciente.\n4. Aumentar camas.\n\n0. Salir del programa\n");
        scanf("%d", &option);

        switch (option) {
            case 0:
                break;
            case 1:            
                count = 1;
                printf("\n--Lista de pacientes--\n");
                printf("Bed \t Name \t\t Lastname \t Age \t Phone\n");
                for(aux = hospital; aux < final; ++aux){
                    aux->bed = count;
                    printf("%3d \t %10s \t %10s \t %3d \t %10s\n", 
                        aux->bed,
                        aux->name,
                        aux->lastname,
                        aux->age,
                        aux->phone);
                        count++;
                }
                break;
            case 2:            
                count = 0;
                for (aux = hospital; aux < final; ++aux)
                {
                    if(aux->name != NULL){
                        count++;
                    }
                }
                if(count == beds){
                    printf("\n");
                    printf("No hay camas disponibles, favor de solicitar mas.\n");
                    printf("\n");
                    break;
                }
                available = beds - count;
                printf("\n");
                printf("Hay %d camas ocupadas.\n", count);
                printf("Quédan %d camas disponibles.\n", available);
                printf("\n");

                printf("\n");
                printf("Ingresar numero de pacientes: ");
                scanf("%d", &newPatients);
                printf("\n");

                if(newPatients > available){
                    printf("\n");
                    printf("Error, lugares disponibles: %d\n", available);
                    printf("Favor de solicitar mas camas\n");
                    printf("\n");
                    newPatients = available;
                }

                count += newPatients;
                available = beds - count;
                for (aux = start; aux < start + newPatients; ++aux)
                {
                    if (aux->name != NULL)
                    {
                        do
                        {
                            ++aux;
                            ++newPatients;
                        } while (aux->name != NULL);
                    }

                    aux->name = (char *)malloc(sizeof(char) * 12);
                    printf("\n");
                    printf("\nIngrese el nombre del paciente numero %d: ", aux->bed);
                    scanf("%s", aux->name);

                    aux->lastname = (char *)malloc(sizeof(char) * 12);
                    printf("Ingrese el apellido del paciente numero %d: ", aux->bed);
                    scanf("%s", aux->lastname);

                    printf("Ingrese la edad del paciente numero %d: ", aux->bed);
                    scanf("%d", &aux->age);

                    aux->phone = (char *)malloc(sizeof(char) * 12);
                    printf("Ingrese el telefono del paciente numero %d: ", aux->bed);
                    scanf("%s", aux->phone);
                    printf("\n");            
                }

                break;
            case 3:                       
                do{
                    printf("\n");
                    printf("Ingrese el número de la cama: ");
                    scanf("%d", &bedNumber);
                    printf("\n");

                    if(bedNumber > beds)
                        printf("Numero de cama invalida \n");

                } while(bedNumber > beds);


                aux = hospital + bedNumber-1;
                aux->name = NULL;
                aux->lastname = NULL;
                aux->phone = NULL;
                aux->age = 0;

                printf("Paciente en cama %d dado de alta satisfactoriamente\n", aux->bed);
                printf("\n");

                break;
            case 4:            
                if(available != 0){
                    printf("Favor de ocupar todas las camas antes de solicitar mas\n");
                    break;
                }

                do 
                {
                    printf("\n");
                    printf("Numero de camas (multiplo de 5): ");
                    scanf("%d", &moreBeds);
                    printf("\n");
                }
                while(moreBeds % 5 != 0);            
                
                beds += moreBeds;
                hospital = (room*) realloc(hospital, sizeof(room) * beds);
                final = hospital + beds;

                break;

            default:
                printf("Opcion invalida\n");
        }

    } while (option != 0);

    for(aux = hospital; aux < final; ++aux){
        free(aux->name);
        free(aux->lastname);
        free(aux->phone);
    }

    free(hospital);

    return 0;
}