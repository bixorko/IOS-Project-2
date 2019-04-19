#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>

int control(int argc, char *argv[]);

sem_t mutex;

int main(int argc, char *argv[])
{
    int controler = control(argc, argv);
    if (controler == -1) {
        return -1;
    }

    if (sem_init(&mutex,0,0) == 0){
        printf("Initialized mutex to 0!\n");
    }

    pid_t hackers = fork();
    if (hackers == 0){
        printf("ChildOfHackers\n");
        exit(0);
    }
    sem_close(&mutex);

    printf("ParentOfHackers\n");

    pid_t serfs = fork();
    if (serfs == 0){
        printf("ChildOfSerfs\n");
        exit(0);
    }
    sleep(2);
    printf("ParentOfSerfs\n");

    return 0;
}

int control(int argc, char *argv[])
{
    if (argc != 7){
        fprintf(stderr, "Zle zadane argumenty!\n");
        return -1;
    }

    char *pEnd = NULL;
    int P, H, S, R, W, C;

    P = (int)strtol(argv[1], &pEnd, 10);
    if (strlen(pEnd) != 0 || P < 2 || (P % 2) != 0){
        fprintf(stderr, "Zle zadany argument 'P'!\n");
        return -1;
    }

    H = (int)strtol(argv[2], &pEnd, 10);
    if (strlen(pEnd) != 0 || H < 0 || H > 2000){
        fprintf(stderr, "Zle zadany argument 'H'!\n");
        return -1;
    }

    S = (int)strtol(argv[3], &pEnd, 10);
    if (strlen(pEnd) != 0 || S < 0 || S > 2000){
        fprintf(stderr, "Zle zadany argument 'S'!\n");
        return -1;
    }

    R = (int)strtol(argv[4], &pEnd, 10);
    if (strlen(pEnd) != 0 || R < 0 || R > 2000){
        fprintf(stderr, "Zle zadany argument 'R'!\n");
        return -1;
    }

    W = (int)strtol(argv[5], &pEnd, 10);
    if (strlen(pEnd) != 0 || W < 20 || W > 2000){
        fprintf(stderr, "Zle zadany argument 'W'!\n");
        return -1;
    }

    C = (int)strtol(argv[6], &pEnd, 10);
    if (strlen(pEnd) != 0 || C < 5){
        fprintf(stderr, "Zle zadany argument 'C'!\n");
        return -1;
    }

    return 0;
}