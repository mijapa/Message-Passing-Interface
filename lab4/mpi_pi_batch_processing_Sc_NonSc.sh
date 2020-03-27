#!/bin/bash -l
#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --exclusive
#SBATCH --time=01:00:00
#SBATCH --constraint="intel"
#SBATCH --mem-per-cpu=100MB
#SBATCH --partition=plgrid
#SBATCH --account=plgmpatyk2020a
#SBATCH --mail-type=ALL
#SBATCH --mail-user=michaljanpatyk@gmail.com

module add plgrid/tools/openmpi
mpicc -std=c11 -o mpi_pi_reduce mpi_pi_reduce.c

echo "n,p,s,pi,m" > non_scalable.csv
for m in 10^7 10^8 10^9; do
  for ((i=1; i<=$1; i++)); do
    echo "$i"
    for ((j=1; j<=$3; j++)); do
      mpiexec -np "$i" ./mpi_pi_reduce "$m" "non_scalable.csv" "$m"
    done
  done
done

echo "n,p,s,pi,m" > scalable.csv
for m in 10^7 10^8 10^9; do
  for ((i=1; i<=$1; i++)); do
    echo "$i"
    for ((j=1; j<=$3; j++)); do
      ((k = $(expr $m \* $i)))
      mpiexec -np "$i" ./mpi_pi_reduce "$k" "scalable.csv" "$m"
    done
  done
done
