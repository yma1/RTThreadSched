# RTThreadSched
1. How to install
  ./make


2. How to run
  ./SCHED -c configuration_file [-o output_file] [-t temperature_file]
  
  The configuration_file specifies how many tasks will run, what's the period, and other related parameters. You can check "conf_sample.txt" to get more details. Right now, it only supports 9 tasks from MiBench: rad2deg, deg2rad, solveCubic, qsort1D, qsort3D, susan, bitcount, bit_count, and ntbl_bitcount.
  
  The output_file saves the real-time performance of tasks. If it is not specified, the default is ./output
  
  The temperature_file saves the captured temperature. If it is not specified, the default is ./temperature


3. Misc.
  a) The time unit of task's arrival time, job's release time, period ,and deadline are millisecond
  
  b) Tasks' jobs are threads, and they are triggered by the main thread

  c) If a job misses its deadline, it will be terminated immediately (Why not keep it running? Because tasks are not thread-safe)

  c) At each 0.1 millsecond, the main thread scans and checks all tasks/threads, and then controls them (releases a new job, or terminates an existing job)

  d) The dynamic CPU core management (e.g. "cpuquiet" for Tegra) must be disenabled
  
