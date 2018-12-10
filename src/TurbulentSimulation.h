//
// Created by ulrich on 26.11.18.
//

#ifndef NS_TURBULENTSIMULATION_H
#define NS_TURBULENTSIMULATION_H

#include <stencils/TurbulenceVTKStencil.h>
#include "Simulation.h"
#include "stencils/WallDistanceStencil.h"
#include "stencils/ObstacleDistanceStencil.h"


class TurbulentSimulation : public Simulation {
protected:
    WallDistanceStencil _wallDistanceStencil;
    ObstacleDistanceStencil _obstacleDistanceStencil;
    GlobalBoundaryIterator<FlowField> _wallDistanceIterator;
    FieldIterator<FlowField> _obstacleDistanceIterator;
    TurbulenceVTKStencil _turbulenceVTKStencil;
    FieldIterator<FlowField>_turbulenceVTKIterator;
    TurbulentViscosityStencil _turbulentViscosityStencil;
    FieldIterator<FlowField> _turbulentViscosityIterator;
    TurbulenceFGHStencil _turbulenceFGHStencil;
    FieldIterator<FlowField> _turbulenceFGHIterator;
public:
    TurbulentSimulation(Parameters &parameters, FlowField &flowField) :
    Simulation(parameters, flowField),
    _wallDistanceStencil(parameters, flowField),
    _obstacleDistanceStencil(parameters),
    _wallDistanceIterator(flowField, parameters, _wallDistanceStencil, 2, -1),
    _obstacleDistanceIterator(flowField, parameters, _obstacleDistanceStencil, 1),
    _turbulenceVTKStencil(parameters),
    _turbulenceVTKIterator(flowField, parameters, _turbulenceVTKStencil, 1),
    _turbulentViscosityStencil(parameters),
    _turbulentViscosityIterator(flowField, parameters, _turbulentViscosityStencil, 1),
    _turbulenceFGHStencil(parameters),
    _turbulenceFGHIterator(flowField, parameters, _turbulenceFGHStencil) {

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

    void solveTimestep() override {
        // int rank = _parameters.parallel.rank;
        // determine and set max. timestep which is allowed in this simulation
        setTimeStep();
        // compute turbulent viscosity
        _turbulentViscosityIterator.iterate();
        // compute fgh
        _turbulenceFGHIterator.iterate();
        // set global boundary values
        _wallFGHIterator.iterate();
        // compute the right hand side
        _rhsIterator.iterate();
        // if(rank==1)printArray(1, "Before Pressure solve");
        // solve for pressure
        _solver.solve();
        // if(rank==1)printArray(1, "After Pressure solve");
        // communicate pressure values
        _parallelManager.exchangePressure();
        // if(rank==1)printArray(1, "After Pressure exchange");

        // compute velocity
        _velocityIterator.iterate();
        // set obstacle boundaries
        // if(rank==1)printArray(1, "After Velocity solve");
        _obstacleIterator.iterate();
        // communicate velocity values
        // if(rank==1)printArray(1, "After boundary updates");
        _parallelManager.exchangeVelocity();
        _parallelManager.exchangeViscosity();
        // if(rank==1)printArray(1, "After velocity communication");
        // Iterate for velocities on the boundary
        _wallVelocityIterator.iterate();
    }

    /** plots the flow field. */
    void plotVTK(int timeStep) override {
        // Skip output when timestep equals zero
        if(timeStep == 0.0)
            return;

        // iterate stencil over _flowField
        _turbulenceVTKIterator.iterate();
        // write flow field information to vtk file
        _turbulenceVTKStencil.write(timeStep);
    }
};

#endif //NS_TURBULENTSIMULATION_H
