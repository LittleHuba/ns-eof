#include <XdmfHDF5ControllerDSM.hpp>
#include "XDMFStencil.h"

XDMFStencil::XDMFStencil(const Parameters &parameters) : FieldStencil(parameters) {
    // Get some initial information about the grid
    auto cellsX = static_cast<const unsigned int>(parameters.parallel.localSize[0]);
    auto cellsY = static_cast<const unsigned int>(parameters.parallel.localSize[1]);
    const unsigned int cellsZ = (_parameters.geometry.dim == 3) ? static_cast<const unsigned int>(parameters.parallel.localSize[2]) : 1;

    const unsigned int pointsX = cellsX + 1;
    const unsigned int pointsY = cellsY + 1;
    const unsigned int pointsZ = (_parameters.geometry.dim == 3) ? cellsZ + 1 : 1;

    std::string filename = _parameters.xdmf.prefix + ".h5";
    unsigned int dataspaceAllocated = 32; // TODO

    int size;
    MPI_Comm_size(PETSC_COMM_WORLD, &size);

    writer = XdmfHDF5WriterDSM::New(filename, PETSC_COMM_WORLD, dataspaceAllocated, 0, size - 1);
    writer->setMode(XdmfHeavyDataWriter::Hyperslab);

    root = XdmfDomain::New();

    temporalGrid = XdmfGridCollection::New();
    temporalGrid->setType(XdmfGridCollectionType::Temporal());
    root->insert(temporalGrid);

    shared_ptr<XdmfArray> xCoordinates = XdmfArray::New();
    xCoordinates->initialize<FLOAT>(pointsX);
    for (int i = 2; i < cellsX + 3; i++)
        xCoordinates->pushBack(_parameters.meshsize->getPosX(i, 0, 0));

    shared_ptr<XdmfArray> yCoordinates = XdmfArray::New();
    yCoordinates->initialize<FLOAT>(pointsY);
    for (int i = 2; i < cellsY + 3; i++)
        yCoordinates->pushBack(_parameters.meshsize->getPosY(0, i, 0));

    shared_ptr<XdmfRectilinearGrid> grid;
    if (_parameters.geometry.dim == 3) {
        shared_ptr<XdmfArray> zCoordinates = XdmfArray::New();
        zCoordinates->initialize<FLOAT>(pointsZ);
        for (int i = 2; i < cellsZ + 3; i++)
            zCoordinates->pushBack(_parameters.meshsize->getPosZ(0, 0, i));

        grid = XdmfRectilinearGrid::New(xCoordinates, yCoordinates, zCoordinates);
    } else
        grid = XdmfRectilinearGrid::New(xCoordinates, yCoordinates);

    topology = grid->getTopology();
    geometry = grid->getGeometry();

    velocity->initialize<FLOAT>(cellsX * cellsY * cellsZ * 3);
    pressure->initialize<FLOAT>(cellsX * cellsY * cellsZ);
    viscosity->initialize<FLOAT>(cellsX * cellsY * cellsZ);
    wallDistance->initialize<FLOAT>(cellsX * cellsY * cellsZ);
}

XDMFStencil::~XDMFStencil() {

    if (_parameters.parallel.rank == 0)
    {
        writer->stopDSM();
    }

    MPI_Barrier(PETSC_COMM_WORLD);
}

inline void XDMFStencil::apply(FlowField &flowField, int i, int j) {
    if ((flowField.getFlags().getValue(i, j) & OBSTACLE_SELF) == 0) {
        // Get pressure and velocity
        FLOAT pressure;
        FLOAT velocity[2];
        flowField.getPressureAndVelocity(pressure, velocity, i, j);

        // TODO write using DSM!!!
        this->pressure->pushBack(pressure);
        this->velocity->pushBack(velocity[0]);
        this->velocity->pushBack(velocity[1]);

        this->viscosity->pushBack(flowField.getTurbulentViscosity().getScalar(i, j));
        this->wallDistance->pushBack(flowField.getNearestWallDistance().getScalar(i, j));
    } else {

        // TODO write using DSM!!!
        this->pressure->pushBack(0.0);
        this->velocity->pushBack(0.0);
        this->velocity->pushBack(0.0);
        this->viscosity->pushBack(0.0);
        this->wallDistance->pushBack(0.0);
    }
}

inline void XDMFStencil::apply(FlowField &flowField, int i, int j, int k) {
    if ((flowField.getFlags().getValue(i, j, k) & OBSTACLE_SELF) == 0) {
        // Get pressure and velocity
        FLOAT pressure;
        FLOAT velocity[3];
        flowField.getPressureAndVelocity(pressure, velocity, i, j, k);

        // TODO write using DSM!!!
        this->pressure->pushBack(pressure);
        this->velocity->pushBack(velocity[0]);
        this->velocity->pushBack(velocity[1]);
        this->velocity->pushBack(velocity[2]);

        this->viscosity->pushBack(flowField.getTurbulentViscosity().getScalar(i, j, k));
        this->wallDistance->pushBack(flowField.getNearestWallDistance().getScalar(i, j, k));
    } else {

        // TODO write using DSM!!!
        this->pressure->pushBack(0.0);
        this->velocity->pushBack(0.0);
        this->velocity->pushBack(0.0);
        this->velocity->pushBack(0.0);
        this->viscosity->pushBack(0.0);
        this->wallDistance->pushBack(0.0);
    }
}

void XDMFStencil::write(int timeStep) {

    shared_ptr<XdmfUnstructuredGrid> timestepGrid = XdmfUnstructuredGrid::New();
    timestepGrid->setGeometry(geometry);
    timestepGrid->setTopology(topology);

    //Add time
    shared_ptr<XdmfTime> time = XdmfTime::New(timeStep);
    timestepGrid->setTime(time);

    //Add attribute
    shared_ptr<XdmfAttribute> pressureAttr = XdmfAttribute::New();
    pressureAttr->setCenter(XdmfAttributeCenter::Cell());
    pressureAttr->setName("pressure");
    pressureAttr->insert(pressure);
    timestepGrid->insert(pressureAttr);

    shared_ptr<XdmfAttribute> velocityAttr = XdmfAttribute::New();
    velocityAttr->setCenter(XdmfAttributeCenter::Cell());
    velocityAttr->setName("velocity");
    velocityAttr->insert(velocity);
    velocityAttr->setType(XdmfAttributeType::Vector());
    velocityAttr->setElementDegree(3);
    timestepGrid->insert(velocityAttr);

    shared_ptr<XdmfAttribute> viscosityAttr = XdmfAttribute::New();
    viscosityAttr->setCenter(XdmfAttributeCenter::Cell());
    viscosityAttr->setName("viscosity");
    viscosityAttr->insert(viscosity);
    timestepGrid->insert(viscosityAttr);

    shared_ptr<XdmfAttribute> wallDistanceAttr = XdmfAttribute::New();
    wallDistanceAttr->setCenter(XdmfAttributeCenter::Cell());
    wallDistanceAttr->setName("wallDistance");
    wallDistanceAttr->insert(wallDistance);
    timestepGrid->insert(wallDistanceAttr);

    temporalGrid->insert(timestepGrid);

    //Write
    root->accept(writer);

    this->pressure->clear();
    this->velocity->clear();
    this->viscosity->clear();
    this->wallDistance->clear();
}
