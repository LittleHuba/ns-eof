#include "ObstacleDistanceStencil.h"

ObstacleDistanceStencil::ObstacleDistanceStencil(const Parameters &parameters) : FieldStencil(parameters) {
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
        wallDistance = std::fmin(wallDistance, _parameters.meshsize->getDy(i, j) / 2.0);
    }

    if (flags & OBSTACLE_LEFT || flags & OBSTACLE_RIGHT) {
        horiVertObstacle = true;
        wallDistance = std::fmin(wallDistance, _parameters.meshsize->getDx(i, j) / 2.0);
    }

    // If we found an obstacle with a shared face the distance won't get smaller, so return
    if (horiVertObstacle)
        return;

    // Check for obstacles at the edges of the cell
    if (flowField.getFlags().getValue(i + 1, j + 1) & OBSTACLE_SELF ||
        flowField.getFlags().getValue(i + 1, j - 1) & OBSTACLE_SELF ||
        flowField.getFlags().getValue(i - 1, j - 1) & OBSTACLE_SELF ||
        flowField.getFlags().getValue(i - 1, j + 1) & OBSTACLE_SELF) {
        wallDistance = std::fmin(wallDistance,
                                std::hypot(_parameters.meshsize->getDx(i, j), _parameters.meshsize->getDy(i, j)) / 2.0);
    }

    /*
     * Manage values of neighbors
     * ==========================
     */

    // Take minimum of x distance
    wallDistance = std::fmin(wallDistance,
                            std::fmin(flowField.getNearestWallDistance().getScalar(i - 1, j),
                                     flowField.getNearestWallDistance().getScalar(i + 1, j))
                            + _parameters.meshsize->getDx(i, j) / 2.0);


    // Take minimum of y distance
    wallDistance = std::fmin(wallDistance,
                            std::fmin(flowField.getNearestWallDistance().getScalar(i, j - 1),
                                     flowField.getNearestWallDistance().getScalar(i, j + 1))
                            + _parameters.meshsize->getDy(i, j) / 2.0);

    // Take minimum of diagonal distance
    wallDistance = std::fmin(wallDistance,
                            std::fmin(
                                    std::fmin(flowField.getNearestWallDistance().getScalar(i - 1, j - 1),
                                             flowField.getNearestWallDistance().getScalar(i + 1, j - 1)),
                                    std::fmin(flowField.getNearestWallDistance().getScalar(i - 1, j + 1),
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
        wallDistance = std::fmin(wallDistance, _parameters.meshsize->getDy(i, j, k) / 2.0);
    }

    if (flags & OBSTACLE_LEFT || flags & OBSTACLE_RIGHT) {
        horiVertObstacle = true;
        wallDistance = std::fmin(wallDistance, _parameters.meshsize->getDx(i, j, k) / 2.0);
    }

    if (flags & OBSTACLE_FRONT || flags & OBSTACLE_BACK) {
        horiVertObstacle = true;
        wallDistance = std::fmin(wallDistance, _parameters.meshsize->getDz(i, j, k) / 2.0);
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
        wallDistance = std::fmin(wallDistance,
                                std::hypot(std::hypot(_parameters.meshsize->getDx(i, j, k),
                                                      _parameters.meshsize->getDy(i, j, k)),
                                           _parameters.meshsize->getDz(i, j, k)) / 2.0);
    }

    /*
     * Manage values of neighbors
     * ==========================
     */

    // Take minimum of x distance
    wallDistance = std::fmin(wallDistance,
                            std::fmin(flowField.getNearestWallDistance().getScalar(i - 1, j, k),
                                     flowField.getNearestWallDistance().getScalar(i + 1, j, k))
                            + _parameters.meshsize->getDx(i, j, k) / 2.0);


    // Take minimum of y distance
    wallDistance = std::fmin(wallDistance,
                            std::fmin(flowField.getNearestWallDistance().getScalar(i, j - 1, k),
                                     flowField.getNearestWallDistance().getScalar(i, j + 1, k))
                            + _parameters.meshsize->getDy(i, j, k) / 2.0);

    // Take minimum of z distance
    wallDistance = std::fmin(wallDistance,
                            std::fmin(flowField.getNearestWallDistance().getScalar(i, j, k - 1),
                                     flowField.getNearestWallDistance().getScalar(i, j, k + 1))
                            + _parameters.meshsize->getDz(i, j, k) / 2.0);

    // Take minimum of diagonal distance
    wallDistance = std::fmin(wallDistance,
                            std::fmin(
                                    std::fmin(
                                            std::fmin(flowField.getNearestWallDistance().getScalar(i - 1, j - 1, k - 1),
                                                     flowField.getNearestWallDistance().getScalar(i + 1, j - 1, k - 1)),
                                            std::fmin(flowField.getNearestWallDistance().getScalar(i - 1, j + 1, k - 1),
                                                     flowField.getNearestWallDistance().getScalar(i + 1, j + 1, k - 1))
                                    ),
                                    std::fmin(
                                            std::fmin(flowField.getNearestWallDistance().getScalar(i - 1, j - 1, k + 1),
                                                     flowField.getNearestWallDistance().getScalar(i + 1, j - 1, k + 1)),
                                            std::fmin(flowField.getNearestWallDistance().getScalar(i - 1, j + 1, k + 1),
                                                     flowField.getNearestWallDistance().getScalar(i + 1, j + 1, k + 1))
                                    )
                            ) + std::hypot(std::hypot(_parameters.meshsize->getDx(i, j, k),
                                                      _parameters.meshsize->getDy(i, j, k)),
                                           _parameters.meshsize->getDz(i, j, k)) / 2.0);
}
