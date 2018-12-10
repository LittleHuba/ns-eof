#include "TurbulentViscosityStencil.h"
#include "StencilFunctions.h"
#include "Definitions.h"

TurbulentViscosityStencil::TurbulentViscosityStencil(const Parameters &parameters) : FieldStencil<FlowField>(parameters) {}


void TurbulentViscosityStencil::apply(FlowField &flowField, int i, int j)
{
    const int obstacle = flowField.getFlags().getValue(i, j);

    if ((obstacle & OBSTACLE_SELF) == 0){   // If the cell is fluid
      
        loadLocalVelocity2D(  flowField, _localVelocity, i, j);
        loadLocalMeshsize2D(_parameters, _localMeshsize, i, j);

        FLOAT delta;
        //length in x direction
        FLOAT x=_parameters.meshsize->getPosX(i,j) + _parameters.meshsize->getDx(i, j) / 2;
        //Boundary layer thickness for turbulent boundary layer
        delta=4.91*x/std::sqrt(_parameters.flow.Re*x/_parameters.geometry.lengthX);

        FLOAT &viscosity = flowField.getTurbulentViscosity().getScalar(i, j);
        FLOAT kappa = _parameters.turbulence.kappa;
        FLOAT &h = flowField.getNearestWallDistance().getScalar(i, j);
        FLOAT mixingLength = fmin(kappa * h, 0.09 * delta);
        viscosity = mixingLength * mixingLength * computeSTP2D(_localVelocity, _localMeshsize);
    }
}



void TurbulentViscosityStencil::apply(FlowField &flowField, int i, int j, int k)
{
    const int obstacle = flowField.getFlags().getValue(i, j, k);

    if ((obstacle & OBSTACLE_SELF) == 0){   // If the cell is fluid

        loadLocalVelocity3D(  flowField, _localVelocity, i, j, k);
        loadLocalMeshsize3D(_parameters, _localMeshsize, i, j, k);

        FLOAT delta;
        //length in x direction
        FLOAT x=_parameters.meshsize->getPosX(i,j,k) + _parameters.meshsize->getDx(i, j, k) / 2;
        //Boundary layer thickness for turbulent boundary layer
        delta=4.91*x/std::sqrt(_parameters.flow.Re*x/_parameters.geometry.lengthX);

        FLOAT &viscosity = flowField.getTurbulentViscosity().getScalar(i, j, k);
        FLOAT kappa = _parameters.turbulence.kappa;
        FLOAT &h = flowField.getNearestWallDistance().getScalar(i, j, k);
        FLOAT mixingLength = fmin(kappa * h, 0.09 * delta);
        viscosity = mixingLength * mixingLength * computeSTP2D(_localVelocity, _localMeshsize);
    }
}
