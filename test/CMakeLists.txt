find_package (Boost COMPONENTS unit_test_framework REQUIRED)

include_directories (${TEST_SOURCE_DIR}/src
        ${Boost_INCLUDE_DIRS})

add_definitions (-DBOOST_TEST_DYN_LINK)

add_executable (Test test_main.cpp stencils/communicationStencils/Direction.test.cpp)
target_link_libraries (Test
        ${PETSC_LIBRARIES}
        ns-eof
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test (NAME NS-EOF-Test COMMAND boost_test)
