#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#include "functions.h"
#define  READ 0
#define  WRITE 1

typedef int bool;
#define TRUE 1
#define FALSE 0

int main(int argc, char** argv){
    clock_t start_t, end_t;
    start_t = clock();
    int status;
    int height = atoi(argv[0]);
    int filesize = getfilesize(argv[1]);
    int start = atoi(argv[4]);
    int finish = atoi(argv[5]);
    int size = atoi(argv[6]);
    //printf("Height = %d\t MERGER %d, parent %d, start %s, finish %s, size %d\n",
    //        height, (int)getpid(), (int)getppid(), argv[4], argv[5], size);
    height--;                             //lower the height
    sprintf(argv[0], "%d", height);
    Times t = make_new_process(height, argv);
    end_t = clock();
    double st = (double)start_t;
    double en = (double)end_t;
    double sp_mg_time = (double)((en - st)/CLOCKS_PER_SEC);
        (t.sp_mg).min += sp_mg_time;      //min sum of chlid's min time and fathers time
        (t.sp_mg).max += sp_mg_time;      //max sum of chlid's max time and fathers time
    write(1, &t, sizeof(Times));          //write it to pipe for the father
    exit(0);
}
