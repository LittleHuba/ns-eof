#ifndef _VTK_STENCIL_H_
#define _VTK_STENCIL_H_

#include "../Definitions.h"
#include "../Parameters.h"
#include "../Stencil.h"
#include "../FlowField.h"
#include <string>
#include <mpi.h>
#include <XdmfDomain.hpp>
#include <XdmfGridCollection.hpp>
#include <XdmfHDF5WriterDSM.hpp>

/** Stencil for writing VTK files
 *
 * When iterated with, creates a XDMF file.
 */
class XDMFStencil : public FieldStencil<FlowField> {
protected:
    shared_ptr<XdmfHDF5WriterDSM> writer;
    shared_ptr<XdmfDomain> root;
    shared_ptr<XdmfTopology> topology;
    shared_ptr<XdmfGeometry> geometry;

    shared_ptr<XdmfArray> velocity;
    shared_ptr<XdmfArray> pressure;
    shared_ptr<XdmfArray> viscosity;
    shared_ptr<XdmfArray> wallDistance;


public:

    /** Constructor
     *
     * @param prefix String with the prefix of the name of the VTK files
     */
    explicit XDMFStencil(const Parameters &parameters);

    ~XDMFStencil() override;

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
