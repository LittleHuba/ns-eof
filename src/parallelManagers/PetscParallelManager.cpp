//
// Created by tommaso on 22/11/18.
//

#include "PetscParallelManager.h"

PetscParallelManager::PetscParallelManager(const Parameters &parameters, FlowField &flowField)
        : parameters(parameters), flowField(flowField),
        pressureSendStencil(parameters), pressureRecvStencil(parameters),
        velocitySendStencil(parameters), velocityRecvStencil(parameters),
        pressureSendIterator(flowField, parameters, pressureSendStencil, 2, -1), //todo: here check if these iterators need any offset
        pressureRecvIterator(flowField, parameters, pressureRecvStencil, 2, -1),
        velocitySendIterator(flowField, parameters, velocitySendStencil, 1, 0),
        velocityRecvIterator(flowField, parameters, velocityRecvStencil, 1, 0)
{
    initializeSizes(parameters, flowField);
    rank = parameters.parallel.rank;
    numberDirection = parameters.geometry.dim *2;
    initializeNeighbours(parameters);
    allocateBuffers();
    pressureSendStencil.initializeBuffers(pressureSendBuffers);
    pressureRecvStencil.initializeBuffers(pressureRecvBuffers);
    velocitySendStencil.initializeBuffers(velocitySendBuffers);
    velocityRecvStencil.initializeBuffers(velocityRecvBuffers);
}

void PetscParallelManager::initializeNeighbours(const Parameters &parameters)
{
    neighbours[LEFT] = parameters.parallel.leftNb;
    neighbours[RIGHT] = parameters.parallel.rightNb;
    neighbours[BOTTOM] = parameters.parallel.bottomNb;
    neighbours[TOP] = parameters.parallel.topNb;
    neighbours[FRONT] = parameters.parallel.frontNb;
    neighbours[BACK] = parameters.parallel.backNb;
}

PetscParallelManager::~PetscParallelManager()
{
    for(int d=0; d<numberDirection; ++d){
        delete[] pressureSendBuffers[d];
        delete[] pressureRecvBuffers[d];
        delete[] velocitySendBuffers[d];
        delete[] velocityRecvBuffers[d];
    }
    delete[] pressureSendBuffers;
    delete[] pressureRecvBuffers;
    delete[] velocitySendBuffers;
    delete[] velocityRecvBuffers;
}

void PetscParallelManager::initializeSizes(const Parameters &parameters, const FlowField &flowField)
{
    // Nx = flowField.getCellsX();
    // Ny = flowField.getCellsY();
    // Nz = flowField.getCellsZ();

    Nx = parameters.parallel.localSize[0] + 2;
    Ny = parameters.parallel.localSize[1] + 2;
    Nz = parameters.parallel.localSize[2] + 2;
    dim = parameters.geometry.dim;
    if (dim == 2) // Safety measure to inconsistently-configured Nz
    {
        Nz = 1;
    }
    
    sizes[LEFT] = Ny * Nz;
    sizes[RIGHT] = sizes[LEFT];
    sizes[BOTTOM] = Nx * Nz;
    sizes[TOP] = sizes[BOTTOM];
    sizes[FRONT] = Nx * Ny;
    sizes[BACK] = sizes[FRONT];
    if(parameters.parallel.rank==0) std::cout << "Sizes are " << sizes[LEFT] << " " << sizes[BOTTOM] << std::endl;

    if (parameters.geometry.dim == 2) // Safety measure to inconsistently-configured Nz
    {
        sizes[FRONT] = 0; 
        sizes[BACK] = 0; 
    }
}

void PetscParallelManager::allocateBuffers()
{
    pressureSendBuffers = new FLOAT *[numberDirection];
    pressureRecvBuffers = new FLOAT *[numberDirection];
    velocitySendBuffers = new Triple<FLOAT> *[numberDirection];
    velocityRecvBuffers = new Triple<FLOAT> *[numberDirection];
    
    // int totalSize = sizes[LEFT] + sizes[RIGHT] + sizes[BOTTOM] + sizes[TOP] + sizes[FRONT] + sizes[BACK];
    // pressureSendBuffers[0] = new FLOAT[totalSize];
    // pressureRecvBuffers[0] = new FLOAT[totalSize];
    // velocitySendBuffers[0] = new Triple<FLOAT>[totalSize];
    // velocityRecvBuffers[0] = new Triple<FLOAT>[totalSize];
    // int offset = 0;

    for (int d = 0; d < numberDirection; ++d)
    {
        tempPressureSendBuffer = new FLOAT[sizes[d]];
        tempPressureRecvBuffer = new FLOAT[sizes[d]];
        tempVelocitySendBuffer = new Triple<FLOAT>[sizes[d]];
        tempVelocityRecvBuffer = new Triple<FLOAT>[sizes[d]];   
        pressureSendBuffers[d] = tempPressureSendBuffer;
        pressureRecvBuffers[d] = tempPressureRecvBuffer;
        velocitySendBuffers[d] = tempVelocitySendBuffer;
        velocityRecvBuffers[d] = tempVelocityRecvBuffer;
    }
}

void PetscParallelManager::exchangePressure()
{
    readValues(pressureSendIterator);
    communicateValues(pressureSendBuffers, pressureRecvBuffers, PRESSURE_COMM);
    writeValues(pressureRecvIterator);
}

void PetscParallelManager::exchangeVelocity()
{
    readValues(velocitySendIterator);
    communicateValues<Triple<FLOAT>>(velocitySendBuffers, velocityRecvBuffers, VELOCITY_COMM);
    writeValues(velocityRecvIterator);
}

void PetscParallelManager::readValues(ParallelBoundaryIterator<FlowField> iterator)
{
    iterator.iterate();
}

void PetscParallelManager::writeValues(ParallelBoundaryIterator<FlowField> iterator)
{
    iterator.iterate();
}

template<class BufferValue>
void PetscParallelManager::communicateValues(BufferValue **sendBuffer, BufferValue **recvBuffer, int COMM_TYPE)
{
    for (int sDirection = 0; sDirection < numberDirection; ++sDirection)
    {
        int rDirection = DirectionUtils::getOppositeDirection(sDirection); // This should load commu channel more evenly
//        int rDirection = sDirection; // Debug
        int sNbRank = neighbours[sDirection];
        int rNbRank = neighbours[rDirection];
        BufferValue *sBuf = sendBuffer[sDirection];
        BufferValue *rBuf = recvBuffer[rDirection];
        int sSize = sizes[sDirection] * sizeof(BufferValue);
        int rSize = sizes[rDirection] * sizeof(BufferValue);

        // if(rank==0) std::cout << "sSize " << sSize << "Comm " << COMM_TYPE << std::endl;
        // if(rank==0) std::cout << "rSize " << rSize << "Comm " << COMM_TYPE << std::endl;
        //todo: Check if this sendrecv can actually be done more efficiently with a bunch of Isend and Irecv to be synced out of the for loop
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Sendrecv(sBuf, sSize, MPI_BYTE, sNbRank, COMM_TYPE, rBuf, rSize, MPI_BYTE, rNbRank, COMM_TYPE, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    // Debug
    // if (rank==0)
    // {
    //     std::cout << "Done with commu! :)" << std::endl;
    //     std::flush(std::cout);
    // }
    //
}
