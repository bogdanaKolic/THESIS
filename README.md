This repository contains all the code and data related to my Bachelor Thesis: *Vulnerability Assessment for Rowhammer Attack*.

This project deals with several topics:
- precise and accurate sampling of programs for obtaining their hardware event traces
- simulation of the RH attack on three different systems and gathering their traces
- training and testing ML models for classifying the attack code based on its traces

The repository contains several folders:
- **PAPI_sampling** was used in the development of the method for periodic sampling of the code and reading its traces from the HPCs using PAPI; it is connected to the first part of the project. 
- **DATA_GENERATION** contains PAPI-instrumented code for generating data sets that are used for training and testing our ML models, as well as the code that is to be sampled with perf for the same purpose
- **benchmarks** contains the STREAM benchmark code; it is used fro creating LOAD condition on our systems
- **ML_models** has the jupiter-notebooks with the models we trained on the data sets from **DATA_GENERATION**
- **attack_timing** contains code for obtaining the minimal time it takes to *potentialy* cause bit flips on our machines
- **useful_system_information** provides the specification of the systems we work with and the events that can be counted on them using PAPI

More details can be found in the report.
