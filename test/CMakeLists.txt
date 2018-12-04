find_package (Boost COMPONENTS unit_test_framework REQUIRED)

include_directories (${TEST_SOURCE_DIR}/src
        ${Boost_INCLUDE_DIRS})

add_definitions (-DBOOST_TEST_DYN_LINK)

include_directories(stencils)

add_executable (boost_test
        test_main.cpp
        stencils/communicationStencils/Direction.test.cpp
        stencils/VTKStencilTests.cpp)

target_link_libraries (boost_test
        ${PETSC_LIBRARIES}
        ns-eof
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test (NAME NS-EOF-Test COMMAND boost_test)
