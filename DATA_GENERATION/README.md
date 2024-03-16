The code in this folder serves the purpose of generating the data sets that are used for training and testing our machine learning models.<br>
This can either be done manually or by using the appropriate script. The scripts for data generation with load do not introduce load themselves (this should still be done manually), 
but the amount of load should be specified by altering the `load` field. Before a run, the field `laptop` should also be modified according to the machine.
The Makefile is also provided. <br>
A file *code_type_papi.c* is used for sampling with PAPI on ASUS and LENOVO machines, and *code_type_perf.c* for sampling with perf on DELL machine.<br>
### The code for the attack comes in several variants:
  - `attack` is the code obtained from the authors of [Vulnerability Assessment
of the Rowhammer Attack Using Machine Learning and the gem5 Simulator -Work in Progress](https://hal.umontpellier.fr/hal-03196090)
  - `attack_ntmp` modifies the hammerinf function from `attack` to use non-temporal **MOVNTI** instruction 
  - `rowhammer-test` is simplified code from [google/rowhammer-test](https://github.com/google/rowhammer-test) and the number next to its name signifies how many aggressor rows are used for hammering
  - `double-sided-rowhammer` is the double-sided version of the attack from the [same repository](https://github.com/google/rowhammer-test)

### The no-attack code:
  - The `no_flushing` is derived from the `attack` code by ommitting the **CLFLUSH** instruction
  - The `random` code uses the same memory (buffer) initialization as `attack`, but it makes random accesses on the buffer instead of focusing on two aggressor addresses
  - `stream` is the [STREAM benchmark](https://www.cs.virginia.edu/stream/) with modified parameters `STREAM_ARRAY_SIZE` and `NTIMES`
<br>

The frequency of the sampling with PAPI is controlled by the `PERIOD` parameter (given in microseconds). To change it, alter this parameter directly in the code.<br>

Each machine used for data generation has a separate folder to store the generate data and related files. In particular, one can find files containing data generated from each code in both LOAD and NO LOAD conditions, python scripts for collecting all the data in one file or processing it for easier plotting. Graphs used in the report are also present.
In addition, data on our DELL machine obtained with perf had to be preprocessed to have the same form as data we obtain using papi. The folder `raw_data` contains the originial perf-generated data and a python script for processing it. 

Script *merge.py* in this directory can be used for combining the samples from all three files into one file.
