//
// Created by tommaso on 22/11/18.
//

#include <boost/test/unit_test.hpp>
#include "../../../src/stencils/communicationStencils/Direction.h"

BOOST_AUTO_TEST_SUITE(TestDirection)

BOOST_AUTO_TEST_CASE(DirectionEnumDefinition)
{
    BOOST_CHECK_GE(LEFT, 0);    BOOST_CHECK_LE(LEFT, NUMBER_OF_DIRECTIONS);
    BOOST_CHECK_GE(RIGHT, 0);   BOOST_CHECK_LE(RIGHT, NUMBER_OF_DIRECTIONS);
    BOOST_CHECK_GE(BOTTOM, 0);  BOOST_CHECK_LE(BOTTOM, NUMBER_OF_DIRECTIONS);
    BOOST_CHECK_GE(TOP, 0);     BOOST_CHECK_LE(TOP, NUMBER_OF_DIRECTIONS);
    BOOST_CHECK_GE(FRONT, 0);   BOOST_CHECK_LE(FRONT, NUMBER_OF_DIRECTIONS);
    BOOST_CHECK_GE(BACK, 0);    BOOST_CHECK_LE(BACK, NUMBER_OF_DIRECTIONS);
    
    BOOST_CHECK_EQUAL(abs(LEFT - RIGHT), 1);
    BOOST_CHECK_EQUAL(abs(BOTTOM - TOP), 1);
    BOOST_CHECK_EQUAL(abs(FRONT - BACK), 1);
}

BOOST_AUTO_TEST_CASE(OppositeDirectionCheck)
{
    BOOST_CHECK_EQUAL(DirectionUtils::getOppositeDirection(LEFT), RIGHT);
    BOOST_CHECK_EQUAL(DirectionUtils::getOppositeDirection(RIGHT), LEFT);
    BOOST_CHECK_EQUAL(DirectionUtils::getOppositeDirection(BOTTOM), TOP);
    BOOST_CHECK_EQUAL(DirectionUtils::getOppositeDirection(TOP), BOTTOM);
    BOOST_CHECK_EQUAL(DirectionUtils::getOppositeDirection(FRONT), BACK);
    BOOST_CHECK_EQUAL(DirectionUtils::getOppositeDirection(BACK), FRONT);
}

BOOST_AUTO_TEST_SUITE_END()
