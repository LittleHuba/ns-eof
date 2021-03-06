//
// Created by tommaso on 22/11/18.
//

#ifndef NS_PETSPARALLELMANAGER_H
#define NS_PETSPARALLELMANAGER_H


#include <Definitions.h>
#include <Parameters.h>
#include <stencils/communicationStencils/PressureBufferFillStencil.h>
#include <stencils/communicationStencils/PressureBufferReadStencil.h>
#include <stencils/communicationStencils/Direction.h>
#include <Iterators.h>
#include <stencils/communicationStencils/VelocityBufferFillStencil.h>
#include <stencils/communicationStencils/VelocityBufferReadStencil.h>
#include <stencils/communicationStencils/ViscosityBufferFillStencil.h>
#include <stencils/communicationStencils/ViscosityBufferReadStencil.h>

#include <stencils/communicationStencils/WallDistanceBufferFillStencil.h>
#include <stencils/communicationStencils/WallDistanceBufferReadStencil.h>

typedef enum Tag {PRESSURE_COMM=0, VELOCITY_COMM=1, VISCOSITY_COMM=2, WALLDISTANCE_COMM=3} Tag;

class PetscParallelManager
{
private:
    const Parameters &parameters;
    FlowField &flowField;
    int Nx, Ny, Nz;
    int sizes[NUMBER_OF_DIRECTIONS]; // Size of each boundary face of the cube
    int rank, dim, numberDirection;
    int neighbours[NUMBER_OF_DIRECTIONS]; // Ranks of neighbours
    
    FLOAT **pressureSendBuffers;
    FLOAT **pressureRecvBuffers;
    Triple<FLOAT> **velocitySendBuffers;
    Triple<FLOAT> **velocityRecvBuffers;
    FLOAT **viscositySendBuffers;
    FLOAT **viscosityRecvBuffers;

    FLOAT **wallDistanceSendBuffers;
    FLOAT **wallDistanceRecvBuffers;

    FLOAT *tempPressureSendBuffer;
    FLOAT *tempPressureRecvBuffer;
    Triple<FLOAT> *tempVelocitySendBuffer;
    Triple<FLOAT> *tempVelocityRecvBuffer;
    FLOAT *tempViscositySendBuffer;
    FLOAT *tempViscosityRecvBuffer;

    FLOAT *tempWallDistanceViscositySendBuffer;
    FLOAT *tempWallDistanceViscosityRecvBuffer;
    
    PressureBufferFillStencil pressureSendStencil;
    PressureBufferReadStencil pressureRecvStencil;
    VelocityBufferFillStencil velocitySendStencil;
    VelocityBufferReadStencil velocityRecvStencil;
    ViscosityBufferFillStencil viscositySendStencil;
    ViscosityBufferReadStencil viscosityRecvStencil;

    WallDistanceBufferFillStencil wallDistanceSendStencil;
    WallDistanceBufferReadStencil wallDistanceRecvStencil;


    ParallelBoundaryIterator<FlowField> pressureSendIterator;
    ParallelBoundaryIterator<FlowField> pressureRecvIterator;
    ParallelBoundaryIterator<FlowField> velocitySendIterator;
    ParallelBoundaryIterator<FlowField> velocityRecvIterator;
    ParallelBoundaryIterator<FlowField> viscositySendIterator;
    ParallelBoundaryIterator<FlowField> viscosityRecvIterator;
    ParallelBoundaryIterator<FlowField> wallDistanceSendIterator;
    ParallelBoundaryIterator<FlowField> wallDistanceRecvIterator;


public:
    PetscParallelManager(const Parameters &parameters, FlowField &flowField);
    
    void exchangePressure();
    
    void exchangeVelocity();

    void exchangeViscosity();

    void exchangeWallDistance();
    
    virtual ~PetscParallelManager();

private:
    void initializeSizes(const Parameters &parameters, const FlowField &flowField);
    
    void initializeNeighbours(const Parameters &parameters);
    
    void allocateBuffers();
    
    /**
     * Read values from the grid into the appropriate buffer using the given iterator.
     * @param iterator
     */
    void readValues(ParallelBoundaryIterator<FlowField> iterator);
    
    /**
     * Write values into the grid from the given buffer using the given iterator.
     * @param iterator
     */
    void writeValues(ParallelBoundaryIterator<FlowField> iterator);
    
    /**
     * Communicate with the neighbours using the given buffers.
     * @tparam BufferValue
     * @param recvBuffer
     */
    template<class BufferValue>
    void communicateValues(BufferValue **sendBuffer, BufferValue **recvBuffer, int COMM_TYPE);
};


#endif //NS_PETSPARALLELMANAGER_H

