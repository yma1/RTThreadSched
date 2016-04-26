#include "main.h"

int set_thread_affinity(int thread_cpu)
{
  cpu_set_t thread_cpu_set;
  CPU_ZERO(&thread_cpu_set);
  CPU_SET(thread_cpu, &thread_cpu_set);
  pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &thread_cpu_set);

  CPU_ZERO(&thread_cpu_set);
  pthread_getaffinity_np(pthread_self(), sizeof(cpu_set_t), &thread_cpu_set);
  if (!CPU_ISSET(thread_cpu, &thread_cpu_set))
    return -1;
  if (1 != CPU_COUNT(&thread_cpu_set))
    return -1;
  return 0;
}

void *qsort1D_thread(void *args)
{
  task *running_thread = (task *)args;
  set_thread_affinity(running_thread -> cpu);

  qsort1D("./benchmark_input/qsort1D.dat");

  pthread_mutex_lock(running_thread -> thread_lock);
  running_thread -> state = 1; //this job is already finished
  pthread_mutex_unlock(running_thread -> thread_lock);
  return ((void *)0);
}

void *qsort3D_thread(void *args)
{
  task *running_thread = (task *)args;
  set_thread_affinity(running_thread -> cpu);

  qsort3D("./benchmark_input/qsort3D.dat");
  
  pthread_mutex_lock(running_thread -> thread_lock);
  running_thread -> state = 1;
  pthread_mutex_unlock(running_thread -> thread_lock);
  return ((void *)0);
}

void *rad2deg_thread(void *args)
{
  task *running_thread = (task *)args;
  set_thread_affinity(running_thread -> cpu);

  rad2deg(90);

  pthread_mutex_lock(running_thread -> thread_lock);
  running_thread -> state = 1;
  pthread_mutex_unlock(running_thread -> thread_lock);
  return ((void *)0);
}

void *deg2rad_thread(void *args)
{
  task *running_thread = (task *)args;
  set_thread_affinity(running_thread -> cpu);

  deg2rad(90);

  pthread_mutex_lock(running_thread -> thread_lock);
  running_thread -> state = 1;
  pthread_mutex_unlock(running_thread -> thread_lock);
  return ((void *)0);
}

void *solveCubic_thread(void *args)
{
  task *running_thread = (task *)args;
  set_thread_affinity(running_thread -> cpu);

  double a = 1.0, b = -13.7, c = 1.0, d = -35.0;
  int solutions;
  double  x[3];

  solveCubic(a, b, c, d, &solutions, x);  

  pthread_mutex_lock(running_thread -> thread_lock);
  running_thread -> state = 1;
  pthread_mutex_unlock(running_thread -> thread_lock);
  return ((void *)0);
}

void *susan_thread(void *args)
{
  task *running_thread = (task *)args;
  set_thread_affinity(running_thread -> cpu);

  int argc_susan = 4;
  char *argv_susan[] = {"susan", "./benchmark_input/susan.pgm", "/dev/null", "-s"};
  susan(argc_susan, argv_susan);

  pthread_mutex_lock(running_thread -> thread_lock);
  running_thread -> state = 1;
  pthread_mutex_unlock(running_thread -> thread_lock);
  return ((void *)0);
}

void *bit_count_thread(void *args)
{
  task *running_thread = (task *)args;
  set_thread_affinity(running_thread -> cpu);

  bit_count(1);
 
  pthread_mutex_lock(running_thread -> thread_lock);
  running_thread -> state = 1;
  pthread_mutex_unlock(running_thread -> thread_lock);
  return ((void *)0);
}

void *bitcount_thread(void *args)
{
  task *running_thread = (task *)args;
  set_thread_affinity(running_thread -> cpu);

  bitcount(1);  

  pthread_mutex_lock(running_thread -> thread_lock);
  running_thread -> state = 1;
  pthread_mutex_unlock(running_thread -> thread_lock);
  return ((void *)0);
}

void *ntbl_bitcount_thread(void *args)
{
  task *running_thread = (task *)args;
  set_thread_affinity(running_thread -> cpu);

  ntbl_bitcount(1);

  pthread_mutex_lock(running_thread -> thread_lock);
  running_thread -> state = 1;
  pthread_mutex_unlock(running_thread -> thread_lock);
  return ((void *)0);
}



