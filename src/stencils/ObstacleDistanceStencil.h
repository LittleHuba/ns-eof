#ifndef _WALL_DISTANCE_STENCIL_H_
#define _WALL_DISTANCE_STENCIL_H_

#include "../Definitions.h"
#include "../Parameters.h"
#include "../Stencil.h"
#include "../FlowField.h"

/** Stencil for writing VTK files
 *
 * When iterated with, creates a VTK file.
 */
class ObstacleDistanceStencil : public FieldStencil<FlowField> {

public:

    /** Constructor
     *
     */
    explicit ObstacleDistanceStencil(const Parameters &parameters, FlowField &flowField);

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

};

#endif
