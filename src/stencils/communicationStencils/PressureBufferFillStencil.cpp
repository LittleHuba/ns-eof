//
// Created by tommaso on 22/11/18.
//

#include "PressureBufferFillStencil.h"
#include "Direction.h"

void PressureBufferFillStencil::applyLeftWall(FlowField &flowField, int i, int j)
{
    //todo: check how to properly offset j in order to get a shorter array
    buffers[LEFT][j] = flowField.getPressure().getScalar(i + 1, j);
}

void PressureBufferFillStencil::applyRightWall(FlowField &flowField, int i, int j)
{
    buffers[RIGHT][j] = flowField.getPressure().getScalar(i - 1, j);
}

void PressureBufferFillStencil::applyBottomWall(FlowField &flowField, int i, int j)
{
    //Here we assume low j is bottom!
    buffers[BOTTOM][i] = flowField.getPressure().getScalar(i, j + 1);
}

void PressureBufferFillStencil::applyTopWall(FlowField &flowField, int i, int j)
{
    buffers[TOP][i] = flowField.getPressure().getScalar(i, j - 1);
}

void PressureBufferFillStencil::applyLeftWall(FlowField &flowField, int i, int j, int k)
{
    int Nk = flowField.getCellsZ();
    buffers[LEFT][j * Nk + k] = flowField.getPressure().getScalar(i + 1, j, k); // stored k-wise
}

void PressureBufferFillStencil::applyRightWall(FlowField &flowField, int i, int j, int k)
{
    int Nk = flowField.getCellsZ();
    buffers[RIGHT][j * Nk + k] = flowField.getPressure().getScalar(i - 1, j, k); // stored k-wise
}

void PressureBufferFillStencil::applyBottomWall(FlowField &flowField, int i, int j, int k)
{
    int Nk = flowField.getCellsZ();
    buffers[BOTTOM][i * Nk + k] = flowField.getPressure().getScalar(i, j + 1, k); // stored k-wise
}

void PressureBufferFillStencil::applyTopWall(FlowField &flowField, int i, int j, int k)
{
    int Nk = flowField.getCellsZ();
    buffers[TOP][i * Nk + k] = flowField.getPressure().getScalar(i, j - 1, k); // stored k-wise
}

void PressureBufferFillStencil::applyFrontWall(FlowField &flowField, int i, int j, int k)
{
    //Here we assume low k is front!
    int Nj = flowField.getCellsY();
    buffers[FRONT][i * Nj + j] = flowField.getPressure().getScalar(i, j, k + 1); // stored j-wise
}

void PressureBufferFillStencil::applyBackWall(FlowField &flowField, int i, int j, int k)
{
    int Nj = flowField.getCellsY();
    buffers[BACK][i * Nj + j] = flowField.getPressure().getScalar(i, j, k - 1); // stored j-wise
}

