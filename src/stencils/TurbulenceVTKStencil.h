#ifndef _TVTK_STENCIL_H_
#define _TVTK_STENCIL_H_

#include "../Definitions.h"
#include "../Parameters.h"
#include "VTKStencil.h"
#include "../FlowField.h"
#include <string>
#include <fstream>
#include <sstream>

/** Stencil for writing VTK files
 *
 * When iterated with, creates a VTK file.
 */
class TurbulenceVTKStencil : public VTKStencil {
private:
    std::stringstream _viscosityStream;

    public:

        /** Constructor
         *
         * @param prefix String with the prefix of the name of the VTK files
         */
        explicit TurbulenceVTKStencil ( const Parameters & parameters );

    /** 2D operation for one position
     *
     * @param flowField State of the flow field
     * @param i Position in the x direction
     * @param j Position in the y direction
     */
        void apply ( FlowField & flowField, int i, int j ) override;

        /** 3D operation for one position
         *
         * @param flowField State of the flow field
         * @param i Position in the x direction
         * @param j Position in the y direction
         * @param k Position in the z direction
         */
        void apply ( FlowField & flowField, int i, int j, int k ) override;

        /** Writes the information to the file
         * @param flowField Flow field to be written
         */
        void write ( FlowField & flowField, int timeStep ) override;

};

#endif