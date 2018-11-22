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

class PetsParallelManager
{
private:
    const Parameters &parameters;
    FlowField &flowField;
    int Nx, Ny, Nz;
    int sizes[6]; // Size of each boundary face of the cube
    
    FLOAT **sendBuffers;
    FLOAT **recvBuffers;
    
    ParallelBoundaryIterator<FlowField> pressureBoundaryIterator;
    
    PressureBufferFillStencil pressureSendStencil;
    PressureBufferReadStencil pressureRecvStencil;
    
public:
    PetsParallelManager(const Parameters &parameters, FlowField &flowField);
    
    virtual ~PetsParallelManager();

private:
    void allocateBuffers();
    
    void initializeSizes(const Parameters &parameters, const FlowField &flowField);
};


#endif //NS_PETSPARALLELMANAGER_H
