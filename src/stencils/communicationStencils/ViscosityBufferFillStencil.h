//
// Created by keefe on 12/4/18.
//

#ifndef NS_VISCOSITYBUFFERFILLSTENCIL_H
#define NS_VISCOSITYBUFFERFILLSTENCIL_H


#include "CommunicationStencil.h"

class ViscosityBufferFillStencil : public CommunicationStencil<FLOAT>{
public:
    explicit ViscosityBufferFillStencil(const Parameters &parameters) : CommunicationStencil(parameters)
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


#endif //NS_VISCOSITYBUFFERFILLSTENCIL_H
