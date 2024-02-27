This repository contains files used for generation of hardware event traces from the Rowhammer attack code and benchmarks.

- **attack_no_attack_code** contains variations of the RH attack and non-malicious code
- **PAPI_codes** contains  instrumented code from *attack_no_attack_code* so that we can obtain its hardware event traces with the PAPI library
- **PAPI_sampling** is used for testing sampling methods of the PAPI generated data
- **benchmarks** contains the STREAM benchmark code
