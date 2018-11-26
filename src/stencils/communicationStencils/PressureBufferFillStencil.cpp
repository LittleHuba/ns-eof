//
// Created by tommaso on 22/11/18.
//

#include "PressureBufferFillStencil.h"
#include "Direction.h"

PressureBufferFillStencil::PressureBufferFillStencil(const Parameters &parameters, int lowOffset, int highOffset)
        : CommunicationStencil(parameters, lowOffset,
                               highOffset)
{}

void PressureBufferFillStencil::applyLeftWall(FlowField &flowField, int i, int j)
{
    buffers[LEFT][j] = flowField.getPressure().getScalar(lowOffset + i, j);
}

void PressureBufferFillStencil::applyRightWall(FlowField &flowField, int i, int j)
{
    buffers[RIGHT][j] = flowField.getPressure().getScalar(highOffset + i, j);
}

void PressureBufferFillStencil::applyBottomWall(FlowField &flowField, int i, int j)
{
    buffers[BOTTOM][i] = flowField.getPressure().getScalar(i, lowOffset + j);
}

void PressureBufferFillStencil::applyTopWall(FlowField &flowField, int i, int j)
{
    buffers[TOP][i] = flowField.getPressure().getScalar(i, highOffset + j);
}

void PressureBufferFillStencil::applyLeftWall(FlowField &flowField, int i, int j, int k)
{
    int Nk = flowField.getCellsZ();
    buffers[LEFT][j * Nk + k] = flowField.getPressure().getScalar(lowOffset + i, j, k); // stored k-wise
}

void PressureBufferFillStencil::applyRightWall(FlowField &flowField, int i, int j, int k)
{
    int Nk = flowField.getCellsZ();
    buffers[RIGHT][j * Nk + k] = flowField.getPressure().getScalar(highOffset + i, j, k); // stored k-wise
}

void PressureBufferFillStencil::applyBottomWall(FlowField &flowField, int i, int j, int k)
{
    int Nk = flowField.getCellsZ();
    buffers[BOTTOM][i * Nk + k] = flowField.getPressure().getScalar(i, lowOffset + j, k); // stored k-wise
}

void PressureBufferFillStencil::applyTopWall(FlowField &flowField, int i, int j, int k)
{
    int Nk = flowField.getCellsZ();
    buffers[TOP][i * Nk + k] = flowField.getPressure().getScalar(i, highOffset + j, k); // stored k-wise
}

void PressureBufferFillStencil::applyFrontWall(FlowField &flowField, int i, int j, int k)
{
    int Nj = flowField.getCellsY();
    buffers[FRONT][i * Nj + j] = flowField.getPressure().getScalar(i, j, lowOffset + k); // stored j-wise
}

void PressureBufferFillStencil::applyBackWall(FlowField &flowField, int i, int j, int k)
{
    int Nj = flowField.getCellsY();
    buffers[BACK][i * Nj + j] = flowField.getPressure().getScalar(i, j, highOffset + k); // stored j-wise
}

