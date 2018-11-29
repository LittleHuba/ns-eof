#include "ObstacleDistanceStencil.h"

ObstacleDistanceStencil::ObstacleDistanceStencil(const Parameters &parameters, FlowField &flowField) : FieldStencil(
        parameters) {
    int cellsX = parameters.parallel.localSize[0];
    int cellsY = parameters.parallel.localSize[1];
    int cellsZ = parameters.parallel.localSize[2];

    // Reinitialize the wall distance to the maximum value to simplify first iteration
    if (parameters.geometry.dim == 2) {
        for (int j = 2; j < cellsY; ++j) {
            for (int i = 2; i < cellsX; ++i) {
                flowField.getNearestWallDistance().getScalar(i, j) = MY_FLOAT_MAX;
            }
        }
    } else if (parameters.geometry.dim == 3) {
        for (int k = 2; k < cellsZ; ++k) {
            for (int j = 2; j < cellsY; ++j) {
                for (int i = 2; i < cellsX; ++i) {
                    flowField.getNearestWallDistance().getScalar(i, j, k) = MY_FLOAT_MAX;
                }
            }
        }
    }
}

inline void ObstacleDistanceStencil::apply(FlowField &flowField, int i, int j) {
    FLOAT &wallDistance = flowField.getNearestWallDistance().getScalar(i, j);

    /*
     * Manage obstacles
     * ================
     */
    int flags = flowField.getFlags().getValue(i, j);

    // Set distance to wall to 0 if cell is obstacle itself and return since wall distance won't get smaller
    if (flags & OBSTACLE_SELF) {
        wallDistance = 0;
        return;
    }

    // Remember if we found an obstacle with a shared face
    bool horiVertObstacle = false;

    // Set the distance to wall to half the cell width/height if it has an obstacle at one of its faces
    if (flags & OBSTACLE_TOP || flags & OBSTACLE_BOTTOM) {
        horiVertObstacle = true;
        wallDistance = std::min(wallDistance, _parameters.meshsize->getDy(i, j) / 2.0);
    }

    if (flags & OBSTACLE_LEFT || flags & OBSTACLE_RIGHT) {
        horiVertObstacle = true;
        wallDistance = std::min(wallDistance, _parameters.meshsize->getDx(i, j) / 2.0);
    }

    // If we found an obstacle with a shared face the distance won't get smaller, so return
    if (horiVertObstacle)
        return;

    // Check for obstacles at the edges of the cell
    if (flowField.getFlags().getValue(i + 1, j + 1) & OBSTACLE_SELF ||
        flowField.getFlags().getValue(i + 1, j - 1) & OBSTACLE_SELF ||
        flowField.getFlags().getValue(i - 1, j - 1) & OBSTACLE_SELF ||
        flowField.getFlags().getValue(i - 1, j + 1) & OBSTACLE_SELF) {
        wallDistance = std::min(wallDistance,
                                std::hypot(_parameters.meshsize->getDx(i, j), _parameters.meshsize->getDy(i, j)) / 2.0);
    }

    /*
     * Manage values of neighbors
     * ==========================
     */

    // Take minimum of x distance
    wallDistance = std::min(wallDistance,
                            std::min(flowField.getNearestWallDistance().getScalar(i - 1, j),
                                     flowField.getNearestWallDistance().getScalar(i + 1, j))
                            + _parameters.meshsize->getDx(i, j) / 2.0);


    // Take minimum of y distance
    wallDistance = std::min(wallDistance,
                            std::min(flowField.getNearestWallDistance().getScalar(i, j - 1),
                                     flowField.getNearestWallDistance().getScalar(i, j + 1))
                            + _parameters.meshsize->getDy(i, j) / 2.0);

    // Take minimum of diagonal distance
    wallDistance = std::min(wallDistance,
                            std::min(
                                    std::min(flowField.getNearestWallDistance().getScalar(i - 1, j - 1),
                                             flowField.getNearestWallDistance().getScalar(i + 1, j - 1)),
                                    std::min(flowField.getNearestWallDistance().getScalar(i - 1, j + 1),
                                             flowField.getNearestWallDistance().getScalar(i + 1, j + 1))
                            ) + std::hypot(_parameters.meshsize->getDx(i, j), _parameters.meshsize->getDy(i, j)) / 2.0);

}

inline void ObstacleDistanceStencil::apply(FlowField &flowField, int i, int j, int k) {
    FLOAT &wallDistance = flowField.getNearestWallDistance().getScalar(i, j, k);

    /*
     * Manage obstacles
     * ================
     */
    int flags = flowField.getFlags().getValue(i, j, k);
    // Set distance to wall to 0 if cell is obstacle itself and return since wall distance won't get smaller
    if (flags & OBSTACLE_SELF) {
        wallDistance = 0;
        return;
    }

    // Remember if we found an obstacle with a shared face
    bool horiVertObstacle = false;

    // Set the distance to wall to half the cell width/height if it has an obstacle at one of its faces
    if (flags & OBSTACLE_TOP || flags & OBSTACLE_BOTTOM) {
        horiVertObstacle = true;
        wallDistance = std::min(wallDistance, _parameters.meshsize->getDy(i, j, k) / 2.0);
    }

    if (flags & OBSTACLE_LEFT || flags & OBSTACLE_RIGHT) {
        horiVertObstacle = true;
        wallDistance = std::min(wallDistance, _parameters.meshsize->getDx(i, j, k) / 2.0);
    }

    if (flags & OBSTACLE_FRONT || flags & OBSTACLE_BACK) {
        horiVertObstacle = true;
        wallDistance = std::min(wallDistance, _parameters.meshsize->getDz(i, j, k) / 2.0);
    }

    // If we found an obstacle with a shared face the distance won't get smaller, so return
    if (horiVertObstacle)
        return;

    // Check for obstacles at the edges of the cell
    if (flowField.getFlags().getValue(i + 1, j + 1, k + 1) & OBSTACLE_SELF ||
        flowField.getFlags().getValue(i + 1, j + 1, k - 1) & OBSTACLE_SELF ||
        flowField.getFlags().getValue(i + 1, j - 1, k + 1) & OBSTACLE_SELF ||
        flowField.getFlags().getValue(i + 1, j - 1, k - 1) & OBSTACLE_SELF ||
        flowField.getFlags().getValue(i - 1, j + 1, k + 1) & OBSTACLE_SELF ||
        flowField.getFlags().getValue(i - 1, j + 1, k - 1) & OBSTACLE_SELF ||
        flowField.getFlags().getValue(i - 1, j - 1, k + 1) & OBSTACLE_SELF ||
        flowField.getFlags().getValue(i - 1, j - 1, k - 1) & OBSTACLE_SELF) {
        wallDistance = std::min(wallDistance,
                                std::hypot(std::hypot(_parameters.meshsize->getDx(i, j, k),
                                                      _parameters.meshsize->getDy(i, j, k)),
                                           _parameters.meshsize->getDz(i, j, k)) / 2.0);
    }

    /*
     * Manage values of neighbors
     * ==========================
     */

    // Take minimum of x distance
    wallDistance = std::min(wallDistance,
                            std::min(flowField.getNearestWallDistance().getScalar(i - 1, j, k),
                                     flowField.getNearestWallDistance().getScalar(i + 1, j, k))
                            + _parameters.meshsize->getDx(i, j, k) / 2.0);


    // Take minimum of y distance
    wallDistance = std::min(wallDistance,
                            std::min(flowField.getNearestWallDistance().getScalar(i, j - 1),
                                     flowField.getNearestWallDistance().getScalar(i, j + 1))
                            + _parameters.meshsize->getDy(i, j, k) / 2.0);

    // Take minimum of z distance
    wallDistance = std::min(wallDistance,
                            std::min(flowField.getNearestWallDistance().getScalar(i, j, k - 1),
                                     flowField.getNearestWallDistance().getScalar(i, j, k + 1))
                            + _parameters.meshsize->getDz(i, j, k) / 2.0);

    // Take minimum of diagonal distance
    wallDistance = std::min(wallDistance,
                            std::min(
                                    std::min(
                                            std::min(flowField.getNearestWallDistance().getScalar(i - 1, j - 1, k - 1),
                                                     flowField.getNearestWallDistance().getScalar(i + 1, j - 1, k - 1)),
                                            std::min(flowField.getNearestWallDistance().getScalar(i - 1, j + 1, k - 1),
                                                     flowField.getNearestWallDistance().getScalar(i + 1, j + 1, k - 1))
                                    ),
                                    std::min(
                                            std::min(flowField.getNearestWallDistance().getScalar(i - 1, j - 1, k + 1),
                                                     flowField.getNearestWallDistance().getScalar(i + 1, j - 1, k + 1)),
                                            std::min(flowField.getNearestWallDistance().getScalar(i - 1, j + 1, k + 1),
                                                     flowField.getNearestWallDistance().getScalar(i + 1, j + 1, k + 1))
                                    )
                            ) + std::hypot(std::hypot(_parameters.meshsize->getDx(i, j, k),
                                                      _parameters.meshsize->getDy(i, j, k)),
                                           _parameters.meshsize->getDz(i, j, k)) / 2.0);
}
