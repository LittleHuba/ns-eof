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
    flowField.getVelocity().getVector(i, lowOffset + j)[0] = buffers[BOTTOM][j].x;
    flowField.getVelocity().getVector(i, lowOffset + j)[1] = buffers[BOTTOM][j].y;
}

void VelocityBufferReadStencil::applyTopWall(FlowField &flowField, int i, int j)
{
    flowField.getVelocity().getVector(i, highOffset + j)[0] = buffers[TOP][j].x;
    flowField.getVelocity().getVector(i, highOffset + j)[1] = buffers[TOP][j].y;
}

void VelocityBufferReadStencil::applyLeftWall(FlowField &flowField, int i, int j, int k)
{
    flowField.getVelocity().getVector(lowOffset + i, j, k)[0] = buffers[LEFT][j].x;
    flowField.getVelocity().getVector(lowOffset + i, j, k)[1] = buffers[LEFT][j].y;
    flowField.getVelocity().getVector(lowOffset + i, j, k)[2] = buffers[LEFT][j].z;
}

void VelocityBufferReadStencil::applyRightWall(FlowField &flowField, int i, int j, int k)
{
    flowField.getVelocity().getVector(highOffset + i, j, k)[0] = buffers[RIGHT][j].x;
    flowField.getVelocity().getVector(highOffset + i, j, k)[1] = buffers[RIGHT][j].y;
    flowField.getVelocity().getVector(highOffset + i, j, k)[2] = buffers[RIGHT][j].z;
}

void VelocityBufferReadStencil::applyBottomWall(FlowField &flowField, int i, int j, int k)
{
    flowField.getVelocity().getVector(i, lowOffset + j, k)[0] = buffers[BOTTOM][j].x;
    flowField.getVelocity().getVector(i, lowOffset + j, k)[1] = buffers[BOTTOM][j].y;
    flowField.getVelocity().getVector(i, lowOffset + j, k)[2] = buffers[BOTTOM][j].z;
}

void VelocityBufferReadStencil::applyTopWall(FlowField &flowField, int i, int j, int k)
{
    flowField.getVelocity().getVector(i, highOffset + j, k)[0] = buffers[TOP][j].x;
    flowField.getVelocity().getVector(i, highOffset + j, k)[1] = buffers[TOP][j].y;
    flowField.getVelocity().getVector(i, highOffset + j, k)[2] = buffers[TOP][j].z;
}

void VelocityBufferReadStencil::applyFrontWall(FlowField &flowField, int i, int j, int k)
{
    flowField.getVelocity().getVector(i, lowOffset + j, k)[0] = buffers[FRONT][j].x;
    flowField.getVelocity().getVector(i, lowOffset + j, k)[1] = buffers[FRONT][j].y;
    flowField.getVelocity().getVector(i, lowOffset + j, k)[2] = buffers[FRONT][j].z;
}

void VelocityBufferReadStencil::applyBackWall(FlowField &flowField, int i, int j, int k)
{
    flowField.getVelocity().getVector(i, j, highOffset + k)[0] = buffers[BACK][j].x;
    flowField.getVelocity().getVector(i, j, highOffset + k)[1] = buffers[BACK][j].y;
    flowField.getVelocity().getVector(i, j, highOffset + k)[2] = buffers[BACK][j].z;
}
