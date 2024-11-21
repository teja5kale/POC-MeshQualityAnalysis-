#pragma once

#include "ModifiedTriangulation.h"

namespace MeshOperations
{
	class MeshInformation
	{
	public:
		double triangleDensity(ModifiedTriangulation triangulation);
		double objectLength();
		double objectBreadth();
		double objectHeight();
		size_t numberOfTriangles(ModifiedTriangulation triangulation);
		size_t numberOfVertices(ModifiedTriangulation triangulation);
	};
}