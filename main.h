#ifndef __USE_GNU
  #define __USE_GNU
#endif
#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <sched.h>
#include <pthread.h>

#define FUNCS 9
#define CPUS 4

void init(char *);
long get_current_mill_sec();

pthread_attr_t attr; //thread's state must be detached, otherwise, the ulimit will be hit
int exe_time; //the execution time of the whole process (in second)

// functions and data structure for taskset
typedef struct
{
  char *name; //task's name
  pthread_mutex_t *thread_lock; //protect the task's state
  pthread_t tid; //tid of thread
  void *(*func)(void *); //the functionality
  long start; //start time of next job in millisecond
  int deadline; //deadline in millisecond
  int cpu; //task's affinity
  int period_type; //period_type: fixed period or variant period
  int period; //period
  int period2; //only used when period is a random value
  int jobs; //number of jobs, -1 for infinity
  int state; //0: one job is running; 1: no job is running
  int completion; //number of completed jobs
  int miss_deadline; //number of jobs missing deadline
}task;

task *taskset[FUNCS];
static char *task_names[] = {"qsort1D", "qsort3D", "rad2deg", "deg2rad", "solveCubic", "susan", "bit_count", "bitcount", "ntbl_bitcount"};
void *qsort1D_thread(void *);
void *qsort3D_thread(void *);
void *rad2deg_thread(void *);
void *deg2rad_thread(void *);
void *solveCubic_thread(void *);
void *susan_thread(void *);
void *bit_count_thread(void *);
void *bitcount_thread(void *);
void *ntbl_bitcount_thread(void *);
void print_taskset(); //for test
static void *(*task_funcs[FUNCS])(void *) = 
{
  qsort1D_thread,
  qsort3D_thread,
  rad2deg_thread,
  deg2rad_thread,
  solveCubic_thread,
  susan_thread,
  bit_count_thread,
  bitcount_thread,
  ntbl_bitcount_thread
};

/*============================= for MiBench ============================== */
#define UNLIMIT //in qsort.c
#define MAXARRAY 60000 // in qsort.c. This number, if too large, will cause a seg. fault!!
#define PI  (4*atan(1)) //in basicmath.c
#define MAX_LOOPS 360 //in basicmath.c for rad2deg and deg2rad
#define MAX_LOOPS_BITCOUNT 75000//in bitcount.c

#ifndef PPC //in susan.c
typedef int        TOTAL_TYPE; /* this is faster for "int" but should be "float" for large d masks */
#else
typedef float      TOTAL_TYPE; /* for my PowerPC accelerator only */
#endif
#define  exit_error(IFB,IFC) { fprintf(stderr,IFB,IFC); exit(0); }
#define  FTOI(a) ( (a) < 0 ? ((int)(a-0.5)) : ((int)(a+0.5)) )
#define SEVEN_SUPP           /* size for non-max corner suppression; SEVEN_SUPP or FIVE_SUPP */
#define MAX_CORNERS   15000  /* max corners per frame */
typedef  unsigned char uchar;
typedef  struct {int x,y,info, dx, dy, I;} CORNER_LIST[MAX_CORNERS];

int qsort3D(char *); //in qsort.c
int qsort1D(char *); //in qsort.c

double rad2deg(double); //in basicmath.c
double deg2rad(double); //in basicmath.c
void solveCubic(double, double, double, double, int *, double *); //in basicmath.c

int bit_count(long); //in bitcount.c
int bitcount(long); //in bitcount.c
int ntbl_bitcount(long int); //in bitcount.c

void susan(int, char **); //in susan.c


