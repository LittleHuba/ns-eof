find_package (Boost COMPONENTS unit_test_framework REQUIRED)

include_directories (${TEST_SOURCE_DIR}/src
        ${Boost_INCLUDE_DIRS})

add_definitions (-DBOOST_TEST_DYN_LINK)

include_directories(stencils)

add_executable (boost_test
        DummyTestSuite.cpp
        stencils/VTKStencilTests.cpp
        stencils/TurbulenceVTKStencilTests.cpp
        TestingFunctions.h
	vt_interpolate_Test.cpp)

target_link_libraries (boost_test
        ${PETSC_LIBRARIES}
        ns-eof
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test (NAME NS-EOF-Test COMMAND boost_test)
