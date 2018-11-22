//
// Created by tommaso on 22/11/18.
//

#ifndef NS_PRESSUREBUFFERFILLSTENCIL_H
#define NS_PRESSUREBUFFERFILLSTENCIL_H


#include <FlowField.h>
#include <Stencil.h>

class PressureBufferFillStencil : public BoundaryStencil<FlowField>
{
private:
    FLOAT **buffers;

public:
    PressureBufferFillStencil(const Parameters &parameters, FLOAT **buffers) : BoundaryStencil(parameters),
                                                                               buffers(buffers)
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


#endif //NS_PRESSUREBUFFERFILLSTENCIL_H
