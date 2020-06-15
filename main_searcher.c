#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "functions.h"

#define  READ 0
#define  WRITE 1

int main(int argc, char* argv[]){

    clock_t start_t, end_t;
    start_t = clock();
    //printf("SEARCHER %d, parent %d\t, argv1 %s, argv2 %s, argv3 %s, argv4 %s, argv5 %s, argv6 %s\n",
    //        (int)getpid(), (int)getppid(), argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]);
    int start = atoi(argv[4]);
    int size = atoi(argv[5]);

    int k = read_from_bin_file(argv[0], start, size, argv[1]);    //call this for reading, searching, and writing to pipe
    end_t = clock();
    Min_Max_Times m;
    m.min = (double)start_t;
    m.max = (double)end_t;
    write(1, &m, sizeof(Min_Max_Times));
    exit(0);
}
