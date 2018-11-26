//
// Created by ulrich on 26.11.18.
//

#include "WallDistanceStencil.h"

WallDistanceStencil::WallDistanceStencil(const Parameters &parameters) : BoundaryStencil(parameters) {

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
    if (_parameters.walls.typeBottom == DIRICHLET && _parameters.walls.vectorBottom[2] == 0.0) {
        FLOAT &wallDistance = flowField.getNearestWallDistance().getScalar(i, j, k);
        wallDistance = std::min(wallDistance, _parameters.meshsize->getDy(i, j, k) / 2.0);
    }
}

void WallDistanceStencil::applyTopWall(FlowField &flowField, int i, int j, int k) {
    if (_parameters.walls.typeTop == DIRICHLET && _parameters.walls.vectorTop[2] == 0.0) {
        FLOAT &wallDistance = flowField.getNearestWallDistance().getScalar(i, j, k);
        wallDistance = std::min(wallDistance, _parameters.meshsize->getDy(i, j, k) / 2.0);
    }
}

void WallDistanceStencil::applyFrontWall(FlowField &flowField, int i, int j, int k) {
    if (_parameters.walls.typeFront == DIRICHLET && _parameters.walls.vectorFront[1] == 0.0) {
        FLOAT &wallDistance = flowField.getNearestWallDistance().getScalar(i, j, k);
        wallDistance = std::min(wallDistance, _parameters.meshsize->getDz(i, j, k) / 2.0);
    }
}

void WallDistanceStencil::applyBackWall(FlowField &flowField, int i, int j, int k) {
    if (_parameters.walls.typeBack == DIRICHLET && _parameters.walls.vectorBack[1] == 0.0) {
        FLOAT &wallDistance = flowField.getNearestWallDistance().getScalar(i, j, k);
        wallDistance = std::min(wallDistance, _parameters.meshsize->getDz(i, j, k) / 2.0);
    }
}
