//
// Created by tommaso on 23/11/18.
//

#include "VelocityBufferFillStencil.h"
#include "Direction.h"

void VelocityBufferFillStencil::applyLeftWall(FlowField &flowField, int i, int j)
{
    FLOAT vX = flowField.getVelocity().getVector(i, j)[0];
    FLOAT vY = flowField.getVelocity().getVector(i + 1, j)[1];
    buffers[LEFT][j] = {vX, vY, 0};
}

void VelocityBufferFillStencil::applyRightWall(FlowField &flowField, int i, int j)
{
    buffers[RIGHT][j] = flowField.getVelocity().getVector(i - 1, j);
}

void VelocityBufferFillStencil::applyBottomWall(FlowField &flowField, int i, int j)
{
    FLOAT vX = flowField.getVelocity().getVector(i, j + 1)[0];
    FLOAT vY = flowField.getVelocity().getVector(i, j)[1];
    buffers[BOTTOM][i] = {vX, vY, 0};
}

void VelocityBufferFillStencil::applyTopWall(FlowField &flowField, int i, int j)
{
    buffers[TOP][i] = flowField.getVelocity().getVector(i, j - 1);
}

void VelocityBufferFillStencil::applyLeftWall(FlowField &flowField, int i, int j, int k)
{
    FLOAT vX = flowField.getVelocity().getVector(i, j, k)[0];
    FLOAT vY = flowField.getVelocity().getVector(i + 1, j, k)[1];
    FLOAT vZ = flowField.getVelocity().getVector(i + 1, j, k)[2];
    int Nk = flowField.getCellsZ();
    buffers[LEFT][j * Nk + k] = {vX, vY, vZ};
//    buffers[LEFT][j * Nk + k].x = 1; //DEBUG
}

void VelocityBufferFillStencil::applyRightWall(FlowField &flowField, int i, int j, int k)
{
    int Nk = flowField.getCellsZ();
    buffers[RIGHT][j * Nk + k] = flowField.getVelocity().getVector(i - 1, j, k);
//    buffers[RIGHT][j * Nk + k].x = 1; //DEBUG
}

void VelocityBufferFillStencil::applyBottomWall(FlowField &flowField, int i, int j, int k)
{
    FLOAT vX = flowField.getVelocity().getVector(i, j + 1, k)[0];
    FLOAT vY = flowField.getVelocity().getVector(i, j, k)[1];
    FLOAT vZ = flowField.getVelocity().getVector(i, j + 1, k)[2];
    int Nk = flowField.getCellsZ();
    buffers[BOTTOM][i * Nk + k] = {vX, vY, vZ};
}

void VelocityBufferFillStencil::applyTopWall(FlowField &flowField, int i, int j, int k)
{
    int Nk = flowField.getCellsZ();
    buffers[TOP][i * Nk + k] = flowField.getVelocity().getVector(i, j - 1, k);
}

void VelocityBufferFillStencil::applyFrontWall(FlowField &flowField, int i, int j, int k)
{
    FLOAT vX = flowField.getVelocity().getVector(i, j, k + 1)[0];
    FLOAT vY = flowField.getVelocity().getVector(i, j, k + 1)[1];
    FLOAT vZ = flowField.getVelocity().getVector(i, j, k)[2];
    int Nj = flowField.getCellsY();
    buffers[FRONT][i * Nj + j] = {vX, vY, vZ};
}

void VelocityBufferFillStencil::applyBackWall(FlowField &flowField, int i, int j, int k)
{
    int Nj = flowField.getCellsY();
    buffers[BACK][i * Nj + j] = flowField.getVelocity().getVector(i, j, k - 1);
}
