#include "BoundingBox.h"
#include <algorithm>
MeshOperations::BoundingBox::BoundingBox()
{
}

MeshOperations::BoundingBox::~BoundingBox()
{
}

void MeshOperations::BoundingBox::findMinMax(double x, double y, double z)
{
    xMin = std::min(xMin, x);
    yMin = std::min(yMin, y);
    zMin = std::min(zMin, z);

    xMax = std::max(xMax, x);
    yMax = std::max(yMax, y);
    zMax = std::max(zMax, z);
}

void MeshOperations::BoundingBox::createBoundingBoxTriangulation()
{

    boundingBoxArray[0] = xMin;
    boundingBoxArray[1] = yMin;
    boundingBoxArray[2] = zMin;
    boundingBoxArray[3] = xMax;
    boundingBoxArray[4] = yMin;
    boundingBoxArray[5] = zMin;
    boundingBoxArray[6] = xMax;
    boundingBoxArray[7] = yMax;
    boundingBoxArray[8] = zMin;
    boundingBoxArray[9] = xMin;
    boundingBoxArray[10] = yMax;
    boundingBoxArray[11] = zMin;
    boundingBoxArray[12] = xMin;
    boundingBoxArray[13] = yMin;
    boundingBoxArray[14] = zMax;
    boundingBoxArray[15] = xMax;
    boundingBoxArray[16] = yMin;
    boundingBoxArray[17] = zMax;
    boundingBoxArray[18] = xMax;
    boundingBoxArray[19] = yMax;
    boundingBoxArray[20] = zMax;
    boundingBoxArray[21] = xMin;
    boundingBoxArray[22] = yMax;
    boundingBoxArray[23] = zMax;
}