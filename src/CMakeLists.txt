find_package(PETSc COMPONENTS CXX)

include_directories(.)
include_directories(3rdparty/tinyxml2)
include_directories(parallelManagers)
include_directories(solvers)
include_directories(stencils)
include_directories(${PETSC_INCLUDES})

add_definitions(${PETSC_DEFINITIONS})

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    message(">>> Debug mode")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -ggdb -pg")
ELSEIF(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
    message(">>> RelWithDebInfo mode")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -pg")
ELSE(CMAKE_BUILD_TYPE MATCHES Debug)
    message(">>> Release mode")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
ENDIF(CMAKE_BUILD_TYPE MATCHES Debug)

add_library(ns-eof
        3rdparty/tinyxml2/tinyxml2.cpp
        3rdparty/tinyxml2/tinyxml2.h
        parallelManagers/PetscParallelConfiguration.cpp
        parallelManagers/PetscParallelConfiguration.h
        parallelManagers/PetscParallelManager.cpp
        parallelManagers/PetscParallelManager.h
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
        stencils/TurbulenceFGHStencil.cpp
        stencils/TurbulenceFGHStencil.h
        stencils/TurbulenceVTKStencil.cpp
        stencils/TurbulenceVTKStencil.h
        stencils/VelocityStencil.cpp
        stencils/VelocityStencil.h
        stencils/VTKStencil.h
        stencils/VTKStencil.cpp
	stencils/TurbulentViscosityStencil.h
	stencils/TurbulentViscosityStencil.cpp
        stencils/ObstacleDistanceStencil.h
        stencils/ObstacleDistanceStencil.cpp
        stencils/WallDistanceStencil.h
        stencils/WallDistanceStencil.cpp
        stencils/communicationStencils/CommunicationStencil.cpp
		stencils/communicationStencils/Direction.cpp
		stencils/communicationStencils/PressureBufferFillStencil.cpp
		stencils/communicationStencils/PressureBufferReadStencil.cpp
		stencils/communicationStencils/VelocityBufferFillStencil.cpp
		stencils/communicationStencils/VelocityBufferReadStencil.cpp
		stencils/communicationStencils/CommunicationStencil.h
		stencils/communicationStencils/Direction.h
		stencils/communicationStencils/PressureBufferFillStencil.h
		stencils/communicationStencils/PressureBufferReadStencil.h
		stencils/communicationStencils/VelocityBufferFillStencil.h
		stencils/communicationStencils/VelocityBufferReadStencil.h
		stencils/communicationStencils/WallDistanceBufferFillStencil.h
		stencils/communicationStencils/WallDistanceBufferReadStencil.h
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
        Meshsize.cpp
        Meshsize.h
        MeshsizeFactory.h
        Parameters.h
        SimpleTimer.cpp
        SimpleTimer.h
        Simulation.h
		Stencil.h 
		TurbulentSimulation.h stencils/communicationStencils/ViscosityBufferFillStencil.cpp stencils/communicationStencils/ViscosityBufferFillStencil.h stencils/communicationStencils/ViscosityBufferReadStencil.cpp stencils/communicationStencils/ViscosityBufferReadStencil.h stencils/communicationStencils/WallDistanceBufferFillStencil.cpp stencils/communicationStencils/WallDistanceBufferFillStencil.h stencils/communicationStencils/WallDistanceBufferReadStencil.cpp stencils/communicationStencils/WallDistanceBufferReadStencil.h)

target_link_libraries(ns-eof ${PETSC_LIBRARIES})

add_executable(ns main.cpp)
target_link_libraries(ns ns-eof)

if(MPI_COMPILE_FLAGS)
    set_target_properties(ns PROPERTIES
            COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()

if(MPI_LINK_FLAGS)
    set_target_properties(ns PROPERTIES
            LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()
