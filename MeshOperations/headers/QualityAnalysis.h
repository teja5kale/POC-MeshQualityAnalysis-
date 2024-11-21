#pragma once

#include "ModifiedTriangulation.h"

namespace MeshOperations
{
	class QualityAnalysis
	{
		double clamp(double value, double min, double max);
		double calculateDistanceBetweenPoints(std::vector<double> firstPoint, std::vector<double> secondPoint);
		double calculateAngleBetweenVectors(std::vector<double> v1, std::vector<double> v2);
		double calculateSingleTriangleAspectRatio(Geometry::Point p1, Geometry::Point p2, Geometry::Point p3);
		double calculateSingleTriangleInteriorAngle(Geometry::Point p1, Geometry::Point p2, Geometry::Point p3);
	public:
		double caculateTotalsurfaceArea(ModifiedTriangulation triangulation);
		ModifiedTriangulation createOrthogonalityTriangulation(ModifiedTriangulation triangulation);
		ModifiedTriangulation createAspectRatioTriangulation(ModifiedTriangulation triangulation);
	};
}
