//
// Created by tommaso on 22/11/18.
//

#include "PressureBufferReadStencil.h"
#include "Direction.h"

void PressureBufferReadStencil::applyLeftWall(FlowField &flowField, int i, int j)
{
    //todo: check how to properly offset j in order to get a shorter array
    flowField.getPressure().getScalar(i, j) = buffers[LEFT][j];
}

void PressureBufferReadStencil::applyRightWall(FlowField &flowField, int i, int j)
{
    flowField.getPressure().getScalar(i, j) = buffers[RIGHT][j];
}

void PressureBufferReadStencil::applyBottomWall(FlowField &flowField, int i, int j)
{
    //Here we assume low j is bottom!
    flowField.getPressure().getScalar(i, j) = buffers[BOTTOM][i];
}

void PressureBufferReadStencil::applyTopWall(FlowField &flowField, int i, int j)
{
    flowField.getPressure().getScalar(i, j) = buffers[TOP][i];
}

void PressureBufferReadStencil::applyLeftWall(FlowField &flowField, int i, int j, int k)
{
    int Nk = flowField.getCellsZ();
    flowField.getPressure().getScalar(i, j, k) = buffers[LEFT][j * Nk + k]; // stored k-wise
}

void PressureBufferReadStencil::applyRightWall(FlowField &flowField, int i, int j, int k)
{
    int Nk = flowField.getCellsZ();
    flowField.getPressure().getScalar(i, j, k) = buffers[RIGHT][j * Nk + k]; // stored k-wise
}

void PressureBufferReadStencil::applyBottomWall(FlowField &flowField, int i, int j, int k)
{
    int Nk = flowField.getCellsZ();
    flowField.getPressure().getScalar(i, j, k) = buffers[BOTTOM][i * Nk + k]; // stored k-wise
}

void PressureBufferReadStencil::applyTopWall(FlowField &flowField, int i, int j, int k)
{
    int Nk = flowField.getCellsZ();
    flowField.getPressure().getScalar(i, j, k) = buffers[TOP][i * Nk + k]; // stored k-wise
}

void PressureBufferReadStencil::applyFrontWall(FlowField &flowField, int i, int j, int k)
{
    //Here we assume low k is front!
    int Nj = flowField.getCellsY();
    flowField.getPressure().getScalar(i, j, k) = buffers[FRONT][i * Nj + j]; // stored j-wise
}

void PressureBufferReadStencil::applyBackWall(FlowField &flowField, int i, int j, int k)
{
    int Nj = flowField.getCellsY();
    flowField.getPressure().getScalar(i, j, k) = buffers[BACK][i * Nj + j]; // stored j-wise
}
