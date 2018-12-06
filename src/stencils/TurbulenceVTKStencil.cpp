#include "TurbulenceVTKStencil.h"

TurbulenceVTKStencil::TurbulenceVTKStencil(const Parameters &parameters) : VTKStencil(parameters) {
    this->_viscosityStream.precision(6);
    this->_viscosityStream << std::fixed;
}

inline void TurbulenceVTKStencil::apply(FlowField &flowField, int i, int j) {
    VTKStencil::apply(flowField, i, j);

    if ((flowField.getFlags().getValue(i, j) & OBSTACLE_SELF) == 0) {
        // Write viscosity to viscosity data stream
        this->_viscosityStream << flowField.getTurbulentViscosity().getScalar(i, j) << std::endl;
    } else {
        // Write viscosity to viscosity data stream
        this->_viscosityStream << 0.0 << std::endl;
    }
}

inline void TurbulenceVTKStencil::apply(FlowField &flowField, int i, int j, int k) {
    VTKStencil::apply(flowField, i, j, k);

    if ((flowField.getFlags().getValue(i, j, k) & OBSTACLE_SELF) == 0) {
        // Write viscosity to viscosity data stream
        this->_viscosityStream << flowField.getTurbulentViscosity().getScalar(i, j, k) << std::endl;
    } else {
        // Write viscosity to viscosity data stream
        this->_viscosityStream << 0.0 << std::endl;
    }
}

void TurbulenceVTKStencil::write(FlowField &flowField, int timeStep) {
    std::ofstream vtkFile;
    write(flowField, timeStep, &vtkFile);
}

void TurbulenceVTKStencil::write(FlowField &flowField, int timeStep, std::basic_ofstream<char> *vtkFile) {
    if(_parameters.parallel.rank==0) std::cout << "Writing VTK output for timestep " << std::to_string(timeStep) << std::endl;
    std::string filename = _parameters.vtk.prefix + "_" + std::to_string(_parameters.parallel.rank)
                           + "_" + std::to_string(timeStep) + ".vtk";

    vtkFile->open(filename);
    if (vtkFile->is_open()) {
        // Write the header and points to the file
        *vtkFile << this->_pointsStream.str();

        // Write the pressure data to the file
        *vtkFile << "SCALARS pressure float 1" << std::endl;
        *vtkFile << "LOOKUP_TABLE default" << std::endl;
        *vtkFile << this->_pressureStream.rdbuf();

        // Write the velocity data to the file
        *vtkFile << std::endl;
        *vtkFile << "VECTORS velocity float" << std::endl;
        *vtkFile << this->_velocityStream.rdbuf();

        // Write the viscosity data to the file
        *vtkFile << std::endl;
        *vtkFile << "SCALARS viscosity float 1" << std::endl;
        *vtkFile << "LOOKUP_TABLE default" << std::endl;
        *vtkFile << this->_viscosityStream.rdbuf();
        *vtkFile << std::endl << std::endl;

        vtkFile->close();
    } else std::cerr << "Unable to open vtk output file for timestep " << timeStep << std::endl;

    // Empty the data streams for the next timestep
    this->_pressureStream << std::fixed;
    this->_velocityStream << std::scientific;
    this->_viscosityStream << std::fixed;
}
