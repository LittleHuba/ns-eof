#include "VTKStencil.h"

VTKStencil::VTKStencil(const Parameters &parameters) : FieldStencil(parameters) {
    // Create the structured grid
    _polyData = vtkSmartPointer<vtkPolyData>::New();

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
             k <= parameters.parallel.firstCorner[2] + parameters.parallel.localSize[2];
             ++k) {
            for (int j = parameters.parallel.firstCorner[1];
                 j <= parameters.parallel.firstCorner[1] + parameters.parallel.localSize[1];
                 ++j) {
                for (int i = parameters.parallel.firstCorner[0];
                     i <= parameters.parallel.firstCorner[0] + parameters.parallel.localSize[0];
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
    _pressureScalars->SetNumberOfComponents(1);
    _velocityVectors = vtkSmartPointer<vtkDoubleArray>::New();
    _velocityVectors->SetNumberOfComponents(parameters.geometry.dim);

    _writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();


    // Link the everything to the grid
    _polyData->SetPoints(_points);
    _points->Delete();

    // We can provide our data to the writer, since we will update the data within the structure
    _writer->SetInputData(_polyData);
}

void VTKStencil::apply(FlowField &flowField, int i, int j) {
    FLOAT pressure;
    FLOAT velocity[2];
    flowField.getPressureAndVelocity(pressure, velocity, i, j);
    _pressureScalars->InsertNextValue(pressure);
    _velocityVectors->InsertNextTuple(velocity);
}

void VTKStencil::apply(FlowField &flowField, int i, int j, int k) {
    FLOAT pressure;
    FLOAT velocity[3];
    flowField.getPressureAndVelocity(pressure, velocity, i, j, k);
    _pressureScalars->InsertNextValue(pressure);
    _velocityVectors->InsertNextTuple(velocity);
}

void VTKStencil::write(FlowField &flowField, int timeStep) {
    _polyData->GetCellData()->SetVectors(_velocityVectors);
    _polyData->GetPointData()->SetScalars(_pressureScalars);
    std::string filename = _parameters.vtk.prefix + "_" + std::to_string(timeStep) + ".vts";
    _writer->SetFileName(filename.c_str());
    _writer->Write();

    // Prepare data structures for next timestep
    _velocityVectors->Reset();
    _pressureScalars->Reset();
}

VTKStencil::~VTKStencil() {
    _velocityVectors->Delete();
    _pressureScalars->Delete();
    _polyData->Delete();
    _writer->Delete();
}
