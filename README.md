# ns-eof
`NS-EOF` code for Team 1 in the Turbulent Flow Simulation on HPC Systems course at TUM (WiSe 18-19).

## Build
The project uses `CMake` for builds.
The easiest way to build is to use one of our pre-made build scripts:
- If you have GNU's `gcc`, you can use: `./cmake-with-gcc.sh`
- If you have Intel's `icc`, you can use: `./cmake-with-icc.sh`

Both these scripts link the executable as `ns` in the current working directory, so the last one used overrides the previous ones.

In case you want to customize your build or need to point non-standard library paths, simply copy one of the scripts and adjust it to your needs.

## Run
Running `ns` requires MPI, even for a single process run.

To run `ns` simply use:
```
mpirun -np <N> ./ns <conf_file_name>.xml
```
where:
- `<N>` is the desired number of MPI processes
- `<conf_file_name>.xml` is the path to a valid configuration file

Enjoy! :)
