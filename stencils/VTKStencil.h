#ifndef _VTK_STENCIL_H_
#define _VTK_STENCIL_H_

#include "../Definitions.h"
#include "../Parameters.h"
#include "../Stencil.h"
#include "../FlowField.h"
#include <string>
#include <fstream>
#include <sstream>
#include <vtk/vtkCellArray.h>
#include <vtk/vtkPoints.h>
#include <vtk/vtkPointData.h>
#include <vtk/vtkCellData.h>
#include <vtk/vtkDoubleArray.h>
#include <vtk/vtkXMLPolyDataWriter.h>
#include <vtk/vtkPolyData.h>
#include <vtk/vtkSmartPointer.h>

/** Stencil for writting VTK files
 *
 * When iterated with, creates a VTK file.
 */
class VTKStencil : public FieldStencil<FlowField> {
    vtkSmartPointer<vtkPolyData> _polyData;
    vtkSmartPointer<vtkPoints> _points;
    vtkSmartPointer<vtkDoubleArray> _pressureScalars;
    vtkSmartPointer<vtkDoubleArray> _velocityVectors;
    vtkSmartPointer<vtkXMLPolyDataWriter> _writer;

    public:

        /** Constructor
         *
         * @param prefix String with the prefix of the name of the VTK files
         */
        VTKStencil ( const Parameters & parameters );

    /** 2D operation for one position
     *
     * @param flowField State of the flow field
     * @param i Position in the x direction
     * @param j Position in the y direction
     */
        void apply ( FlowField & flowField, int i, int j );

        /** 3D operation for one position
         *
         * @param flowField State of the flow field
         * @param i Position in the x direction
         * @param j Position in the y direction
         * @param k Position in the z direction
         */
        void apply ( FlowField & flowField, int i, int j, int k );

        /** Writes the information to the file
         * @param flowField Flow field to be written
         */
        void write ( FlowField & flowField, int timeStep );

    ~VTKStencil() override;

};

#endif
