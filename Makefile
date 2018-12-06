#### The petsc environment can also be fixed in the makefile
PETSC_DIR=/usr/local
# PETSC_ARCH=arch-darwin-c-debug
# PETSC_DIR = 
# PETSC_ARCH = 
include ${PETSC_DIR}/conf/variables

# default gnu compiler (currently not used)
CC = g++
# compiler wrapper for mac-cluster
# CC = 
# compiler on Ubuntu
#
#
#CC = mpic++
CFLAGS = -Wall -Werror -O3 
SRCDIR = ./
INCLUDE = -I. -Istencils ${PETSC_CC_INCLUDES}


NSMAIN = main.o

OBJ = DataStructures.o Configuration.o 3rdparty/tinyxml2/tinyxml2.o SimpleTimer.o

NSOBJ = FlowField.o LinearSolver.o Meshsize.o\
stencils/MaxUStencil.o stencils/MovingWallStencils.o stencils/PeriodicBoundaryStencils.o\
stencils/FGHStencil.o solvers/SORSolver.o solvers/PetscSolver.o \
stencils/RHSStencil.o stencils/VelocityStencil.o \
stencils/VTKStencil.o parallelManagers/PetscParallelConfiguration.o\
parallelManagers/PetscParallelManager.o\
GlobalBoundaryFactory.o\
stencils/BFStepInitStencil.o stencils/NeumannBoundaryStencils.o stencils/BFInputStencils.o stencils/ObstacleStencil.o\

all: ns

ns: 
	@printf "\033[0;31mUsing make is deprecated!!! Use cmake instead (Read the README for instructions)\033[0m\r\n"
	$(OBJ) $(NSOBJ) $(NSMAIN)
	$(CC) -o ns $(OBJ) $(NSOBJ) $(NSMAIN) $(PETSC_KSP_LIB) -lstdc++ $(CFLAGS)


%.o: %.cpp
	$(CC) -c $(CFLAGS) $(INCLUDE) -o $*.o $*.cpp $(PETSC_KSP_LIB) -lstdc++

cleanall clean:
	for name in  ns main.o $(NSOBJ) $(OBJ) ; do \
	if [ -f $$name ]; then rm $$name; fi; \
	done;
