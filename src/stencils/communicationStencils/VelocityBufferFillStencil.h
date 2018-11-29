//
// Created by tommaso on 23/11/18.
//

#ifndef NS_VELOCITYBUFFERFILLSTENCIL_H
#define NS_VELOCITYBUFFERFILLSTENCIL_H


#include "CommunicationStencil.h"

class VelocityBufferFillStencil : public CommunicationStencil<Triple<FLOAT>>
{
public:
    explicit VelocityBufferFillStencil(const Parameters &parameters) : CommunicationStencil(parameters)
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


#endif //NS_VELOCITYBUFFERFILLSTENCIL_H
