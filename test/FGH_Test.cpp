#include <boost/test/unit_test.hpp>
#include "../../src/stencils/StencilFunctions.h"
#include "../../src/Iterators.h"
#include "../TestingFunctions.h"

BOOST_AUTO_TEST_SUITE(FGH_Tests)


BOOST_AUTO_TEST_CASE(FGH3D_const)
{
    auto parameters = new DummyParameters(3, 3);

    FLOAT _localVelocity [ 27 *3 ];
    FLOAT _localMeshsize [ 27 *3 ];
    FLOAT _localViscosity [ 27*3 ];

    parameters->get().environment.gx=10;
    parameters->get().environment.gy=10;
    parameters->get().environment.gz=10;
    parameters->get().flow.Re=1000;

    auto flowField = new FlowField(3, 3, 3);
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            for(int k=0; k<3; k++){
                flowField->getVelocity().getVector(i,j,k)[0]=1;
                flowField->getVelocity().getVector(i,j,k)[1]=1;
                flowField->getVelocity().getVector(i,j,k)[2]=1;
                flowField->getTurbulentViscosity().getScalar(i,j,k)=1;
            }
        }
    }

    loadLocalViscosity3D(*flowField, _localViscosity, 1, 1, 1);
    loadLocalVelocity3D(*flowField,_localVelocity,1,1,1);
    loadLocalMeshsize3D(parameters->get(),_localMeshsize,1,1,1);


    BOOST_CHECK_CLOSE_FRACTION(computeTurbulentF3D(_localVelocity,_localMeshsize,_localViscosity,parameters->get(),0.1),2,0.001);
    BOOST_CHECK_CLOSE_FRACTION(computeTurbulentG3D(_localVelocity,_localMeshsize,_localViscosity,parameters->get(),0.1),2,0.001);
    BOOST_CHECK_CLOSE_FRACTION(computeTurbulentH3D(_localVelocity,_localMeshsize,_localViscosity,parameters->get(),0.1),2,0.001);


    delete flowField;
    delete parameters;
}



BOOST_AUTO_TEST_CASE(FGH2D_const)
{
    auto parameters = new DummyParameters(2, 3);

    FLOAT _localVelocity [ 27 *3 ];
    FLOAT _localMeshsize [ 27 *3 ];
    FLOAT _localViscosity [ 27*3 ];

    parameters->get().environment.gx=10;
    parameters->get().environment.gy=10;
    parameters->get().environment.gz=10;
    parameters->get().flow.Re=1000;

    auto flowField = new FlowField(3, 3);
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
                flowField->getVelocity().getVector(i,j)[0]=1;
                flowField->getVelocity().getVector(i,j)[1]=1;
                flowField->getTurbulentViscosity().getScalar(i,j)=1;
        }
    }

    loadLocalViscosity2D(*flowField, _localViscosity, 1, 1);
    loadLocalVelocity2D(*flowField,_localVelocity,1,1);
    loadLocalMeshsize2D(parameters->get(),_localMeshsize,1,1);


    BOOST_CHECK_CLOSE_FRACTION(computeTurbulentF2D(_localVelocity,_localMeshsize,_localViscosity,parameters->get(),0.1),2,0.001);
    BOOST_CHECK_CLOSE_FRACTION(computeTurbulentG2D(_localVelocity,_localMeshsize,_localViscosity,parameters->get(),0.1),2,0.001);


    delete flowField;
    delete parameters;
}

BOOST_AUTO_TEST_SUITE_END()
