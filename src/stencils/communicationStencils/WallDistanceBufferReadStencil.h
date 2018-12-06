//
// Created by keefe on 12/4/18.
//

#ifndef NS_WALLDISTANCEBUFFERREADSTENCIL_H
#define NS_WALLDISTANCEBUFFERREADSTENCIL_H

#include "CommunicationStencil.h"

class WallDistanceBufferReadStencil :public CommunicationStencil<FLOAT>{
public:
    explicit WallDistanceBufferReadStencil(const Parameters &parameters) : CommunicationStencil(parameters)
    {}

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


#endif //NS_WALLDISTANCEBUFFERREADSTENCIL_H
