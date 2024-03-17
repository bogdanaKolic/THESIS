This directory was used for developing a method for periodically reading HPCs with PAPI. Folder *sampling_methods* contains different code that we tested and that lead to our final solution.
The chosen approach is the file *timer_thread_sigblock.c*. We use it as template for sampling programs used for data generation.
We try to measure the accuracy of our approach by comparing the total count of events for the used programs when they are sampled and when they are not. The obtained data and the corresponding analysis can be found in the folder *error_statistics*.
The sampling programs in this directory that we run to perform the tests write out the traces obtained from HPCs to files - we store this files in folders *ASUS* and *LENOVO*, according to the machine on which we perform the tests.

The `get_diff()` function is taken from https://github.com/google/rowhammer-test.


Original attack/no-attack code that we use to derive our programs:
- attack is slightly modified code received from Loïc France
- attack_ntmp is received from Loïc France and modified according to http://seclab.cs.sunysb.edu/seclab/pubs/host16.pdf
- no_flushing and random are modified instances of the code received from Loïc France
- rowhammer_test originally comes from the https://github.com/google/rowhammer-test repository, but we add modifications
- double_sided_rowhammer comes from the https://github.com/google/rowhammer-test repository, its original creator is Thomas Dullien, and we also modify it
- stream is the tuned STREAM benchmark https://www.cs.virginia.edu/stream/