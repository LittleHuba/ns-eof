//
// Created by ulrich on 26.11.18.
//

#ifndef NS_WALLDISTANCESTENCIL_H
#define NS_WALLDISTANCESTENCIL_H


#include <FlowField.h>
#include <Stencil.h>

class WallDistanceStencil : public BoundaryStencil<FlowField> {
public:
    WallDistanceStencil(const Parameters &parameters);

    void applyLeftWall(FlowField &flowField, int i, int j) override;

    void applyRightWall(FlowField &flowField, int i, int j) override;

    void applyBottomWall(FlowField &flowField, int i, int j) override;

    void applyTopWall(FlowField &flowField, int i, int j) override;

    void applyLeftWall(FlowField &flowField, int i, int j, int k) override;

    void applyRightWall(FlowField &flowField, int i, int j, int k) override;

    void applyBottomWall(FlowField &flowField, int i, int j, int k) override;

    void applyTopWall(FlowField &flowField, int i, int j, int k) override;

    void applyFrontWall(FlowField &flowField, int i, int j, int k) override;

    void applyBackWall(FlowField &flowField, int i, int j, int k) override;
};


#endif //NS_WALLDISTANCESTENCIL_H
