//
// Created by ulrich on 26.11.18.
//

#include "WallDistanceStencil.h"

WallDistanceStencil::WallDistanceStencil(const Parameters &parameters, FlowField &flowField) : BoundaryStencil(parameters) {
    int cellsX = parameters.parallel.localSize[0];
    int cellsY = parameters.parallel.localSize[1];
    int cellsZ = parameters.parallel.localSize[2];

    // Reinitialize the wall distance to the maximum value to simplify first iteration
    if (parameters.geometry.dim == 2) {
        for (int j = 2; j < cellsY + 2; ++j) {
            for (int i = 2; i < cellsX + 2; ++i) {
                flowField.getNearestWallDistance().getScalar(i, j) = MY_FLOAT_MAX;
            }
        }
    } else if (parameters.geometry.dim == 3) {
        for (int k = 2; k < cellsZ + 2; ++k) {
            for (int j = 2; j < cellsY + 2; ++j) {
                for (int i = 2; i < cellsX + 2; ++i) {
                    flowField.getNearestWallDistance().getScalar(i, j, k) = MY_FLOAT_MAX;
                }
            }
        }
    }
}

void WallDistanceStencil::applyLeftWall(FlowField &flowField, int i, int j) {
    if (_parameters.walls.typeLeft == DIRICHLET && _parameters.walls.vectorLeft[0] == 0.0) {
        FLOAT &wallDistance = flowField.getNearestWallDistance().getScalar(i, j);
        wallDistance = std::min(wallDistance, _parameters.meshsize->getDx(i, j) / 2.0);
    }
}

void WallDistanceStencil::applyRightWall(FlowField &flowField, int i, int j) {
    if (_parameters.walls.typeRight == DIRICHLET && _parameters.walls.vectorRight[0] == 0.0) {
        FLOAT &wallDistance = flowField.getNearestWallDistance().getScalar(i, j);
        wallDistance = std::min(wallDistance, _parameters.meshsize->getDx(i, j) / 2.0);
    }
}

void WallDistanceStencil::applyBottomWall(FlowField &flowField, int i, int j) {
    if (_parameters.walls.typeBottom == DIRICHLET && _parameters.walls.vectorBottom[1] == 0.0) {
        FLOAT &wallDistance = flowField.getNearestWallDistance().getScalar(i, j);
        wallDistance = std::min(wallDistance, _parameters.meshsize->getDy(i, j) / 2.0);
    }
}

void WallDistanceStencil::applyTopWall(FlowField &flowField, int i, int j) {
    if (_parameters.walls.typeTop == DIRICHLET && _parameters.walls.vectorTop[1] == 0.0) {
        FLOAT &wallDistance = flowField.getNearestWallDistance().getScalar(i, j);
        wallDistance = std::min(wallDistance, _parameters.meshsize->getDy(i, j) / 2.0);
    }
}

void WallDistanceStencil::applyLeftWall(FlowField &flowField, int i, int j, int k) {
    if (_parameters.walls.typeLeft == DIRICHLET && _parameters.walls.vectorLeft[0] == 0.0) {
        FLOAT &wallDistance = flowField.getNearestWallDistance().getScalar(i, j, k);
        wallDistance = std::min(wallDistance, _parameters.meshsize->getDx(i, j, k) / 2.0);
    }
}

void WallDistanceStencil::applyRightWall(FlowField &flowField, int i, int j, int k) {
    if (_parameters.walls.typeRight == DIRICHLET && _parameters.walls.vectorRight[0] == 0.0) {
        FLOAT &wallDistance = flowField.getNearestWallDistance().getScalar(i, j, k);
        wallDistance = std::min(wallDistance, _parameters.meshsize->getDx(i, j, k) / 2.0);
    }
}

void WallDistanceStencil::applyBottomWall(FlowField &flowField, int i, int j, int k) {
    if (_parameters.walls.typeBottom == DIRICHLET && _parameters.walls.vectorBottom[1] == 0.0) {
        FLOAT &wallDistance = flowField.getNearestWallDistance().getScalar(i, j, k);
        wallDistance = std::min(wallDistance, _parameters.meshsize->getDy(i, j, k) / 2.0);
    }
}

void WallDistanceStencil::applyTopWall(FlowField &flowField, int i, int j, int k) {
    if (_parameters.walls.typeTop == DIRICHLET && _parameters.walls.vectorTop[1] == 0.0) {
        FLOAT &wallDistance = flowField.getNearestWallDistance().getScalar(i, j, k);
        wallDistance = std::min(wallDistance, _parameters.meshsize->getDy(i, j, k) / 2.0);
    }
}

void WallDistanceStencil::applyFrontWall(FlowField &flowField, int i, int j, int k) {
    if (_parameters.walls.typeFront == DIRICHLET && _parameters.walls.vectorFront[2] == 0.0) {
        FLOAT &wallDistance = flowField.getNearestWallDistance().getScalar(i, j, k);
        wallDistance = std::min(wallDistance, _parameters.meshsize->getDz(i, j, k) / 2.0);
    }
}

void WallDistanceStencil::applyBackWall(FlowField &flowField, int i, int j, int k) {
    if (_parameters.walls.typeBack == DIRICHLET && _parameters.walls.vectorBack[2] == 0.0) {
        FLOAT &wallDistance = flowField.getNearestWallDistance().getScalar(i, j, k);
        wallDistance = std::min(wallDistance, _parameters.meshsize->getDz(i, j, k) / 2.0);
    }
}
