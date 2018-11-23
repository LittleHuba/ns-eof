//
// Created by tommaso on 23/11/18.
//

#include <cassert>
#include "VelocityBufferReadStencil.h"
#include "Direction.h"

void VelocityBufferReadStencil::applyLeftWall(FlowField &flowField, int i, int j)
{
    flowField.getVelocity().getVector(i - 1, j)[0] = buffers[LEFT][j].x;
    flowField.getVelocity().getVector(i, j)[1] = buffers[LEFT][j].y;
}

void VelocityBufferReadStencil::applyRightWall(FlowField &flowField, int i, int j)
{
    flowField.getVelocity().getVector(i, j)[0] = buffers[RIGHT][j].x;
    flowField.getVelocity().getVector(i, j)[1] = buffers[RIGHT][j].y;
}

void VelocityBufferReadStencil::applyBottomWall(FlowField &flowField, int i, int j)
{
    flowField.getVelocity().getVector(i, j)[0] = buffers[BOTTOM][j].x;
    flowField.getVelocity().getVector(i, j - 1)[1] = buffers[BOTTOM][j].y;
}

void VelocityBufferReadStencil::applyTopWall(FlowField &flowField, int i, int j)
{
    flowField.getVelocity().getVector(i, j)[0] = buffers[TOP][j].x;
    flowField.getVelocity().getVector(i, j)[1] = buffers[TOP][j].y;
}

void VelocityBufferReadStencil::applyLeftWall(FlowField &flowField, int i, int j, int k)
{
    flowField.getVelocity().getVector(i - 1, j, k)[0] = buffers[LEFT][j].x;
    flowField.getVelocity().getVector(i, j, k)[1] = buffers[LEFT][j].y;
    flowField.getVelocity().getVector(i, j, k)[2] = buffers[LEFT][j].z;
//    int rank;
//    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//    std::cout << ""
//    assert(buffers[LEFT][j].x == 1); //DEBUG
}

void VelocityBufferReadStencil::applyRightWall(FlowField &flowField, int i, int j, int k)
{
    flowField.getVelocity().getVector(i, j, k)[0] = buffers[RIGHT][j].x;
    flowField.getVelocity().getVector(i, j, k)[1] = buffers[RIGHT][j].y;
    flowField.getVelocity().getVector(i, j, k)[2] = buffers[RIGHT][j].z;
//    assert(buffers[RIGHT][j].x == 1); //DEBUG
}

void VelocityBufferReadStencil::applyBottomWall(FlowField &flowField, int i, int j, int k)
{
    flowField.getVelocity().getVector(i, j, k)[0] = buffers[BOTTOM][j].x;
    flowField.getVelocity().getVector(i, j - 1, k)[1] = buffers[BOTTOM][j].y;
    flowField.getVelocity().getVector(i, j, k)[2] = buffers[BOTTOM][j].z;
}

void VelocityBufferReadStencil::applyTopWall(FlowField &flowField, int i, int j, int k)
{
    flowField.getVelocity().getVector(i, j, k)[0] = buffers[TOP][j].x;
    flowField.getVelocity().getVector(i, j, k)[1] = buffers[TOP][j].y;
    flowField.getVelocity().getVector(i, j, k)[2] = buffers[TOP][j].z;
}

void VelocityBufferReadStencil::applyFrontWall(FlowField &flowField, int i, int j, int k)
{
    flowField.getVelocity().getVector(i, j, k)[0] = buffers[FRONT][j].x;
    flowField.getVelocity().getVector(i, j, k)[1] = buffers[FRONT][j].y;
    flowField.getVelocity().getVector(i, j, k - 1)[2] = buffers[FRONT][j].z;
}

void VelocityBufferReadStencil::applyBackWall(FlowField &flowField, int i, int j, int k)
{
    flowField.getVelocity().getVector(i, j, k)[0] = buffers[BACK][j].x;
    flowField.getVelocity().getVector(i, j, k)[1] = buffers[BACK][j].y;
    flowField.getVelocity().getVector(i, j, k)[2] = buffers[BACK][j].z;
}
