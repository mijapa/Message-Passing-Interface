#!/bin/bash -l
#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --exclusive
#SBATCH --time=00:01:00
#SBATCH --constraint="intel"
#SBATCH --mem-per-cpu=100MB
#SBATCH --partition=plgrid
#SBATCH --account=plgmpatyk2020a
#SBATCH --mail-type=ALL
#SBATCH --mail-user=michaljanpatyk@gmail.com

module add plgrid/tools/openmpi
mpicc -std=c11 -o reduce_pi reduce_pi.c

echo "n, p, s, pi" > non_scalable.csv
for ((i=1; i<=$1; i++)); do
  echo "$i"
  for ((j=1; j<=$3; j++)); do
      mpiexec -np "$i" ./reduce_pi "$2" "non_scalable.csv"
  done
done

echo "n, p, s, pi" > scalable.csv
for ((i=1; i<=$1; i++)); do
  echo "$i"
  for ((j=1; j<=$3; j++)); do
    ((k = $(expr $2 \* $i)))
    mpiexec -np "$i" ./reduce_pi "$k" "scalable.csv"
  done
done
