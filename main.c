#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <signal.h>
#include <sys/mman.h>

//***************************************//
//                SEMAPHORES             //
//***************************************//

#define SEMAPHORE1_NAME "/semaphore_synchronization1"
#define SEMAPHORE2_NAME "/semaphore_synchronization2"

//***************************************//
//                FUNCTIONS              //
//***************************************//
int control(int argc, char *argv[]);
void proc1();
void proc2();

//***************************************//
//       DEFINES && GLOBAL VARIABLES     //
//***************************************//
sem_t *semaphore1;
sem_t *semaphore2;
#define shmSIZE sizeof(int)
#define shmKEY "/ios-proj2"
#define shmHACK "/ios-proj2-hack"
#define shmSERF "/ios-proj2-serf"

int main(int argc, char *argv[])
{
    setbuf(stdout,NULL);
    setbuf(stderr,NULL);

    int controler = control(argc, argv);
    if (controler == -1){
        return -1;
    }

    // create two locked semaphores
    if ((semaphore1 = sem_open(SEMAPHORE1_NAME, O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED)
    {
        // handle error
        return -1;
    }
    sem_close(semaphore1);

    if ((semaphore2 = sem_open(SEMAPHORE2_NAME, O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED)
    {
        // handle error
        return -1;
    }
    sem_close(semaphore2);

    int pidSerf;
    int pidHack;
    int *shm;
    int *hack;
    int *serf;
    int shmID;
    int hackID;
    int serfID;
    pid_t consPID;
    pid_t prodPID;

    shmID = shm_open(shmKEY, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(shmID, shmSIZE);
    hackID = shm_open(shmHACK, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(hackID, shmSIZE);
    serfID = shm_open(shmSERF, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(serfID, shmSIZE);
    shm = (int*)mmap(NULL, shmSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmID, 0);
    hack = (int*)mmap(NULL, shmSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, hackID, 0);
    serf = (int*)mmap(NULL, shmSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, serfID, 0);
    close(shmID);
    close(hackID);
    close(serfID);
    *shm = 0;
    *hack = 0;
    *serf = 0;
    munmap(shm, shmSIZE);
    munmap(hack, shmSIZE);
    munmap(serf, shmSIZE);





    pidHack = fork();
    //HACKER PROCES
    if (pidHack == 0) { // child
        proc1();
        exit(0);
    } else if (pidHack > 0) consPID = pidHack;
    else{
        //error handle
        return -1;
    }

    //SERF PROCES
    pidSerf = fork();
    if (pidSerf == 0) { // child
        proc2();
        exit(0);
    } else if (pidSerf > 0) prodPID = pidSerf;
    else{
        //error handle
        return -1;
    }





    // pockame az vsichni skonci
    waitpid(consPID, NULL, 0);
    waitpid(prodPID, NULL, 0);

    // zrusime zdroje
    shm_unlink(shmKEY);
    shm_unlink(shmHACK);
    shm_unlink(shmSERF);
    sem_unlink(SEMAPHORE1_NAME);
    sem_unlink(SEMAPHORE2_NAME);

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

void proc1() {
    sem_t *semaphore1;
    sem_t *semaphore2;
    semaphore1 = sem_open(SEMAPHORE1_NAME, O_RDWR);
    semaphore2 = sem_open(SEMAPHORE2_NAME, O_RDWR);

    int shmID;
    int hackID;
    int *shm;
    int *hack;

    shmID = shm_open(shmKEY, O_RDWR, S_IRUSR | S_IWUSR);
    hackID = shm_open(shmHACK, O_RDWR, S_IRUSR | S_IWUSR);
    shm = (int*)mmap(NULL, shmSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmID, 0);
    hack = (int*)mmap(NULL, shmSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, hackID, 0);
    close(shmID);
    close(hackID);

    for (int i=0; i<5; i++) {
        sem_post(semaphore1);
        sem_wait(semaphore2);
        *shm = *shm + 1;
        *hack = *hack + 1;
        fprintf(stdout, "%d: HACKER %d: starts\n", *shm, *hack);
        usleep(500);
    }

    munmap(shm, shmSIZE);
    munmap(hack, shmSIZE);
}

void proc2() {
    sem_t *semaphore1;
    sem_t *semaphore2;
    semaphore1 = sem_open(SEMAPHORE1_NAME, O_RDWR);
    semaphore2 = sem_open(SEMAPHORE2_NAME, O_RDWR);

    int shmID;
    int serfID;
    int *shm;
    int *serf;

    // kontrola uspesnosti !!!
    shmID = shm_open(shmKEY, O_RDWR, S_IRUSR | S_IWUSR);
    serfID = shm_open(shmSERF, O_RDWR, S_IRUSR | S_IWUSR);
    shm = (int*)mmap(NULL, shmSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmID, 0);
    serf = (int*)mmap(NULL, shmSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, serfID, 0);
    close(shmID);
    close(serfID);

    for (int i=0; i<5; i++) {
        sem_wait(semaphore1);
        *serf = *serf + 1;
        *shm = *shm + 1;
        fprintf(stdout, "%d: SERF %d: starts\n", *shm, *serf);
        sem_post(semaphore2);
        usleep((useconds_t)random() % 2000);
    }

    munmap(shm, shmSIZE);
    munmap(serf, shmSIZE);
}