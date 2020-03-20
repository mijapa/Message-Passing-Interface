mpicc -o hello_proc_c hello_proc.c
mpiexec -machinefile ./node5_6 -np 2 ./hello_proc_c

##Zeus
plg-show-grants
squeue --start
scancel <jobID>
scontrol show job <jobID>
scontrol show node <nodes_list>

#interactive
srun --nodes=1 --ntasks=1 --exclusive --time=00:10:00 --partition=plgrid --account=plgmpatyk2020a --pty /bin/bash
module add plgrid/tools/openmpi

#batch
sbatch pi.sh 1 1
