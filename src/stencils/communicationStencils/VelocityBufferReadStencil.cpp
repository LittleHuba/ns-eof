//
// Created by tommaso on 23/11/18.
//

#include <cassert>
#include "VelocityBufferReadStencil.h"
#include "Direction.h"

void VelocityBufferReadStencil::applyLeftWall(FlowField &flowField, int i, int j)
{
    flowField.getVelocity().getVector(i - 1, j)[0] = buffers[LEFT][j-1].x;
    flowField.getVelocity().getVector(i, j)[1] = buffers[LEFT][j-1].y;
}

void VelocityBufferReadStencil::applyRightWall(FlowField &flowField, int i, int j)
{

    flowField.getVelocity().getVector(i, j)[0] = buffers[RIGHT][j-1].x;
    flowField.getVelocity().getVector(i, j)[1] = buffers[RIGHT][j-1].y;
}

void VelocityBufferReadStencil::applyBottomWall(FlowField &flowField, int i, int j)
{
    flowField.getVelocity().getVector(i, j)[0] = buffers[BOTTOM][i-1].x;
    flowField.getVelocity().getVector(i, j - 1)[1] = buffers[BOTTOM][i-1].y;
}

void VelocityBufferReadStencil::applyTopWall(FlowField &flowField, int i, int j)
{
    flowField.getVelocity().getVector(i, j)[0] = buffers[TOP][i-1].x;
    flowField.getVelocity().getVector(i, j)[1] = buffers[TOP][i-1].y;
}

void VelocityBufferReadStencil::applyLeftWall(FlowField &flowField, int i, int j, int k)
{
    // flowField.getVelocity().getVector(i - 1, j, k)[0] = buffers[LEFT][j].x;
    // flowField.getVelocity().getVector(i, j, k)[1] = buffers[LEFT][j].y;
    // flowField.getVelocity().getVector(i, j, k)[2] = buffers[LEFT][j].z;
    int Nk = flowField.getCellsZ() - 1;
    // if(_parameters.parallel.rank==0) std::cout << "Left Wall Read" << "(" << i << ", " << j << ", " << k << ") " << (j-1) * Nk + (k-1) << std::endl;
    flowField.getVelocity().getVector(i - 1, j, k)[0] = buffers[LEFT][(j-1) * Nk + (k-1)].x;
    flowField.getVelocity().getVector(i, j, k)[1] = buffers[LEFT][(j-1) * Nk + (k-1)].y;
    flowField.getVelocity().getVector(i, j, k)[2] = buffers[LEFT][(j-1) * Nk + (k-1)].z;
//    int rank;
//    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//    std::cout << ""
//    assert(buffers[LEFT][j].x == 1); //DEBUG
}

void VelocityBufferReadStencil::applyRightWall(FlowField &flowField, int i, int j, int k)
{
    int Nk = flowField.getCellsZ() - 1;
    // if(_parameters.parallel.rank==0) std::cout << "Right Wall Read" << "(" << i << ", " << j << ", " << k << ") " << (j-1) * Nk + (k-1) << std::endl;
    flowField.getVelocity().getVector(i, j, k)[0] = buffers[RIGHT][(j-1) * Nk + (k-1)].x;
    flowField.getVelocity().getVector(i, j, k)[1] = buffers[RIGHT][(j-1) * Nk + (k-1)].y;
    flowField.getVelocity().getVector(i, j, k)[2] = buffers[RIGHT][(j-1) * Nk + (k-1)].z;


//    assert(buffers[RIGHT][j].x == 1); //DEBUG
}

void VelocityBufferReadStencil::applyBottomWall(FlowField &flowField, int i, int j, int k)
{
    // flowField.getVelocity().getVector(i, j, k)[0] = buffers[BOTTOM][j].x;
    // flowField.getVelocity().getVector(i, j - 1, k)[1] = buffers[BOTTOM][j].y;
    // flowField.getVelocity().getVector(i, j, k)[2] = buffers[BOTTOM][j].z;

    int Nk = flowField.getCellsZ() - 1;
    flowField.getVelocity().getVector(i, j, k)[0] = buffers[BOTTOM][(i-1) * Nk + (k-1)].x;
    flowField.getVelocity().getVector(i, j - 1, k)[1] = buffers[BOTTOM][(i-1) * Nk + (k-1)].y;
    flowField.getVelocity().getVector(i, j, k)[2] = buffers[BOTTOM][(i-1) * Nk + (k-1)].z;
}

void VelocityBufferReadStencil::applyTopWall(FlowField &flowField, int i, int j, int k)
{
    int Nk = flowField.getCellsZ() - 1 ;
    flowField.getVelocity().getVector(i, j, k)[0] = buffers[TOP][(i-1) * Nk + (k-1)].x;
    flowField.getVelocity().getVector(i, j, k)[1] = buffers[TOP][(i-1) * Nk + (k-1)].y;
    flowField.getVelocity().getVector(i, j, k)[2] = buffers[TOP][(i-1) * Nk + (k-1)].z;
}

void VelocityBufferReadStencil::applyFrontWall(FlowField &flowField, int i, int j, int k)
{
    // flowField.getVelocity().getVector(i, j, k)[0] = buffers[FRONT][j].x;
    // flowField.getVelocity().getVector(i, j, k)[1] = buffers[FRONT][j].y;
    // flowField.getVelocity().getVector(i, j, k - 1)[2] = buffers[FRONT][j].z;
    int Nj = flowField.getCellsY() - 1;
    flowField.getVelocity().getVector(i, j, k)[0] = buffers[FRONT][(i-1) * Nj + (j-1)].x;
    flowField.getVelocity().getVector(i, j, k)[1] = buffers[FRONT][(i-1) * Nj + (j-1)].y;
    flowField.getVelocity().getVector(i, j, k - 1)[2] = buffers[FRONT][(i-1) * Nj + (j-1)].z;
}

void VelocityBufferReadStencil::applyBackWall(FlowField &flowField, int i, int j, int k)
{
    int Nj = flowField.getCellsY() - 1 ;
    flowField.getVelocity().getVector(i, j, k)[0] = buffers[BACK][(i-1) * Nj + (j-1)].x;
    flowField.getVelocity().getVector(i, j, k)[1] = buffers[BACK][(i-1) * Nj + (j-1)].y;
    flowField.getVelocity().getVector(i, j, k)[2] = buffers[BACK][(i-1) * Nj + (j-1)].z;
}
