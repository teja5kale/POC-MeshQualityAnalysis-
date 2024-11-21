#pragma once

#include "Triangulation.h"
#include "ModifiedTriangle.h"

namespace MeshOperations
{
	class ModifiedTriangulation : public Geometry::Triangulation
	{
	public:
		ModifiedTriangulation();
		~ModifiedTriangulation();
		double _minX = 0;
		double _maxX = 0;
		double _minY = 0;
		double _maxY = 0;
		double _minZ = 0;
		double _maxZ = 0;
		std::vector<MeshOperations::ModifiedTriangle> mTriangles;
		std::vector<double> convertIndicesToDoublePoints(Geometry::Point p);
	};
}
