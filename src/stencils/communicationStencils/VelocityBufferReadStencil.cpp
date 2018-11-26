//
// Created by tommaso on 23/11/18.
//

#include <cassert>
#include "VelocityBufferReadStencil.h"
#include "Direction.h"

VelocityBufferReadStencil::VelocityBufferReadStencil(const Parameters &parameters, int lowOffset, int highOffset)
        : CommunicationStencil(parameters, lowOffset,
                               highOffset)
{}

void VelocityBufferReadStencil::applyLeftWall(FlowField &flowField, int i, int j)
{
    flowField.getVelocity().getVector(lowOffset + i, j)[0] = buffers[LEFT][j].x;
    flowField.getVelocity().getVector(lowOffset + i, j)[1] = buffers[LEFT][j].y;
}

void VelocityBufferReadStencil::applyRightWall(FlowField &flowField, int i, int j)
{
    flowField.getVelocity().getVector(highOffset + i, j)[0] = buffers[RIGHT][j].x;
    flowField.getVelocity().getVector(highOffset + i, j)[1] = buffers[RIGHT][j].y;
}

void VelocityBufferReadStencil::applyBottomWall(FlowField &flowField, int i, int j)
{
    flowField.getVelocity().getVector(i, lowOffset + j)[0] = buffers[BOTTOM][i].x;
    flowField.getVelocity().getVector(i, lowOffset + j)[1] = buffers[BOTTOM][i].y;
}

void VelocityBufferReadStencil::applyTopWall(FlowField &flowField, int i, int j)
{
    flowField.getVelocity().getVector(i, highOffset + j)[0] = buffers[TOP][i].x;
    flowField.getVelocity().getVector(i, highOffset + j)[1] = buffers[TOP][i].y;
}

void VelocityBufferReadStencil::applyLeftWall(FlowField &flowField, int i, int j, int k)
{
    int Nk = flowField.getCellsZ();
    flowField.getVelocity().getVector(lowOffset + i, j, k)[0] = buffers[LEFT][j * Nk + k].x;
    flowField.getVelocity().getVector(lowOffset + i, j, k)[1] = buffers[LEFT][j * Nk + k].y;
    flowField.getVelocity().getVector(lowOffset + i, j, k)[2] = buffers[LEFT][j * Nk + k].z;
}

void VelocityBufferReadStencil::applyRightWall(FlowField &flowField, int i, int j, int k)
{
    int Nk = flowField.getCellsZ();
    flowField.getVelocity().getVector(highOffset + i, j, k)[0] = buffers[RIGHT][j * Nk + k].x;
    flowField.getVelocity().getVector(highOffset + i, j, k)[1] = buffers[RIGHT][j * Nk + k].y;
    flowField.getVelocity().getVector(highOffset + i, j, k)[2] = buffers[RIGHT][j * Nk + k].z;
}

void VelocityBufferReadStencil::applyBottomWall(FlowField &flowField, int i, int j, int k)
{
    int Nk = flowField.getCellsZ();
    flowField.getVelocity().getVector(i, lowOffset + j, k)[0] = buffers[BOTTOM][i * Nk + k].x;
    flowField.getVelocity().getVector(i, lowOffset + j, k)[1] = buffers[BOTTOM][i * Nk + k].y;
    flowField.getVelocity().getVector(i, lowOffset + j, k)[2] = buffers[BOTTOM][i * Nk + k].z;
}

void VelocityBufferReadStencil::applyTopWall(FlowField &flowField, int i, int j, int k)
{
    int Nk = flowField.getCellsZ();
    flowField.getVelocity().getVector(i, highOffset + j, k)[0] = buffers[TOP][i * Nk + k].x;
    flowField.getVelocity().getVector(i, highOffset + j, k)[1] = buffers[TOP][i * Nk + k].y;
    flowField.getVelocity().getVector(i, highOffset + j, k)[2] = buffers[TOP][i * Nk + k].z;
}

void VelocityBufferReadStencil::applyFrontWall(FlowField &flowField, int i, int j, int k)
{
    int Nj = flowField.getCellsY();
    flowField.getVelocity().getVector(i, lowOffset + j, k)[0] = buffers[FRONT][i * Nj + j].x;
    flowField.getVelocity().getVector(i, lowOffset + j, k)[1] = buffers[FRONT][i * Nj + j].y;
    flowField.getVelocity().getVector(i, lowOffset + j, k)[2] = buffers[FRONT][i * Nj + j].z;
}

void VelocityBufferReadStencil::applyBackWall(FlowField &flowField, int i, int j, int k)
{
    int Nj = flowField.getCellsY();
    flowField.getVelocity().getVector(i, j, highOffset + k)[0] = buffers[BACK][i * Nj + j].x;
    flowField.getVelocity().getVector(i, j, highOffset + k)[1] = buffers[BACK][i * Nj + j].y;
    flowField.getVelocity().getVector(i, j, highOffset + k)[2] = buffers[BACK][i * Nj + j].z;
}
