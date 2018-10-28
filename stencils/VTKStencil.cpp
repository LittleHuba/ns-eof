#include "VTKStencil.h"

VTKStencil::VTKStencil(const Parameters &parameters) : FieldStencil(parameters) {
    // Create the structured grid
    _structuredGrid = vtkSmartPointer<vtkStructuredGrid>::New();
    // Set the dimensions of the grid
    // TODO: this would be more beautiful if we could directly use the array
    _structuredGrid->SetDimensions(parameters.parallel.localSize[0],
                                   parameters.parallel.localSize[1],
                                   parameters.parallel.localSize[2]);

    // Create the points for the grid
    _points = vtkSmartPointer<vtkPoints>::New();

    // Write the points of the grid
    if (parameters.geometry.dim == 2) {
        for (int j = parameters.parallel.firstCorner[1];
             j < parameters.parallel.firstCorner[1] + parameters.parallel.localSize[1];
             ++j) {
            for (int i = parameters.parallel.firstCorner[0];
                 i < parameters.parallel.firstCorner[0] + parameters.parallel.localSize[0];
                 ++i) {
                _points->InsertNextPoint(parameters.meshsize->getPosX(i, j),
                                         parameters.meshsize->getPosY(i, j),
                                         0);
            }
        }
    }

    if (parameters.geometry.dim == 3) {
        for (int k = parameters.parallel.firstCorner[2];
             k < parameters.parallel.firstCorner[2] + parameters.parallel.localSize[2];
             ++k) {
            for (int j = parameters.parallel.firstCorner[1];
                 j < parameters.parallel.firstCorner[1] + parameters.parallel.localSize[1];
                 ++j) {
                for (int i = parameters.parallel.firstCorner[0];
                     i < parameters.parallel.firstCorner[0] + parameters.parallel.localSize[0];
                     ++i) {
                    _points->InsertNextPoint(parameters.meshsize->getPosX(i, j, k),
                                             parameters.meshsize->getPosY(i, j, k),
                                             parameters.meshsize->getPosZ(i, j, k));
                }
            }
        }
    }

    // Create the data structures
    _pressureScalars = vtkSmartPointer<vtkDoubleArray>::New();
    _pressureScalars->SetNumberOfComponents(parameters.geometry.dim);
    _pressureScalars->SetNumberOfTuples(
            parameters.parallel.localSize[0] * parameters.parallel.localSize[1] * parameters.parallel.localSize[2]);
    _velocityVectors = vtkSmartPointer<vtkDoubleArray>::New();

    _writer = vtkSmartPointer<vtkXMLStructuredGridWriter>::New();


    // Link the everything to the grid
    _structuredGrid->SetPoints(_points);
    _points->Delete();
    _structuredGrid->GetPointData()->SetVectors(_velocityVectors);
    _structuredGrid->GetCellData()->SetScalars(_pressureScalars);

    // We can provide our data to the writer, since we will update the data within the structure
    _writer->SetInputData(_structuredGrid);
}

void VTKStencil::apply(FlowField &flowField, int i, int j) {
    FLOAT pressure;
    FLOAT velocity[2];
    flowField.getPressureAndVelocity(pressure, velocity, i, j);
    _pressureScalars->SetValue(i * j, pressure);
    _velocityVectors->SetTuple(i * j, velocity);
}

void VTKStencil::apply(FlowField &flowField, int i, int j, int k) {
    FLOAT pressure;
    FLOAT velocity[3];
    flowField.getPressureAndVelocity(pressure, velocity, i, j, k);
    _pressureScalars->SetValue(i * j * k, pressure);
    _velocityVectors->SetTuple(i * j * k, velocity);
}

void VTKStencil::write(FlowField &flowField, int timeStep) {
    std::string filename = _parameters.vtk.prefix + "_" + std::to_string(timeStep) + ".vts";
    _writer->SetFileName(filename.c_str());
    _writer->Write();
}

VTKStencil::~VTKStencil() {
}
