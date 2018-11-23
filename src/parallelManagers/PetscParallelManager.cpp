//
// Created by tommaso on 22/11/18.
//

#include "PetscParallelManager.h"

PetscParallelManager::PetscParallelManager(const Parameters &parameters, FlowField &flowField)
        : parameters(parameters), flowField(flowField),
        pressureSendStencil(parameters), pressureRecvStencil(parameters),
        velocitySendStencil(parameters), velocityRecvStencil(parameters),
        pressureSendIterator(flowField, parameters, pressureSendStencil, 1, 0), //todo: here check if these iterators need any offset
        pressureRecvIterator(flowField, parameters, pressureRecvStencil, 1, 0),
        velocitySendIterator(flowField, parameters, velocitySendStencil, 1, 0),
        velocityRecvIterator(flowField, parameters, velocityRecvStencil, 1, 0)
{
    initializeSizes(parameters, flowField);
    rank = parameters.parallel.rank;
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
    delete[] pressureSendBuffers[0];
    delete[] pressureRecvBuffers[0];
    delete[] pressureSendBuffers;
    delete[] pressureRecvBuffers;
}

void PetscParallelManager::initializeSizes(const Parameters &parameters, const FlowField &flowField)
{
    Nx = flowField.getCellsX();
    Ny = flowField.getCellsY();
    Nz = flowField.getCellsZ();
    
    if (parameters.geometry.dim == 2) // Safety measure to inconsistently-configured Nz
    {
        Nz = 1;
    }
    
    sizes[LEFT] = Ny * Nz;
    sizes[RIGHT] = sizes[LEFT];
    sizes[BOTTOM] = Nx * Nz;
    sizes[TOP] = sizes[BOTTOM];
    sizes[FRONT] = Nx * Ny;
    sizes[BACK] = sizes[FRONT];
}

void PetscParallelManager::allocateBuffers()
{
    pressureSendBuffers = new FLOAT *[NUMBER_OF_DIRECTIONS];
    pressureRecvBuffers = new FLOAT *[NUMBER_OF_DIRECTIONS];
    velocitySendBuffers = new Triple<FLOAT> *[NUMBER_OF_DIRECTIONS];
    velocityRecvBuffers = new Triple<FLOAT> *[NUMBER_OF_DIRECTIONS];
    
    int totalSize = sizes[LEFT] + sizes[RIGHT] + sizes[BOTTOM] + sizes[TOP] + sizes[FRONT] + sizes[BACK];
    pressureSendBuffers[0] = new FLOAT[totalSize];
    pressureRecvBuffers[0] = new FLOAT[totalSize];
    velocitySendBuffers[0] = new Triple<FLOAT>[totalSize];
    velocityRecvBuffers[0] = new Triple<FLOAT>[totalSize];
    int offset = 0;
    for (int d = 1; d < NUMBER_OF_DIRECTIONS; ++d)
    {
        offset += sizes[d];
        pressureSendBuffers[d] = pressureSendBuffers[0] + offset;
        pressureRecvBuffers[d] = pressureRecvBuffers[0] + offset;
        velocitySendBuffers[d] = velocitySendBuffers[0] + offset;
        velocityRecvBuffers[d] = velocityRecvBuffers[0] + offset;
    }
}

void PetscParallelManager::exchangePressure()
{
    readValues(pressureSendIterator);
    communicateValues(pressureSendBuffers, pressureRecvBuffers);
    writeValues(pressureRecvIterator);
}

void PetscParallelManager::exchangeVelocity()
{
    readValues(velocitySendIterator);
    communicateValues(velocitySendBuffers, velocityRecvBuffers);
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
void PetscParallelManager::communicateValues(BufferValue **sendBuffer, BufferValue **recvBuffer)
{
    for (int sDirection = 0; sDirection < NUMBER_OF_DIRECTIONS; ++sDirection)
    {
        int rDirection = DirectionUtils::getOppositeDirection(sDirection); // This should load commu channel more evenly
//        int rDirection = sDirection; // Debug
        int sNbRank = neighbours[sDirection];
        int rNbRank = neighbours[rDirection];
        BufferValue *sBuf = sendBuffer[sDirection];
        BufferValue *rBuf = recvBuffer[rDirection];
        int sSize = sizes[sDirection] * sizeof(BufferValue);
        int rSize = sizes[rDirection] * sizeof(BufferValue);
        //todo: Check if this sendrecv can actually be done more efficiently with a bunch of Isend and Irecv to be synced out of the for loop
        MPI_Sendrecv(sBuf, sSize, MPI_BYTE, sNbRank, PRESSURE_COMM, rBuf, rSize, MPI_BYTE, rNbRank, PRESSURE_COMM, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    // Debug
    if (rank==0)
    {
        std::cout << "Done with commu! :)" << std::endl;
        std::flush(std::cout);
    }
    //
}
