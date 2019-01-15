#include "XDMFStencil.h"

XDMFStencil::XDMFStencil(FlowField &flowField, const Parameters &parameters) : FieldStencil(parameters) {
    
    // Get some initial information about the grid
    cellsX = static_cast<const unsigned int>(parameters.parallel.localSize[0]);
    cellsY = static_cast<const unsigned int>(parameters.parallel.localSize[1]);
    cellsZ = (_parameters.geometry.dim == 3) ? static_cast<const unsigned int>(parameters.parallel.localSize[2]) : 0;
    
    firstCornerX = _parameters.parallel.firstCorner[0];
    firstCornerY = _parameters.parallel.firstCorner[1];
    firstCornerZ = _parameters.parallel.firstCorner[2];
    isLeft = firstCornerX == 0;
    isBottom = firstCornerY == 0;
    isFront = firstCornerZ == 0;

    
    unsigned int pointsX;
    unsigned int pointsY;
    unsigned int pointsZ;
    unsigned int iStart;
    unsigned int jStart;
    unsigned int kStart;

    if (isLeft)
    {
        pointsX = cellsX + 1;
        iStart = 2;
    }
    else
    {
        pointsX = cellsX;
        iStart = 3;
    }
    
    if (isBottom)
    {
        pointsY = cellsY + 1;
        jStart = 2;
    }
    else
    {
        pointsY = cellsY;
        jStart = 3;
    }
    
    if (isFront)
    {
        pointsZ = cellsZ + 1;
        kStart = 2;
    }
    else
    {
        pointsZ = cellsZ;
        kStart = 3;
    }

    writeCellsZ = cellsZ;
    if(_parameters.geometry.dim == 2){
        writeCellsZ = 1;
    }
    
    allPointsX = _parameters.geometry.sizeX + 1;
    allPointsY = _parameters.geometry.sizeY + 1;
    allCellsX = _parameters.geometry.sizeX;
    allCellsY = _parameters.geometry.sizeY;
    if(_parameters.geometry.dim == 2){
        allPointsZ = 1;
        allCellsZ = 1;
    }
    else {
        allPointsZ = _parameters.geometry.sizeZ + 1;
        allCellsZ = _parameters.geometry.sizeZ;
    }
    
    unsigned long points = pointsX * pointsY * pointsZ;
    MPI_Allreduce(&points, &allPoints, 1, MPI_UNSIGNED_LONG, MPI_SUM, PETSC_COMM_WORLD);
    MPI_Scan(&points, &previousPoints, 1, MPI_UNSIGNED_LONG, MPI_SUM, PETSC_COMM_WORLD);
    previousPoints -= points;

    if(_parameters.geometry.dim == 3){
        cells = cellsX * cellsY * cellsZ;
    }
    else{
        cells = cellsX * cellsY;
    }

    MPI_Allreduce(&cells, &allCells, 1, MPI_UNSIGNED_LONG, MPI_SUM, PETSC_COMM_WORLD);
    MPI_Scan(&cells, &previousCells, 1, MPI_UNSIGNED_LONG, MPI_SUM, PETSC_COMM_WORLD);
    previousCells -= cells;

    //Create the hdf5 file.
    hid_t plist_file_id = H5Pcreate(H5P_FILE_ACCESS);
    H5Pset_fapl_mpio(plist_file_id, PETSC_COMM_WORLD, MPI_INFO_NULL);

    file_id = H5Fcreate((parameters.xdmf.prefix + ".h5").c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, plist_file_id);
    H5Pclose(plist_file_id); // Release list identifier

    //get information needed to open datasets in collective mpi mode
    dxpl_id = H5Pcreate(H5P_DATASET_XFER);
    H5Pset_dxpl_mpio(dxpl_id, H5FD_MPIO_COLLECTIVE);
    int rank = _parameters.parallel.rank;
    
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //write geometry X
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    std::cout << "[Rank=" << rank << "] "
              << "allPointsX=" << allPointsX
              << ", firstCornerX=" << firstCornerX+(int)(!isLeft)
              << ", pointsX=" << pointsX
              << std::endl;
    
    //create dataspace and dataset
    hsize_t geoX_dims[2] = {static_cast<hsize_t>(allPointsX), 1};
    hid_t geoX_dataspace_id = H5Screate_simple(2, geoX_dims, nullptr);
//    sizeof(FLOAT) == sizeof(double)
    hid_t geoX_dataset_id = H5Dcreate(file_id, "geometryX", H5T_NATIVE_DOUBLE, geoX_dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    //select hyperslab of the dataspace
    hsize_t geoX_start[2] = {(hsize_t) (firstCornerX+(int)(!isLeft)), 0}; // The "+1" here is because the leftmost block covers 1 extra point.
    hsize_t geoX_count[2] = {pointsX, 1};
    H5Sselect_hyperslab(geoX_dataspace_id, H5S_SELECT_SET, geoX_start, nullptr, geoX_count, nullptr);

    //create a hyperslab selection of the vector in the memory
    hsize_t geoX_memspace_dims[2] = {(hsize_t) pointsX, 1};
    hid_t geoX_memspace = H5Screate_simple(2, geoX_memspace_dims, nullptr);
    hsize_t geoX_memory_start[2] = {0, 0};
    hsize_t geoX_memory_count[2] = {(hsize_t) pointsX, 1};
    H5Sselect_hyperslab(geoX_memspace, H5S_SELECT_SET, geoX_memory_start, nullptr, geoX_memory_count, nullptr);

    std::vector<FLOAT> geometryX = std::vector<FLOAT>();
    geometryX.reserve(pointsX);
    for (unsigned int i = iStart; i < cellsX + 3; ++i)
    {
        geometryX.push_back(_parameters.meshsize->getPosX(i, 0));
    }
    
    //write vector to dataset
    //TODO we might have to take into account that we do not use float but double as datatype
    H5Dwrite(geoX_dataset_id, H5T_NATIVE_DOUBLE, geoX_memspace, geoX_dataspace_id, dxpl_id, geometryX.data());
    
    //close dataset and dataspaces
    H5Dclose(geoX_dataset_id);
    H5Sclose(geoX_dataspace_id);
    H5Sclose(geoX_memspace);
    
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //write geometry Y
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    std::cout << "[Rank=" << rank << "] "
              << "allPointsY=" << allPointsY
              << ", firstCornerY=" << firstCornerY+(int)(!isBottom)
              << ", pointsY=" << pointsY
              << std::endl;
    
    //create dataspace and dataset
    hsize_t geoY_dims[2] = {static_cast<hsize_t>(allPointsY), 1};
    hid_t geoY_dataspace_id = H5Screate_simple(2, geoY_dims, nullptr);
//    sizeof(FLOAT) == sizeof(double)
    hid_t geoY_dataset_id = H5Dcreate(file_id, "geometryY", H5T_NATIVE_DOUBLE, geoY_dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    
    //select hyperslab of the dataspace
    hsize_t geoY_start[2] = {(hsize_t) (firstCornerY+(int)(!isBottom)), 0}; // The "+1" here is because the leftmost block covers 1 extra point.
    hsize_t geoY_count[2] = {pointsY, 1};
    H5Sselect_hyperslab(geoY_dataspace_id, H5S_SELECT_SET, geoY_start, nullptr, geoY_count, nullptr);
    
    //create a hyperslab selection of the vector in the memory
    hsize_t geoY_memspace_dims[2] = {(hsize_t) pointsY, 1};
    hid_t geoY_memspace = H5Screate_simple(2, geoY_memspace_dims, nullptr);
    hsize_t geoY_memory_start[2] = {0, 0};
    hsize_t geoY_memory_count[2] = {(hsize_t) pointsY, 1};
    H5Sselect_hyperslab(geoY_memspace, H5S_SELECT_SET, geoY_memory_start, nullptr, geoY_memory_count, nullptr);
    
    std::vector<FLOAT> geometryY = std::vector<FLOAT>();
    geometryY.reserve(pointsY);
    for (unsigned int j = jStart; j < cellsY + 3; ++j)
    {
        geometryY.push_back(_parameters.meshsize->getPosY(0, j));
    }
    
    //write vector to dataset
    //TODO we might have to take into account that we do not use float but double as datatype
    H5Dwrite(geoY_dataset_id, H5T_NATIVE_DOUBLE, geoY_memspace, geoY_dataspace_id, dxpl_id, geometryY.data());
    
    //close dataset and dataspaces
    H5Dclose(geoY_dataset_id);
    H5Sclose(geoY_dataspace_id);
    H5Sclose(geoY_memspace);
    
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //write geometry Z
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if (parameters.geometry.dim == 3)
    {
        std::cout << "[Rank=" << rank << "] "
                  << "allPointsZ=" << allPointsZ
                  << ", firstCornerZ=" << firstCornerZ+(int)(!isFront)
                  << ", pointsZ=" << pointsZ
                  << std::endl;
    
        //create dataspace and dataset
        hsize_t geoZ_dims[2] = {static_cast<hsize_t>(allPointsZ), 1};
        hid_t geoZ_dataspace_id = H5Screate_simple(2, geoZ_dims, nullptr);
//    sizeof(FLOAT) == sizeof(double)
        hid_t geoZ_dataset_id = H5Dcreate(file_id, "geometryZ", H5T_NATIVE_DOUBLE, geoZ_dataspace_id, H5P_DEFAULT,
                                          H5P_DEFAULT, H5P_DEFAULT);
    
        //select hyperslab of the dataspace
        hsize_t geoZ_start[2] = {(hsize_t) (firstCornerZ+(int)(!isLeft)),
                                 0}; // The "+1" here is because the leftmost block covers 1 extra point.
        hsize_t geoZ_count[2] = {pointsZ, 1};
        H5Sselect_hyperslab(geoZ_dataspace_id, H5S_SELECT_SET, geoZ_start, nullptr, geoZ_count, nullptr);
    
        //create a hyperslab selection of the vector in the memory
        hsize_t geoZ_memspace_dims[2] = {(hsize_t) pointsZ, 1};
        hid_t geoZ_memspace = H5Screate_simple(2, geoZ_memspace_dims, nullptr);
        hsize_t geoZ_memory_start[2] = {0, 0};
        hsize_t geoZ_memory_count[2] = {(hsize_t) pointsZ, 1};
        H5Sselect_hyperslab(geoZ_memspace, H5S_SELECT_SET, geoZ_memory_start, nullptr, geoZ_memory_count, nullptr);
    
        std::vector<FLOAT> geometryZ = std::vector<FLOAT>();
        geometryZ.reserve(pointsZ);
        for (unsigned int k = kStart; k < cellsZ + 3; ++k)
        {
            geometryZ.push_back(_parameters.meshsize->getPosZ(0, 0, k));
        }
    
        //write vector to dataset
        //TODO we might have to take into account that we do not use float but double as datatype
        H5Dwrite(geoZ_dataset_id, H5T_NATIVE_DOUBLE, geoZ_memspace, geoZ_dataspace_id, dxpl_id, geometryZ.data());
    
        //close dataset and dataspaces
        H5Dclose(geoZ_dataset_id);
        H5Sclose(geoZ_dataspace_id);
        H5Sclose(geoZ_memspace);
    }
    
    

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //write wallDistance
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


    //create dataspace and dataset
    hsize_t wd_dims[3] = {static_cast<hsize_t>(allCellsZ), static_cast<hsize_t>(allCellsY), static_cast<hsize_t>(allCellsX)};
    hid_t wd_dataspace_id = H5Screate_simple(3, wd_dims, nullptr);
    hid_t wd_dataset_id = H5Dcreate(file_id, "wallDistance", H5T_NATIVE_DOUBLE, wd_dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    //select hyperslab of the dataspace
    hsize_t wd_start[3] = {(hsize_t) firstCornerZ, (hsize_t) firstCornerY, (hsize_t)firstCornerX};
    hsize_t wd_count[3] = {(hsize_t) writeCellsZ, (hsize_t) cellsY, (hsize_t)cellsX};

    H5Sselect_hyperslab(wd_dataspace_id, H5S_SELECT_SET, wd_start, nullptr, wd_count, nullptr);

    //create a hyperslab selection of the vector in the memory
    hsize_t wd_memspace_dims[1] = {(hsize_t) cells};
    hid_t wd_memspace = H5Screate_simple(1, wd_memspace_dims, nullptr);
    hsize_t wd_memory_start[1] = {0};
    hsize_t wd_memory_count[1] = {(hsize_t) cells};
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
    H5Dwrite(wd_dataset_id, H5T_NATIVE_DOUBLE, wd_memspace, wd_dataspace_id, dxpl_id, wallDistance.data());

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
    hsize_t vel_dims[4] = {static_cast<hsize_t>(allCellsZ), static_cast<hsize_t>(allCellsY), static_cast<hsize_t>(allCellsX), 3};
    hid_t vel_dataspace_id = H5Screate_simple(4, vel_dims, nullptr);
    hid_t vel_dataset_id = H5Dcreate(file_id, ("velocity" + std::to_string(timestep)).c_str(), H5T_NATIVE_DOUBLE, vel_dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    //select hyperslab of the dataspace
    hsize_t vel_start[4] = {(hsize_t) firstCornerZ, (hsize_t) firstCornerY, (hsize_t)firstCornerX, 0};
    hsize_t vel_count[4] = {(hsize_t) writeCellsZ, (hsize_t) cellsY, (hsize_t)cellsX, 3};

    H5Sselect_hyperslab(vel_dataspace_id, H5S_SELECT_SET, vel_start, nullptr, vel_count, nullptr);

    //create a hyperslab selection of the vector in the memory
    hsize_t vel_memspace_dims[2] = {(hsize_t) cells, 3};
    hid_t vel_memspace = H5Screate_simple(2, vel_memspace_dims, nullptr);
    hsize_t vel_memory_start[2] = {0, 0};
    hsize_t vel_memory_count[2] = {(hsize_t) cells, 3};
    H5Sselect_hyperslab(vel_memspace, H5S_SELECT_SET, vel_memory_start, nullptr, vel_memory_count, nullptr);

    //write vector to dataset
    H5Dwrite(vel_dataset_id, H5T_NATIVE_DOUBLE, vel_memspace, vel_dataspace_id, dxpl_id, this->velocity.data());

    //close dataset and dataspaces
    H5Dclose(vel_dataset_id);
    H5Sclose(vel_dataspace_id);
    H5Sclose(vel_memspace);

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //write pressure
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //create dataspace and dataset

    hsize_t p_dims[3] = {static_cast<hsize_t>(allCellsZ), static_cast<hsize_t>(allCellsY), static_cast<hsize_t>(allCellsX)};
    hid_t p_dataspace_id = H5Screate_simple(3, p_dims, nullptr);
    hid_t p_dataset_id = H5Dcreate(file_id, ("pressure" + std::to_string(timestep)).c_str(), H5T_NATIVE_DOUBLE, p_dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    //select hyperslab of the dataspace
    hsize_t p_start[3] = {(hsize_t) firstCornerZ, (hsize_t) firstCornerY, (hsize_t)firstCornerX};
    hsize_t p_count[3] = {(hsize_t) writeCellsZ, (hsize_t) cellsY, (hsize_t)cellsX};

    H5Sselect_hyperslab(p_dataspace_id, H5S_SELECT_SET, p_start, nullptr, p_count, nullptr);

    //create a hyperslab selection of the vector in the memory
    hsize_t p_memspace_dims[1] = {(hsize_t) cells};
    hid_t p_memspace = H5Screate_simple(1, p_memspace_dims, nullptr);
    hsize_t p_memory_start[1] = {0};
    hsize_t p_memory_count[1] = {(hsize_t) cells};
    H5Sselect_hyperslab(p_memspace, H5S_SELECT_SET, p_memory_start, nullptr, p_memory_count, nullptr);
    //write vector to dataset
    H5Dwrite(p_dataset_id, H5T_NATIVE_DOUBLE, p_memspace, p_dataspace_id, dxpl_id, this->pressure.data());

    //close dataset and dataspaces
    H5Dclose(p_dataset_id);
    H5Sclose(p_dataspace_id);
    H5Sclose(p_memspace);

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //write viscosity
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //create dataspace and dataset
    hsize_t vc_dims[3] = {static_cast<hsize_t>(allCellsZ), static_cast<hsize_t>(allCellsY), static_cast<hsize_t>(allCellsX)};
    hid_t vc_dataspace_id = H5Screate_simple(3, vc_dims, nullptr);
    hid_t vc_dataset_id = H5Dcreate(file_id, ("viscosity" + std::to_string(timestep)).c_str(), H5T_NATIVE_DOUBLE, vc_dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    //select hyperslab of the dataspace
    hsize_t vc_start[3] = {(hsize_t) firstCornerZ, (hsize_t) firstCornerY, (hsize_t)firstCornerX};
    hsize_t vc_count[3] = {(hsize_t) writeCellsZ, (hsize_t) cellsY, (hsize_t)cellsX};
    H5Sselect_hyperslab(vc_dataspace_id, H5S_SELECT_SET, vc_start, nullptr, vc_count, nullptr);

    //create a hyperslab selection of the vector in the memory
    hsize_t vc_memspace_dims[1] = {(hsize_t) cells};
    hid_t vc_memspace = H5Screate_simple(1, vc_memspace_dims, nullptr);
    hsize_t vc_memory_start[1] = {0};
    hsize_t vc_memory_count[1] = {(hsize_t) cells};
    H5Sselect_hyperslab(vc_memspace, H5S_SELECT_SET, vc_memory_start, nullptr, vc_memory_count, nullptr);

    //write vector to dataset
    H5Dwrite(vc_dataset_id, H5T_NATIVE_DOUBLE, vc_memspace, vc_dataspace_id, dxpl_id, viscosity.data());

    //close dataset and dataspaces
    H5Dclose(vc_dataset_id);
    H5Sclose(vc_dataspace_id);
    H5Sclose(vc_memspace);

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Add timestep to XDMF-File
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if (_parameters.parallel.rank != 0)
    {
        velocity.clear();
        pressure.clear();
        viscosity.clear();
        return;
    }
    
    std::string topologyType, numberOfElementsString;
    std::string geometryZString;
    if (_parameters.geometry.dim == 2)
    {
        topologyType = "2DRectMesh";
        numberOfElementsString = std::to_string(allPointsX) + " " + std::to_string(allPointsY);
        geometryZString = "";
    }
    else
    {
        topologyType = "3DRectMesh";
        numberOfElementsString = std::to_string(allPointsX) + " " + std::to_string(allPointsY) + " " + std::to_string(allPointsZ);
        geometryZString = "<DataItem NumberType=\"Float\" Precision=\"8\" Format=\"HDF\" Dimensions=\"" 
                + std::to_string(allPointsZ) + "\">"
                + _parameters.xdmf.prefix + ".h5:/geometryZ</DataItem>";
    }
    
    if (timestep == 0) {
        xdmfFile << "<?xml version=\"1.0\" ?>" << std::endl
                 << "<!DOCTYPE Xdmf SYSTEM \"Xdmf.dtd\" []>" << std::endl
                 << "<Xdmf Version=\"2.0\">" << std::endl
                 << "    <Domain>" << std::endl
                 << "    <Grid CollectionType=\"Temporal\" GridType=\"Collection\" Name=\"FE time series\">" << std::endl
                 << "        <Grid>" << std::endl
                 // Time
                 << "        <Time Value=\"" << timestep << "\"/>" << std::endl
                 // Data
                 << "            <Topology TopologyType=\"" << topologyType << "\" NumberOfElements=\""
                                    << numberOfElementsString << "\"/>" << std::endl
//                 << "            </Topology>" << std::endl
                 << "            <Geometry GeometryType=\"VXVYVZ\">" << std::endl
                 << "                <DataItem NumberType=\"Float\" Precision=\"8\" Format=\"HDF\" Dimensions=\"" << allPointsX << "\">" << _parameters.xdmf.prefix
                 << ".h5:/geometryX</DataItem>" << std::endl
                 << "                <DataItem NumberType=\"Float\" Precision=\"8\" Format=\"HDF\" Dimensions=\"" << allPointsY << "\">" << _parameters.xdmf.prefix
                 << ".h5:/geometryY</DataItem>" << std::endl
                 << "                " << geometryZString << std::endl
                 << "            </Geometry>" << std::endl
                 
                 << "            <Attribute Name=\"velocity\" Type=\"Vector\" Center=\"Cell\">" << std::endl
                 << "                <DataItem NumberType=\"Float\" Precision=\"8\" Format=\"HDF\" Dimensions=\"" << allCells
                 << " 3\">" + _parameters.xdmf.prefix + ".h5:/velocity0</DataItem>" << std::endl
                 << "            </Attribute>" << std::endl
                 << "            <Attribute Name=\"pressure\" Type=\"Scalar\" Center=\"Cell\">" << std::endl
                 << "                <DataItem NumberType=\"Float\" Precision=\"8\" Format=\"HDF\" Dimensions=\"" << allCells
                 << "\">" + _parameters.xdmf.prefix + ".h5:/pressure0</DataItem>" << std::endl
                 << "            </Attribute>" << std::endl
                 << "            <Attribute Name=\"wallDistance\" Type=\"Scalar\" Center=\"Cell\">" << std::endl
                 << "                <DataItem NumberType=\"Float\" Precision=\"8\" Format=\"HDF\" Dimensions=\"" << allCells
                 << "\">" + _parameters.xdmf.prefix + ".h5:/wallDistance</DataItem>" << std::endl
                 << "            </Attribute>" << std::endl
                 << "            <Attribute Name=\"viscosity\" Type=\"Scalar\" Center=\"Cell\">" << std::endl
                 << "                <DataItem NumberType=\"Float\" Precision=\"8\" Format=\"HDF\" Dimensions=\"" << allCells
                 << "\">" + _parameters.xdmf.prefix + ".h5:/viscosity0</DataItem>" << std::endl
                 << "            </Attribute>" << std::endl
                 
                 << "        </Grid>" << std::endl
                 << "    </Grid>" << std::endl
                 << "    </Domain>" << std::endl
                 << "</Xdmf>";
    } else {
        //write new timestep
        xdmfFile << "        <Grid>" << std::endl
                << "        <Time Value=\"" << timestep << "\"/>" << std::endl
                << "            <Topology TopologyType=\"" << topologyType << "\" NumberOfElements=\""
                                << numberOfElementsString << "\"/>" << std::endl
//                 << "            </Topology>" << std::endl
                << "            <Geometry GeometryType=\"VXVYVZ\">" << std::endl
                << "                <DataItem NumberType=\"Float\" Precision=\"8\" Format=\"HDF\" Dimensions=\"" << allPointsX << "\">" << _parameters.xdmf.prefix
                << ".h5:/geometryX</DataItem>" << std::endl
                << "                <DataItem NumberType=\"Float\" Precision=\"8\" Format=\"HDF\" Dimensions=\"" << allPointsY << "\">" << _parameters.xdmf.prefix
                << ".h5:/geometryY</DataItem>" << std::endl
                << "                " << geometryZString << std::endl
                << "            </Geometry>" << std::endl
        
                << "            <Attribute Name=\"velocity\" Type=\"Vector\" Center=\"Cell\">" << std::endl
                << "                <DataItem NumberType=\"Float\" Precision=\"8\" Format=\"HDF\" Dimensions=\"" << allCells
                << " 3\">" + _parameters.xdmf.prefix + ".h5:/velocity" << timestep << "</DataItem>" << std::endl
                << "            </Attribute>" << std::endl
                << "            <Attribute Name=\"pressure\" Type=\"Scalar\" Center=\"Cell\">" << std::endl
                << "                <DataItem NumberType=\"Float\" Precision=\"8\" Format=\"HDF\" Dimensions=\"" << allCells
                << "\">" + _parameters.xdmf.prefix + ".h5:/pressure" << timestep << "</DataItem>" << std::endl
                << "            </Attribute>" << std::endl
                << "            <Attribute Name=\"viscosity\" Type=\"Scalar\" Center=\"Cell\">" << std::endl
                << "                <DataItem NumberType=\"Float\" Precision=\"8\" Format=\"HDF\" Dimensions=\"" << allCells
                << "\">" + _parameters.xdmf.prefix + ".h5:/viscosity" << timestep << "</DataItem>" << std::endl
                << "            </Attribute>" << std::endl
                << "            <Attribute Name=\"wallDistance\" Type=\"Scalar\" Center=\"Cell\">" << std::endl
                << "                <DataItem NumberType=\"Float\" Precision=\"8\" Format=\"HDF\" Dimensions=\"" << allCells
                << "\">" + _parameters.xdmf.prefix + ".h5:/wallDistance</DataItem>" << std::endl
                << "            </Attribute>" << std::endl
                
                << "        </Grid>" << std::endl
                 << "    </Grid>" << std::endl
                 << "    </Domain>" << std::endl
                 << "</Xdmf>";
    }

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Reset for next timestep
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Set cursor position so new timestep can be inserted
    //TODO Update offset to include temporal grid
    xdmfFile.seekp(-32, std::ios_base::cur); // This should actually be a good value, so that we write right after the "</Grid>" closing tag.

    //reinit buffers for attributes
    velocity.clear();
    pressure.clear();
    viscosity.clear();
}
