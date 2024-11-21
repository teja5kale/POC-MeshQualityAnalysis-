#pragma once
namespace MeshOperations
{
    class BoundingBox {

    public:

        double  xMin;
        double  yMin;
        double  zMin;
        double  xMax;
        double  yMax;
        double  zMax;

        double boundingBoxArray[24];

        BoundingBox();
        ~BoundingBox();

        void findMinMax(double x, double y, double z);
        void createBoundingBoxTriangulation();
    };
}