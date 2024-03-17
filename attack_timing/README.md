This section contains the attack code which performs 10000 activations of the aggressor row (reported to be the minimal observed number of activation that causes bit flips on DDR4 chips).
The code comes in two variants: one that corresponds to the code used for gathering event traces with perf and the other one that would be instrumented with PAPI.
The code that is sampled by perf is timed from the Terminal, using the `time` function. The results can be found in the file *dell_timing.txt*.
The other uses `gettimeofday()` function inside the code to time only the section of code that would be sampled by PAPI to count hardware-level events. The `get_diff()` function is taken from https://github.com/google/rowhammer-test. The timings can be obtained automatically by running the *time.sh* script. The results of running these programs on ASUS and LENOVO machines are stored in *timings.txt*.

