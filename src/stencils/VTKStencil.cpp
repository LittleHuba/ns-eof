#include "VTKStencil.h"

VTKStencil::VTKStencil(const Parameters &parameters) : FieldStencil(parameters) {
    // Get some initial information about the grid
    int cellsX = parameters.parallel.localSize[0];
    int cellsY = parameters.parallel.localSize[1];
    int cellsZ = 0;

    if(_parameters.geometry.dim == 3){
       cellsZ = parameters.parallel.localSize[2];
    }

    int pointsX = cellsX + 1;
    int pointsY = cellsY + 1;
    int pointsZ = cellsZ + 1;

    // Write the header of the file
    this->_pointsStream.precision(6);
    this->_pointsStream << std::fixed;
    this->_pointsStream << "# vtk DataFile Version 2.0" << std::endl;
    this->_pointsStream << "Some generic header information for whatever reason" << std::endl;
    this->_pointsStream << "ASCII" << std::endl
                        << std::endl;

    // Write the header for the point coordinates
    this->_pointsStream << "DATASET STRUCTURED_GRID" << std::endl;
    this->_pointsStream << "DIMENSIONS " << pointsX << " " << pointsY << " " << pointsZ << std::endl;
    this->_pointsStream << "POINTS " << pointsX * pointsY * pointsZ << " float" << std::endl;

    // Write the points of the grid
    for (int k = 2; k < cellsZ + 3; k++) {
            for (int j = 2; j < cellsY + 3; j++) {
                for (int i = 2; i < cellsX + 3; i++) {

                    this->_pointsStream << (_parameters.meshsize->getPosX(i, j, k)) << " "
                            << (_parameters.meshsize->getPosY(i, j, k)) << " "
                            << (_parameters.meshsize->getPosZ(i, j, k)) << std::endl;
                }
            }
        }


    if(_parameters.geometry.dim == 2){
        cellsZ = 1;
    }

    this->_pointsStream << std::endl;
    if (parameters.geometry.dim == 2)
        this->_pointsStream << "CELL_DATA " << cellsX * cellsY << std::endl;
    else if (parameters.geometry.dim == 3)
        this->_pointsStream << "CELL_DATA " << cellsX * cellsY * cellsZ << std::endl;

    // Set some parameters for the pressure and velocity streams
    this->_pressureStream.precision(6);
    this->_pressureStream << std::fixed;
    this->_velocityStream.precision(6);
    this->_velocityStream << std::scientific;
}

inline void VTKStencil::apply(FlowField &flowField, int i, int j) {
    if ((flowField.getFlags().getValue(i, j) & OBSTACLE_SELF) == 0) {
        // Get pressure and velocity
        FLOAT pressure;
        FLOAT velocity[2];
        flowField.getPressureAndVelocity(pressure, velocity, i, j);

        // Write pressure to pressure data stream
        this->_pressureStream << pressure << std::endl;

        // Write velocity components to velocity data stream
        this->_velocityStream << velocity[0] << " " << velocity[1] << " " << 0.0 << std::endl;
    } else {
        // Write pressure to pressure data stream
        this->_pressureStream << 0.0 << std::endl;

        // Write velocity components to velocity data stream
        this->_velocityStream << 0.0 << " " << 0.0 << " " << 0.0 << std::endl;
    }
}

inline void VTKStencil::apply(FlowField &flowField, int i, int j, int k) {
    if ((flowField.getFlags().getValue(i, j, k) & OBSTACLE_SELF) == 0) {
        // Get pressure and velocity
        FLOAT pressure;
        FLOAT velocity[3];
        flowField.getPressureAndVelocity(pressure, velocity, i, j, k);

        // Write pressure to pressure data stream
        this->_pressureStream << pressure << std::endl;

        // Write velocity components to velocity data stream
        this->_velocityStream << velocity[0] << " " << velocity[1] << " " << velocity[2] << std::endl;
    } else {
        // Write pressure to pressure data stream
        this->_pressureStream << 0.0 << std::endl;

        // Write velocity components to velocity data stream
        this->_velocityStream << 0.0 << " " << 0.0 << " " << 0.0 << std::endl;
    }
}

void VTKStencil::write(int timeStep) {
    std::ofstream vtkFile;
    write(timeStep, vtkFile);
}

void VTKStencil::write(int timeStep, std::basic_ofstream<char> &vtkFile) {
    if(_parameters.parallel.rank==0) std::cout << "Writing VTK output for timestep " << std::to_string(timeStep) << std::endl;
    std::string filename = _parameters.vtk.prefix + "_" + std::to_string(_parameters.parallel.rank) + "_" + std::to_string(timeStep) + ".vtk";

    vtkFile.open(filename);
    if (vtkFile.is_open()) {
        // Write the header and points to the file
        vtkFile << this->_pointsStream.str();

        // Write the pressure data to the file
        vtkFile << "SCALARS pressure float 1" << std::endl;
        vtkFile << "LOOKUP_TABLE default" << std::endl;
        vtkFile << this->_pressureStream.rdbuf();

        // Write the velocity data to the file
        vtkFile << std::endl;
        vtkFile << "VECTORS velocity float" << std::endl;
        vtkFile << this->_velocityStream.rdbuf();
        vtkFile << std::endl << std::endl;

        vtkFile.close();
    } else std::cerr << "Unable to open vtk output file for timestep " << timeStep << std::endl;

    // Empty the data streams for the next timestep
    this->_pressureStream << std::fixed;
    this->_velocityStream << std::scientific;
}
