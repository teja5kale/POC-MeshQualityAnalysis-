#include "MeshInformation.h"
#include "QualityAnalysis.h"
#include "Point.h"
#include "BoundingBox.h"
#include <set>

using namespace MeshOperations;

double MeshInformation::triangleDensity(ModifiedTriangulation triangulation)
{
    QualityAnalysis qualityAnalysis;
    double meshDensity = 0.0;

    double _surfaceArea = qualityAnalysis.caculateTotalsurfaceArea(triangulation);

    if (_surfaceArea != 0.0)
    {
        meshDensity = triangulation.mTriangles.size() / _surfaceArea;
    }
    return meshDensity;
}

double MeshInformation::objectLength()
{
    BoundingBox boundingBox;
    return boundingBox.xMax - boundingBox.xMin;
}

double MeshInformation::objectBreadth()
{
    BoundingBox boundingBox;
    return boundingBox.yMax - boundingBox.yMin;
}

double MeshInformation::objectHeight()
{
    BoundingBox boundingBox;
    return boundingBox.zMax - boundingBox.zMin;
}

size_t MeshInformation::numberOfTriangles(ModifiedTriangulation triangulation)
{
    return triangulation.mTriangles.size();
}

size_t MeshInformation::numberOfVertices(ModifiedTriangulation triangulation)
{
    std::vector<Geometry::Point> verticesList;
    for (auto triangle : triangulation.mTriangles)
    {
        verticesList.push_back(triangle.P1());
        verticesList.push_back(triangle.P2());
        verticesList.push_back(triangle.P3());
    }
    std::set<Geometry::Point> verticesSet(verticesList.begin(), verticesList.end());
    return verticesSet.size();
}
