//
// Created by tommaso on 22/11/18.
//

#include "PetsParallelManager.h"

PetsParallelManager::PetsParallelManager(const Parameters &parameters, FlowField &flowField)
        : parameters(parameters), flowField(flowField), pressureSendStencil(parameters), pressureRecvStencil(parameters)
{
    initializeSizes(parameters, flowField);
    allocateBuffers();
    pressureSendStencil =
}

PetsParallelManager::~PetsParallelManager()
{
    delete[] sendBuffers[0];
    delete[] recvBuffers[0];
}

void PetsParallelManager::initializeSizes(const Parameters &parameters, const FlowField &flowField)
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

void PetsParallelManager::allocateBuffers()
{
    int totalSize = sizes[LEFT] + sizes[RIGHT] + sizes[BOTTOM] + sizes[TOP] + sizes[FRONT] + sizes[BACK];
    FLOAT *sBuf = new FLOAT[totalSize];
    FLOAT *rBuf = new FLOAT[totalSize];
    int offset = 0;
    for (int d = 0; d < NUMBER_OF_DIRECTIONS; ++d)
    {
        sendBuffers[d] = sBuf + offset;
        recvBuffers[d] = rBuf + offset;
        offset += sizes[d];
    }
}
