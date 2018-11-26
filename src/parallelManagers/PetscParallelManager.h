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

typedef enum Tag {PRESSURE_TAG=0, VELOCITY_TAG=1, GENERIC_TAG=666} Tag;

class PetscParallelManager
{
private:
    const Parameters &parameters;
    FlowField &flowField;
    int Nx, Ny, Nz;
    int sizes[NUMBER_OF_DIRECTIONS]; // Size of each boundary face of the cube
    int rank;
    int neighbours[NUMBER_OF_DIRECTIONS]; // Ranks of neighbours
    
    FLOAT **pressureSendBuffers;
    FLOAT **pressureRecvBuffers;
    Triple<FLOAT> **velocitySendBuffers;
    Triple<FLOAT> **velocityRecvBuffers;
    
    PressureBufferFillStencil pressureSendStencil;
    PressureBufferReadStencil pressureRecvStencil;
    VelocityBufferFillStencil velocitySendStencil;
    VelocityBufferReadStencil velocityRecvStencil;

    ParallelBoundaryIterator<FlowField> pressureSendIterator;
    ParallelBoundaryIterator<FlowField> pressureRecvIterator;
    ParallelBoundaryIterator<FlowField> velocitySendIterator;
    ParallelBoundaryIterator<FlowField> velocityRecvIterator;

public:
    PetscParallelManager(const Parameters &parameters, FlowField &flowField);
    
    void exchangePressure();
    
    void exchangeVelocity();
    
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
    void communicateValues(BufferValue **sendBuffer, BufferValue **recvBuffer, std::string commTypeStr);
};


#endif //NS_PETSPARALLELMANAGER_H
