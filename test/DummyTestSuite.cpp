#define BOOST_TEST_MODULE NSEOFTests
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(FailTest)
{
    BOOST_CHECK_EQUAL(5, 2);
}

BOOST_AUTO_TEST_CASE(PassTest)
{
    BOOST_CHECK_EQUAL(4, 4);
}