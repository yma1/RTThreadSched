#include "main.h"

extern task *taskset[FUNCS];
extern pthread_attr_t attr;
extern exe_time;

int release_thread(long current_time)
{
  int index;
  int err;

  for (index = 0; index < FUNCS; index++)
  {
    //if a task a) is configured; b) meets the release time; c) its previous job has completed
    //then release a new job (create a new thread)
    if ((taskset[index] != NULL) && (taskset[index] -> start == current_time) && (taskset[index] -> state == 1))
    {
      taskset[index] -> state = 0;
      err = pthread_create(&(taskset[index] -> tid), &attr, taskset[index] -> func, taskset[index]);

      if (err != 0)
      {
        printf("error in thread generation: %d\n", err);
        exit(-1);
      }
    }
  }

  return 0;
}

void check_thread(long current_time)
{

  int index;
  for (index = 0; index < FUNCS; index++)
  {
    // if a job reaches its deadline
    if ((taskset[index] != NULL) && 
        (taskset[index] -> deadline + taskset[index] -> start == current_time))
    {
      taskset[index] -> jobs -= 1;
      if (taskset[index] -> jobs != 0)
      {
        // update the next release time
        if (taskset[index] -> period_type == 0)
          taskset[index] -> start += taskset[index] -> period;
        else
          taskset[index] -> start += rand() % (taskset[index] -> period2
                                  - taskset[index] -> period)
                                  + taskset[index] -> period;
      }
      else
        taskset[index] -> start = -1; // all jobs are already completed

      //if this job has finished already
      if (taskset[index] -> state == 1)
      {
        taskset[index] -> completion += 1;
      }
      else //if this job has not finished yet
      {
        taskset[index] -> miss_deadline += 1;
        pthread_mutex_lock(taskset[index] -> thread_lock);
        taskset[index] -> state = 1;
        pthread_mutex_unlock(taskset[index] -> thread_lock);
        // terminate corresponding pthread
        pthread_cancel(taskset[index] -> tid);
      }
      
    }
  }

}

//writ the task's real-time performance into a file
void output_result(char *output_file_path)
{
  int index = 0;
  FILE *output_file = fopen(output_file_path, "w");
  for (index = 0; index < FUNCS; index++)
  {
    if (taskset[index] != NULL)
    {
      fprintf(output_file, "name: %s\n", taskset[index] -> name);
      fprintf(output_file, "completion: %d\n", taskset[index] -> completion);
      fprintf(output_file, "miss_deadline: %d\n", taskset[index] -> miss_deadline);
      fprintf(output_file, "deadline miss ratio: %f\n\n\n\n", 
                                   (float)taskset[index] -> miss_deadline 
                                   / (taskset[index] -> miss_deadline + taskset[index] -> completion));
     
      pthread_mutex_destroy(taskset[index] -> thread_lock);
      free(taskset[index]);
      taskset[index] = NULL;
    }
  }
  fclose(output_file);
}

//write the captured temperature into a file
void temperature_collect(FILE *temperature_file)
{ 
  float temperature;
  char *buff_t = (char *)malloc(sizeof(char) * 16);

  FILE *temperature_sensor = fopen("/sys/devices/virtual/thermal/thermal_zone0/temp", "r");
  fgets(buff_t, 16, temperature_sensor);
  temperature = atoi(buff_t) / (float)1000;
  fprintf(temperature_file, "%0.1f\t", temperature);
  fclose(temperature_sensor);

  temperature_sensor = fopen("/sys/devices/virtual/thermal/thermal_zone1/temp", "r");
  fgets(buff_t, 16, temperature_sensor);
  temperature = atoi(buff_t) / (float)1000;
  fprintf(temperature_file, "%0.1f\t", temperature);
  fclose(temperature_sensor);
  
  temperature_sensor = fopen("/sys/devices/virtual/thermal/thermal_zone2/temp", "r");
  fgets(buff_t, 16, temperature_sensor);
  temperature = atoi(buff_t) / (float)1000;
  fprintf(temperature_file, "%0.1f\n", temperature);
  fclose(temperature_sensor);
  free(buff_t);
}

int main(int argc, char **argv)
{
  // save the output and captured temperature at files output_file_path and temperature_file_path
  int argv_scan = 1;
  char *conf_file_path = NULL;
  char *temperature_file_path = NULL;
  char *output_file_path = NULL;
  while (argv_scan < argc)
  {
    if (!strcmp(argv[argv_scan], "-o"))
      output_file_path = argv[argv_scan + 1];
    if (!strcmp(argv[argv_scan], "-c"))
      conf_file_path = argv[argv_scan + 1];
    if (!strcmp(argv[argv_scan], "-t"))
      temperature_file_path = argv[argv_scan + 1];
    argv_scan += 2;
  }
  
  if (conf_file_path == NULL)
  {
    printf("no configuration file\n");
    exit(-1);
  }
  if (output_file_path == NULL)
    output_file_path = "./output";
  if (temperature_file_path == NULL)
    temperature_file_path = "./temperature";  

  // initialize taskset and other parameters 
  init(conf_file_path);

  // write the first line of temperature file
  FILE *temperature_file = fopen(temperature_file_path, "w");
  if (temperature_file == NULL)
  {
    printf("cannot write temperature to file %s\n", temperature_file_path);
    exit(-1);
  }
  fprintf(temperature_file, "CPU\tGPU\tMEM\n");

  long current_time = get_current_mill_sec();
  long sys_start_time = get_current_mill_sec();
  long time_in_sec = 1000;
  do
  {
    release_thread(current_time); //check whether it is the time to release job(s)
    check_thread(current_time); //check whether job(s) reach to the deadline
    usleep(100); //sleep 0.1 millisecond
    current_time = get_current_mill_sec();
    
    //capture and save temperature at each second
    if (current_time - sys_start_time >= time_in_sec)
    {
      temperature_collect(temperature_file);
      time_in_sec += 1000;
    }
  }while((current_time - sys_start_time) < exe_time * 1000);

  output_result(output_file_path);
  fclose(temperature_file);
  return 0;
}

















