#include <boost/test/unit_test.hpp>
#include "../../src/stencils/WallDistanceStencil.h"
#include "../../src/Iterators.h"
#include "../TestingFunctions.h"
#include "../../src/FlowField.h"

BOOST_AUTO_TEST_SUITE(WallDistanceStencilsTests)

    BOOST_AUTO_TEST_CASE(leftIsWall2D) {
        auto parameters = new DummyParameters(2, 3);

        // Set wall condition
        parameters->get().walls.typeLeft = DIRICHLET;
        parameters->get().walls.vectorLeft[0] = 0.0;
        parameters->get().walls.vectorLeft[1] = 1.0;
        parameters->get().walls.vectorLeft[2] = 1.0;

        parameters->get().walls.typeRight = DIRICHLET;
        parameters->get().walls.vectorRight[0] = 1.0;
        parameters->get().walls.vectorRight[1] = 1.0;
        parameters->get().walls.vectorRight[2] = 1.0;

        parameters->get().walls.typeTop = DIRICHLET;
        parameters->get().walls.vectorTop[0] = 1.0;
        parameters->get().walls.vectorTop[1] = 1.0;
        parameters->get().walls.vectorTop[2] = 1.0;

        parameters->get().walls.typeBottom = DIRICHLET;
        parameters->get().walls.vectorBottom[0] = 1.0;
        parameters->get().walls.vectorBottom[1] = 1.0;
        parameters->get().walls.vectorBottom[2] = 1.0;

        auto flowField = new FlowField(3, 3);

        auto wallDistanceStencil = new WallDistanceStencil(parameters->get(), *flowField);
        auto wallDistanceIterator = new GlobalBoundaryIterator<FlowField>(*flowField, parameters->get(), *wallDistanceStencil, 2, -1);

        wallDistanceIterator->iterate();

        // Create FlowField to check against
        auto checkFlowField = new FlowField(3, 3);
        for (int j = 2; j < 5; ++j) {
            for (int i = 2; i < 5; ++i) {
                if (i == 2)
                    checkFlowField->getNearestWallDistance().getScalar(i, j) = 0.1666666666;
                else
                    checkFlowField->getNearestWallDistance().getScalar(i, j) = MY_FLOAT_MAX;
            }
        }

        // Check for correctness
        ScalarField & result = flowField->getNearestWallDistance();
        ScalarField & expected = checkFlowField->getNearestWallDistance();
        for (int j = 2; j < 5; ++j) {
            for (int i = 2; i < 5; ++i) {
                BOOST_CHECK_CLOSE_FRACTION(result.getScalar(i, j), expected.getScalar(i, j), 0.000000001);
            }
        }

        delete checkFlowField;
        delete wallDistanceIterator;
        delete wallDistanceStencil;
        delete flowField;
        delete parameters;
    }

    BOOST_AUTO_TEST_CASE(noWall2D) {
        auto parameters = new DummyParameters(2, 3);

        // Set wall condition
        parameters->get().walls.typeLeft = DIRICHLET;
        parameters->get().walls.vectorLeft[0] = 1.0;
        parameters->get().walls.vectorLeft[1] = 1.0;
        parameters->get().walls.vectorLeft[2] = 1.0;

        parameters->get().walls.typeRight = DIRICHLET;
        parameters->get().walls.vectorRight[0] = 1.0;
        parameters->get().walls.vectorRight[1] = 1.0;
        parameters->get().walls.vectorRight[2] = 1.0;

        parameters->get().walls.typeTop = DIRICHLET;
        parameters->get().walls.vectorTop[0] = 1.0;
        parameters->get().walls.vectorTop[1] = 1.0;
        parameters->get().walls.vectorTop[2] = 1.0;

        parameters->get().walls.typeBottom = DIRICHLET;
        parameters->get().walls.vectorBottom[0] = 1.0;
        parameters->get().walls.vectorBottom[1] = 1.0;
        parameters->get().walls.vectorBottom[2] = 1.0;

        auto flowField = new FlowField(3, 3);

        auto wallDistanceStencil = new WallDistanceStencil(parameters->get(), *flowField);
        auto wallDistanceIterator = new GlobalBoundaryIterator<FlowField>(*flowField, parameters->get(), *wallDistanceStencil, 2, -1);

        wallDistanceIterator->iterate();

        // Create FlowField to check against
        auto checkFlowField = new FlowField(3, 3);
        for (int j = 2; j < 5; ++j) {
            for (int i = 2; i < 5; ++i) {
                    checkFlowField->getNearestWallDistance().getScalar(i, j) = MY_FLOAT_MAX;
            }
        }

        // Check for correctness
        ScalarField & result = flowField->getNearestWallDistance();
        ScalarField & expected = checkFlowField->getNearestWallDistance();
        for (int j = 2; j < 5; ++j) {
            for (int i = 2; i < 5; ++i) {
                BOOST_CHECK_CLOSE_FRACTION(result.getScalar(i, j), expected.getScalar(i, j), 0.000000001);
            }
        }

        delete checkFlowField;
        delete wallDistanceIterator;
        delete wallDistanceStencil;
        delete flowField;
        delete parameters;
    }

    BOOST_AUTO_TEST_CASE(rightIsWall2D) {
        auto parameters = new DummyParameters(2, 3);

        // Set wall condition
        parameters->get().walls.typeLeft = DIRICHLET;
        parameters->get().walls.vectorLeft[0] = 1.0;
        parameters->get().walls.vectorLeft[1] = 1.0;
        parameters->get().walls.vectorLeft[2] = 1.0;

        parameters->get().walls.typeRight = DIRICHLET;
        parameters->get().walls.vectorRight[0] = 0.0;
        parameters->get().walls.vectorRight[1] = 1.0;
        parameters->get().walls.vectorRight[2] = 1.0;

        parameters->get().walls.typeTop = DIRICHLET;
        parameters->get().walls.vectorTop[0] = 1.0;
        parameters->get().walls.vectorTop[1] = 1.0;
        parameters->get().walls.vectorTop[2] = 1.0;

        parameters->get().walls.typeBottom = DIRICHLET;
        parameters->get().walls.vectorBottom[0] = 1.0;
        parameters->get().walls.vectorBottom[1] = 1.0;
        parameters->get().walls.vectorBottom[2] = 1.0;

        auto flowField = new FlowField(3, 3);

        auto wallDistanceStencil = new WallDistanceStencil(parameters->get(), *flowField);
        auto wallDistanceIterator = new GlobalBoundaryIterator<FlowField>(*flowField, parameters->get(), *wallDistanceStencil, 2, -1);

        wallDistanceIterator->iterate();

        // Create FlowField to check against
        auto checkFlowField = new FlowField(3, 3);
        for (int j = 2; j < 5; ++j) {
            for (int i = 2; i < 5; ++i) {
                if (i == 4)
                    checkFlowField->getNearestWallDistance().getScalar(i, j) = 0.1666666666;
                else
                    checkFlowField->getNearestWallDistance().getScalar(i, j) = MY_FLOAT_MAX;
            }
        }

        // Check for correctness
        ScalarField & result = flowField->getNearestWallDistance();
        ScalarField & expected = checkFlowField->getNearestWallDistance();
        for (int j = 2; j < 5; ++j) {
            for (int i = 2; i < 5; ++i) {
                BOOST_CHECK_CLOSE_FRACTION(result.getScalar(i, j), expected.getScalar(i, j), 0.000000001);
            }
        }

        delete checkFlowField;
        delete wallDistanceIterator;
        delete wallDistanceStencil;
        delete flowField;
        delete parameters;
    }

    BOOST_AUTO_TEST_CASE(bottomIsWall2D) {
        auto parameters = new DummyParameters(2, 3);

        // Set wall condition
        parameters->get().walls.typeLeft = DIRICHLET;
        parameters->get().walls.vectorLeft[0] = 1.0;
        parameters->get().walls.vectorLeft[1] = 1.0;
        parameters->get().walls.vectorLeft[2] = 1.0;

        parameters->get().walls.typeRight = DIRICHLET;
        parameters->get().walls.vectorRight[0] = 1.0;
        parameters->get().walls.vectorRight[1] = 1.0;
        parameters->get().walls.vectorRight[2] = 1.0;

        parameters->get().walls.typeTop = DIRICHLET;
        parameters->get().walls.vectorTop[0] = 1.0;
        parameters->get().walls.vectorTop[1] = 1.0;
        parameters->get().walls.vectorTop[2] = 1.0;

        parameters->get().walls.typeBottom = DIRICHLET;
        parameters->get().walls.vectorBottom[0] = 1.0;
        parameters->get().walls.vectorBottom[1] = 0.0;
        parameters->get().walls.vectorBottom[2] = 1.0;

        auto flowField = new FlowField(3, 3);

        auto wallDistanceStencil = new WallDistanceStencil(parameters->get(), *flowField);
        auto wallDistanceIterator = new GlobalBoundaryIterator<FlowField>(*flowField, parameters->get(), *wallDistanceStencil, 2, -1);

        wallDistanceIterator->iterate();

        // Create FlowField to check against
        auto checkFlowField = new FlowField(3, 3);
        for (int j = 2; j < 5; ++j) {
            for (int i = 2; i < 5; ++i) {
                if (j == 2)
                    checkFlowField->getNearestWallDistance().getScalar(i, j) = 0.1666666666;
                else
                    checkFlowField->getNearestWallDistance().getScalar(i, j) = MY_FLOAT_MAX;
            }
        }

        // Check for correctness
        ScalarField & result = flowField->getNearestWallDistance();
        ScalarField & expected = checkFlowField->getNearestWallDistance();
        for (int j = 2; j < 5; ++j) {
            for (int i = 2; i < 5; ++i) {
                BOOST_CHECK_CLOSE_FRACTION(result.getScalar(i, j), expected.getScalar(i, j), 0.000000001);
            }
        }

        delete checkFlowField;
        delete wallDistanceIterator;
        delete wallDistanceStencil;
        delete flowField;
        delete parameters;
    }

    BOOST_AUTO_TEST_CASE(topIsWall2D) {
        auto parameters = new DummyParameters(2, 3);

        // Set wall condition
        parameters->get().walls.typeLeft = DIRICHLET;
        parameters->get().walls.vectorLeft[0] = 1.0;
        parameters->get().walls.vectorLeft[1] = 1.0;
        parameters->get().walls.vectorLeft[2] = 1.0;

        parameters->get().walls.typeRight = DIRICHLET;
        parameters->get().walls.vectorRight[0] = 1.0;
        parameters->get().walls.vectorRight[1] = 1.0;
        parameters->get().walls.vectorRight[2] = 1.0;

        parameters->get().walls.typeTop = DIRICHLET;
        parameters->get().walls.vectorTop[0] = 1.0;
        parameters->get().walls.vectorTop[1] = 0.0;
        parameters->get().walls.vectorTop[2] = 1.0;

        parameters->get().walls.typeBottom = DIRICHLET;
        parameters->get().walls.vectorBottom[0] = 1.0;
        parameters->get().walls.vectorBottom[1] = 1.0;
        parameters->get().walls.vectorBottom[2] = 1.0;

        auto flowField = new FlowField(3, 3);

        auto wallDistanceStencil = new WallDistanceStencil(parameters->get(), *flowField);
        auto wallDistanceIterator = new GlobalBoundaryIterator<FlowField>(*flowField, parameters->get(), *wallDistanceStencil, 2, -1);

        wallDistanceIterator->iterate();

        // Create FlowField to check against
        auto checkFlowField = new FlowField(3, 3);
        for (int j = 2; j < 5; ++j) {
            for (int i = 2; i < 5; ++i) {
                if (j == 4)
                    checkFlowField->getNearestWallDistance().getScalar(i, j) = 0.1666666666;
                else
                    checkFlowField->getNearestWallDistance().getScalar(i, j) = MY_FLOAT_MAX;
            }
        }

        // Check for correctness
        ScalarField & result = flowField->getNearestWallDistance();
        ScalarField & expected = checkFlowField->getNearestWallDistance();
        for (int j = 2; j < 5; ++j) {
            for (int i = 2; i < 5; ++i) {
                BOOST_CHECK_CLOSE_FRACTION(result.getScalar(i, j), expected.getScalar(i, j), 0.000000001);
            }
        }

        delete checkFlowField;
        delete wallDistanceIterator;
        delete wallDistanceStencil;
        delete flowField;
        delete parameters;
    }

    BOOST_AUTO_TEST_CASE(leftIsWall3D) {
        auto parameters = new DummyParameters(3, 3);

        // Set wall condition
        parameters->get().walls.typeLeft = DIRICHLET;
        parameters->get().walls.vectorLeft[0] = 0.0;
        parameters->get().walls.vectorLeft[1] = 1.0;
        parameters->get().walls.vectorLeft[2] = 1.0;

        parameters->get().walls.typeRight = DIRICHLET;
        parameters->get().walls.vectorRight[0] = 1.0;
        parameters->get().walls.vectorRight[1] = 1.0;
        parameters->get().walls.vectorRight[2] = 1.0;

        parameters->get().walls.typeTop = DIRICHLET;
        parameters->get().walls.vectorTop[0] = 1.0;
        parameters->get().walls.vectorTop[1] = 1.0;
        parameters->get().walls.vectorTop[2] = 1.0;

        parameters->get().walls.typeBottom = DIRICHLET;
        parameters->get().walls.vectorBottom[0] = 1.0;
        parameters->get().walls.vectorBottom[1] = 1.0;
        parameters->get().walls.vectorBottom[2] = 1.0;

        parameters->get().walls.typeFront = DIRICHLET;
        parameters->get().walls.vectorFront[0] = 1.0;
        parameters->get().walls.vectorFront[1] = 1.0;
        parameters->get().walls.vectorFront[2] = 1.0;

        parameters->get().walls.typeBack = DIRICHLET;
        parameters->get().walls.vectorBack[0] = 1.0;
        parameters->get().walls.vectorBack[1] = 1.0;
        parameters->get().walls.vectorBack[2] = 1.0;

        auto flowField = new FlowField(3, 3, 3);

        auto wallDistanceStencil = new WallDistanceStencil(parameters->get(), *flowField);
        auto wallDistanceIterator = new GlobalBoundaryIterator<FlowField>(*flowField, parameters->get(), *wallDistanceStencil, 2, -1);

        wallDistanceIterator->iterate();

        // Create FlowField to check against
        auto checkFlowField = new FlowField(3, 3, 3);
        for (int k = 2; k < 5; k++) {
            for (int j = 2; j < 5; j++) {
                for (int i = 2; i < 5; i++) {
                    if (i == 2)
                        checkFlowField->getNearestWallDistance().getScalar(i, j, k) = 0.1666666666;
                    else
                        checkFlowField->getNearestWallDistance().getScalar(i, j, k) = MY_FLOAT_MAX;
                }
            }
        }

        // Check for correctness
        ScalarField & result = flowField->getNearestWallDistance();
        ScalarField & expected = checkFlowField->getNearestWallDistance();
        for (int k = 2; k < 5; ++k) {
            for (int j = 2; j < 5; ++j) {
                for (int i = 2; i < 5; ++i) {
                    BOOST_CHECK_CLOSE_FRACTION(result.getScalar(i, j, k), expected.getScalar(i, j, k), 0.000000001);
                }
            }
        }

        delete checkFlowField;
        delete wallDistanceIterator;
        delete wallDistanceStencil;
        delete flowField;
        delete parameters;
    }

    BOOST_AUTO_TEST_CASE(rightIsWall3D) {
        auto parameters = new DummyParameters(3, 3);

        // Set wall condition
        parameters->get().walls.typeLeft = DIRICHLET;
        parameters->get().walls.vectorLeft[0] = 1.0;
        parameters->get().walls.vectorLeft[1] = 1.0;
        parameters->get().walls.vectorLeft[2] = 1.0;

        parameters->get().walls.typeRight = DIRICHLET;
        parameters->get().walls.vectorRight[0] = 0.0;
        parameters->get().walls.vectorRight[1] = 1.0;
        parameters->get().walls.vectorRight[2] = 1.0;

        parameters->get().walls.typeTop = DIRICHLET;
        parameters->get().walls.vectorTop[0] = 1.0;
        parameters->get().walls.vectorTop[1] = 1.0;
        parameters->get().walls.vectorTop[2] = 1.0;

        parameters->get().walls.typeBottom = DIRICHLET;
        parameters->get().walls.vectorBottom[0] = 1.0;
        parameters->get().walls.vectorBottom[1] = 1.0;
        parameters->get().walls.vectorBottom[2] = 1.0;

        parameters->get().walls.typeFront = DIRICHLET;
        parameters->get().walls.vectorFront[0] = 1.0;
        parameters->get().walls.vectorFront[1] = 1.0;
        parameters->get().walls.vectorFront[2] = 1.0;

        parameters->get().walls.typeBack = DIRICHLET;
        parameters->get().walls.vectorBack[0] = 1.0;
        parameters->get().walls.vectorBack[1] = 1.0;
        parameters->get().walls.vectorBack[2] = 1.0;

        auto flowField = new FlowField(3, 3, 3);

        auto wallDistanceStencil = new WallDistanceStencil(parameters->get(), *flowField);
        auto wallDistanceIterator = new GlobalBoundaryIterator<FlowField>(*flowField, parameters->get(), *wallDistanceStencil, 2, -1);

        wallDistanceIterator->iterate();

        // Create FlowField to check against
        auto checkFlowField = new FlowField(3, 3, 3);
        for (int k = 2; k < 5; k++) {
            for (int j = 2; j < 5; j++) {
                for (int i = 2; i < 5; i++) {
                    if (i == 4)
                        checkFlowField->getNearestWallDistance().getScalar(i, j, k) = 0.1666666666;
                    else
                        checkFlowField->getNearestWallDistance().getScalar(i, j, k) = MY_FLOAT_MAX;
                }
            }
        }

        // Check for correctness
        ScalarField & result = flowField->getNearestWallDistance();
        ScalarField & expected = checkFlowField->getNearestWallDistance();
        for (int k = 2; k < 5; ++k) {
            for (int j = 2; j < 5; ++j) {
                for (int i = 2; i < 5; ++i) {
                    BOOST_CHECK_CLOSE_FRACTION(result.getScalar(i, j, k), expected.getScalar(i, j, k), 0.000000001);
                }
            }
        }

        delete checkFlowField;
        delete wallDistanceIterator;
        delete wallDistanceStencil;
        delete flowField;
        delete parameters;
    }

    BOOST_AUTO_TEST_CASE(topIsWall3D) {
        auto parameters = new DummyParameters(3, 3);

        // Set wall condition
        parameters->get().walls.typeLeft = DIRICHLET;
        parameters->get().walls.vectorLeft[0] = 1.0;
        parameters->get().walls.vectorLeft[1] = 1.0;
        parameters->get().walls.vectorLeft[2] = 1.0;

        parameters->get().walls.typeRight = DIRICHLET;
        parameters->get().walls.vectorRight[0] = 1.0;
        parameters->get().walls.vectorRight[1] = 1.0;
        parameters->get().walls.vectorRight[2] = 1.0;

        parameters->get().walls.typeTop = DIRICHLET;
        parameters->get().walls.vectorTop[0] = 1.0;
        parameters->get().walls.vectorTop[1] = 0.0;
        parameters->get().walls.vectorTop[2] = 1.0;

        parameters->get().walls.typeBottom = DIRICHLET;
        parameters->get().walls.vectorBottom[0] = 1.0;
        parameters->get().walls.vectorBottom[1] = 1.0;
        parameters->get().walls.vectorBottom[2] = 1.0;

        parameters->get().walls.typeFront = DIRICHLET;
        parameters->get().walls.vectorFront[0] = 1.0;
        parameters->get().walls.vectorFront[1] = 1.0;
        parameters->get().walls.vectorFront[2] = 1.0;

        parameters->get().walls.typeBack = DIRICHLET;
        parameters->get().walls.vectorBack[0] = 1.0;
        parameters->get().walls.vectorBack[1] = 1.0;
        parameters->get().walls.vectorBack[2] = 1.0;

        auto flowField = new FlowField(3, 3, 3);

        auto wallDistanceStencil = new WallDistanceStencil(parameters->get(), *flowField);
        auto wallDistanceIterator = new GlobalBoundaryIterator<FlowField>(*flowField, parameters->get(), *wallDistanceStencil, 2, -1);

        wallDistanceIterator->iterate();

        // Create FlowField to check against
        auto checkFlowField = new FlowField(3, 3, 3);
        for (int k = 2; k < 5; k++) {
            for (int j = 2; j < 5; j++) {
                for (int i = 2; i < 5; i++) {
                    if (j == 4)
                        checkFlowField->getNearestWallDistance().getScalar(i, j, k) = 0.1666666666;
                    else
                        checkFlowField->getNearestWallDistance().getScalar(i, j, k) = MY_FLOAT_MAX;
                }
            }
        }

        // Check for correctness
        ScalarField & result = flowField->getNearestWallDistance();
        ScalarField & expected = checkFlowField->getNearestWallDistance();
        for (int k = 2; k < 5; ++k) {
            for (int j = 2; j < 5; ++j) {
                for (int i = 2; i < 5; ++i) {
                    BOOST_CHECK_CLOSE_FRACTION(result.getScalar(i, j, k), expected.getScalar(i, j, k), 0.000000001);
                }
            }
        }

        delete checkFlowField;
        delete wallDistanceIterator;
        delete wallDistanceStencil;
        delete flowField;
        delete parameters;
    }

    BOOST_AUTO_TEST_CASE(bottomIsWall3D) {
        auto parameters = new DummyParameters(3, 3);

        // Set wall condition
        parameters->get().walls.typeLeft = DIRICHLET;
        parameters->get().walls.vectorLeft[0] = 1.0;
        parameters->get().walls.vectorLeft[1] = 1.0;
        parameters->get().walls.vectorLeft[2] = 1.0;

        parameters->get().walls.typeRight = DIRICHLET;
        parameters->get().walls.vectorRight[0] = 1.0;
        parameters->get().walls.vectorRight[1] = 1.0;
        parameters->get().walls.vectorRight[2] = 1.0;

        parameters->get().walls.typeTop = DIRICHLET;
        parameters->get().walls.vectorTop[0] = 1.0;
        parameters->get().walls.vectorTop[1] = 1.0;
        parameters->get().walls.vectorTop[2] = 1.0;

        parameters->get().walls.typeBottom = DIRICHLET;
        parameters->get().walls.vectorBottom[0] = 1.0;
        parameters->get().walls.vectorBottom[1] = 0.0;
        parameters->get().walls.vectorBottom[2] = 1.0;

        parameters->get().walls.typeFront = DIRICHLET;
        parameters->get().walls.vectorFront[0] = 1.0;
        parameters->get().walls.vectorFront[1] = 1.0;
        parameters->get().walls.vectorFront[2] = 1.0;

        parameters->get().walls.typeBack = DIRICHLET;
        parameters->get().walls.vectorBack[0] = 1.0;
        parameters->get().walls.vectorBack[1] = 1.0;
        parameters->get().walls.vectorBack[2] = 1.0;

        auto flowField = new FlowField(3, 3, 3);

        auto wallDistanceStencil = new WallDistanceStencil(parameters->get(), *flowField);
        auto wallDistanceIterator = new GlobalBoundaryIterator<FlowField>(*flowField, parameters->get(), *wallDistanceStencil, 2, -1);

        wallDistanceIterator->iterate();

        // Create FlowField to check against
        auto checkFlowField = new FlowField(3, 3, 3);
        for (int k = 2; k < 5; k++) {
            for (int j = 2; j < 5; j++) {
                for (int i = 2; i < 5; i++) {
                    if (j == 2)
                        checkFlowField->getNearestWallDistance().getScalar(i, j, k) = 0.1666666666;
                    else
                        checkFlowField->getNearestWallDistance().getScalar(i, j, k) = MY_FLOAT_MAX;
                }
            }
        }

        // Check for correctness
        ScalarField & result = flowField->getNearestWallDistance();
        ScalarField & expected = checkFlowField->getNearestWallDistance();
        for (int k = 2; k < 5; ++k) {
            for (int j = 2; j < 5; ++j) {
                for (int i = 2; i < 5; ++i) {
                    BOOST_CHECK_CLOSE_FRACTION(result.getScalar(i, j, k), expected.getScalar(i, j, k), 0.000000001);
                }
            }
        }

        delete checkFlowField;
        delete wallDistanceIterator;
        delete wallDistanceStencil;
        delete flowField;
        delete parameters;
    }

    BOOST_AUTO_TEST_CASE(frontIsWall3D) {
        auto parameters = new DummyParameters(3, 3);

        // Set wall condition
        parameters->get().walls.typeLeft = DIRICHLET;
        parameters->get().walls.vectorLeft[0] = 1.0;
        parameters->get().walls.vectorLeft[1] = 1.0;
        parameters->get().walls.vectorLeft[2] = 1.0;

        parameters->get().walls.typeRight = DIRICHLET;
        parameters->get().walls.vectorRight[0] = 1.0;
        parameters->get().walls.vectorRight[1] = 1.0;
        parameters->get().walls.vectorRight[2] = 1.0;

        parameters->get().walls.typeTop = DIRICHLET;
        parameters->get().walls.vectorTop[0] = 1.0;
        parameters->get().walls.vectorTop[1] = 1.0;
        parameters->get().walls.vectorTop[2] = 1.0;

        parameters->get().walls.typeBottom = DIRICHLET;
        parameters->get().walls.vectorBottom[0] = 1.0;
        parameters->get().walls.vectorBottom[1] = 1.0;
        parameters->get().walls.vectorBottom[2] = 1.0;

        parameters->get().walls.typeFront = DIRICHLET;
        parameters->get().walls.vectorFront[0] = 1.0;
        parameters->get().walls.vectorFront[1] = 1.0;
        parameters->get().walls.vectorFront[2] = 0.0;

        parameters->get().walls.typeBack = DIRICHLET;
        parameters->get().walls.vectorBack[0] = 1.0;
        parameters->get().walls.vectorBack[1] = 1.0;
        parameters->get().walls.vectorBack[2] = 1.0;

        auto flowField = new FlowField(3, 3, 3);

        auto wallDistanceStencil = new WallDistanceStencil(parameters->get(), *flowField);
        auto wallDistanceIterator = new GlobalBoundaryIterator<FlowField>(*flowField, parameters->get(), *wallDistanceStencil, 2, -1);

        wallDistanceIterator->iterate();

        // Create FlowField to check against
        auto checkFlowField = new FlowField(3, 3, 3);
        for (int k = 2; k < 5; k++) {
            for (int j = 2; j < 5; j++) {
                for (int i = 2; i < 5; i++) {
                    if (k == 2)
                        checkFlowField->getNearestWallDistance().getScalar(i, j, k) = 0.1666666666;
                    else
                        checkFlowField->getNearestWallDistance().getScalar(i, j, k) = MY_FLOAT_MAX;
                }
            }
        }

        // Check for correctness
        ScalarField & result = flowField->getNearestWallDistance();
        ScalarField & expected = checkFlowField->getNearestWallDistance();
        for (int k = 2; k < 5; ++k) {
            for (int j = 2; j < 5; ++j) {
                for (int i = 2; i < 5; ++i) {
                    BOOST_CHECK_CLOSE_FRACTION(result.getScalar(i, j, k), expected.getScalar(i, j, k), 0.000000001);
                }
            }
        }

        delete checkFlowField;
        delete wallDistanceIterator;
        delete wallDistanceStencil;
        delete flowField;
        delete parameters;
    }

    BOOST_AUTO_TEST_CASE(backIsWall3D) {
        auto parameters = new DummyParameters(3, 3);

        // Set wall condition
        parameters->get().walls.typeLeft = DIRICHLET;
        parameters->get().walls.vectorLeft[0] = 1.0;
        parameters->get().walls.vectorLeft[1] = 1.0;
        parameters->get().walls.vectorLeft[2] = 1.0;

        parameters->get().walls.typeRight = DIRICHLET;
        parameters->get().walls.vectorRight[0] = 1.0;
        parameters->get().walls.vectorRight[1] = 1.0;
        parameters->get().walls.vectorRight[2] = 1.0;

        parameters->get().walls.typeTop = DIRICHLET;
        parameters->get().walls.vectorTop[0] = 1.0;
        parameters->get().walls.vectorTop[1] = 1.0;
        parameters->get().walls.vectorTop[2] = 1.0;

        parameters->get().walls.typeBottom = DIRICHLET;
        parameters->get().walls.vectorBottom[0] = 1.0;
        parameters->get().walls.vectorBottom[1] = 1.0;
        parameters->get().walls.vectorBottom[2] = 1.0;

        parameters->get().walls.typeFront = DIRICHLET;
        parameters->get().walls.vectorFront[0] = 1.0;
        parameters->get().walls.vectorFront[1] = 1.0;
        parameters->get().walls.vectorFront[2] = 1.0;

        parameters->get().walls.typeBack = DIRICHLET;
        parameters->get().walls.vectorBack[0] = 1.0;
        parameters->get().walls.vectorBack[1] = 1.0;
        parameters->get().walls.vectorBack[2] = 0.0;

        auto flowField = new FlowField(3, 3, 3);

        auto wallDistanceStencil = new WallDistanceStencil(parameters->get(), *flowField);
        auto wallDistanceIterator = new GlobalBoundaryIterator<FlowField>(*flowField, parameters->get(), *wallDistanceStencil, 2, -1);

        wallDistanceIterator->iterate();

        // Create FlowField to check against
        auto checkFlowField = new FlowField(3, 3, 3);
        for (int k = 2; k < 5; k++) {
            for (int j = 2; j < 5; j++) {
                for (int i = 2; i < 5; i++) {
                    if (k == 4)
                        checkFlowField->getNearestWallDistance().getScalar(i, j, k) = 0.1666666666;
                    else
                        checkFlowField->getNearestWallDistance().getScalar(i, j, k) = MY_FLOAT_MAX;
                }
            }
        }

        // Check for correctness
        ScalarField & result = flowField->getNearestWallDistance();
        ScalarField & expected = checkFlowField->getNearestWallDistance();
        for (int k = 2; k < 5; ++k) {
            for (int j = 2; j < 5; ++j) {
                for (int i = 2; i < 5; ++i) {
                    BOOST_CHECK_CLOSE_FRACTION(result.getScalar(i, j, k), expected.getScalar(i, j, k), 0.000000001);
                }
            }
        }

        delete checkFlowField;
        delete wallDistanceIterator;
        delete wallDistanceStencil;
        delete flowField;
        delete parameters;
    }

    BOOST_AUTO_TEST_CASE(noWall3D) {
        auto parameters = new DummyParameters(3, 3);

        // Set wall condition
        parameters->get().walls.typeLeft = DIRICHLET;
        parameters->get().walls.vectorLeft[0] = 1.0;
        parameters->get().walls.vectorLeft[1] = 1.0;
        parameters->get().walls.vectorLeft[2] = 1.0;

        parameters->get().walls.typeRight = DIRICHLET;
        parameters->get().walls.vectorRight[0] = 1.0;
        parameters->get().walls.vectorRight[1] = 1.0;
        parameters->get().walls.vectorRight[2] = 1.0;

        parameters->get().walls.typeTop = DIRICHLET;
        parameters->get().walls.vectorTop[0] = 1.0;
        parameters->get().walls.vectorTop[1] = 1.0;
        parameters->get().walls.vectorTop[2] = 1.0;

        parameters->get().walls.typeBottom = DIRICHLET;
        parameters->get().walls.vectorBottom[0] = 1.0;
        parameters->get().walls.vectorBottom[1] = 1.0;
        parameters->get().walls.vectorBottom[2] = 1.0;

        parameters->get().walls.typeFront = DIRICHLET;
        parameters->get().walls.vectorFront[0] = 1.0;
        parameters->get().walls.vectorFront[1] = 1.0;
        parameters->get().walls.vectorFront[2] = 1.0;

        parameters->get().walls.typeBack = DIRICHLET;
        parameters->get().walls.vectorBack[0] = 1.0;
        parameters->get().walls.vectorBack[1] = 1.0;
        parameters->get().walls.vectorBack[2] = 1.0;

        auto flowField = new FlowField(3, 3, 3);

        auto wallDistanceStencil = new WallDistanceStencil(parameters->get(), *flowField);
        auto wallDistanceIterator = new GlobalBoundaryIterator<FlowField>(*flowField, parameters->get(), *wallDistanceStencil, 2, -1);

        wallDistanceIterator->iterate();

        // Create FlowField to check against
        auto checkFlowField = new FlowField(3, 3, 3);
        for (int k = 2; k < 5; k++) {
            for (int j = 2; j < 5; j++) {
                for (int i = 2; i < 5; i++) {
                    checkFlowField->getNearestWallDistance().getScalar(i, j, k) = MY_FLOAT_MAX;
                }
            }
        }

        // Check for correctness
        ScalarField & result = flowField->getNearestWallDistance();
        ScalarField & expected = checkFlowField->getNearestWallDistance();
        for (int k = 2; k < 5; ++k) {
            for (int j = 2; j < 5; ++j) {
                for (int i = 2; i < 5; ++i) {
                    BOOST_CHECK_CLOSE_FRACTION(result.getScalar(i, j, k), expected.getScalar(i, j, k), 0.000000001);
                }
            }
        }

        delete checkFlowField;
        delete wallDistanceIterator;
        delete wallDistanceStencil;
        delete flowField;
        delete parameters;
    }

BOOST_AUTO_TEST_SUITE_END()