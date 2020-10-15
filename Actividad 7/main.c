//Julio Villazón | A01370190 -> Trabajo hecho con Jesus Gonzalez
//Jesus Gonzalez | A01422050 -> Trabajo hecho con Julio Villazón

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <ftw.h>
#include <fcntl.h>

int grabar = 0;

int searchDir(char *);
int rmrf(char *);
int unlink_cb(const char *, const struct stat *, int, struct FTW *);
void gestor(int);
void traverseDirectoryTree(char *);
int get_file_size(const char *);

int main(int argc, char *argv[])
{
    sigset_t conjunto, pendientes;
    struct sigaction senal;
    struct dirent *dir;
    char path[256], pathFile[256];
    char *nopt, *topt;
    int nArc, tempo;
    int cmd, iFd, sz;

    while ((cmd = getopt(argc, argv, "n:t:")) != -1)
        switch (cmd)
        {
        case 'n':
            nopt = optarg;
            break;

        case 't':
            topt = optarg;
            break;

        case '?':
            if (strchr("nt", optopt) != NULL)
                fprintf(stderr, "La opción -%c requiere un argumento.\n", optopt);
            else if (isprint(optopt))
                fprintf(stderr, "Opción desconocida'-%c'.\n", optopt);
            else
                fprintf(stderr, "Carácter no válido '\\x%x'.\n", optopt);
            return 1;
        default:
            printf("Te faltó poner los argumentos (Son 2).\n");
            abort();
        }

    nArc = atoi(nopt);
    tempo = atoi(topt);

    //Bloquar señañles menos SIGALARM
    sigemptyset(&conjunto);

    sigaddset(&conjunto, SIGHUP);sigaddset(&conjunto, SIGINT);sigaddset(&conjunto, SIGQUIT);
    sigaddset(&conjunto, SIGILL);sigaddset(&conjunto, SIGTRAP);sigaddset(&conjunto, SIGABRT);
    sigaddset(&conjunto, SIGIOT);sigaddset(&conjunto, SIGEMT);sigaddset(&conjunto, SIGFPE);
    sigaddset(&conjunto, SIGKILL);sigaddset(&conjunto, SIGBUS);sigaddset(&conjunto, SIGSEGV);
    sigaddset(&conjunto, SIGSYS);sigaddset(&conjunto, SIGPIPE);sigaddset(&conjunto, SIGTERM);
    sigaddset(&conjunto, SIGURG);sigaddset(&conjunto, SIGSTOP);sigaddset(&conjunto, SIGTSTP);
    sigaddset(&conjunto, SIGCONT);sigaddset(&conjunto, SIGCHLD);sigaddset(&conjunto, SIGTTIN);
    sigaddset(&conjunto, SIGTTOU);sigaddset(&conjunto, SIGIO);sigaddset(&conjunto, SIGXCPU);
    sigaddset(&conjunto, SIGXFSZ);sigaddset(&conjunto, SIGVTALRM);sigaddset(&conjunto, SIGPROF);
    sigaddset(&conjunto, SIGWINCH);sigaddset(&conjunto, SIGINFO);sigaddset(&conjunto, SIGUSR1);
    sigaddset(&conjunto, SIGUSR2);

    sigprocmask(SIG_BLOCK, &conjunto, NULL);

    //Checar acrchivo
    printf("Ingrese el nombre del archivo a buscar: ");
    scanf("%s", path);
    searchDir(path);

    //gestor SIGALARM
    senal.sa_handler = gestor;
    sigaction(SIGALRM, &senal, 0);
    
    //Ciclo crear a0, a1....an ()
    for (int i = 0; i < nArc; ++i)
    {
        sprintf(pathFile, "%s/a%d.txt", path, i);
        iFd = creat(pathFile, 0644);

        //Abrir an
        iFd = open(pathFile, O_RDWR, 0);

        //Establecer un temporizador t
        alarm(tempo);

        //Escribir en el archivo "X" mientra grabar valga 1
        grabar = 1;
        while(grabar){
            sz = write(iFd, "X\n", strlen("X"));
        }

        //Escribir en el archivp las señales que recibio y las pendientes
        sigpending(&pendientes);

        if (sigismember(&pendientes, SIGINT))
            sz = write(iFd, "Ctrl+C\n", strlen("Ctrl+C"));
        if (sigismember(&pendientes, SIGTSTP))
            sz = write(iFd, "Ctrl+Z\n", strlen("Ctrl+Z"));

        close(iFd);
        
    }

    return 0;
}

int searchDir(char *path)
{
    struct stat st = {0};

    if (stat(path, &st) == -1)
    {
        mkdir(path, 0700);
    }
    else
    {
        rmrf(path);
        mkdir(path, 0700);
    }
}

int unlink_cb(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    int rv = remove(fpath);

    if (rv)
        perror(fpath);

    return rv;
}

int rmrf(char *path)
{
    return nftw(path, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
}

void gestor(int s)
{
    //Cuanod el temporizador termine cambiar grabar a 0
    grabar = 0;
}