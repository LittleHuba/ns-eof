#include <boost/test/unit_test.hpp>
#include "../../src/stencils/WallDistanceStencil.h"
#include "../../src/stencils/ObstacleDistanceStencil.h"
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

    BOOST_AUTO_TEST_CASE(obstacle2D) {
        auto parameters = new DummyParameters(2, 10);

        // Set wall condition
        parameters->get().walls.typeLeft = DIRICHLET;
        parameters->get().walls.vectorLeft[0] = 0.0;
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

        auto flowField = new FlowField(10, 10);

        // Set the appropriate obstacle flags
        flowField->getFlags().getValue(4, 5) = OBSTACLE_RIGHT;
        flowField->getFlags().getValue(4, 6) = OBSTACLE_RIGHT;
        flowField->getFlags().getValue(4, 7) = OBSTACLE_RIGHT;

        flowField->getFlags().getValue(5, 4) = OBSTACLE_TOP;
        flowField->getFlags().getValue(5, 5) = OBSTACLE_SELF + OBSTACLE_TOP + OBSTACLE_RIGHT;
        flowField->getFlags().getValue(5, 6) = OBSTACLE_SELF + OBSTACLE_TOP + OBSTACLE_RIGHT + OBSTACLE_BOTTOM;
        flowField->getFlags().getValue(5, 7) = OBSTACLE_SELF + OBSTACLE_RIGHT + OBSTACLE_BOTTOM;
        flowField->getFlags().getValue(5, 8) = OBSTACLE_BOTTOM;

        flowField->getFlags().getValue(6, 4) = OBSTACLE_TOP;
        flowField->getFlags().getValue(6, 5) = OBSTACLE_SELF + OBSTACLE_TOP + OBSTACLE_RIGHT + OBSTACLE_LEFT;
        flowField->getFlags().getValue(6, 6) = OBSTACLE_SELF + OBSTACLE_TOP + OBSTACLE_RIGHT + OBSTACLE_LEFT + OBSTACLE_BOTTOM;
        flowField->getFlags().getValue(6, 7) = OBSTACLE_SELF + OBSTACLE_RIGHT + OBSTACLE_LEFT + OBSTACLE_BOTTOM;
        flowField->getFlags().getValue(6, 8) = OBSTACLE_BOTTOM;

        flowField->getFlags().getValue(7, 4) = OBSTACLE_TOP;
        flowField->getFlags().getValue(7, 5) = OBSTACLE_SELF + OBSTACLE_TOP + OBSTACLE_LEFT;
        flowField->getFlags().getValue(7, 6) = OBSTACLE_SELF + OBSTACLE_TOP + OBSTACLE_LEFT + OBSTACLE_BOTTOM;
        flowField->getFlags().getValue(7, 7) = OBSTACLE_SELF + OBSTACLE_LEFT + OBSTACLE_BOTTOM;
        flowField->getFlags().getValue(7, 8) = OBSTACLE_BOTTOM;

        flowField->getFlags().getValue(8, 5) = OBSTACLE_LEFT;
        flowField->getFlags().getValue(8, 6) = OBSTACLE_LEFT;
        flowField->getFlags().getValue(8, 7) = OBSTACLE_LEFT;

        auto wallDistanceStencil = new WallDistanceStencil(parameters->get(), *flowField);
        auto wallDistanceIterator = new GlobalBoundaryIterator<FlowField>(*flowField, parameters->get(), *wallDistanceStencil, 2, -1);

        wallDistanceIterator->iterate();

        auto obstacleDistanceStencil = new ObstacleDistanceStencil(parameters->get());
        auto obstacleDistanceIterator = new FieldIterator<FlowField>(*flowField, parameters->get(), *obstacleDistanceStencil, 1);

        obstacleDistanceIterator->iterate();
        obstacleDistanceIterator->iterateBackwards();

        // Create FlowField to check against
        auto checkFlowField = new FlowField(10, 10);

        // Column 0
        for (int i = 2; i < 12; ++i) {
            checkFlowField->getNearestWallDistance().getScalar(2, i) = 0.05;
        }

        // Column 1
        for (int i = 2; i < 12; ++i) {
            checkFlowField->getNearestWallDistance().getScalar(3, i) = 0.1;
        }

        // Column 2
        checkFlowField->getNearestWallDistance().getScalar(4, 2) = 0.15;
        checkFlowField->getNearestWallDistance().getScalar(4, 3) = 0.12;
        checkFlowField->getNearestWallDistance().getScalar(4, 4) = 0.0707;
        checkFlowField->getNearestWallDistance().getScalar(4, 5) = 0.05;
        checkFlowField->getNearestWallDistance().getScalar(4, 6) = 0.05;
        checkFlowField->getNearestWallDistance().getScalar(4, 7) = 0.05;
        checkFlowField->getNearestWallDistance().getScalar(4, 8) = 0.0707;
        checkFlowField->getNearestWallDistance().getScalar(4, 9) = 0.12;
        checkFlowField->getNearestWallDistance().getScalar(4, 10) = 0.15;
        checkFlowField->getNearestWallDistance().getScalar(4, 11) = 0.15;

        // Column 3-5
        for (int i = 5; i <= 7; ++i) {
            checkFlowField->getNearestWallDistance().getScalar(i, 2) = 0.15;
            checkFlowField->getNearestWallDistance().getScalar(i, 3) = 0.1;
            checkFlowField->getNearestWallDistance().getScalar(i, 4) = 0.05;
            checkFlowField->getNearestWallDistance().getScalar(i, 5) = 0.0;
            checkFlowField->getNearestWallDistance().getScalar(i, 6) = 0.0;
            checkFlowField->getNearestWallDistance().getScalar(i, 7) = 0.0;
            checkFlowField->getNearestWallDistance().getScalar(i, 8) = 0.05;
            checkFlowField->getNearestWallDistance().getScalar(i, 9) = 0.1;
            checkFlowField->getNearestWallDistance().getScalar(i, 10) = 0.15;
            checkFlowField->getNearestWallDistance().getScalar(i, 11) = 0.2;
        }

        // Column 6
        checkFlowField->getNearestWallDistance().getScalar(8, 2) = 0.17;
        checkFlowField->getNearestWallDistance().getScalar(8, 3) = 0.12;
        checkFlowField->getNearestWallDistance().getScalar(8, 4) = 0.0707;
        checkFlowField->getNearestWallDistance().getScalar(8, 5) = 0.05;
        checkFlowField->getNearestWallDistance().getScalar(8, 6) = 0.05;
        checkFlowField->getNearestWallDistance().getScalar(8, 7) = 0.05;
        checkFlowField->getNearestWallDistance().getScalar(8, 8) = 0.0707;
        checkFlowField->getNearestWallDistance().getScalar(8, 9) = 0.12;
        checkFlowField->getNearestWallDistance().getScalar(8, 10) = 0.17;
        checkFlowField->getNearestWallDistance().getScalar(8, 11) = 0.2;

        // Column 7
        checkFlowField->getNearestWallDistance().getScalar(9, 2) = 0.15;
        checkFlowField->getNearestWallDistance().getScalar(9, 3) = 0.141;
        checkFlowField->getNearestWallDistance().getScalar(9, 4) = 0.12;
        checkFlowField->getNearestWallDistance().getScalar(9, 5) = 0.1;
        checkFlowField->getNearestWallDistance().getScalar(9, 6) = 0.1;
        checkFlowField->getNearestWallDistance().getScalar(9, 7) = 0.1;
        checkFlowField->getNearestWallDistance().getScalar(9, 8) = 0.12;
        checkFlowField->getNearestWallDistance().getScalar(9, 9) = 0.141;
        checkFlowField->getNearestWallDistance().getScalar(9, 10) = 0.15;
        checkFlowField->getNearestWallDistance().getScalar(9, 11) = 0.15;

        // Column 8
        for (int i = 2; i < 12; ++i) {
            checkFlowField->getNearestWallDistance().getScalar(10, i) = 0.1;
        }

        // Column 9
        for (int i = 2; i < 12; ++i) {
            checkFlowField->getNearestWallDistance().getScalar(11, i) = 0.05;
        }

        // Check for correctness
        ScalarField & result = flowField->getNearestWallDistance();
        ScalarField & expected = checkFlowField->getNearestWallDistance();
        for (int j = 2; j < 12; ++j) {
            for (int i = 2; i < 12; ++i) {
                BOOST_CHECK_CLOSE_FRACTION(result.getScalar(i, j), expected.getScalar(i, j), 0.009);
                BOOST_TEST_MESSAGE("was performed at index (" + std::to_string(i) + "," + std::to_string(j) + ")");
            }
        }

        delete checkFlowField;
        delete obstacleDistanceIterator;
        delete obstacleDistanceStencil;
        delete wallDistanceIterator;
        delete wallDistanceStencil;
        delete flowField;
        delete parameters;
    }

BOOST_AUTO_TEST_SUITE_END()