# Hardware Performance Counters, the PAPI library and Sampling

Files in this directory represent attempts to obtain accurate periodic readings from the HPCs using functions from the ***PAPI*** library.<br>

## Code used for sampling

#### 1. add_native and handle_error.c
 - template files for using low-level **PAPI** in order to count native events.
 - **handle_error.c** contains a function for interpreting papi error codes

#### 2. timer_interrupt
- main computations are periodically interrupted using signals (`SIGVTALRM`) and a virtual interval timer (`ITIMER_VIRTUAL`)
- `PAPI_read` called at each interrupt to read the counters
<details>
 <summary> timer_handler </summary>
 <pre>
  void timer_handler(int sig){
    /* The function called at the interrupt */ <br>
    /* Read the counters */
    if (PAPI_read(EventSet, values) != PAPI_OK){
            printf("read error\n");
            handle_error(1);
        } <br>
    /* Print the result */
    printf("result (%s): %lld\n", EVENT, values[0]);
    }</pre>
</details>

#### 3. timer_stop_interrupt
- main computations are periodically interrupted using signals (`SIGVTALRM`) and a virtual interval timer (`ITIMER_VIRTUAL`)
- `PAPI_stop` called at each interrupt to stop and read the counters, then `PAPI_start` is used to reastart them
<details>
 <summary> timer_handler </summary>
 <pre>
  void timer_handler(int sig){
    /* The function called at the interrupt */ <br>
    /* Read the counters by stopping them */
    if (PAPI_stop(EventSet, values) != PAPI_OK){
            printf("stop error\n");
            handle_error(1);
        } <br>
    /* Print the result */
    printf("result (%s): %lld\n", EVENT, values[0]); <br>
    /* Add it to the count */
    sum += values[0]; <br>
    /* Restart the counters */
    if (PAPI_start(EventSet) != PAPI_OK){
            printf("start error\n");
            handle_error(1);
        }
   }
</pre>
</details>

#### 4. timer_thread

- test whether putting a timer in a separate thread improves the accuracy when periodically reading the HPCs using interval timer interrupts
- count the events from both threads, change the code in one, then the other, and observe the changes in the event count
- motivation is avoiding counting the events from the interrupts [***Hardware Interrupt Extra Counts***, page 4, section 3.1.1](https://icl.utk.edu/files/publications/2010/icl-utk-451-2010.pdf)

#### 5. timer_thread_sigblock

- further implement signal blocking to ensure that the interrupts are not handled (reading the counters) in the thread where the sampled code is running
- use `gettimeofday` and `ITIMER_REAL` (with the signal `SIGALRM`) to check that the interrupts are handled at equal intervals

#### 6. sampling_overflow

- perform sampling by using the `PAPI_overflow` function
- the interrupts occur when the threshold number of a specific counted event (`UNHALTED_CORE_CYCLES`) is reached
<details>
 <summary> handler </summary>
 <pre>
    /* Find the code for the native event */
    retval = PAPI_event_name_to_code(CYCLES, &count);
    if (retval != PAPI_OK){
        printf("event error");
        handle_error(retval);
    } <br>
    /* Add it to the eventset */
    retval = PAPI_add_event(EventSet, count);
    if (retval != PAPI_OK) handle_error(retval); <br>
    /* Set overflow */
    retval = PAPI_overflow(EventSet, count, PERIOD, 0, handler);
    if( retval != PAPI_OK){
        printf("overflow error\n");
        handle_error(retval);
    }
 </pre>
</details>

## Benchmarks and accuracy testing

#### 1. total_count_benchmark

- this benchmark is used to count the total number of the event occurrences that are sampled with
  - timer_interrupt
  - timer_stop_interrupt
  - timer_thread
  - timer_thread_sigblock
<details>
 <summary> Code snippet </summary>
 <pre>
  /* Start counting */
    if (PAPI_start(EventSet) != PAPI_OK){
        printf("start error");
        handle_error(1);
    }<br>
    /* Do some computation here */ 
    int j = 0;
    for(int i = 0; i<10000000; i++){
        j = j+i;
    }<br>
    PAPI_read(EventSet, values);<br>
    printf("result (%s): %lld\n", EVENT, values[0]);<br>
    if (PAPI_stop(EventSet, values) != PAPI_OK)
        handle_error(1);<br>
    printf(" end result (%s): %lld\n", EVENT, values[0]);
 </pre>
</details>

#### 2. total_count_overflow_benchmark

- this benchmark is used to count the total number of event occurrences sampled with sampling_overflow,
  as well as the total number of cycles used for sampling (`UNHALTED_CORE_CYCLES`)

### Results and conclusions

#### Overflow sampling

- 11th Gen Intel© Core™ i5-1135G7 @ 2.40GHz × 4 processor >> the number of cycles?
- Options when handling overflow:
  - do nothing
  - measure time passed
  - stop/start counters to read them
  - call `PAPI_read`
  - call `PAPI_read` and print the values
- Does not *seem* to count the events accurately compared to the benchmark results

#### Timer interruptions

- precision: microseconds
- timer_thread_sigblock *seems* to produce the most accurate counts (comparing the total number of counts with the benchmark) and has periodic interruptions

## Questions

### 1. Selecting the correct timer

#### [FROM THE MAN PAGES:](https://linux.die.net/man/2/setitimer)

**ITIMER_REAL**
- decrements in real time, and delivers SIGALRM upon expiration.

**ITIMER_VIRTUAL**
- decrements only when the process is executing, and delivers SIGVTALRM upon expiration.

**ITIMER_PROF**
- decrements both when the process executes and when the system is executing on behalf of the process. Coupled with ITIMER_VIRTUAL, this timer is usually used to profile the time spent by the application in user and kernel space. SIGPROF is delivered upon expiration. 

### 2. Scientifically correct comparison of the results

### 3. Which event to count to generate the overflow

#### [FROM THE MAN PAGES:](https://man7.org/linux/man-pages/man2/perf_event_open.2.html)

**PERF_COUNT_HW_CPU_CYCLES**
- Total cycles.  Be wary of what happens during CPU frequency scaling.

**PERF_COUNT_HW_REF_CPU_CYCLES**
- (since Linux 3.3)
- Total cycles; not affected by CPU frequency scaling.
