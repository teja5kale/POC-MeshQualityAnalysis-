#pragma once

#include "Triangle.h"

namespace MeshOperations
{
    class ModifiedTriangle : public Geometry::Triangle
    {
        std::vector<double> color = { 0.0, 1.0, 0.0 };
    public:
        ModifiedTriangle(Geometry::Point& normal, Geometry::Point& p1, Geometry::Point& p2, Geometry::Point& p3);
        std::vector<double> Color();
        void setColor(double r, double g, double b);
    };
}
