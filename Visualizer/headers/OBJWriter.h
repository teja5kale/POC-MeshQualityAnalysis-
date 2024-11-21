#pragma once
#include "Point.h"
#include "Writer.h"
#include "ModifiedTriangulation.h"
#include <vector>
#include <map>
#include "ModifiedTriangle.h"
using namespace Geometry;

class ObjWriter : public Writer
{
public:
    void Write(const std::string& filename, const ModifiedTriangulation& triangulation, QProgressBar*);

private:
    void findAndAddPoint(Point point, std::vector<Point>& pointList, std::map<Point, int>& uniqueValueMap);
    std::string formulateVertex(ModifiedTriangulation triangulation, Point point);
    std::string formulateVertexNormal(ModifiedTriangulation triangulation, Point point);
    std::string formulateFace(ModifiedTriangle triangle, std::map<Point, int>, std::map<Point, int>);
};
