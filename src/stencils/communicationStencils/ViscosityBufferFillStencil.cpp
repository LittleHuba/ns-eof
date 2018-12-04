//
// Created by keefe on 12/4/18.
//

#include "ViscosityBufferFillStencil.h"
#include "Direction.h"


void ViscosityBufferFillStencil::applyLeftWall(FlowField &flowField, int i, int j)
{
    //todo: check how to properly offset j in order to get a shorter array
    buffers[LEFT][j-2] = flowField.getTurbulentViscosity().getScalar(i, j);

    // if(_parameters.parallel.rank ==0) std::cout << "Pressure Left Wall " << i << " " << j << " " << flowField.getPressure().getScalar(i, j) << std::endl;
}

void ViscosityBufferFillStencil::applyRightWall(FlowField &flowField, int i, int j)
{
    buffers[RIGHT][j-2] = flowField.getTurbulentViscosity().getScalar(i, j);
    // if(_parameters.parallel.rank ==0) std::cout << "Pressure Right Wall " << i << " " << j << " " << flowField.getPressure().getScalar(i, j) << std::endl;
}

void ViscosityBufferFillStencil::applyBottomWall(FlowField &flowField, int i, int j)
{
    buffers[BOTTOM][i-2] = flowField.getTurbulentViscosity().getScalar(i, j);
    // if(_parameters.parallel.rank ==0) std::cout << "Pressure Bottom Wall " << i << " " << j << " " << flowField.getPressure().getScalar(i, j) << std::endl;
}

void ViscosityBufferFillStencil::applyTopWall(FlowField &flowField, int i, int j)
{
    buffers[TOP][i-2] = flowField.getTurbulentViscosity().getScalar(i, j);
    // if(_parameters.parallel.rank ==0) std::cout << "Pressure Top Wall " << i << " " << j << " " << flowField.getPressure().getScalar(i, j) << std::endl;
}

void ViscosityBufferFillStencil::applyLeftWall(FlowField &flowField, int i, int j, int k)
{
    // int Nk = flowField.getCellsZ() - 1;
    int Nk = _parameters.parallel.localSize[2];
    // if(_parameters.parallel.rank==0) std::cout << "Left Wall Fill" << "(" << i << ", " << j << ", " << k << ") " << (j-2) * Nk + (k-2) << std::endl;
    buffers[LEFT][(j-2) * Nk + (k-2)] = flowField.getTurbulentViscosity().getScalar(i, j, k); // stored k-wise
}

void ViscosityBufferFillStencil::applyRightWall(FlowField &flowField, int i, int j, int k)
{
    // int Nk = flowField.getCellsZ() - 1;
    int Nk = _parameters.parallel.localSize[2];
    // if(_parameters.parallel.rank==0) std::cout << "Right Wall Fill" << "(" << i << ", " << j << ", " << k << ") " << (j-2) * Nk + (k-2) << std::endl;
    buffers[RIGHT][(j-2) * Nk + (k-2)] = flowField.getTurbulentViscosity().getScalar(i , j, k); // stored k-wise
}

void ViscosityBufferFillStencil::applyBottomWall(FlowField &flowField, int i, int j, int k)
{
    // int Nk = flowField.getCellsZ() - 1;
    int Nk = _parameters.parallel.localSize[2];
    buffers[BOTTOM][(i-2) * Nk + (k-2)] = flowField.getTurbulentViscosity().getScalar(i, j, k); // stored k-wise
}

void ViscosityBufferFillStencil::applyTopWall(FlowField &flowField, int i, int j, int k)
{
    // int Nk = flowField.getCellsZ() - 1;
    int Nk = _parameters.parallel.localSize[2];
    buffers[TOP][(i-2) * Nk + (k-2)] = flowField.getTurbulentViscosity().getScalar(i, j, k); // stored k-wise
}

void ViscosityBufferFillStencil::applyFrontWall(FlowField &flowField, int i, int j, int k)
{
    //Here we assume low k is front!
    // int Nj = flowField.getCellsY() - 1;
    int Nj = _parameters.parallel.localSize[1];
    buffers[FRONT][(i-2) * Nj + (j-2)] = flowField.getTurbulentViscosity().getScalar(i, j, k); // stored j-wise
}

void ViscosityBufferFillStencil::applyBackWall(FlowField &flowField, int i, int j, int k)
{
    // int Nj = flowField.getCellsY() - 1;
    int Nj = _parameters.parallel.localSize[1];
    buffers[BACK][(i-2) * Nj + (j-2)] = flowField.getTurbulentViscosity().getScalar(i, j, k); // stored j-wise
}