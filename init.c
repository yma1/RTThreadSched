#include "main.h"

extern task *taskset[FUNCS];
extern pthread_attr_t attr;
extern exe_time;

void print_taskset()
{
  int index = 0;
  for (index = 0; index < FUNCS; index++)
  {
     if (taskset[index] != NULL)
     {
       printf("name: %s\n", taskset[index] -> name);
       printf("start: %ld\n", taskset[index] -> start);
       printf("deadline: %d\n", taskset[index] -> deadline);
       printf("period_type: %d\n", taskset[index] -> period_type);
       printf("period: %d\n", taskset[index] -> period);
       printf("period2: %d\n", taskset[index] -> period2);
       printf("cpu: %d\n", taskset[index] -> cpu);
       printf("jobs: %d\n", taskset[index] -> jobs);
       printf("state: %d\n", taskset[index] -> state);
       printf("completion: %d\n", taskset[index] -> completion);
       printf("miss_deadline: %d\n", taskset[index] -> miss_deadline);
     }
  }
}

void process_affinity(char *str_process_cpu)
{
  int process_cpu = atoi(str_process_cpu);
  if (process_cpu > CPUS || process_cpu < 0)
  {
    printf("error to set process affinity\n");
    exit(-1);
  }
  cpu_set_t process_cpu_set;
  CPU_ZERO(&process_cpu_set);
  CPU_SET(process_cpu, &process_cpu_set);
  sched_setaffinity(0, sizeof(cpu_set_t), &process_cpu_set);
}

//initialize the taskset
void init_conf(char *conf_line, int *index, long current_mill_sec)
{
  char *key = strtok(conf_line, ":");
  char *value = strtok(NULL, ":");
  value = strtok(value, "\n");

  if (!strcmp(key, "name"))
  {    
    for (*index = 0; *index < FUNCS; (*index)++)
    {
      if (!strcmp(value, task_names[*index]))
      {
        if (taskset[*index] != NULL)
        {
          printf("the task %s already exists\n", value);
          exit(-1);
        }
        taskset[*index] = (task *)malloc(sizeof(task));
        taskset[*index] -> name = (char *)malloc(sizeof(char) *32);
        strcpy(taskset[*index] -> name, value);
        taskset[*index] -> func = task_funcs[*index];
        taskset[*index] -> completion = 0;
        taskset[*index] -> miss_deadline = 0;
        taskset[*index] -> state = 1;
        taskset[*index] -> thread_lock = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
        pthread_mutex_init(taskset[*index] -> thread_lock, NULL);
        break;
      }
    }
    if (*index == FUNCS)
    {
      printf("cannot recognize %s\n", value);
      exit(-1);
    }
  }

  if (!strcmp(key, "start"))
    taskset[*index] -> start = (long)(atoi(value)) + current_mill_sec;
  if (!strcmp(key, "deadline"))
    taskset[*index] -> deadline = atoi(value);
  if (!strcmp(key, "cpu"))
    taskset[*index] -> cpu = atoi(value);
  if (!strcmp(key, "period_type"))
    taskset[*index] -> period_type = atoi(value);
  if (!strcmp(key, "jobs"))
    taskset[*index] -> jobs = atoi(value);
  if (!strcmp(key, "period"))
  {
    if (taskset[*index] -> period_type == 0)
    {
      taskset[*index] -> period = atoi(value);
      taskset[*index] -> period2 = -1;
    }
    else
    {
      taskset[*index] -> period = atoi(strtok(value, ","));
      taskset[*index] -> period2 = atoi(strtok(NULL, ","));
    }
  }
}

long get_current_mill_sec()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  long current_mill_sec = (long)((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
  return current_mill_sec;
}

void init(char *conf_file_path)
{
  srand(time(NULL));
 
  //all threads are detached
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  FILE *conf_file = fopen(conf_file_path, "r");
  if (conf_file == NULL)
  {
    printf("cannot open configuration file %s\n", conf_file_path);
    exit(-1);
  }
  char *read_buf = (char *)malloc(sizeof(char) * 128);
  char *key;
  char *value;
  int index = 0;
  long current_mill_sec;

  //set the process's affinity
  fgets(read_buf, 128, conf_file);
  while ((read_buf[0] == '#') || (read_buf[0] == '\n'))
    fgets(read_buf, 128, conf_file);
  key = strtok(read_buf, ":");
  value = strtok(NULL, ":");
  if (strcmp(key, "cpu"))
  {
    printf("the first configuration must be the process running core\n");
    exit(-1);
  }
  process_affinity(value);

  //set the execution time of the whole process
  fgets(read_buf, 128, conf_file);
  while ((read_buf[0] == '#') || (read_buf[0] == '\n'))
    fgets(read_buf, 128, conf_file);
  key = strtok(read_buf, ":");
  value = strtok(NULL, ":");
  if (strcmp(key, "exe"))
  {
    printf("the second configuration must be the execution time\n");
    exit(-1);
  }
  exe_time = atoi(value); 

  //initialize the taskset
  current_mill_sec = get_current_mill_sec();
  while (fgets(read_buf, 128, conf_file) != NULL)
  {
    if ((read_buf[0] == '#') || (read_buf[0] == '\n'))
      continue;
    init_conf(read_buf, &index, current_mill_sec);
  }
  
  fclose(conf_file);
  free(read_buf);
//  print_taskset();
}
