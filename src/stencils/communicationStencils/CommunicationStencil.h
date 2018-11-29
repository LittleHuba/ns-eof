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
    
protected:
    explicit CommunicationStencil(const Parameters &parameters) : BoundaryStencil(parameters) {}
    
public:
    virtual void initializeBuffers(BufferValue **buffers)
    {
        this->buffers = buffers;
    }
};


#endif //NS_COMMUNICATIONSTENCIL_H
