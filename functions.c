
#include "functions.h"
#define  READ 0
#define  WRITE 1

int getfilesize(char *namefile){
    FILE *fpb;
    Record rec;
    long lSize;
    int numOfrecords, i;
    //printf("Im going to open %s\n",namefile);
    fpb = fopen (namefile,"rb");
    if (fpb==NULL) {
        printf("Cannot open binary file\n");
        return 0;
    }
    fseek (fpb , 0 , SEEK_END);
    lSize = ftell (fpb);
    rewind (fpb);
    numOfrecords = (int) lSize/sizeof(rec);
    fclose (fpb);
    return numOfrecords;
}

int squares(int a, int x){
    int b =1;
    for (int i = 1; i <= x; i++)
        b = b*a;      //calculate 2^h
    return b;
}

int calcthesum(char *height){
    int h = atoi(height);
    int max = squares(2,h);
    int sum = 0;
    for (int i = 1; i <= max; i++){
        sum += i;     //calculate SUM for -s option
    }
    return sum;
}

int calcthesize(char *filename, char *number, char *height){
    int k = getfilesize(filename);    //calculate SUM for -s option
    int i = atoi(number);
    int sum = calcthesum(height);
    return ((k*i)/sum);
}

int calc_starter_record(char *filename, char *number, char *height){
    int k = getfilesize(filename);
    int current = atoi(number);
    int sum = calcthesum(height);
    int num_of_recs = 0;
    for (int i = 1; i < current; i++){
        num_of_recs += (k*i)/sum;
    }
    return num_of_recs;     //calculate the size of records for the -s option
}
int call_strstr(char *string, char *substring){
    char *ret;
    ret = strstr(string, substring);
    if (ret == NULL)
        return 0;
    else
        return 1;
}

int check_for_substring(Record *recs, char *pattern){
    char bf_long[sizeof(long) + 1],
        bf_int[sizeof(int) + 1],
        bf_float[2*sizeof(float)];
    sprintf(bf_long, "%ld", recs->custid);
    sprintf(bf_int, "%d", recs->HouseID);
    sprintf(bf_float, "%.2f", recs->amount);

    if(call_strstr(bf_long, pattern) == 0){
        if(call_strstr(bf_int, pattern) == 0){
            if(call_strstr(bf_float, pattern) == 0){
                if(call_strstr(recs->FirstName, pattern) == 0){
                    if(call_strstr(recs->LastName, pattern) == 0){
                        if(call_strstr(recs->Street, pattern) == 0){
                            if(call_strstr(recs->City, pattern) == 0){
                                if(call_strstr(recs->postcode, pattern) == 0){
                                    return 0;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 1;
}

int read_from_bin_file(char *name, int start, int size, char *pattern){
    int filedes;
    int numOfrecords, i = 1;
    filedes = open(name, O_RDONLY);
    if (filedes== -1) {
       	printf("Cannot access binary file\n");
       	return 1;
    	}
    Record recs;
    int nread=0, a=0;
    lseek (filedes , start*sizeof(Record) , SEEK_SET);
    while(i <= size){
        if((nread=read(filedes, &recs , sizeof(Record))) > 0){
            if(check_for_substring(&recs, pattern) == 1){
                write(1, &recs, sizeof(Record));
                a++;
            }
        }
        i++;
    }
    close(filedes);
    return a;
}
void printstruct(Record *recs){
    printf("%ld %s %s  %s %d %s %s %-9.2f\n", recs->custid, recs->LastName, recs->FirstName, recs->Street, recs->HouseID, recs->City, recs->postcode, recs->amount);
}
double store_the_min(Min_Max_Times *t, double st){
    if ((t->min) > st)
        return st;
    else
        return t->min;
}
double store_the_max(Min_Max_Times *t, double st){
    if ((t->max) < st)
        return st;
    else
        return t->max;
}
void Calc_The_Times(Min_Max_Times *t, double st1, double st2){
    if ((t->min) == 0.0 && (t->max) == 0.0){
        if(st1 > st2){
            t->min = st2;
            t->max = st1;
        }
        else{
            t->min = st1;
            t->max = st2;
        }
    }
    else{
        t->min = store_the_min(t, st1);
        t->min = store_the_min(t, st2);

        t->max = store_the_max(t, st1);
        t->max = store_the_max(t, st2);
    }
}
void Print_The_Times(Times t){
    printf("SERCHERS: MIN %f, MAX %f, AVG %f\n", (t.se).min, (t.se).max, ( (t.se).min + (t.se).max )/2);
    printf("SPLITTERS: MIN %f, MAX %f, AVG %f\n", (t.sp_mg).min, (t.sp_mg).max, ( (t.sp_mg).min + (t.sp_mg).max )/2);
}
void Intitial_Times(Times *t){
    (t->sp_mg).min = 0.0;
    (t->sp_mg).max = 0.0;
    (t->se).min = 0.0;
    (t->se).max = 0.0;
}

int set_file_size(int start, int size, char bf[][4], int *flag){
    sprintf(bf[0], "%d", start);
    if (size % 2 != 0){
        if (*flag){
            *(flag) = 0;
            size++;
        }
    }
    size = size/2;
    sprintf(bf[1], "%d", size);
    return (start + (size));    // set the file's size for the other option
}


int callTheExecutable(int height){
    if(height >= 1){
        return 0;
    }
    else{         //check if the height is 1
        return 1;
    }
}
Times make_new_process(int height, char *argv[]){
    Times t;
    Intitial_Times(&t);
    int status,
        flag = 1,
        start = atoi(argv[4]),
        finish = atoi(argv[5]),
        size = atoi(argv[6]);
        int i = 0;
        int sz = 0;
    char buffer[3][100];
    int fd1[2], bytes;
    if (pipe(fd1) == -1){ perror("pipe1"); exit(1); }
    int left_finish = (start + finish)/2;     //start and finish records for each child  of this process
    int right_start = left_finish +1;
    size = size/2;
    sprintf(buffer[0], "%d", start);
    sprintf(buffer[1], "%d", left_finish);
    sprintf(buffer[2], "%d", size);
        //printf("L\targv0 = %s, argv1 %s, argv2 %s, argv3 %s, argv4 %s, argv5 %s, argv6 %s\n",
        //        argv[0], argv[1], argv[2], argv[3], buffer[0], buffer[1], buffer[2]);
    pid_t pid1 = fork();
    if (pid1 == 0){                           //Left child-process
        if(callTheExecutable(height)){
            int ret = strcmp(argv[3], "-s");
            if (ret == 0){                      // -s
                //int sum;
                int starter_record = calc_starter_record(argv[1], buffer[0], argv[7]);      //the sizeof records that have been read before the current searcher
                sprintf(buffer[1], "%d", starter_record);
                char last_searcher[4];
                int num_of_searchers = squares(2, atoi(argv[7]));
                sprintf(last_searcher, "%d", num_of_searchers);
                int ret2 = strcmp(buffer[0], last_searcher);
                if (ret2 == 0){
                    int filesize = getfilesize(argv[1]);
                    int rest_rec = filesize - starter_record;
                    sprintf(buffer[2], "%d", rest_rec);      //sizeof records that will read the searcher 2^h
                }
                else{
                    int sum = calcthesize(argv[1], buffer[0], argv[7]);     //sizeof records that will read the searchers 1 - (2^h)-1
                    sprintf(buffer[2], "%d", sum);
                }
            }
            else{
                int starter_record = (start-1)*size;
                sprintf(buffer[1], "%d", starter_record);
                char last_searcher[4];
                int num_of_searchers = squares(2, atoi(argv[7]));
                sprintf(last_searcher, "%d", num_of_searchers);
                int ret2 = strcmp(buffer[0], last_searcher);
                if (ret2 == 0){                                 //sizeof records that will read the searcher 2^h
                    int filesize = getfilesize(argv[1]);
                    int rest_rec = filesize - (start-1)*size;
                    sprintf(buffer[2], "%d", rest_rec);
                }
                else{
                    sprintf(buffer[2], "%d", size);             //sizeof records that will read the searchers 1 - (2^h)-1
                }
            }
            close(fd1[READ]);
            dup2(fd1[WRITE],1);
            close(fd1[WRITE]);
            execlp("./main_searcher", argv[1], argv[2], argv[3], buffer[0], buffer[1], buffer[2], argv[7], (char *)NULL);
                                    // file, Pattern, -s(else garbage), height, current searcher, starting rec, size of rec, NUM OF SERCHERS
        }
        else{
            close(fd1[READ]);
            dup2(fd1[WRITE],1);
            close(fd1[WRITE]);
            execlp("./main_splitter_merger", argv[0], argv[1], argv[2], argv[3], buffer[0], buffer[1], buffer[2], argv[7], (char *)NULL);
                                            //remaining Height, file, Pattern, -s, start rec, finish rec, size rec, HEIGHT
        }
    }
    else{
        Min_Max_Times m;
        int f;
        int bytes1, bytes2;
        double se_stat1 = 0,
            se_stat2 = 0,
            sp_stat1 = 0.0,
            sp_stat2 = 0.0;
        int fd2[2];
            sprintf(buffer[0], "%d", right_start);
            sprintf(buffer[1], "%d", finish);
            if (pipe(fd2) == -1){ perror("pipe2"); exit(1); }
        pid_t pid2 = fork();
        if (pid2 == 0){                       //Right child-process
            if(callTheExecutable(height)){
                int ret = strcmp(argv[3], "-s");
                if (ret == 0){                      // -s
                    int starter_record = calc_starter_record(argv[1], buffer[0], argv[7]);      //the sizeof records that have been read before the current searcher
                    sprintf(buffer[1], "%d", starter_record);
                    char last_searcher[4];
                    int num_of_searchers = squares(2, atoi(argv[7]));
                    sprintf(last_searcher, "%d", num_of_searchers);
                    int ret2 = strcmp(buffer[0], last_searcher);
                    if (ret2 == 0){
                        int filesize = getfilesize(argv[1]);
                        int rest_rec = filesize - starter_record;
                        sprintf(buffer[2], "%d", rest_rec);      //sizeof records that will read the searcher 2^h
                    }
                    else{
                        int sum = calcthesize(argv[1], buffer[0], argv[7]);
                        //sizeof records that will read the searchers 1 - (2^h)-1
                        sprintf(buffer[2], "%d", sum);
                    }
                }
                else{                         //without -s
                    int starter_record = (right_start-1)*size;
                    sprintf(buffer[1], "%d", starter_record);
                    char last_searcher[4];
                    int num_of_searchers = squares(2, atoi(argv[7]));
                    sprintf(last_searcher, "%d", num_of_searchers);
                    int ret2 = strcmp(buffer[0], last_searcher);
                    if (ret2 == 0){       //sizeof records that will read the searcher 2^h
                        int filesize = getfilesize(argv[1]);
                        int rest_rec = filesize - (right_start-1)*size;
                        sprintf(buffer[2], "%d", rest_rec);
                    }
                    else{           //sizeof records that will read the searchers 1 - (2^h)-1h
                        sprintf(buffer[2], "%d", size);
                    }
                }
                close(fd2[READ]);
                dup2(fd2[WRITE],1);
                close(fd2[WRITE]);
                execlp("./main_searcher", argv[1], argv[2], argv[3], buffer[0], buffer[1], buffer[2], argv[7], (char *)NULL);
                                            // file, Pattern, -s(else garbage), height, current searcher, starting rec, size of rec, NUM OF SERCHERS
            }
            else{
                close(fd2[READ]);
                dup2(fd2[WRITE],1);
                close(fd2[WRITE]);
                execlp("./main_splitter_merger", argv[0], argv[1], argv[2], argv[3], buffer[0], buffer[1], buffer[2], argv[7], (char *)NULL);
                                                //remaining Height, file, Pattern, -s, start rec, finish rec, size rec, HEIGHT
            }
        }
        else{
            Record rec2, rec1;
            bool flag1 = TRUE, flag2 = TRUE;
            int num_fd1 =0,num_fd2 =0, stat =0;
            while ((flag1 == TRUE) || (flag2 == TRUE)) {  //while exists records to be read
                if (flag1 == TRUE){                       //read from pipe fd1, left child process
                    close(fd1[WRITE]);                    //read records
                    bytes1=read(fd1[READ], &rec1, sizeof(Record));
                    if (bytes1 == sizeof(Record)){
                        write(1, &rec1, sizeof(Record));
                        num_fd1++;
                    }
                    else if (bytes1 == sizeof(Min_Max_Times)){    //read Searcher's times
                        memcpy(&m, &rec1, sizeof(Min_Max_Times));
                        se_stat1 = (double)((m.max - m.min)/CLOCKS_PER_SEC);
                        Calc_The_Times(&(t.se), se_stat1, se_stat1);
                        (t.sp_mg).min = (t.se).min + (t.se).max;
                        (t.sp_mg).max = (t.se).min + (t.se).max;
                        f++;
                        flag1 = FALSE;
                        close(fd1[READ]);
                    }
                    else if (bytes1 == sizeof(Times)){    //read splitter_merger's times
                        Times t1;
                        Intitial_Times(&t1);
                        memcpy(&t1, &rec1, sizeof(Times));
                        se_stat1 = (t1.se).min;
                        se_stat2 = (t1.se).max;
                        Calc_The_Times(&(t.se), se_stat1, se_stat2);
                        sp_stat1 += (t1.sp_mg).min;
                        sp_stat2 += (t1.sp_mg).max;
                        Calc_The_Times(&(t.sp_mg), sp_stat1, sp_stat2);
                        flag1 = FALSE;
                        close(fd1[READ]);
                    }
                }
                if (flag2 == TRUE){                 //read from pipe fd2, right child process
                    close(fd2[WRITE]);
                    bytes2=read(fd2[READ], &rec2, sizeof(Record));
                    if (bytes2 == sizeof(Record)){
                        write(1, &rec2, sizeof(Record));
                        num_fd2++;
                    }
                    else if (bytes2 == sizeof(Min_Max_Times)){      //read Searcher's times
                        memcpy(&m, &rec2, sizeof(Min_Max_Times));
                        se_stat2 = (double)((m.max - m.min)/CLOCKS_PER_SEC);
                        Calc_The_Times(&(t.se), se_stat2, se_stat2);
                        (t.sp_mg).min = (t.se).min + (t.se).max;
                        (t.sp_mg).max = (t.se).min + (t.se).max;
                        f++;
                        flag2 = FALSE;
                        close(fd2[READ]);
                    }
                    else if (bytes2 == sizeof(Times)){        //read splitter_merger's times
                        Times t2;
                        Intitial_Times(&t2);
                        memcpy(&t2, &rec2, sizeof(Times));
                        se_stat1 = (t2.se).min;
                        se_stat2 = (t2.se).max;
                        Calc_The_Times(&(t.se), se_stat1, se_stat2);
                        sp_stat1 += (t2.sp_mg).min;
                        sp_stat2 += (t2.sp_mg).max;
                        Calc_The_Times(&(t.sp_mg), sp_stat1, sp_stat2);
                        flag2 = FALSE;
                        close(fd2[READ]);
                     }
                }
            }
        }
        int ex2 = waitpid(pid2,NULL,0);       //wait for kids
        int ex1 = waitpid(pid1,NULL,0);
    }
    return t;
}
