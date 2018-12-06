//
// Created by ulrich on 26.11.18.
//

#ifndef NS_TURBULENTSIMULATION_H
#define NS_TURBULENTSIMULATION_H

#include "Simulation.h"
#include "stencils/WallDistanceStencil.h"
#include "stencils/ObstacleDistanceStencil.h"

class TurbulentSimulation : public Simulation {
protected:
    WallDistanceStencil _wallDistanceStencil;
    ObstacleDistanceStencil _obstacleDistanceStencil;
    GlobalBoundaryIterator<FlowField> _wallDistanceIterator;
    FieldIterator<FlowField> _obstacleDistanceIterator;
public:
    TurbulentSimulation(Parameters &parameters, FlowField &flowField) :
    _wallDistanceStencil(parameters, flowField),
    _obstacleDistanceStencil(parameters),
    _wallDistanceIterator(flowField, parameters, _wallDistanceStencil, 2, -1),
    _obstacleDistanceIterator(flowField, parameters, _obstacleDistanceStencil, 1),
    Simulation(parameters, flowField) {

    }

    void initializeFlowField() override {
        Simulation::initializeFlowField();

        // Iterate over the global boundaries to once get the walls
        _wallDistanceIterator.iterate();

        // Iterate twice (once forward once backward) to get the right distances to obstacles
        _obstacleDistanceIterator.iterate();
        _obstacleDistanceIterator.iterateBackwards();

        // transfer wall distance
        _parallelManager.exchangeWallDistance();

        _obstacleDistanceIterator.iterate();
        _obstacleDistanceIterator.iterateBackwards();
    }
};

#endif //NS_TURBULENTSIMULATION_H
