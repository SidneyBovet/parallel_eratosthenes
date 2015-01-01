# Parallel Eratosthenes

## Project
The goal of this project is to analyse the performance and power consumption of a Beowulf cluster made ofRaspberry Pi, a low-cost, credit card-sized computer. It uses a distributed implementation of the famous Sieve of Eratosthenes. 

## Building
Simply run `make`, `make seq`, `make par`, or `make netstats` in order to respectively build everything, the sequential or parallel implementation of Eratosthenes' Sieve or the network testing tool.
##### Building Requirements
The serial implementation should compile on almost every machine. An implementation of MPI (such as [MPICH](http://www.mpich.org/downloads/)) is required to run the parallel implementation and the network statistics tool.
##### The `make_and_scp.sh` script
This script compiles all the executables and sends them via ssh's `scp` to all the machines in the `machines` file in order for the machines to later run these executables in an MPI environment.

## Author
Sidney Bovet - December 2014