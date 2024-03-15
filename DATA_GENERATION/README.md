The code in this folder serves the purpose of generating the data sets that are used for training and testing our machine learning models.<br>
This can either be done manually or by using the appropriate script. The scripts for data generation with load do not introduce load themselves (this should still be done manually), 
but the amount of load should be specified by altering the `load` field. Before a run, the field `laptop` should also be modified according to the machine.
The Makefile is also provided. <br>
A file *code_type_papi.c* is used for sampling with PAPI on ASUS and LENOVO machines, and *code_type_perf.c* for sampling with perf on DELL machine.<br>
The code for the attack comes in several variants:
  - The `attack` is the code obtained from the authors of ...
  - The `attack_ntmp` is modified `attack` which uses non-temporal instruction **MOVNTI**
  - The `rowhammer-test` is simplified code from ... and the number next to its name signifies how many aggressor rows are used for hammering
  - The `double-sided-rowhammer` is the double-sided version of the attack from the same repository
<br>
The no-attack code:
  - The `no_flushing` is the `attack` code without the **CLFLUSH** instruction
  - The `random` code uses the same memory initialization as `attack`, but it makes random accesses on the buffer instead of focusing on two aggressor addresses
  - `stream` is the STREAM benchmark with modified parameters `STREAM_ARRAY_SIZE` and `NTIMES`
<br>
Script *merge.py*   is used for combining the samples into one file.
