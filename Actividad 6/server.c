#include <stdio.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

#define TCP_PORT 8000

void gestor_ctrl(int);

int main(int argc, const char *argv[])
{

    struct sockaddr_in direccion;
    char buffer[1000];

    int servidor, cliente;

    int leidos, escritos;

    struct sigaction senal;

    if (argc != 2)
    {
        printf("Error. Use: %s A.B.C.D \n", argv[0]);
        exit(-1);
    }

    //Crear el socket
    servidor = socket(PF_INET, SOCK_STREAM, 0);

    // Enlace con el socket
    direccion.sin_port = htons(TCP_PORT);
    direccion.sin_family = AF_INET;
    inet_aton(argv[1], &direccion.sin_addr);

    bind(servidor, (struct sockaddr *)&direccion, sizeof(direccion));

    // Escuchar a los clientes
    listen(servidor, 10);

    // Comunicación
    socklen_t tamano = sizeof(direccion);

    cliente = accept(servidor, (struct sockaddr *)&direccion, &tamano);

    sigaction(SIGTSTP, NULL, &senal);
    sigaction(SIGINT, NULL, &senal);

    if (cliente >= 0)
    {
        printf("Aceptando conexiones en %s:%d \n",
               inet_ntoa(direccion.sin_addr),
               ntohs(direccion.sin_port));

        senal.sa_handler = gestor_ctrl;

        if (sigaction(SIGTSTP, &senal, 0) == -1 || sigaction(SIGINT, &senal, 0) == -1)
        {
            printf("Error en el gestor de señales.\n");
            exit(-1);
        }

        
        // Leer de socket y escribir en pantalla
        while (leidos = read(cliente, &buffer, sizeof(buffer)))
        {
            write(fileno(stdout), &buffer, leidos);

            leidos = read(fileno(stdin), &buffer, sizeof(buffer));
            write(cliente, &buffer, leidos);
        }
    }

    // Cerrar el socket

    sigaction(SIGTSTP, &senal, NULL);
    sigaction(SIGINT, &senal, NULL);

    close(servidor);
    close(cliente);

    return 0;
}

void gestor_ctrl(int sid)
{
    if (sid == SIGTSTP)
    {
        printf("%d\n", sid);
    }
    else if (sid == SIGINT)
    {
        printf("%d\n", sid);
    }
}