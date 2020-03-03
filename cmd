mpicc -o hello_proc_c hello_proc.c
mpiexec -machinefile ./node5_6 -np 2 ./hello_proc_c