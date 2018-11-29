#include <boost/test/unit_test.hpp>
#include "../../src/stencils/VTKStencil.h"
#include "../../src/Iterators.h"

class TestingVTKStencil : public VTKStencil {

public:
    explicit TestingVTKStencil(const Parameters &parameters) : VTKStencil(parameters) {
    }

    std::string getPointsStream() {
        return VTKStencil::_pointsStream.str();
    }

    std::string getVelocityStream() {
        return VTKStencil::_velocityStream.str();
    }

    std::string getPressureStream() {
        return VTKStencil::_pressureStream.str();
    }
};

class DummyParameters {
    Parameters *parameters;

public:
    explicit DummyParameters(int dimensions) {
        parameters = new Parameters();
        parameters->parallel.rank = 0;
        parameters->parallel.localSize[0] = 2;
        parameters->parallel.localSize[1] = 2;
        if (dimensions == 3)
            parameters->parallel.localSize[2] = 2;
        else
            parameters->parallel.localSize[2] = 0;

        parameters->parallel.firstCorner[0] = 0;
        parameters->parallel.firstCorner[1] = 0;
        parameters->parallel.firstCorner[2] = 0;

        parameters->geometry.dim = dimensions;

        parameters->geometry.lengthX = 1;
        parameters->geometry.lengthY = 1;

        if (dimensions == 3)
            parameters->geometry.lengthZ = 1;
        else
            parameters->geometry.lengthZ = 0;

        parameters->geometry.sizeX = 2;
        parameters->geometry.sizeY = 2;

        if (dimensions == 3)
            parameters->geometry.sizeZ = 2;
        else
            parameters->geometry.sizeZ = 1;

        parameters->meshsize = new UniformMeshsize(*parameters);

        parameters->vtk.prefix = "testOut";
    }

    Parameters &get() {
        return *parameters;
    }

    ~DummyParameters() {
        delete parameters;
    }
};

BOOST_AUTO_TEST_SUITE(VTKStencilTests)

    BOOST_AUTO_TEST_CASE(construct2D) {
        auto parameters = new DummyParameters(2);
        auto vtkStencil = new TestingVTKStencil(parameters->get());
        auto flowField = new FlowField(2, 2);

        BOOST_CHECK_EQUAL(vtkStencil->getPointsStream(), "# vtk DataFile Version 2.0\n"
                                                         "Some generic header information for whatever reason\n"
                                                         "ASCII\n"
                                                         "\n"
                                                         "DATASET STRUCTURED_GRID\n"
                                                         "DIMENSIONS 3 3 1\n"
                                                         "POINTS 9 float\n"
                                                         "0.000000 0.000000 0.000000\n"
                                                         "0.500000 0.000000 0.000000\n"
                                                         "1.000000 0.000000 0.000000\n"
                                                         "0.000000 0.500000 0.000000\n"
                                                         "0.500000 0.500000 0.000000\n"
                                                         "1.000000 0.500000 0.000000\n"
                                                         "0.000000 1.000000 0.000000\n"
                                                         "0.500000 1.000000 0.000000\n"
                                                         "1.000000 1.000000 0.000000\n"
                                                         "\n"
                                                         "CELL_DATA 4\n");

        delete flowField;
        delete vtkStencil;
        delete parameters;
    }

    BOOST_AUTO_TEST_CASE(construct3D) {
        auto parameters = new DummyParameters(3);
        auto vtkStencil = new TestingVTKStencil(parameters->get());
        auto flowField = new FlowField(2, 2, 2);

        BOOST_CHECK_EQUAL(vtkStencil->getPointsStream(), "# vtk DataFile Version 2.0\n"
                                                         "Some generic header information for whatever reason\n"
                                                         "ASCII\n"
                                                         "\n"
                                                         "DATASET STRUCTURED_GRID\n"
                                                         "DIMENSIONS 3 3 3\n"
                                                         "POINTS 27 float\n"
                                                         "0.000000 0.000000 0.000000\n"
                                                         "0.500000 0.000000 0.000000\n"
                                                         "1.000000 0.000000 0.000000\n"
                                                         "0.000000 0.500000 0.000000\n"
                                                         "0.500000 0.500000 0.000000\n"
                                                         "1.000000 0.500000 0.000000\n"
                                                         "0.000000 1.000000 0.000000\n"
                                                         "0.500000 1.000000 0.000000\n"
                                                         "1.000000 1.000000 0.000000\n"
                                                         "0.000000 0.000000 0.500000\n"
                                                         "0.500000 0.000000 0.500000\n"
                                                         "1.000000 0.000000 0.500000\n"
                                                         "0.000000 0.500000 0.500000\n"
                                                         "0.500000 0.500000 0.500000\n"
                                                         "1.000000 0.500000 0.500000\n"
                                                         "0.000000 1.000000 0.500000\n"
                                                         "0.500000 1.000000 0.500000\n"
                                                         "1.000000 1.000000 0.500000\n"
                                                         "0.000000 0.000000 1.000000\n"
                                                         "0.500000 0.000000 1.000000\n"
                                                         "1.000000 0.000000 1.000000\n"
                                                         "0.000000 0.500000 1.000000\n"
                                                         "0.500000 0.500000 1.000000\n"
                                                         "1.000000 0.500000 1.000000\n"
                                                         "0.000000 1.000000 1.000000\n"
                                                         "0.500000 1.000000 1.000000\n"
                                                         "1.000000 1.000000 1.000000\n"
                                                         "\n"
                                                         "CELL_DATA 8\n");

        delete flowField;
        delete vtkStencil;
        delete parameters;
    }

    BOOST_AUTO_TEST_CASE(apply2DCheckPressure) {
        auto *parameters = new DummyParameters(2);
        auto vtkStencil = new TestingVTKStencil(parameters->get());
        auto flowField = new FlowField(2, 2);
        auto vtkIterator = new FieldIterator<FlowField>(*flowField, parameters->get(), *vtkStencil, 1);

        for (int j = parameters->get().parallel.firstCorner[1] + 2; j < parameters->get().parallel.localSize[1] + 2; j++) {
            for (int i = parameters->get().parallel.firstCorner[0] + 2; i < parameters->get().parallel.localSize[0] + 2; i++) {
                flowField->getPressure().getScalar(i, j) = i * j;
            }
        }

        vtkIterator->iterate();

        BOOST_CHECK_EQUAL(vtkStencil->getPressureStream(), "4.000000\n"
                                                           "6.000000\n"
                                                           "6.000000\n"
                                                           "9.000000\n");

        delete vtkIterator;
        delete flowField;
        delete vtkStencil;
        delete parameters;
    }

    BOOST_AUTO_TEST_CASE(apply2DCheckVelocity) {
        auto *parameters = new DummyParameters(2);
        auto vtkStencil = new TestingVTKStencil(parameters->get());
        auto flowField = new FlowField(2, 2);
        auto vtkIterator = new FieldIterator<FlowField>(*flowField, parameters->get(), *vtkStencil, 1);

        for (int j = parameters->get().parallel.firstCorner[1] + 2; j < parameters->get().parallel.localSize[1] + 2; j++) {
            for (int i = parameters->get().parallel.firstCorner[0] + 2; i < parameters->get().parallel.localSize[0] + 2; i++) {
                flowField->getVelocity().getVector(i, j)[0] = i;
                flowField->getVelocity().getVector(i, j)[1] = j;
                flowField->getVelocity().getVector(i, j)[2] = i * j;
            }
        }

        vtkIterator->iterate();

        BOOST_CHECK_EQUAL(vtkStencil->getVelocityStream(), "1.000000e+00 1.000000e+00 0.000000e+00\n"
                                                           "2.500000e+00 1.000000e+00 0.000000e+00\n"
                                                           "1.000000e+00 2.500000e+00 0.000000e+00\n"
                                                           "2.500000e+00 2.500000e+00 0.000000e+00\n");

        delete vtkIterator;
        delete flowField;
        delete vtkStencil;
        delete parameters;
    }

    BOOST_AUTO_TEST_CASE(apply3DCheckPressure) {
        auto *parameters = new DummyParameters(3);
        auto vtkStencil = new TestingVTKStencil(parameters->get());
        auto flowField = new FlowField(2, 2, 2);
        auto vtkIterator = new FieldIterator<FlowField>(*flowField, parameters->get(), *vtkStencil, 1);

        for (int k = parameters->get().parallel.firstCorner[2] + 2; k < parameters->get().parallel.localSize[2] + 2; k++) {
            for (int j = parameters->get().parallel.firstCorner[1] + 2; j < parameters->get().parallel.localSize[1] + 2; j++) {
                for (int i = parameters->get().parallel.firstCorner[0] + 2; i < parameters->get().parallel.localSize[0] + 2; i++) {
                    flowField->getPressure().getScalar(i, j, k) = i * j * k;
                }
            }
        }

        vtkIterator->iterate();

        BOOST_CHECK_EQUAL(vtkStencil->getPressureStream(), "8.000000\n"
                                                           "12.000000\n"
                                                           "12.000000\n"
                                                           "18.000000\n"
                                                           "12.000000\n"
                                                           "18.000000\n"
                                                           "18.000000\n"
                                                           "27.000000\n");

        delete vtkIterator;
        delete flowField;
        delete vtkStencil;
        delete parameters;
    }

    BOOST_AUTO_TEST_CASE(apply3DCheckVelocity) {
        auto *parameters = new DummyParameters(3);
        auto vtkStencil = new TestingVTKStencil(parameters->get());
        auto flowField = new FlowField(2, 2, 2);
        auto vtkIterator = new FieldIterator<FlowField>(*flowField, parameters->get(), *vtkStencil, 1);

        for (int k = parameters->get().parallel.firstCorner[2] + 2; k < parameters->get().parallel.localSize[2] + 2; k++) {
            for (int j = parameters->get().parallel.firstCorner[1] + 2; j < parameters->get().parallel.localSize[1] + 2; j++) {
                for (int i = parameters->get().parallel.firstCorner[0] + 2; i < parameters->get().parallel.localSize[0] + 2; i++) {
                    flowField->getVelocity().getVector(i, j, k)[0] = i;
                    flowField->getVelocity().getVector(i, j, k)[1] = j;
                    flowField->getVelocity().getVector(i, j, k)[2] = k;
                }
            }
        }

        vtkIterator->iterate();

        BOOST_CHECK_EQUAL(vtkStencil->getVelocityStream(), "1.000000e+00 1.000000e+00 1.000000e+00\n"
                                                           "2.500000e+00 1.000000e+00 1.000000e+00\n"
                                                           "1.000000e+00 2.500000e+00 1.000000e+00\n"
                                                           "2.500000e+00 2.500000e+00 1.000000e+00\n"
                                                           "1.000000e+00 1.000000e+00 2.500000e+00\n"
                                                           "2.500000e+00 1.000000e+00 2.500000e+00\n"
                                                           "1.000000e+00 2.500000e+00 2.500000e+00\n"
                                                           "2.500000e+00 2.500000e+00 2.500000e+00\n");

        delete vtkIterator;
        delete flowField;
        delete vtkStencil;
        delete parameters;
    }

    BOOST_AUTO_TEST_CASE(write) {
        // TODO: test writing of vtk file
        BOOST_TEST_MESSAGE("TODO: Redirect file output to internal stream for testing purposes (question is how to do that nicely");
    }

BOOST_AUTO_TEST_SUITE_END()