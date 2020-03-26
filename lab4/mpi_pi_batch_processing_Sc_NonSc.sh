#!/bin/bash -l
#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --exclusive
#SBATCH --time=00:01:00
#SBATCH --mem-per-cpu=100MB
#SBATCH --partition=plgrid
#SBATCH --account=plgmpatyk2020a
#SBATCH --mail-type=ALL
#SBATCH --mail-user=michaljanpatyk@gmail.com

module add plgrid/tools/openmpi
mpicc -std=c11 -o reduce_pi reduce_pi.c

for ((i=1; i<=$1; i++)); do
    echo "$i"
    mpiexec -np "$i" ./reduce_pi "$2"
done

for ((i=1; i<=$1; i++)); do
    echo "$i"
    ((k = $(expr $2 \* $i)))
    mpiexec -np "$i" ./reduce_pi "$k"
done
