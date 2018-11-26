//
// Created by tommaso on 23/11/18.
//

#include "VelocityBufferFillStencil.h"
#include "Direction.h"

VelocityBufferFillStencil::VelocityBufferFillStencil(const Parameters &parameters, int lowOffset, int highOffset)
        : CommunicationStencil(parameters, lowOffset,
                               highOffset)
{}

void VelocityBufferFillStencil::applyLeftWall(FlowField &flowField, int i, int j)
{
    buffers[LEFT][j] = flowField.getVelocity().getVector(lowOffset + i, j);
}

void VelocityBufferFillStencil::applyRightWall(FlowField &flowField, int i, int j)
{
    buffers[RIGHT][j] = flowField.getVelocity().getVector(highOffset + i, j);
}

void VelocityBufferFillStencil::applyBottomWall(FlowField &flowField, int i, int j)
{
    buffers[BOTTOM][i] = flowField.getVelocity().getVector(i, lowOffset + j);
}

void VelocityBufferFillStencil::applyTopWall(FlowField &flowField, int i, int j)
{
    buffers[TOP][i] = flowField.getVelocity().getVector(i, highOffset + j);
}

void VelocityBufferFillStencil::applyLeftWall(FlowField &flowField, int i, int j, int k)
{
    int Nk = flowField.getCellsZ();
    buffers[LEFT][j * Nk + k] = flowField.getVelocity().getVector(lowOffset + i, j, k);
}

void VelocityBufferFillStencil::applyRightWall(FlowField &flowField, int i, int j, int k)
{
    int Nk = flowField.getCellsZ();
    buffers[RIGHT][j * Nk + k] = flowField.getVelocity().getVector(highOffset + i, j, k);
}

void VelocityBufferFillStencil::applyBottomWall(FlowField &flowField, int i, int j, int k)
{
    int Nk = flowField.getCellsZ();
    buffers[BOTTOM][i * Nk + k] = flowField.getVelocity().getVector(i, lowOffset + j, k);
}

void VelocityBufferFillStencil::applyTopWall(FlowField &flowField, int i, int j, int k)
{
    int Nk = flowField.getCellsZ();
    buffers[TOP][i * Nk + k] = flowField.getVelocity().getVector(i, highOffset + j, k);
}

void VelocityBufferFillStencil::applyFrontWall(FlowField &flowField, int i, int j, int k)
{
    int Nj = flowField.getCellsY();
    buffers[FRONT][i * Nj + j] = flowField.getVelocity().getVector(i, j, lowOffset + k);
}

void VelocityBufferFillStencil::applyBackWall(FlowField &flowField, int i, int j, int k)
{
    int Nj = flowField.getCellsY();
    buffers[BACK][i * Nj + j] = flowField.getVelocity().getVector(i, j, highOffset + k);
}
