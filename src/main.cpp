#include <cstdlib>
#include <cstdio>
#include <iostream>
#include "Configuration.h"
#include "Simulation.h"
#include "parallelManagers/PetscParallelConfiguration.h"
#include "MeshsizeFactory.h"
#include "TurbulentSimulation.h"
#include <iomanip>

int main(int argc, char *argv[]) {

    // Parallelization related. Initialize and identify
    // ---------------------------------------------------
    int rank;   // This processor's identifier
    int nproc;  // Number of processors in the group
    PetscInitialize(&argc, &argv, "petsc_commandline_arg", PETSC_NULL);
    MPI_Comm_size(PETSC_COMM_WORLD, &nproc);
    MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
    std::cout << "Rank: " << rank << ", Nproc: " << nproc << std::endl;
    //----------------------------------------------------


    // read configuration and store information in parameters object
    Configuration configuration(argv[1]);
    Parameters parameters;
    configuration.loadParameters(parameters);
    PetscParallelConfiguration parallelConfiguration(parameters);
    MeshsizeFactory::getInstance().initMeshsize(parameters);
    FlowField *flowField = nullptr;
    Simulation *simulation = nullptr;

#ifdef DEBUG
    std::cout << "Processor " << parameters.parallel.rank << " with index ";
    std::cout << parameters.parallel.indices[0] << ",";
    std::cout << parameters.parallel.indices[1] << ",";
    std::cout << parameters.parallel.indices[2];
    std::cout <<    " is computing the size of its subdomain and obtains ";
    std::cout << parameters.parallel.localSize[0] << ", ";
    std::cout << parameters.parallel.localSize[1] << " and ";
    std::cout << parameters.parallel.localSize[2];
    std::cout << ". Left neighbour: " << parameters.parallel.leftNb;
    std::cout << ", right neighbour: " << parameters.parallel.rightNb;
    std::cout << std::endl;
    std::cout << "Min. meshsizes: " << parameters.meshsize->getDxMin() << ", " << parameters.meshsize->getDyMin() << ", " << parameters.meshsize->getDzMin() << std::endl;
#endif

    // initialise simulation
    flowField = new FlowField(parameters);
    if (parameters.simulation.type == "turbulence") {
        if (rank == 0) { std::cout << "Start turbulent DNS simulation in " << parameters.geometry.dim << "D" << std::endl; }
        if (flowField == nullptr) {handleError(1, "flowField==NULL!"); }
        simulation = new TurbulentSimulation(parameters, *flowField);
    } else if (parameters.simulation.type == "dns") {
        if (rank == 0) { std::cout << "Start DNS simulation in " << parameters.geometry.dim << "D" << std::endl; }
        if (flowField == nullptr) {handleError(1, "flowField==NULL!"); }
        simulation = new Simulation(parameters, *flowField);
    } else {
        handleError(1, "Unknown simulation type! Currently supported: dns, turbulence");
    }
    // call initialization of simulation (initialize flow field)
    if (simulation == nullptr) {handleError(1, "simulation==NULL!"); }
    simulation->initializeFlowField();
    //flowField->getFlags().show();

    FLOAT time = 0.0;
    FLOAT timeStdOut = parameters.stdOut.interval;
    FLOAT timeVTKOut = parameters.vtk.interval;
    FLOAT timeXDMFOut = parameters.xdmf.interval;
    int xdmfTimestep = 0;
    int timeSteps = 0;

    // plot initial state
//    simulation->plotVTK(timeSteps);
    simulation->plotXDMF(xdmfTimestep);
    xdmfTimestep++;

    // time loop
    while (time < parameters.simulation.finalTime) {

        simulation->solveTimestep();

        time += parameters.timestep.dt;

        // std-out: terminal info
        if ((rank == 0) && (timeStdOut <= time)) {
            std::cout << "Current time: " << time << "\ttimestep: " << parameters.timestep.dt << std::endl;
            timeStdOut += parameters.stdOut.interval;
        }

        timeSteps++;

        // trigger VTK output
//        if (timeVTKOut <= time) {
//            simulation->plotVTK(timeSteps);
//            timeVTKOut += parameters.vtk.interval;
//        }

        if (timeXDMFOut <= time) {
            simulation->plotXDMF(xdmfTimestep);
            timeXDMFOut += parameters.xdmf.interval;
            xdmfTimestep++;
        }

//        PetscFinalize();
//        exit(0);
    }

    // plot final output
//    simulation->plotVTK(timeSteps);
    simulation->plotXDMF(xdmfTimestep);

    delete simulation;
    simulation = nullptr;
    delete flowField;
    flowField = nullptr;

    PetscFinalize();
}
