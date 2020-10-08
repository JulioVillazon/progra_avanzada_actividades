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


int traverseDirectoryTree(char *);
int isDir(const char *);
void createDir();
int rmrf(char *);
int unlink_cb(const char *, const struct stat *, int, struct FTW *);
void gestor(int);
int main(int argc, char *argv[])
{
    sigset_t conjunto, pendientes;
    char path[256];
    char *nopt, *topt;
    int nArc, tempo;
    int cmd, exist;

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

    // sigaddset(&conjunto, SIGHUP);sigaddset(&conjunto, SIGINT);sigaddset(&conjunto, SIGQUIT);
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
    traverseDirectoryTree(path);

    //gestor SIGALARM
    signal(SIGALRM, gestor);


    //Ciclo crear a0, a1....an ()

        //Abrir a0, y una vez que termina abrir el a1 y asi sucesivamente
            //Establecer un temporizador t
            //Escribir en el archivo "X" mientra grabar valga 1
            //Cuanod el temporizador termine cambiar grabar a 0
            //Escribir en el archivp las señales que recibio y las pendientes
            //Volver a empezar el ciclo


    //Listar archivos creados y su tamaño


    

    return 0;
}

int traverseDirectoryTree(char *path)
{
    DIR *d;
    struct dirent *dir;
    int exists;


    d = opendir(path);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            char *currentPath = realpath(dir->d_name, NULL);
            if (isDir(currentPath))
            {
                rmrf(path);
                createDir();
            }
            else
            {
                createDir();
            }
        }
        closedir(d);
    }
}

int isDir(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}

void createDir()
{
    int check;
    char *dirname = "datos";

    printf("Directory created\n");
    check = mkdir(dirname, 0777);
    //checking if directory is created
    if (!check)
        printf("Directory created\n");
    else
    {
        printf("Unable to create directory\n");
        exit(1);
    }
    system("dir/p");
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
    printf("Mi alarma se generó hace 10 seg pero estaba bloqueada. Ya pasaron 20 seg.");
}