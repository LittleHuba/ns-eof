#include <boost/test/unit_test.hpp>
#include "../../src/stencils/StencilFunctions.h"
#include "../../src/Iterators.h"
#include "../TestingFunctions.h"

BOOST_AUTO_TEST_SUITE(STP_Tests)

BOOST_AUTO_TEST_CASE(computeSTP2D_Test_ukonst)
{
    auto parameters = new DummyParameters(2, 3);
    FLOAT _localVelocity [ 27 *3 ];
    FLOAT _localMeshsize [ 27 *3 ];
    auto flowField = new FlowField(3, 3);
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
                flowField->getVelocity().getVector(i,j)[0]=2;
        }
    }

    loadLocalVelocity2D(*flowField,_localVelocity,1,1);
    loadLocalMeshsize2D(parameters->get(),_localMeshsize,1,1);

    BOOST_CHECK_CLOSE_FRACTION(computeSTP2D(_localVelocity,_localMeshsize),0,0.001);

    delete flowField;
    delete parameters;
}

BOOST_AUTO_TEST_CASE(computeSTP2D_Test_1)
{
    auto parameters = new DummyParameters(2, 3);
    FLOAT _localVelocity [ 27 *3 ];
    FLOAT _localMeshsize [ 27 *3 ];
    auto flowField = new FlowField(3, 3);
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
                flowField->getVelocity().getVector(i,j)[0]=i+j;
                flowField->getVelocity().getVector(i,j)[1]=j+i;
        }
    }

    loadLocalVelocity2D(*flowField,_localVelocity,1,1);
    loadLocalMeshsize2D(parameters->get(),_localMeshsize,1,1);

    BOOST_CHECK_CLOSE_FRACTION(computeSTP2D(_localVelocity,_localMeshsize),std::sqrt(8/(0.333333*0.3333333)),0.001);

    delete flowField;
    delete parameters;
}


BOOST_AUTO_TEST_CASE(computeSTP3D_Test_ukonst)
{
    auto parameters = new DummyParameters(3, 3);
    FLOAT _localVelocity [ 27 *3 ];
    FLOAT _localMeshsize [ 27 *3 ];
    auto flowField = new FlowField(3, 3, 3);
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            for(int k=0; k<3; k++){
                flowField->getVelocity().getVector(i,j,k)[0]=2;
            }
        }
    }

    loadLocalVelocity3D(*flowField,_localVelocity,1,1,1);
    loadLocalMeshsize3D(parameters->get(),_localMeshsize,1,1,1);

    BOOST_CHECK_CLOSE_FRACTION(computeSTP3D(_localVelocity,_localMeshsize),0,0.001);

    delete flowField;
    delete parameters;
}

BOOST_AUTO_TEST_CASE(computeSTP3D_Test_ulinear)
{
    auto parameters = new DummyParameters(3, 3);
    FLOAT _localVelocity [ 27 *3 ];
    FLOAT _localMeshsize [ 27 *3 ];
    auto flowField = new FlowField(3, 3, 3);
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            for(int k=0; k<3; k++){
                flowField->getVelocity().getVector(i,j,k)[0]=i+j+k;
                flowField->getVelocity().getVector(i,j,k)[1]=i+j+k;
                flowField->getVelocity().getVector(i,j,k)[2]=i+j+k;
            }
        }
    }

    loadLocalVelocity3D(*flowField,_localVelocity,1,1,1);
    loadLocalMeshsize3D(parameters->get(),_localMeshsize,1,1,1);

    BOOST_CHECK_CLOSE_FRACTION(computeSTP3D(_localVelocity,_localMeshsize),9*std::sqrt(2),0.001);

    delete flowField;
    delete parameters;
}


BOOST_AUTO_TEST_CASE(Finitedifference3D_linear_rise)
{
    auto parameters = new DummyParameters(3, 3);
    FLOAT _localVelocity [ 27 *3 ];
    FLOAT _localMeshsize [ 27 *3 ];
    auto flowField = new FlowField(3, 3, 3);
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            for(int k=0; k<3; k++){
                flowField->getVelocity().getVector(i,j,k)[0]=i+j+k;
                flowField->getVelocity().getVector(i,j,k)[1]=i+j+k;
                flowField->getVelocity().getVector(i,j,k)[2]=i+j+k;
            }
        }
    }

    loadLocalVelocity3D(*flowField,_localVelocity,1,1,1);
    loadLocalMeshsize3D(parameters->get(),_localMeshsize,1,1,1);

    BOOST_CHECK_CLOSE_FRACTION(dudx_f(_localVelocity,_localMeshsize),1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dvdy_f(_localVelocity,_localMeshsize),1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dwdz_f(_localVelocity,_localMeshsize),1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dudx_b(_localVelocity,_localMeshsize),1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dvdy_b(_localVelocity,_localMeshsize),1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dwdz_b(_localVelocity,_localMeshsize),1/0.333333,0.001);
//mixed
    BOOST_CHECK_CLOSE_FRACTION(dudym(_localVelocity,_localMeshsize),1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dudzm(_localVelocity,_localMeshsize),1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dvdxm(_localVelocity,_localMeshsize),1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dvdzm(_localVelocity,_localMeshsize),1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dwdxm(_localVelocity,_localMeshsize),1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dwdym(_localVelocity,_localMeshsize),1/0.333333,0.001);
//other
    BOOST_CHECK_CLOSE_FRACTION(dvdz_j1(_localVelocity,_localMeshsize),1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dudz_i1(_localVelocity,_localMeshsize),1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dwdy_k1(_localVelocity,_localMeshsize),1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dudy_i1(_localVelocity,_localMeshsize),1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dwdx_k1(_localVelocity,_localMeshsize),1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dvdx_j1(_localVelocity,_localMeshsize),1/0.333333,0.001);

    BOOST_CHECK_CLOSE_FRACTION(dudyb(_localVelocity,_localMeshsize),1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dudyf(_localVelocity,_localMeshsize),1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dudzb(_localVelocity,_localMeshsize),1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dudzf(_localVelocity,_localMeshsize),1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dwdxb(_localVelocity,_localMeshsize),1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dwdxf(_localVelocity,_localMeshsize),1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dwdyb(_localVelocity,_localMeshsize),1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dwdyf(_localVelocity,_localMeshsize),1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dvdxb(_localVelocity,_localMeshsize),1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dvdxf(_localVelocity,_localMeshsize),1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dvdzb(_localVelocity,_localMeshsize),1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dvdzf(_localVelocity,_localMeshsize),1/0.333333,0.001);


    delete flowField;
    delete parameters;
}

BOOST_AUTO_TEST_CASE(Finitedifference3D_const)
{
    auto parameters = new DummyParameters(3, 3);
    FLOAT _localVelocity [ 27 *3 ];
    FLOAT _localMeshsize [ 27 *3 ];
    auto flowField = new FlowField(3, 3, 3);
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            for(int k=0; k<3; k++){
                flowField->getVelocity().getVector(i,j,k)[0]=1;
                flowField->getVelocity().getVector(i,j,k)[1]=1;
                flowField->getVelocity().getVector(i,j,k)[2]=1;
            }
        }
    }

    loadLocalVelocity3D(*flowField,_localVelocity,1,1,1);
    loadLocalMeshsize3D(parameters->get(),_localMeshsize,1,1,1);

    BOOST_CHECK_CLOSE_FRACTION(dudx_f(_localVelocity,_localMeshsize),0,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dvdy_f(_localVelocity,_localMeshsize),0,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dwdz_f(_localVelocity,_localMeshsize),0,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dudx_b(_localVelocity,_localMeshsize),0,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dvdy_b(_localVelocity,_localMeshsize),0,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dwdz_b(_localVelocity,_localMeshsize),0,0.001);
//mixed
    BOOST_CHECK_CLOSE_FRACTION(dudym(_localVelocity,_localMeshsize),0,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dudzm(_localVelocity,_localMeshsize),0,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dvdxm(_localVelocity,_localMeshsize),0,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dvdzm(_localVelocity,_localMeshsize),0,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dwdxm(_localVelocity,_localMeshsize),0,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dwdym(_localVelocity,_localMeshsize),0,0.001);
//other
    BOOST_CHECK_CLOSE_FRACTION(dvdz_j1(_localVelocity,_localMeshsize),0,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dudz_i1(_localVelocity,_localMeshsize),0,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dwdy_k1(_localVelocity,_localMeshsize),0,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dudy_i1(_localVelocity,_localMeshsize),0,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dwdx_k1(_localVelocity,_localMeshsize),0,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dvdx_j1(_localVelocity,_localMeshsize),0,0.001);

    BOOST_CHECK_CLOSE_FRACTION(dudyb(_localVelocity,_localMeshsize),0,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dudyf(_localVelocity,_localMeshsize),0,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dudzb(_localVelocity,_localMeshsize),0,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dudzf(_localVelocity,_localMeshsize),0,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dwdxb(_localVelocity,_localMeshsize),0,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dwdxf(_localVelocity,_localMeshsize),0,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dwdyb(_localVelocity,_localMeshsize),0,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dwdyf(_localVelocity,_localMeshsize),0,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dvdxb(_localVelocity,_localMeshsize),0,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dvdxf(_localVelocity,_localMeshsize),0,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dvdzb(_localVelocity,_localMeshsize),0,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dvdzf(_localVelocity,_localMeshsize),0,0.001);


    delete flowField;
    delete parameters;
}

BOOST_AUTO_TEST_CASE(Finitedifference3D_linear_decrease)
{
    auto parameters = new DummyParameters(3, 3);
    FLOAT _localVelocity [ 27 *3 ];
    FLOAT _localMeshsize [ 27 *3 ];
    auto flowField = new FlowField(3, 3, 3);
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            for(int k=0; k<3; k++){
                flowField->getVelocity().getVector(i,j,k)[0]=6-i-j-k;
                flowField->getVelocity().getVector(i,j,k)[1]=6-i-j-k;
                flowField->getVelocity().getVector(i,j,k)[2]=6-i-j-k;
            }
        }
    }

    loadLocalVelocity3D(*flowField,_localVelocity,1,1,1);
    loadLocalMeshsize3D(parameters->get(),_localMeshsize,1,1,1);

    BOOST_CHECK_CLOSE_FRACTION(dudx_f(_localVelocity,_localMeshsize),-1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dvdy_f(_localVelocity,_localMeshsize),-1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dwdz_f(_localVelocity,_localMeshsize),-1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dudx_b(_localVelocity,_localMeshsize),-1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dvdy_b(_localVelocity,_localMeshsize),-1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dwdz_b(_localVelocity,_localMeshsize),-1/0.333333,0.001);
//mixed
    BOOST_CHECK_CLOSE_FRACTION(dudym(_localVelocity,_localMeshsize),-1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dudzm(_localVelocity,_localMeshsize),-1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dvdxm(_localVelocity,_localMeshsize),-1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dvdzm(_localVelocity,_localMeshsize),-1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dwdxm(_localVelocity,_localMeshsize),-1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dwdym(_localVelocity,_localMeshsize),-1/0.333333,0.001);
//other
    BOOST_CHECK_CLOSE_FRACTION(dvdz_j1(_localVelocity,_localMeshsize),-1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dudz_i1(_localVelocity,_localMeshsize),-1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dwdy_k1(_localVelocity,_localMeshsize),-1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dudy_i1(_localVelocity,_localMeshsize),-1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dwdx_k1(_localVelocity,_localMeshsize),-1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dvdx_j1(_localVelocity,_localMeshsize),-1/0.333333,0.001);

    BOOST_CHECK_CLOSE_FRACTION(dudyb(_localVelocity,_localMeshsize),-1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dudyf(_localVelocity,_localMeshsize),-1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dudzb(_localVelocity,_localMeshsize),-1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dudzf(_localVelocity,_localMeshsize),-1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dwdxb(_localVelocity,_localMeshsize),-1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dwdxf(_localVelocity,_localMeshsize),-1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dwdyb(_localVelocity,_localMeshsize),-1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dwdyf(_localVelocity,_localMeshsize),-1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dvdxb(_localVelocity,_localMeshsize),-1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dvdxf(_localVelocity,_localMeshsize),-1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dvdzb(_localVelocity,_localMeshsize),-1/0.333333,0.001);
    BOOST_CHECK_CLOSE_FRACTION(dvdzf(_localVelocity,_localMeshsize),-1/0.333333,0.001);


    delete flowField;
    delete parameters;
}

BOOST_AUTO_TEST_SUITE_END()
