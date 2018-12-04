//
// Created by tommaso on 23/11/18.
//

#include "VelocityBufferFillStencil.h"
#include "Direction.h"

void VelocityBufferFillStencil::applyLeftWall(FlowField &flowField, int i, int j)
{
    buffers[LEFT][j-1] = flowField.getVelocity().getVector(i + 1, j);
    // if(_parameters.parallel.rank==3) std::cout << "left " << i << " " << j << " " << flowField.getVelocity().getVector(i , j)[0] << std::endl;
}

void VelocityBufferFillStencil::applyRightWall(FlowField &flowField, int i, int j)
{
    FLOAT vX = flowField.getVelocity().getVector(i - 2, j)[0];
    FLOAT vY = flowField.getVelocity().getVector(i - 1, j)[1];
    // buffers[RIGHT][j] = flowField.getVelocity().getVector(i - 1, j);
    buffers[RIGHT][j-1] = {vX, vY, 0};

    // if(_parameters.parallel.rank==3) std::cout << "right " << i << " " << j << " " << flowField.getVelocity().getVector(i - 1, j)[0] << std::endl;


    // std::cout << buffers[RIGHT][j].x << " " << buffers[RIGHT][j].y << " " << buffers[RIGHT][j].z << std::endl;
}

void VelocityBufferFillStencil::applyBottomWall(FlowField &flowField, int i, int j)
{
    buffers[BOTTOM][i-1] = flowField.getVelocity().getVector(i, j + 1);
    // if(_parameters.parallel.rank==3) std::cout << "bottom " << i << " " << j << " " << flowField.getVelocity().getVector(i , j)[0] << std::endl;
}

void VelocityBufferFillStencil::applyTopWall(FlowField &flowField, int i, int j)
{
    FLOAT vX = flowField.getVelocity().getVector(i, j - 1)[0];
    FLOAT vY = flowField.getVelocity().getVector(i, j - 2)[1];
    // buffers[TOP][i] = flowField.getVelocity().getVector(i, j - 1);
    buffers[TOP][i-1] = {vX, vY, 0};
    // if(_parameters.parallel.rank==3) std::cout << "top " << i << " " << j << " " << flowField.getVelocity().getVector(i , j)[0] << std::endl;
}

void VelocityBufferFillStencil::applyLeftWall(FlowField &flowField, int i, int j, int k)
{
    // FLOAT vX = flowField.getVelocity().getVector(i, j, k)[0];
    // FLOAT vY = flowField.getVelocity().getVector(i + 1, j, k)[1];
    // FLOAT vZ = flowField.getVelocity().getVector(i + 1, j, k)[2];
    int Nk = flowField.getCellsZ() - 1;
    // if(_parameters.parallel.rank==0) std::cout << "Left Wall Fill" << "(" << i << ", " << j << ", " << k << ") " << (j-1) * Nk + (k-1) << std::endl;
    // buffers[LEFT][j * Nk + k] = {vX, vY, vZ};
    buffers[LEFT][(j-1) * Nk + (k-1)] = flowField.getVelocity().getVector(i + 1, j, k);
//    buffers[LEFT][j * Nk + k].x = 1; //DEBUG
}

void VelocityBufferFillStencil::applyRightWall(FlowField &flowField, int i, int j, int k)
{
    FLOAT vX = flowField.getVelocity().getVector(i - 2, j, k)[0];
    FLOAT vY = flowField.getVelocity().getVector(i - 1, j, k)[1];
    FLOAT vZ = flowField.getVelocity().getVector(i - 1, j, k)[2];
    int Nk = flowField.getCellsZ() - 1;
    // if(_parameters.parallel.rank==0) std::cout << "Right Wall Fill" << "(" << i << ", " << j << ", " << k << ") " << (j-1) * Nk + (k-1) << std::endl;
    // buffers[RIGHT][j * Nk + k] = flowField.getVelocity().getVector(i - 1, j, k);
    buffers[RIGHT][(j-1) * Nk + (k-1)] = {vX, vY, vZ};
//    buffers[RIGHT][j * Nk + k].x = 1; //DEBUG
}

void VelocityBufferFillStencil::applyBottomWall(FlowField &flowField, int i, int j, int k)
{
    // FLOAT vX = flowField.getVelocity().getVector(i, j + 1, k)[0];
    // FLOAT vY = flowField.getVelocity().getVector(i, j, k)[1];
    // FLOAT vZ = flowField.getVelocity().getVector(i, j + 1, k)[2];
    int Nk = flowField.getCellsZ() - 1;
    // buffers[BOTTOM][i * Nk + k] = {vX, vY, vZ};
    buffers[BOTTOM][(i-1) * Nk + (k-1)] = flowField.getVelocity().getVector(i, j + 1, k);
}

void VelocityBufferFillStencil::applyTopWall(FlowField &flowField, int i, int j, int k)
{
    FLOAT vX = flowField.getVelocity().getVector(i, j - 1, k)[0];
    FLOAT vY = flowField.getVelocity().getVector(i, j - 2, k)[1];
    FLOAT vZ = flowField.getVelocity().getVector(i, j - 1, k)[2];
    int Nk = flowField.getCellsZ() - 1;
    // buffers[TOP][i * Nk + k] = flowField.getVelocity().getVector(i, j - 1, k);
    buffers[TOP][(i-1) * Nk + (k-1)] = {vX, vY, vZ};
}

void VelocityBufferFillStencil::applyFrontWall(FlowField &flowField, int i, int j, int k)
{
    // FLOAT vX = flowField.getVelocity().getVector(i, j, k + 1)[0];
    // FLOAT vY = flowField.getVelocity().getVector(i, j, k + 1)[1];
    // FLOAT vZ = flowField.getVelocity().getVector(i, j, k)[2];
    int Nj = flowField.getCellsY() - 1;
    // buffers[FRONT][i * Nj + j] = {vX, vY, vZ};
    buffers[FRONT][(i-1) * Nj + (j-1)] = flowField.getVelocity().getVector(i, j, k + 1);
}

void VelocityBufferFillStencil::applyBackWall(FlowField &flowField, int i, int j, int k)
{
    FLOAT vX = flowField.getVelocity().getVector(i, j, k - 1)[0];
    FLOAT vY = flowField.getVelocity().getVector(i, j, k - 1)[1];
    FLOAT vZ = flowField.getVelocity().getVector(i, j, k - 2)[2];
    int Nj = flowField.getCellsY() - 1;
    // buffers[BACK][i * Nj + j] = flowField.getVelocity().getVector(i, j, k - 1);
    buffers[BACK][(i-1) * Nj + (j-1)] = {vX, vY, vZ};
}
