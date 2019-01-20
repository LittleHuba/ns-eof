# ns-eof
`NS-EOF` code for Team 1 in the Turbulent Flow Simulation on HPC Systems course at TUM (WiSe 18-19).

## Dependencies
The project depends on the `petsc`, `boost` and `hdf5` libraries. The minimum versions which compatibility has been tested are:
- `petsc` v.3.5.4
- `boost` v.1.61
- `hdf5` v.1.10.2
### On Cluster
On the `coolmuc` cluster `boost` and `hdf5` are readily available as modules: `boost/1.61_icc` and `hdf5/1.10.2-intel-impi`.
For using `petsc` instead you can point to our build, which is accessible by any user:
```
PETSC_DIR=/home/hpc/t1221/lu26xut/petsc-3.5.4
PETSC_ARCH=arch-linux2-c-debug
```
### On local workstation
The `hdf5` version available on Ubuntu does not link properly, leading to a failing build.
Our suggestion is to build the `hdf5` library from source using cmake with the following steps:
1. Dowload the latest version from [the official HDF5 download page](https://www.hdfgroup.org/downloads/hdf5/source-code/)
2. Unpack the archive and open a terminal inside the folder (e.g. `cd hdf5-1.10.4`)
3. Create a build folder (`mkdir Build`) and go into it (`cd Build`)
4. Configure the build with `ccmake ..`. In the configuration menu make sure to set `HDF5_ENABLE_PARALLEL = ON`.
5. Build and install with `make && sudo make install`

## Build
The project uses `CMake` for builds.
The easiest way to build on the cluster is to use one of our pre-made build scripts:
- If you have GNU's `gcc`, you can use: `./cmake-with-gcc.sh`
- If you have Intel's `icc`, you can use: `./cmake-with-icc.sh`

Both these scripts link the executable as `ns` in the current working directory, so the last one used overrides the previous ones.

In case you want to customize your build or need to point non-standard library paths, simply copy one of the scripts and adjust it to your needs.

## Enable XDMF output
The current version of `NS-EOF` supports XDMF output. To enable it, simply add an `xdmf` tag as follows:
```
<xdmf interval="0.1">outputFile_basename</xdmf>
```
The given basename should not contain any extension, as it will be used as a base name for both the `.xdmf` and `.h5` output files.

**NOTE**: remember to comment or delete any existing `vtk` tag from your configuration, **otherwise both outputs will be generated!**

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
