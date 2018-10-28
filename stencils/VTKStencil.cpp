#include "VTKStencil.h"

VTKStencil::VTKStencil(const Parameters &parameters) : FieldStencil(parameters) {}

void VTKStencil::apply ( FlowField & flowField, int i, int j ) {
}

void VTKStencil::apply ( FlowField & flowField, int i, int j, int k ) {

}

void VTKStencil::write ( FlowField & flowField, int timeStep ) {

}