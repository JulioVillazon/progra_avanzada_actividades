#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void gestor_ctrl(int sid)
{
    if (sid == SIGSTOP)
    {
        printf("Señal %d y se oprimio CTRL-Z\n", sid);
    }
    else if (sid == SIGINT)
    {
        printf("Señal %d y se oprimio CTRL-C\n", sid);
    }
    else
    {
        printf("Señal %d", sid);
    }
}

void contar(char *texto, void (*handler_t)(int))
{
    struct sigaction gestor;

    gestor.sa_handler = handler_t; // signal(SIGINT, handler_t);

    sigaction(SIGINT, &gestor, NULL);
    sigaction(SIGTSTP, &gestor, NULL);

    int i = 1;

    while (i <= 10)
    {
        printf("%d %s...\n", i++, texto);
        sleep(1);
    }
}

int main(int argc, const char *argv[])
{

    /* Obtener manejador previo */
    printf("Guardando manejador inicial...\n");
    struct sigaction gestor;

    sigaction(SIGINT, NULL, &gestor);
    sigaction(SIGTSTP, NULL, &gestor);

    /* Bloque 1 Contando ovejitas */
    contar("verde", gestor_ctrl);

    /* Bloque 3 Contando cabritas */
    contar("rojo", gestor_ctrl);

    /* Restaurar manejador inicial */
    printf("Restaurando manejador inicial...\n");
    sigaction(SIGINT, &gestor, NULL);
    sigaction(SIGTSTP, &gestor, NULL);

    return 0;
}