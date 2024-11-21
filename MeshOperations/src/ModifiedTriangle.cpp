#include "ModifiedTriangle.h"

using namespace MeshOperations;

ModifiedTriangle::ModifiedTriangle(Geometry::Point& normal, Geometry::Point& p1, Geometry::Point& p2, Geometry::Point& p3) :
    Geometry::Triangle(normal, p1, p2, p3)
{
}

std::vector<double> ModifiedTriangle::Color()
{
    return color;
}

void ModifiedTriangle::setColor(double r, double g, double b)
{
    color.clear();
    color.push_back(r);
    color.push_back(g);
    color.push_back(b);
}
