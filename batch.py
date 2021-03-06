#!/bin/bash
#SBATCH -o /home/hpc/t1221/lu26xut/parallel/output/Cavity_Strong_2x2x2.out
#SBATCH -D /home/hpc/t1221/lu26xut/parallel/ns-eof #starting directory
#SBATCH -J strong_2x2x2 #name shown in squeue
#SBATCH --get-user-env #load user environment settings
#SBATCH --clusters=mpp2
#SBATCH --ntasks=8

#SBATCH --mail-type=all
#SBATCH --mail-user=ga53vuw@tum.de
#SBATCH --export=NONE
#SBATCH --time=04:00:00 #runtime
source /etc/profile.d/modules.sh #load modules defined in here

for i in 'seq 1 5'; do
    time mpiexec -np 8 ./ns ./conf_cavity.xml
done

