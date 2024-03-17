This directory was used for developing a method for periodically reading HPCs with PAPI. Folder *sampling_methods* contains different code that we tested and that lead to our final solution.
The chosen approach is the file *timer_thread_sigblock.c*. We use it as template for sampling programs used for data generation.
We try to measure the accuracy of our approach by comparing the total count of events for the used programs when they are sampled and when they are not. The obtained data and the corresponding analysis can be found in the folder *error_statistics*.
