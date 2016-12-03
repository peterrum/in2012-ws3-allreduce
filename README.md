# Objectives

Basic implementations of MPI_Reduce and MPI_Allreduce. Changes:

# Running

After building with CMake run the program with the following command:

``` bash
$ mpirun -np 4 ./main
``` 
with 
* 4 being the number of MPI-processes (np) - it has to be a power of 2. 

# Algorithm
![alt tag](images/reduce.png "Description goes here")

For an implementation for processor counts not equal power of 2 (asymetrical tree) see:
* [https://github.com/peterrum/in2012-ws3-allreduce-sequential](https://github.com/peterrum/in2012-ws3-allreduce-sequential)
* [https://github.com/peterrum/in2012-ws3-allreduce-asymetricaltree](https://github.com/peterrum/in2012-ws3-allreduce-asymetricaltree)

