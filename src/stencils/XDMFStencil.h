#ifndef _XDMF_STENCIL_H_
#define _XDMF_STENCIL_H_

#include "../Definitions.h"
#include "../Parameters.h"
#include "../Stencil.h"
#include "../FlowField.h"
#include <string>
#include <mpi.h>
#include <hdf5.h>
#include <vector>
#include <fstream>

/** Stencil for writing VTK files
 *
 * When iterated with, creates a XDMF file.
 */
class XDMFStencil : public FieldStencil<FlowField> {
protected:
    hid_t file_id;
    hid_t dxpl_id;
    std::ofstream xdmfFile;
    std::vector<FLOAT> velocity;
    std::vector<FLOAT> pressure;
    std::vector<FLOAT> viscosity;

    unsigned long allPoints;
    long allPointsX, allPointsY, allPointsZ;
    unsigned long previousPoints;
    unsigned long cells, cellsX, cellsY, cellsZ;
    unsigned long allCells;
    unsigned long previousCells;


public:

    /** Constructor
     *
     * @param prefix String with the prefix of the name of the VTK files
     */
    explicit XDMFStencil(FlowField &flowField, const Parameters &parameters);

    /** 2D operation for one position
     *
     * @param flowField State of the flow field
     * @param i Position in the x direction
     * @param j Position in the y direction
     */

    void apply(FlowField &flowField, int i, int j) override;

    /** 3D operation for one position
     *
     * @param flowField State of the flow field
     * @param i Position in the x direction
     * @param j Position in the y direction
     * @param k Position in the z direction
     */
    void apply(FlowField &flowField, int i, int j, int k) override;

    /** Writes the information to the file
     * @param timeStep The timestep to write
     */
    virtual void write(int timeStep);

};

#endif
