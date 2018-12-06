#include "TurbulentViscosityStencil.h"
#include "StencilFunctions.h"
#include "Definitions.h"

TurbulentViscosityStencil::TurbulentViscosityStencil(const Parameters &parameters) : FieldStencil<FlowField>(parameters) {}


void TurbulentViscosityStencil::apply(FlowField &flowField, int i, int j)
{
    loadLocalVelocity2D(  flowField, _localVelocity, i, j);
    loadLocalMeshsize2D(_parameters, _localMeshsize, i, j);

    FLOAT delta;
    //length in x direction
    FLOAT x=_parameters.meshsize->getPosX(i,j);
    //Boundary layer thickness for turbulent boundary layer
    delta=0.382*x/std::pow(_parameters.flow.Re*x/_parameters.geometry.lengthX,0.2);

    if(0.09*delta>flowField.getNearestWallDistance().getScalar(i,j))
    {
        flowField.getTurbulentViscosity().getScalar(i,j)=_parameters.turbulence.kappa*_parameters.turbulence.kappa
                *flowField.getNearestWallDistance().getScalar(i,j)*flowField.getNearestWallDistance().getScalar(i,j)
                *computeSTP2D(_localVelocity, _localMeshsize);
    }

    else
    {
        flowField.getTurbulentViscosity().getScalar(i,j)=delta*delta*computeSTP2D(_localVelocity, _localMeshsize);
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
        FLOAT x=_parameters.meshsize->getPosX(i,j,k);
        //Boundary layer thickness for turbulent boundary layer
        delta=0.382*x/std::pow(_parameters.flow.Re*x/_parameters.geometry.lengthX,0.2);

        if(0.09*delta>flowField.getNearestWallDistance().getScalar(i,j,k))
        {
            flowField.getTurbulentViscosity().getScalar(i,j,k)=_parameters.turbulence.kappa*_parameters.turbulence.kappa
                    *flowField.getNearestWallDistance().getScalar(i,j,k)*flowField.getNearestWallDistance().getScalar(i,j,k)
                    *computeSTP3D(_localVelocity, _localMeshsize);
        }

        else
        {
            flowField.getTurbulentViscosity().getScalar(i,j,k)=delta*delta
                    *computeSTP3D(_localVelocity, _localMeshsize);
        }
    }




}
