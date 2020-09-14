//Actividad 1: Aritmética de apuntadores
/*
Se quiere desarrollar una aplicación para llevar el control de pacientes ingresados en un hospital. De cada paciente 
interesa conocer su nombre, apellidos, edad, teléfono de contacto y la cama asignada. Inicialmente, el hospital cuenta 
con un número de camas fijas pero se sabe que si se requiere, es posible incorporar más camas (se identifican con un número 
consecutivo) en cualquier momento para poder atender más pacientes. Por cuestiones de logística, siempre que se requiera 
incrementar el número de camas, se incrementan de 5 en 5.

Diseñe e implemente una aplicación en C que se permita:

Incorporar pacientes al hospital (Tantos como se requiera. Si se llenan las camas disponibles y
llega un nuevo paciente hay que permitir su ingreso).
Conocer qué paciente se encuentra asignado a una cama dada o si ésta se encuentra disponible.
Dar de alta a un paciente por el número de cama (se retira del hospital y la cama queda disponible).
Mostrar el listado de todos los pacientes que se encuentran en el hospital, incluyendo el número de la cama asignada.
Conocer el número de camas disponibles y el número de camas ocupadas..
*/
//Jesús González - A01422050
//16/08/2020

#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int bed;
    char *name;
    char *lastname;
    int age;
    char *phone;
}room;

void viewList();
void addPatient();
void deletePatient();

int main (int argc, const char * argv[]){
    int opc = 0, n = 0, count = 1, morePatients = 0, moreBeds = 0;
    room *hospital;
    room *aux;
    room *final;

    printf("Ingresar cantidad inicial de camas en el hopsital (Debido a cuestiones de logística debe de ser un múltiplo de 5): ");
    scanf("%d", &n);
    if(n%5 != 0){
        printf("No es posible poner esa cantidad, se redondeará al múltiplo de 5 más cercano.\n");
        if(n < 5){
            n = 5;
        }
        else if(n % 5 >= 3)
        {
            n = (5 - (n % 5)) + n;
        }
        else if (n % 5 < 3)
        {
            n = (5 - (n % 5));
        }
    }

    hospital = (room *) malloc(sizeof(room) * n);
    final = hospital + n;

    do
    {
        printf("1. Ver lista de pacientes.\n2. Ingresar pacientes.\n3. Dar de alta pacientes.\n4. Aumentar camas en el hospital.\n");
        scanf("%d", &opc);

        switch (opc)
        {
        case 0:
            break;
        case 1:
            //viewList();
            printf("\n--Lista de pacientes--\n");
            printf("Bed \t Name \t\t Lastname \t Age \t Phone\n");
            for(aux = hospital; aux < final; ++aux){
                printf("%3d \t %10s \t %10s \t %3d \t %10s\n", 
                    aux->bed,
                    aux->name,
                    aux->lastname,
                    aux->age,
                    aux->phone);
            }
            break;
        case 2:
            //addPatient();
            // count = 0;
            // for (aux = hospital; aux < total; ++aux)
            // {
            //     if(aux->name && aux->lastname && aux->phone == NULL)
            //         count++;
            // }
            // if(count == n){
            //     printf("Lo siento no hay más camas dispoibles, regresa al menu para pedir más camas.\n");
            //     break;
            // }
            // printf("Quédan %d camas disponibles", count);

            printf("¿Cuántos pacientes quieres agregar?: ");
            scanf("%d", &morePatients);

            for(aux = hospital; aux < hospital + morePatients; ++aux){
                aux->name = (char *) malloc(sizeof(char) * 12);
                printf("Ingresa el nombre del paciente: ");
                scanf("%s", aux->name);

                aux->lastname = (char *) malloc(sizeof(char) * 12);
                printf("Ingresa el apellido del paciente: ");
                scanf("%s", aux->lastname);

                printf("Ingresa la edad del paciente: ");
                scanf("%d", &aux->age);

                aux->phone = (char *) malloc(sizeof(char) * 10);
                printf("Ingresa el teléfono del paciente: ");
                scanf("%s", aux->phone);
            } 
            break;
        case 3:
            //deletePatient();
            break;
        case 4:
            //addBeds();
            printf("Ingresar cantidad de camas que quieres agregar (Debido a cuestiones de logística debe de ser un múltiplo de 5): ");
            scanf("%d", &moreBeds);
            if (moreBeds% 5 != 0)
            {
                printf("No es posible poner esa cantidad, se redondeara al múltiplo de 5 más cercano.\n");
                if (moreBeds < 5)
                {
                    moreBeds = 5;
                }
                else if (moreBeds % 5 >= 3)
                {
                    moreBeds = (5 - (moreBeds % 5)) + moreBeds;
                }
                else if (moreBeds % 5 < 3)
                {
                    moreBeds = (5 - (moreBeds % 5));
                }
            }

            hospital = (room*) realloc(hospital, sizeof(room) * (n + moreBeds));
            final = hospital + n + moreBeds;

            break;

        default:
            printf("Opción no válida.\n");
        }

    } while (opc != 0);

    for(aux = hospital; aux < final; ++aux){
        free(aux->name);
        free(aux->lastname);
        free(aux->phone);
    }

    free(hospital);

    return 0;
}

void viewList(){

}

void addPatient(){

}

void deletePatient(){

}


// void del()
// {
//     int f, h;
//     printf("Enter the serial number of the patient that you want to delete=");
//     scanf("%d", &f);
//     if (f < num)
//     {
//         while (f < num)
//         {
//             strcpy(x[f].name, x[f + 1].name);
//             strcpy(x[f].disease, x[f + 1].disease);
//             x[f].age = x[f + 1].age;
//             x[f].cabin = x[f + 1].cabin;
//             x[f].phone = x[f + 1].phone;
//             f++;
//         }
//         num--;
//     }
//     else
//         printf("\n\nInvalid Serial number\n");
// }

