//
// Created by tommaso on 22/11/18.
//

#include <cassert>
#include "PetscParallelManager.h"

PetscParallelManager::PetscParallelManager(const Parameters &parameters, FlowField &flowField)
        : parameters(parameters), flowField(flowField),
          pressureSendStencil(parameters, 2, -1),
          pressureRecvStencil(parameters, 1, 0),
          velocitySendStencil(parameters, 2, -2),
          velocityRecvStencil(parameters, 0, 0),
          pressureSendIterator(flowField, parameters, pressureSendStencil, 0, 0),
          pressureRecvIterator(flowField, parameters, pressureRecvStencil, 0, 0),
          velocitySendIterator(flowField, parameters, velocitySendStencil, 0, 0),
          velocityRecvIterator(flowField, parameters, velocityRecvStencil, 0, 0)
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
    if (parameters.geometry.dim == 2)
    {
        sizes[FRONT] = 0;
        sizes[BACK] = sizes[FRONT];
    }
    else // 3D
    {
        sizes[FRONT] = Nx * Ny;
        sizes[BACK] = sizes[FRONT];
    }
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
    communicateValues(pressureSendBuffers, pressureRecvBuffers, "Pressure");
    writeValues(pressureRecvIterator);
}

void PetscParallelManager::exchangeVelocity()
{
    readValues(velocitySendIterator);
    communicateValues(velocitySendBuffers, velocityRecvBuffers, "Velocity");
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
void PetscParallelManager::communicateValues(BufferValue **sendBuffer, BufferValue **recvBuffer,
                                             std::string commTypeStr)
{
    std::flush(std::cout);
    MPI_Barrier(PETSC_COMM_WORLD);
    for (int sDirection = 0; sDirection < NUMBER_OF_DIRECTIONS; ++sDirection)
    {
        int rDirection = DirectionUtils::getOppositeDirection(sDirection); // This ensures we use different send and recv buffers!
        int sNbRank = neighbours[sDirection];
        int rNbRank = neighbours[rDirection];
        BufferValue *sBuf = sendBuffer[sDirection];
        BufferValue *rBuf = recvBuffer[rDirection];
        int sendBytesCount = sizes[sDirection] * sizeof(BufferValue);
        int recvBytesCount = sizes[rDirection] * sizeof(BufferValue);
//        if (rank == 0)
//        {
//            // Debug
//            std::cout << "[" << commTypeStr << "] "
//                      << "Sending " << sendBytesCount << " Bytes to [" << sDirection << ", r=" << sNbRank << "], "
//                      << "Receiving " << recvBytesCount << " Bytes from [" << rDirection << ", r=" << rNbRank << "]"
//                      << std::endl;
//        }
        
        // Permanent commu sanity test
        sBuf[0] = rank;
        //
        
        //todo: Check if this sendrecv can actually be done more efficiently with a bunch of Isend and Irecv to be synced out of the for loop
        MPI_Sendrecv(sBuf, sendBytesCount, MPI_BYTE, sNbRank, GENERIC_TAG, rBuf, recvBytesCount, MPI_BYTE, rNbRank,
                     GENERIC_TAG, PETSC_COMM_WORLD, MPI_STATUS_IGNORE);
        
        // Permanent commu sanity test
        if (rNbRank != -2)
        {
            if (!(rBuf[0] == rNbRank))
            {
                std::cout << "ERROR:NOT_EQUAL: rBuf[0]=" << rBuf[0] << " rNbRank=" << rNbRank << std::endl;
            }
            assert(rBuf[0] == rNbRank);
        }
        //
    }
    MPI_Barrier(PETSC_COMM_WORLD);
    // Debug //TODO: remove this before submitting!
    if (rank == 0)
    {
        std::cout << "Done with " << commTypeStr << " commu! :)" << std::endl;
        std::flush(std::cout);
    }
    //
}
