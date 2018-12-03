//
// Created by ulrich on 01.12.18.
//

#ifndef NS_TESTINGFUNCTIONS_H
#define NS_TESTINGFUNCTIONS_H

#include <fstream>
#include <sstream>
#include "../src/Parameters.h"

class DummyParameters {
    Parameters *parameters;

public:
    explicit DummyParameters(int dimensions) {
        parameters = new Parameters();
        parameters->parallel.rank = 0;
        parameters->parallel.localSize[0] = 2;
        parameters->parallel.localSize[1] = 2;
        if (dimensions == 3)
            parameters->parallel.localSize[2] = 2;
        else
            parameters->parallel.localSize[2] = 0;

        parameters->parallel.firstCorner[0] = 0;
        parameters->parallel.firstCorner[1] = 0;
        parameters->parallel.firstCorner[2] = 0;

        parameters->geometry.dim = dimensions;

        parameters->geometry.lengthX = 1;
        parameters->geometry.lengthY = 1;

        if (dimensions == 3)
            parameters->geometry.lengthZ = 1;
        else
            parameters->geometry.lengthZ = 0;

        parameters->geometry.sizeX = 2;
        parameters->geometry.sizeY = 2;

        if (dimensions == 3)
            parameters->geometry.sizeZ = 2;
        else
            parameters->geometry.sizeZ = 1;

        parameters->meshsize = new UniformMeshsize(*parameters);

        parameters->vtk.prefix = "testOut";
    }

    Parameters &get() {
        return *parameters;
    }

    ~DummyParameters() {
        delete parameters;
    }
};

class testing_ofstream : public std::basic_ofstream<char> {
private:
    std::stringstream testingStream;
public:
    testing_ofstream() : std::ofstream() {
        std::ios::rdbuf(testingStream.rdbuf());
    }

    std::string str() {
        return testingStream.str();
    }
};

#endif //NS_TESTINGFUNCTIONS_H
