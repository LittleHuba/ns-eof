#include "XDMFStencil.h"

XDMFStencil::XDMFStencil(FlowField &flowField, const Parameters &parameters) : FieldStencil(parameters) {

    // Get some initial information about the grid
    auto cellsX = static_cast<const unsigned int>(parameters.parallel.localSize[0]);
    auto cellsY = static_cast<const unsigned int>(parameters.parallel.localSize[1]);
    const unsigned int cellsZ = (_parameters.geometry.dim == 3) ? static_cast<const unsigned int>(parameters.parallel.localSize[2]) : 0;

    const unsigned int pointsX = cellsX + 1;
    const unsigned int pointsY = cellsY + 1;
    const unsigned int pointsZ = cellsZ + 1;

    unsigned long points = pointsX * pointsY * pointsZ;
    MPI_Reduce(&points, &allPoints, 1, MPI_UNSIGNED_LONG, MPI_SUM, 0, PETSC_COMM_WORLD);
    MPI_Scan(&points, &previousPoints, 1, MPI_UNSIGNED_LONG, MPI_SUM, PETSC_COMM_WORLD);
    previousPoints -= points;

    cells = cellsX * cellsY * cellsZ;
    MPI_Reduce(&cells, &allCells, 1, MPI_UNSIGNED_LONG, MPI_SUM, 0, PETSC_COMM_WORLD);
    MPI_Scan(&cells, &previousCells, 1, MPI_UNSIGNED_LONG, MPI_SUM, PETSC_COMM_WORLD);
    previousCells -= cells;

    //Create the hdf5 file.
    hid_t plist_file_id = H5Pcreate(H5P_FILE_ACCESS);
    H5Pset_fapl_mpio(plist_file_id, PETSC_COMM_WORLD, MPI_INFO_NULL);

    file_id = H5Fcreate((parameters.xdmf.prefix + ".h5").c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, plist_file_id);

    //get information needed to open datasets in collective mpi mode
    dxpl_id = H5Pcreate(H5P_DATASET_XFER);
    H5Pset_dxpl_mpio(dxpl_id, H5FD_MPIO_COLLECTIVE);


    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //write geometry
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //create dataspace and dataset
    hsize_t geo_dims[2] = {static_cast<hsize_t>(allPoints), 3};
    hid_t geo_dataspace_id = H5Screate_simple(2, geo_dims, nullptr);
    hid_t geo_dataset_id = H5Dcreate(file_id, "geometry", H5T_NATIVE_FLOAT, geo_dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    //select hyperslab of the dataspace
    hsize_t geo_start[2] = {(hsize_t) previousPoints, 0};
    hsize_t geo_count[2] = {pointsX * pointsY * pointsZ, 3};
    H5Sselect_hyperslab(geo_dataspace_id, H5S_SELECT_SET, geo_start, nullptr, geo_count, nullptr);

    //create a hyperslab selection of the vector in the memory
    hsize_t geo_memspace_dims[2] = {(hsize_t) pointsX * pointsY * pointsZ * 3, 1};
    hid_t geo_memspace = H5Screate_simple(2, geo_memspace_dims, nullptr);
    hsize_t geo_memory_start[2] = {0, 0};
    hsize_t geo_memory_count[2] = {(hsize_t) pointsX * pointsY * pointsZ * 3, 1};
    H5Sselect_hyperslab(geo_memspace, H5S_SELECT_SET, geo_memory_start, nullptr, geo_memory_count, nullptr);

    std::vector<FLOAT> geometry = std::vector<FLOAT>();
    geometry.reserve(points);
    if (parameters.geometry.dim == 2)
        for (unsigned int j = 2; j < cellsY + 2; j++) {
            for (unsigned int i = 2; i < cellsX + 2; i++) {
                geometry.push_back(_parameters.meshsize->getPosX(i, j));
                geometry.push_back(_parameters.meshsize->getPosY(i, j));
                geometry.push_back(0);
            }
        }
    else
        for (unsigned int k = 2; k < cellsZ + 3; k++) {
            for (unsigned int j = 2; j < cellsY + 3; j++) {
                for (unsigned int i = 2; i < cellsX + 3; i++) {
                    geometry.push_back(_parameters.meshsize->getPosX(i, j, k));
                    geometry.push_back(_parameters.meshsize->getPosY(i, j, k));
                    geometry.push_back(_parameters.meshsize->getPosZ(i, j, k));
                }
            }
        }

    //write vector to dataset
    //TODO we might have to take into account that we do not use float but double as datatype
    H5Dwrite(geo_dataset_id, H5T_NATIVE_FLOAT, geo_memspace, geo_dataspace_id, dxpl_id, geometry.data());

    //close dataset and dataspaces
    H5Dclose(geo_dataset_id);
    H5Sclose(geo_dataspace_id);
    H5Sclose(geo_memspace);

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //write topology
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //create dataspace and dataset
    hsize_t topo_dims[2] = {allCells, 4};
    hid_t topo_dataspace_id = H5Screate_simple(2, topo_dims, nullptr);
    hid_t topo_dataset_id = H5Dcreate(file_id, "topology", H5T_NATIVE_ULONG, topo_dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    //select hyperslab of the dataspace
    hsize_t topo_start[2] = {(hsize_t) previousCells, 0};
    hsize_t topo_count[2] = {(hsize_t) cells, 4};
    H5Sselect_hyperslab(topo_dataspace_id, H5S_SELECT_SET, topo_start, nullptr, topo_count, nullptr);

    //create a hyperslab selection of the vector in the memory
    hsize_t topo_memspace_dims[2] = {(hsize_t) cells * 4, 1};
    hid_t topo_memspace = H5Screate_simple(2, topo_memspace_dims, nullptr);
    hsize_t topo_memory_start[2] = {0, 0};
    hsize_t topo_memory_count[2] = {(hsize_t) cells * 4, 1};
    H5Sselect_hyperslab(topo_memspace, H5S_SELECT_SET, topo_memory_start, nullptr, topo_memory_count, nullptr);

    //TODO check if this algorithm is right...
    std::vector<FLOAT> topology = std::vector<FLOAT>();
    if (parameters.geometry.dim == 2) {
        topology.reserve(cells* 4);
        for (unsigned int j = 2; j < cellsY + 2; j++) {
            for (unsigned int i = 2; i < cellsX + 2; i++) {
                topology.push_back(j * i);
                topology.push_back(j * (i + 1));
                topology.push_back((j + 1) * (i + 1));
                topology.push_back((j + 1) * i);
            }
        }
    } else {
        topology.reserve(cells* 8);
        for (unsigned int k = 2; k < cellsZ + 2; k++) {
            for (unsigned int j = 2; j < cellsY + 2; j++) {
                for (unsigned int i = 2; i < cellsX + 2; i++) {
                    topology.push_back(k * j * i);
                    topology.push_back(k * j * (i + 1));
                    topology.push_back(k * (j + 1) * (i + 1));
                    topology.push_back(k * (j + 1) * i);
                    topology.push_back((k + 1) * j * i);
                    topology.push_back((k + 1) * j * (i + 1));
                    topology.push_back((k + 1) * (j + 1) * (i + 1));
                    topology.push_back((k + 1) * (j + 1) * i);
                }
            }
        }
    }

    //write vector to dataset
    H5Dwrite(topo_dataset_id, H5T_NATIVE_ULONG, topo_memspace, topo_dataspace_id, dxpl_id, topology.data());

    //close dataset and dataspaces
    H5Dclose(topo_dataset_id);
    H5Sclose(topo_dataspace_id);
    H5Sclose(topo_memspace);

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //write wallDistance
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //create dataspace and dataset
    hsize_t wd_dims[2] = {allCells, 1};
    hid_t wd_dataspace_id = H5Screate_simple(2, wd_dims, nullptr);
    hid_t wd_dataset_id = H5Dcreate(file_id, "wallDistance", H5T_NATIVE_FLOAT, wd_dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    //select hyperslab of the dataspace
    hsize_t wd_start[2] = {(hsize_t) previousCells, 0};
    hsize_t wd_count[2] = {(hsize_t) cells, 1};
    H5Sselect_hyperslab(wd_dataspace_id, H5S_SELECT_SET, wd_start, nullptr, wd_count, nullptr);

    //create a hyperslab selection of the vector in the memory
    hsize_t wd_memspace_dims[2] = {(hsize_t) cells, 1};
    hid_t wd_memspace = H5Screate_simple(2, wd_memspace_dims, nullptr);
    hsize_t wd_memory_start[2] = {0, 0};
    hsize_t wd_memory_count[2] = {(hsize_t) cells, 1};
    H5Sselect_hyperslab(wd_memspace, H5S_SELECT_SET, wd_memory_start, nullptr, wd_memory_count, nullptr);

    std::vector<FLOAT> wallDistance = std::vector<FLOAT>();
    wallDistance.reserve(cells);
    if (parameters.geometry.dim == 2)
        for (unsigned int j = 2; j < cellsY + 2; j++) {
            for (unsigned int i = 2; i < cellsX + 2; i++) {
                wallDistance.push_back(flowField.getNearestWallDistance().getScalar(i, j));
            }
        }
    else
        for (unsigned int k = 2; k < cellsZ + 2; k++) {
            for (unsigned int j = 2; j < cellsY + 2; j++) {
                for (unsigned int i = 2; i < cellsX + 2; i++) {
                    wallDistance.push_back(flowField.getNearestWallDistance().getScalar(i, j, k));
                }
            }
        }

    //write vector to dataset
    H5Dwrite(wd_dataset_id, H5T_NATIVE_ULONG, wd_memspace, wd_dataspace_id, dxpl_id, wallDistance.data());

    //close dataset and dataspaces
    H5Dclose(wd_dataset_id);
    H5Sclose(wd_dataspace_id);
    H5Sclose(wd_memspace);

    //Create XDMF File
    xdmfFile.open((_parameters.xdmf.prefix + ".xdmf").c_str());

    velocity = std::vector<FLOAT>();
    velocity.reserve(cells * 3);
    pressure = std::vector<FLOAT>();
    pressure.reserve(cells);
    viscosity = std::vector<FLOAT>();
    viscosity.reserve(cells);
}

inline void XDMFStencil::apply(FlowField &flowField, int i, int j) {
    if ((flowField.getFlags().getValue(i, j) & OBSTACLE_SELF) == 0) {
        // Get pressure and velocity
        FLOAT pressure;
        FLOAT velocity[2];
        flowField.getPressureAndVelocity(pressure, velocity, i, j);

        //write data into buffers
        this->pressure.push_back(pressure);
        this->velocity.push_back(velocity[0]);
        this->velocity.push_back(velocity[1]);
        this->velocity.push_back(0.0);
        this->viscosity.push_back(flowField.getTurbulentViscosity().getScalar(i, j));
    } else {
        //write data into buffers
        this->pressure.push_back(0.0);
        this->velocity.push_back(0.0);
        this->velocity.push_back(0.0);
        this->velocity.push_back(0.0);
        this->viscosity.push_back(0.0);
    }
}

inline void XDMFStencil::apply(FlowField &flowField, int i, int j, int k) {
    if ((flowField.getFlags().getValue(i, j, k) & OBSTACLE_SELF) == 0) {
        // Get pressure and velocity
        FLOAT pressure;
        FLOAT velocity[3];
        flowField.getPressureAndVelocity(pressure, velocity, i, j, k);

        //write data into buffers
        this->pressure.push_back(pressure);
        this->velocity.push_back(velocity[0]);
        this->velocity.push_back(velocity[1]);
        this->velocity.push_back(velocity[2]);
        this->viscosity.push_back(flowField.getTurbulentViscosity().getScalar(i, j, k));
    } else {
        //write data into buffers
        this->pressure.push_back(0.0);
        this->velocity.push_back(0.0);
        this->velocity.push_back(0.0);
        this->velocity.push_back(0.0);
        this->viscosity.push_back(0.0);
    }
}

void XDMFStencil::write(int timestep) {
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //write velocity
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //create dataspace and dataset
    hsize_t vel_dims[2] = {allCells, 3};
    hid_t vel_dataspace_id = H5Screate_simple(2, vel_dims, nullptr);
    hid_t vel_dataset_id = H5Dcreate(file_id, ("velocity" + std::to_string(timestep)).c_str(), H5T_NATIVE_FLOAT, vel_dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    //select hyperslab of the dataspace
    hsize_t vel_start[2] = {(hsize_t) previousCells, 0};
    hsize_t vel_count[2] = {(hsize_t) cells, 3};
    H5Sselect_hyperslab(vel_dataspace_id, H5S_SELECT_SET, vel_start, nullptr, vel_count, nullptr);

    //create a hyperslab selection of the vector in the memory
    hsize_t vel_memspace_dims[2] = {(hsize_t) cells, 1};
    hid_t vel_memspace = H5Screate_simple(2, vel_memspace_dims, nullptr);
    hsize_t vel_memory_start[2] = {0, 0};
    hsize_t vel_memory_count[2] = {(hsize_t) cells, 1};
    H5Sselect_hyperslab(vel_memspace, H5S_SELECT_SET, vel_memory_start, nullptr, vel_memory_count, nullptr);

    //write vector to dataset
    H5Dwrite(vel_dataset_id, H5T_NATIVE_ULONG, vel_memspace, vel_dataspace_id, dxpl_id, velocity.data());

    //close dataset and dataspaces
    H5Dclose(vel_dataset_id);
    H5Sclose(vel_dataspace_id);
    H5Sclose(vel_memspace);

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //write pressure
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //create dataspace and dataset
    hsize_t p_dims[2] = {allCells, 1};
    hid_t p_dataspace_id = H5Screate_simple(2, p_dims, nullptr);
    hid_t p_dataset_id = H5Dcreate(file_id, ("pressure" + std::to_string(timestep)).c_str(), H5T_NATIVE_FLOAT, p_dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    //select hyperslab of the dataspace
    hsize_t p_start[2] = {(hsize_t) previousCells, 0};
    hsize_t p_count[2] = {(hsize_t) cells, 1};
    H5Sselect_hyperslab(p_dataspace_id, H5S_SELECT_SET, p_start, nullptr, p_count, nullptr);

    //create a hyperslab selection of the vector in the memory
    hsize_t p_memspace_dims[2] = {(hsize_t) cells, 1};
    hid_t p_memspace = H5Screate_simple(2, p_memspace_dims, nullptr);
    hsize_t p_memory_start[2] = {0, 0};
    hsize_t p_memory_count[2] = {(hsize_t) cells, 1};
    H5Sselect_hyperslab(p_memspace, H5S_SELECT_SET, p_memory_start, nullptr, p_memory_count, nullptr);

    //write vector to dataset
    H5Dwrite(p_dataset_id, H5T_NATIVE_ULONG, p_memspace, p_dataspace_id, dxpl_id, pressure.data());

    //close dataset and dataspaces
    H5Dclose(p_dataset_id);
    H5Sclose(p_dataspace_id);
    H5Sclose(p_memspace);

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //write viscosity
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //create dataspace and dataset
    hsize_t vc_dims[2] = {allCells, 1};
    hid_t vc_dataspace_id = H5Screate_simple(2, vc_dims, nullptr);
    hid_t vc_dataset_id = H5Dcreate(file_id, ("viscosity" + std::to_string(timestep)).c_str(), H5T_NATIVE_FLOAT, vc_dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    //select hyperslab of the dataspace
    hsize_t vc_start[2] = {(hsize_t) previousCells, 0};
    hsize_t vc_count[2] = {(hsize_t) cells, 1};
    H5Sselect_hyperslab(vc_dataspace_id, H5S_SELECT_SET, vc_start, nullptr, vc_count, nullptr);

    //create a hyperslab selection of the vector in the memory
    hsize_t vc_memspace_dims[2] = {(hsize_t) cells, 1};
    hid_t vc_memspace = H5Screate_simple(2, vc_memspace_dims, nullptr);
    hsize_t vc_memory_start[2] = {0, 0};
    hsize_t vc_memory_count[2] = {(hsize_t) cells, 1};
    H5Sselect_hyperslab(vc_memspace, H5S_SELECT_SET, vc_memory_start, nullptr, vc_memory_count, nullptr);

    //write vector to dataset
    H5Dwrite(vc_dataset_id, H5T_NATIVE_ULONG, vc_memspace, vc_dataspace_id, dxpl_id, viscosity.data());

    //close dataset and dataspaces
    H5Dclose(vc_dataset_id);
    H5Sclose(vc_dataspace_id);
    H5Sclose(vc_memspace);

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Add timestep to XDMF-File
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if (_parameters.parallel.rank != 0)
        return;

    if (timestep == 0) {
        xdmfFile << "<?xml version=\"1.0\" ?>" << std::endl
                 << "<!DOCTYPE Xdmf SYSTEM \"Xdmf.dtd\" []>" << std::endl
                 << "<Xdmf Version=\"2.0\">" << std::endl
                 << "    <Domain>" << std::endl
                 //TODO add temporal grid
                 << "        <Grid>" << std::endl
                 //TODO add time
                 << "            <Topology TopologyType=\"topology\" NumberOfElements=\"" << allCells << "\">" << std::endl
                 << "                <DataItem NumberType=\"UInt\" Precision=\"16\" Format=\"HDF\" Dimensions=\"" << allCells << " 4\">" << _parameters.xdmf.prefix
                 << ".h5:/topology</DataItem>" << std::endl
                 << "            </Topology>" << std::endl
                 << "            <Geometry GeometryType=\"XYZ\" NumberOfElements=\"" << allPoints << "\">" << std::endl
                 << "                <DataItem NumberType=\"Float\" Precision=\"8\" Format=\"HDF\" Dimensions=\"" << allPoints << " 3\">" << _parameters.xdmf.prefix
                 << ".h5:/geometry</DataItem>" << std::endl
                 << "            </Geometry>" << std::endl
                 << "            <Attribute Name=\"velocity\" Center=\"Cell\">" << std::endl
                 << "                <DataItem NumberType=\"Float\" Precision=\"8\" Format=\"HDF\" Dimensions=\"" << allCells
                 << " 3\">" + _parameters.xdmf.prefix + ":/velocity0</DataItem>" << std::endl
                 << "            </Attribute>" << std::endl
                 << "            <Attribute Name=\"pressure\" Center=\"Cell\">" << std::endl
                 << "                <DataItem NumberType=\"Float\" Precision=\"8\" Format=\"HDF\" Dimensions=\"" << allCells
                 << " 1\">" + _parameters.xdmf.prefix + ":/pressure0</DataItem>" << std::endl
                 << "            </Attribute>" << std::endl
                 << "            <Attribute Name=\"wallDistance\" Center=\"Cell\">" << std::endl
                 << "                <DataItem NumberType=\"Float\" Precision=\"8\" Format=\"HDF\" Dimensions=\"" << allCells
                 << " 1\">" + _parameters.xdmf.prefix + ":/wallDistance</DataItem>" << std::endl
                 << "            </Attribute>" << std::endl
                 << "            <Attribute Name=\"viscosity\" Center=\"Cell\">" << std::endl
                 << "                <DataItem NumberType=\"Float\" Precision=\"8\" Format=\"HDF\" Dimensions=\"" << allCells
                 << " 1\">" + _parameters.xdmf.prefix + ":/viscosity0</DataItem>" << std::endl
                 << "            </Attribute>" << std::endl
                 << "        </Grid>" << std::endl
                 // TODO add temporal grid closing tag
                 << "    </Domain>" << std::endl
                 << "</Xdmf>";
    } else {
        //write new timestep
        xdmfFile << "        <Grid>" << std::endl
                 //TODO add time
                 << "            <Topology TopologyType=\"topology\" NumberOfElements=\"" << allCells << "\">" << std::endl
                 << "                <DataItem NumberType=\"UInt\" Precision=\"16\" Format=\"HDF\" Dimensions=\"" << allCells << " 4\">" << _parameters.xdmf.prefix
                 << ".h5:/topology</DataItem>" << std::endl
                 << "            </Topology>" << std::endl
                 << "            <Geometry GeometryType=\"XYZ\" NumberOfElements=\"" << allPoints << "\">" << std::endl
                 << "                <DataItem NumberType=\"Float\" Precision=\"8\" Format=\"HDF\" Dimensions=\"" << allPoints << " 3\">" << _parameters.xdmf.prefix
                 << ".h5:/geometry</DataItem>" << std::endl
                 << "            </Geometry>" << std::endl
                 << "            <Attribute Name=\"velocity\" Center=\"Cell\">" << std::endl
                 << "                <DataItem NumberType=\"Float\" Precision=\"8\" Format=\"HDF\" Dimensions=\"" << allCells
                 << " 3\">" + _parameters.xdmf.prefix + ":/velocity" << timestep << "</DataItem>" << std::endl
                 << "            </Attribute>" << std::endl
                 << "            <Attribute Name=\"pressure\" Center=\"Cell\">" << std::endl
                 << "                <DataItem NumberType=\"Float\" Precision=\"8\" Format=\"HDF\" Dimensions=\"" << allCells
                 << " 1\">" + _parameters.xdmf.prefix + ":/pressure" << timestep << "</DataItem>" << std::endl
                 << "            </Attribute>" << std::endl
                 << "            <Attribute Name=\"wallDistance\" Center=\"Cell\">" << std::endl
                 << "                <DataItem NumberType=\"Float\" Precision=\"8\" Format=\"HDF\" Dimensions=\"" << allCells
                 << " 1\">" + _parameters.xdmf.prefix + ":/wallDistance</DataItem>" << std::endl
                 << "            </Attribute>" << std::endl
                 << "            <Attribute Name=\"viscosity\" Center=\"Cell\">" << std::endl
                 << "                <DataItem NumberType=\"Float\" Precision=\"8\" Format=\"HDF\" Dimensions=\"" << allCells
                 << " 1\">" + _parameters.xdmf.prefix + ":/viscosity" << timestep << "</DataItem>" << std::endl
                 << "            </Attribute>" << std::endl
                 << "        </Grid>" << std::endl
                 // TODO add temporal grid closing tag
                 << "    </Domain>" << std::endl
                 << "</Xdmf>";
    }

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Reset for next timestep
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Set cursor position so new timestep can be inserted
    //TODO Update offset to include temporal grid
    xdmfFile.seekp(-21, std::ios_base::cur);

    //reinit buffers for attributes
    velocity.clear();
    pressure.clear();
    viscosity.clear();
}
