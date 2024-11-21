#pragma once
#include <string> 
#include "ModifiedTriangulation.h"
#include "Reader.h"

using namespace Geometry;
using namespace MeshOperations;

class STLReader : public Reader
{
public:
    STLReader();
    ~STLReader();

    void read(const std::string& fileName, ModifiedTriangulation& triangulation);
    bool operator()(double a, double b) const;
};
