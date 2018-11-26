//
// Created by tommaso on 22/11/18.
//

#include <cassert>
#include "PressureBufferReadStencil.h"
#include "Direction.h"

PressureBufferReadStencil::PressureBufferReadStencil(const Parameters &parameters, int lowOffset, int highOffset)
        : CommunicationStencil(parameters, lowOffset,
                               highOffset)
{}

void PressureBufferReadStencil::applyLeftWall(FlowField &flowField, int i, int j)
{
    flowField.getPressure().getScalar(lowOffset + i, j) = buffers[LEFT][j];
}

void PressureBufferReadStencil::applyRightWall(FlowField &flowField, int i, int j)
{
    flowField.getPressure().getScalar(highOffset + i, j) = buffers[RIGHT][j];
}

void PressureBufferReadStencil::applyBottomWall(FlowField &flowField, int i, int j)
{
    flowField.getPressure().getScalar(i, lowOffset + j) = buffers[BOTTOM][i];
}

void PressureBufferReadStencil::applyTopWall(FlowField &flowField, int i, int j)
{
    flowField.getPressure().getScalar(i, highOffset + j) = buffers[TOP][i];
}

void PressureBufferReadStencil::applyLeftWall(FlowField &flowField, int i, int j, int k)
{
    int Nk = flowField.getCellsZ();
    flowField.getPressure().getScalar(lowOffset + i, j, k) = buffers[LEFT][j * Nk + k]; // stored k-wise
}

void PressureBufferReadStencil::applyRightWall(FlowField &flowField, int i, int j, int k)
{
    int Nk = flowField.getCellsZ();
    flowField.getPressure().getScalar(highOffset + i, j, k) = buffers[RIGHT][j * Nk + k]; // stored k-wise
}

void PressureBufferReadStencil::applyBottomWall(FlowField &flowField, int i, int j, int k)
{
    int Nk = flowField.getCellsZ();
    flowField.getPressure().getScalar(i, lowOffset + j, k) = buffers[BOTTOM][i * Nk + k]; // stored k-wise
}

void PressureBufferReadStencil::applyTopWall(FlowField &flowField, int i, int j, int k)
{
    int Nk = flowField.getCellsZ();
    flowField.getPressure().getScalar(i, highOffset + j, k) = buffers[TOP][i * Nk + k]; // stored k-wise
}

void PressureBufferReadStencil::applyFrontWall(FlowField &flowField, int i, int j, int k)
{
    int Nj = flowField.getCellsY();
    flowField.getPressure().getScalar(i, j, lowOffset + k) = buffers[FRONT][i * Nj + j]; // stored j-wise
}

void PressureBufferReadStencil::applyBackWall(FlowField &flowField, int i, int j, int k)
{
    int Nj = flowField.getCellsY();
    flowField.getPressure().getScalar(i, j, highOffset + k) = buffers[BACK][i * Nj + j]; // stored j-wise
}
