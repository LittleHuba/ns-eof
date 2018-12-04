#include <boost/test/unit_test.hpp>
#include "../../src/stencils/StencilFunctions.h"
#include "../../src/Iterators.h"
#include "../TestingFunctions.h"

BOOST_AUTO_TEST_SUITE(vt_interpolation_Tests)

BOOST_AUTO_TEST_CASE(vt_interpolation1)
{
    auto parameters = new DummyParameters(3, 3);
    FLOAT _localViscosity [ 27 *3 ];
    FLOAT _localMeshsize [ 27 *3 ];
    auto flowField = new FlowField(3, 3, 3);
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            for(int k=0; k<3; k++){
                flowField->getTurbulentViscosity().getScalar(i,j,k)=2;
            }
        }
    }

    loadLocalViscosity3D(*flowField, _localViscosity, 1, 1, 1);
    loadLocalMeshsize3D(parameters->get(),_localMeshsize,1,1,1);

    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,0,0,2),2,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,1,0,2),2,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,0,1,2),2,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,1,1,2),2,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,2,0,0),2,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,2,0,1),2,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,2,1,1),2,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,2,1,0),2,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,0,2,0),2,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,1,2,0),2,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,0,2,1),2,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,1,2,1),2,0.001);

    //FLOAT vt_interpolate( const FLOAT * const localViscosity, const FLOAT * const lm, unsigned int i=2, unsigned int j=2, unsigned int k=2)
    delete flowField;
    delete parameters;
}


BOOST_AUTO_TEST_CASE(vt_interpolation2)
{
    auto parameters = new DummyParameters(3, 3);
    FLOAT _localViscosity [ 27 *3 ];
    FLOAT _localMeshsize [ 27 *3 ];
    auto flowField = new FlowField(3, 3, 3);
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            for(int k=0; k<3; k++){
                flowField->getTurbulentViscosity().getScalar(i,j,k)=0;
            }
        }
    }
    for(int i=0; i<3; i++){
        flowField->getTurbulentViscosity().getScalar(i,0,0)=i+1;
    }
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){

            flowField->getTurbulentViscosity().getScalar(i,j,0)=flowField->getTurbulentViscosity().getScalar(i,0,0);
        }
    }

    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            for(int k=0;k<3;k++){
                flowField->getTurbulentViscosity().getScalar(i,j,k)=flowField->getTurbulentViscosity().getScalar(i,j,0);
            }
        }
    }

    loadLocalViscosity3D(*flowField, _localViscosity, 1, 1, 1);
    loadLocalMeshsize3D(parameters->get(),_localMeshsize,1,1,1);

    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,0,0,2),1.5,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,1,0,2),2.5,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,0,1,2),1.5,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,1,1,2),2.5,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,2,0,0),2,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,2,0,1),2,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,2,1,1),2,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,2,1,0),2,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,0,2,0),1.5,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,1,2,0),2.5,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,0,2,1),1.5,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,1,2,1),2.5,0.001);

    //FLOAT vt_interpolate( const FLOAT * const localViscosity, const FLOAT * const lm, unsigned int i=2, unsigned int j=2, unsigned int k=2)
    delete flowField;
    delete parameters;
}


BOOST_AUTO_TEST_CASE(vt_interpolation3)
{
    auto parameters = new DummyParameters(3, 3);
    FLOAT _localViscosity [ 27 *3 ];
    FLOAT _localMeshsize [ 27 *3 ];
    auto flowField = new FlowField(3, 3, 3);
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            for(int k=0; k<3; k++){
                flowField->getTurbulentViscosity().getScalar(i,j,k)=0;
            }
        }
    }
    for(int k=0; k<3; k++){
        flowField->getTurbulentViscosity().getScalar(0,0,k)=k+1;
    }
    for(int k=0; k<3; k++){
        for(int j=0; j<3; j++){

            flowField->getTurbulentViscosity().getScalar(0,j,k)=flowField->getTurbulentViscosity().getScalar(0,0,k);
        }
    }

    for(int k=0;k<3;k++){
        for(int j=0;j<3;j++){
            for(int i=0;i<3;i++){
                flowField->getTurbulentViscosity().getScalar(i,j,k)=flowField->getTurbulentViscosity().getScalar(0,j,k);
            }
        }
    }

    loadLocalViscosity3D(*flowField, _localViscosity, 1, 1, 1);
    loadLocalMeshsize3D(parameters->get(),_localMeshsize,1,1,1);

    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,0,0,2),2,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,1,0,2),2,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,0,1,2),2,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,1,1,2),2,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,2,0,0),1.5,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,2,0,1),2.5,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,2,1,1),2.5,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,2,1,0),1.5,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,0,2,0),1.5,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,1,2,0),1.5,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,0,2,1),2.5,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,1,2,1),2.5,0.001);

    //FLOAT vt_interpolate( const FLOAT * const localViscosity, const FLOAT * const lm, unsigned int i=2, unsigned int j=2, unsigned int k=2)
    delete flowField;
    delete parameters;
}

BOOST_AUTO_TEST_CASE(vt_interpolation4)
{
    auto parameters = new DummyParameters(3, 3);
    FLOAT _localViscosity [ 27 *3 ];
    FLOAT _localMeshsize [ 27 *3 ];
    auto flowField = new FlowField(3, 3, 3);
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            for(int k=0; k<3; k++){
                flowField->getTurbulentViscosity().getScalar(i,j,k)=0;
            }
        }
    }
    for(int j=0; j<3; j++){
        flowField->getTurbulentViscosity().getScalar(0,j,0)=j+1;
    }
    for(int j=0; j<3; j++){
        for(int i=0; i<3; i++){

            flowField->getTurbulentViscosity().getScalar(i,j,0)=flowField->getTurbulentViscosity().getScalar(0,j,0);
        }
    }

    for(int j=0;j<3;j++){
        for(int i=0;i<3;i++){
            for(int k=0;k<3;k++){
                flowField->getTurbulentViscosity().getScalar(i,j,k)=flowField->getTurbulentViscosity().getScalar(i,j,0);
            }
        }
    }

    loadLocalViscosity3D(*flowField, _localViscosity, 1, 1, 1);
    loadLocalMeshsize3D(parameters->get(),_localMeshsize,1,1,1);

    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,0,0,2),1.5,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,1,0,2),1.5,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,0,1,2),2.5,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,1,1,2),2.5,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,2,0,0),1.5,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,2,0,1),1.5,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,2,1,1),2.5,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,2,1,0),2.5,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,0,2,0),2,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,1,2,0),2,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,0,2,1),2,0.001);
    BOOST_CHECK_CLOSE_FRACTION(vt_interpolate(_localViscosity,_localMeshsize,1,2,1),2,0.001);

    //FLOAT vt_interpolate( const FLOAT * const localViscosity, const FLOAT * const lm, unsigned int i=2, unsigned int j=2, unsigned int k=2)
    delete flowField;
    delete parameters;
}


BOOST_AUTO_TEST_SUITE_END()
