//
// Created by tommaso on 22/11/18.
//

#ifndef NS_COMMUNICATIONSTENCIL_H
#define NS_COMMUNICATIONSTENCIL_H


#include <FlowField.h>
#include <Stencil.h>

/**
 * This is the basic communication stencil, extend it to get the fill/read stencils for pressure and velocity.
 * If the target is to handle scalars, you can e.g. use FLOAT as type.
 * If the target is to handle vector-valued quantities, as velocity, then better to use a Triple or some array.
 *
 * @tparam BufferValue
 */
template <class BufferValue>
class CommunicationStencil : public BoundaryStencil<FlowField>
{
protected:
    BufferValue **buffers = nullptr;
    int lowOffset = 0, highOffset = 0;
    
protected:
    CommunicationStencil(const Parameters &parameters, int xOffset, int yOffset)
            : BoundaryStencil(parameters), buffers(buffers), lowOffset(xOffset), highOffset(yOffset)
    {}

public:
    virtual void initializeBuffers(BufferValue **buffers)
    {
        this->buffers = buffers;
    }
};


#endif //NS_COMMUNICATIONSTENCIL_H
