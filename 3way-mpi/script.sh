#!/bin/bash -l
#BATCH --mem=120G
#SBATCH --time=24:00:00
#SBATCH --job-name=gromacs
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=4
 
module purge
module load GROMACS
 
echo "Running Gromacs on $HOSTNAME"
 
export OMP_NUM_THREADS=1
time mpirun -x OMP_NUM_THREADS=1 mpi_solution mdrun -nsteps 500000 -ntomp 1 -v -deffnm 1ns -c 1ns.pdb -nice 0
 
echo "Finished run on $SLURM_NTASKS $HOSTNAME cores"
