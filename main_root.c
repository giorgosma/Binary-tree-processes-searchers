#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#include "functions.h"

#define  READ 0
#define  WRITE 1


int readcommandline(int argc, char **argv, char *name){
    int i = 0, ret;
    do{
        i++;
        if(i >= argc)
            return 0;
        else
            ret = strcmp(argv[i], name);
    }
    while(ret != 0);
    return i;
}

int main(int argc, char* argv[]){
    int status;
    Record **array_of_recs = (Record **)malloc(1 * sizeof(Record *));
    array_of_recs[0] = (Record *)malloc(1 * sizeof(Record));
    int i = 0;

    int i1 = readcommandline(argc, argv, "-h");
    int i2 = readcommandline(argc, argv, "-d");
    int i3 = readcommandline(argc, argv, "-p");
    int i4 = readcommandline(argc, argv, "-s");
    //printf("ROOT %d, parent %d, argv1 %s, argv2 %s, argv3 %s, argv4 %s, argv5 %s, argv6 %s, argv7 %s\n",
    //         (int)getpid(), (int)getppid(), argv[i1], argv[i1+1], argv[i2], argv[i2+1], argv[i3], argv[i3+1], argv[i4]);
    int filesize = getfilesize(argv[i2+1]);
    int height = atoi(argv[i1+1]), st = 1;
    int fin = squares(2, height);
    char start[5], finish[5], sizeoffile[5];
    sprintf(start, "%d", st);
    sprintf(finish, "%d", fin);
    sprintf(sizeoffile, "%d", filesize);
    int fd[2], bytes;
    if (pipe(fd) == -1){ perror("pipe"); exit(1); }
    pid_t pid = fork();
    if (pid == 0){                  //first kid for splitter_merger
        //call exec sp_mg
        close(fd[READ]);
        dup2(fd[WRITE], 1);
        close(fd[WRITE]);
        execlp("./main_splitter_merger", argv[i1+1], argv[i2+1], argv[i3+1], argv[i4], start, finish, sizeoffile, argv[i1+1],  (char *)NULL);
    }
    else{
        Times t;
        double tt;
        Min_Max_Times m;
        Record recs;
        int f, sz = 0;
        close(fd[WRITE]);
        while ((bytes=read(fd[READ], &recs, sizeof(Record))) > 0){          //read from pipe the records
            if (bytes == sizeof(Record)){
                sz++;
                //printf("Read %d bytes and size %ld: \n",bytes, sizeof(Record));
                memcpy(array_of_recs[i], &recs, sizeof(Record));
                 i++;
                 array_of_recs = (Record **)realloc(array_of_recs, (sz+1) * sizeof(Record*));

                array_of_recs[i] = (Record *)malloc(1 * sizeof(Record));
            }
            else {
                memcpy(&t, &recs, sizeof(Times));                     //read from pipe the times
            }
        }
        close(fd[READ]);
        int ex = waitpid(pid,NULL,0);

        int a=0;
        FILE *fp=fopen("outputfile.txt","w");             //write records to a file output.txt
        for (int j = 0; j< sz; j++){
            char OutputFile[200];
            int number = sprintf(OutputFile, "%ld %s %s  %s %d %s %s %f %s", array_of_recs[j]->custid, array_of_recs[j]->LastName, array_of_recs[j]->FirstName, array_of_recs[j]->Street, array_of_recs[j]->HouseID, array_of_recs[j]->City, array_of_recs[j]->postcode, array_of_recs[j]->amount, "\n");
            fwrite(OutputFile, 1, number, fp);
            a++;
        }
        fclose(fp);             //second kid for calling sort
        pid_t pid2 = fork();
        if (pid2 == 0){
            execlp("sort","sort","-g","outputfile.txt",(char *)NULL);
        }
        else{
            int ex1 = waitpid(pid2,NULL,0);
        }
        printf("I PRINTED %d RECORDS\n",sz);
        Print_The_Times(t);
        printf("I am parent %d of the exited %d\n",getpid(),ex);
        for (int j = 0; j<= sz; j++){         //free the memory
            free(array_of_recs[j]);
        }
        free(array_of_recs);
        exit(0);
    }
}
