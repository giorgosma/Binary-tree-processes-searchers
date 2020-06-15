#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef int bool;
#define TRUE 1
#define FALSE 0

typedef struct {
    long  	custid;
	char 	FirstName[20];
	char 	LastName[20];
	char	Street[20];
	int 	HouseID;
	char	City[20];
	char	postcode[6];
	float  	amount;
}Record;

typedef struct {
    double min;
    double max;
} Min_Max_Times;

typedef struct {
    Min_Max_Times sp_mg;
    Min_Max_Times se;
} Times;


int getfilesize(char *namefile);

int squares(int a, int x);
int calcthesum(char *height);
int calcthesize(char *filename, char *number, char *height);
int calc_starter_record(char *filename, char *number, char *height);
int read_from_bin_file(char *name, int start, int size, char *pattern);
int call_strstr(char *string, char *substring);
int check_for_substring(Record *recs, char *pattern);
void printstruct(Record *recs);
double store_the_min(Min_Max_Times *t, double st);
double store_the_max(Min_Max_Times *t, double st);
void Calc_The_Times(Min_Max_Times *t, double st1, double st2);
void Print_The_Times(Times t);
void Intitial_Times(Times *t);
int set_file_size(int start, int size, char bf[][4], int *flag);
int callTheExecutable(int height);
Times make_new_process(int height, char *argv[]);
