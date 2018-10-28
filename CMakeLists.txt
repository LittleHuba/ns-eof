cmake_minimum_required(VERSION 3.12)
project(codeframe)

list(APPEND CMAKE_MODULE_PATH "${codeframe_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${codeframe_SOURCE_DIR}/cmake-modules")

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_COMPILER mpicxx)

find_package(PETSc COMPONENTS CXX)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

include_directories(.)
include_directories(3rdparty/tinyxml2)
include_directories(parallelManagers)
include_directories(solvers)
include_directories(stencils)
include_directories(${PETSC_INCLUDES})

add_definitions(${PETSC_DEFINITIONS})
add_definitions(-g)

add_executable(codeframe
        3rdparty/tinyxml2/tinyxml2.cpp
        3rdparty/tinyxml2/tinyxml2.h
        parallelManagers/PetscParallelConfiguration.cpp
        parallelManagers/PetscParallelConfiguration.h
        solvers/PetscSolver.cpp
        solvers/PetscSolver.h
        solvers/SORSolver.cpp
        solvers/SORSolver.h
        stencils/BFInputStencils.cpp
        stencils/BFInputStencils.h
        stencils/BFStepInitStencil.cpp
        stencils/BFStepInitStencil.h
        stencils/FGHStencil.cpp
        stencils/FGHStencil.h
        stencils/InitTaylorGreenFlowFieldStencil.h
        stencils/MaxUStencil.cpp
        stencils/MaxUStencil.h
        stencils/MovingWallStencils.cpp
        stencils/MovingWallStencils.h
        stencils/NeumannBoundaryStencils.cpp
        stencils/NeumannBoundaryStencils.h
        stencils/ObstacleStencil.cpp
        stencils/ObstacleStencil.h
        stencils/PeriodicBoundaryStencils.cpp
        stencils/PeriodicBoundaryStencils.h
        stencils/RHSStencil.cpp
        stencils/RHSStencil.h
        stencils/StencilFunctions.h
        stencils/VelocityStencil.cpp
        stencils/VelocityStencil.h
        stencils/VTKStencil.h
        stencils/VTKStencil.cpp
        Configuration.cpp
        Configuration.h
        DataStructures.cpp
        DataStructures.h
        Definitions.h
        FlowField.cpp
        FlowField.h
        GlobalBoundaryFactory.cpp
        GlobalBoundaryFactory.h
        Iterators.h
        LinearSolver.cpp
        LinearSolver.h
        main.cpp
        Makefile
        Meshsize.cpp
        Meshsize.h
        MeshsizeFactory.h
        Parameters.h
        SimpleTimer.cpp
        SimpleTimer.h
        Simulation.h
        Stencil.h)

target_link_libraries(codeframe ${PETSC_LIBRARIES} ${VTK_LIBRARIES})

if(MPI_COMPILE_FLAGS)
      set_target_properties(codeframe PROPERTIES
              COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()

if(MPI_LINK_FLAGS)
    set_target_properties(codeframe PROPERTIES
            LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()
