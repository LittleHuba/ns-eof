//
// Created by keefe on 12/4/18.
//

#include "WallDistanceBufferReadStencil.h"
#include "Direction.h"


void WallDistanceBufferReadStencil::applyLeftWall(FlowField &flowField, int i, int j)
{
    //todo: check how to properly offset j in order to get a shorter array
    flowField.getNearestWallDistance().getScalar(i-1, j) = buffers[LEFT][j-2];
    // if(_parameters.parallel.rank ==0) std::cout << "Read Pressure Left Wall " << i << " " << j << " " << buffers[LEFT][j-2] << std::endl;
    // if(_parameters.parallel.rank==3) std::cout << "Reading left " << j - 2 << " " << std::endl;
}

void WallDistanceBufferReadStencil::applyRightWall(FlowField &flowField, int i, int j)
{
    flowField.getNearestWallDistance().getScalar(i+1, j) = buffers[RIGHT][j-2];
    // if(_parameters.parallel.rank==3) std::cout << "Reading right " << j - 2 << " " << std::endl;
    // if(_parameters.parallel.rank ==0) std::cout << "Read Pressure Right Wall " << i << " " << j << " " << buffers[RIGHT][j-2] << std::endl;
}

void WallDistanceBufferReadStencil::applyBottomWall(FlowField &flowField, int i, int j)
{
    //Here we assume low j is bottom!
    flowField.getNearestWallDistance().getScalar(i, j-1) = buffers[BOTTOM][i-2];
    // if(_parameters.parallel.rank ==0) std::cout << "Read Pressure Bottom Wall " << i << " " << j << " " << buffers[BOTTOM][i-2] << std::endl;
    // if(_parameters.parallel.rank==3) std::cout << "Reading bottom " << i - 2 << " " << std::endl;
}

void WallDistanceBufferReadStencil::applyTopWall(FlowField &flowField, int i, int j)
{
    flowField.getNearestWallDistance().getScalar(i,j +1) = buffers[TOP][i-2];
    // if(_parameters.parallel.rank ==0) std::cout << "Read Pressure Top Wall " << i << " " << j << " " << buffers[TOP][i-2] << std::endl;
    // if(_parameters.parallel.rank==3) std::cout << "Reading top " << i - 2 << " " << std::endl;
}

void WallDistanceBufferReadStencil::applyLeftWall(FlowField &flowField, int i, int j, int k)
{
    // int Nk = flowField.getCellsZ() - 1;
    int Nk = _parameters.parallel.localSize[2];
    // if(_parameters.parallel.rank==0) std::cout << "Left Wall Read" << "(" << i << ", " << j << ", " << k << ") " << (j-2) * Nk + (k-2) << std::endl;
    flowField.getNearestWallDistance().getScalar(i - 1, j, k) = buffers[LEFT][(j-2) * Nk + (k-2)]; // stored k-wise
}

void WallDistanceBufferReadStencil::applyRightWall(FlowField &flowField, int i, int j, int k)
{
    // int Nk = flowField.getCellsZ() - 1;
    int Nk = _parameters.parallel.localSize[2];
    // if(_parameters.parallel.rank==0) std::cout << "Right Wall Read" << "(" << i << ", " << j << ", " << k << ") " << (j-2) * Nk + (k-2) << std::endl;
    flowField.getNearestWallDistance().getScalar(i + 1, j, k) = buffers[RIGHT][(j-2) * Nk + (k-2)]; // stored k-wise
}

void WallDistanceBufferReadStencil::applyBottomWall(FlowField &flowField, int i, int j, int k)
{
    // int Nk = flowField.getCellsZ() - 1;
    int Nk = _parameters.parallel.localSize[2];
    flowField.getNearestWallDistance().getScalar(i, j - 1 , k) = buffers[BOTTOM][(i-2) * Nk + (k-2)]; // stored k-wise
}

void WallDistanceBufferReadStencil::applyTopWall(FlowField &flowField, int i, int j, int k)
{
    // int Nk = flowField.getCellsZ() - 1;
    int Nk = _parameters.parallel.localSize[2];
    flowField.getNearestWallDistance().getScalar(i, j + 1, k) = buffers[TOP][(i-2) * Nk + (k-2)]; // stored k-wise
}

void WallDistanceBufferReadStencil::applyFrontWall(FlowField &flowField, int i, int j, int k)
{
    //Here we assume low k is front!
    // int Nj = flowField.getCellsY() - 1;
    int Nj = _parameters.parallel.localSize[1];
    flowField.getNearestWallDistance().getScalar(i, j, k - 1) = buffers[FRONT][(i-2) * Nj + (j-2)]; // stored j-wise
}

void WallDistanceBufferReadStencil::applyBackWall(FlowField &flowField, int i, int j, int k)
{
    // int Nj = flowField.getCellsY() - 1;
    int Nj = _parameters.parallel.localSize[1];
    flowField.getNearestWallDistance().getScalar(i, j, k + 1) = buffers[BACK][(i-2) * Nj + (j-2)]; // stored j-wise
}
