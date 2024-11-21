#include "ModifiedTriangulation.h"
#include "Triangulation.h"

using namespace MeshOperations;

ModifiedTriangulation::ModifiedTriangulation()
{
}

ModifiedTriangulation::~ModifiedTriangulation()
{
}

std::vector<double> ModifiedTriangulation::convertIndicesToDoublePoints(Geometry::Point p)
{
    std::vector<double> doubledPoint;
    double x = UniqueNumbers[p.X()];
    double y = UniqueNumbers[p.Y()];
    double z = UniqueNumbers[p.Z()];
    doubledPoint.push_back(x);
    doubledPoint.push_back(y);
    doubledPoint.push_back(z);
    return doubledPoint;
}
