#include "QualityAnalysis.h"
#include "BoundingBox.h"
#include <cmath>
#include <algorithm>
#include <vector>
#include <set>
#include <iostream>
#include <limits>

#define PI 3.14159265358979323846

using namespace MeshOperations;

double QualityAnalysis::QualityAnalysis::clamp(double value, double min, double max)
{
    return (value < min) ? min : (value > max) ? max : value;
}

double QualityAnalysis::QualityAnalysis::calculateDistanceBetweenPoints(std::vector<double> firstPoint, std::vector<double> secondPoint)
{
    // Calculate the distance using Euclidean distance formula
    double dx = firstPoint[0] - secondPoint[0];
    double dy = firstPoint[1] - secondPoint[1];
    double dz = firstPoint[2] - secondPoint[2];

    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

double QualityAnalysis::QualityAnalysis::calculateAngleBetweenVectors(std::vector<double> v1, std::vector<double> v2) {
    double dotProduct = v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
    double magnitudeV1 = std::sqrt(v1[0] * v1[0] + v1[1] * v1[1] + v1[2] * v1[2]);
    double magnitudeV2 = std::sqrt(v2[0] * v2[0] + v2[1] * v2[1] + v2[2] * v2[2]);
    double cosTheta = dotProduct / (magnitudeV1 * magnitudeV2);
    cosTheta = clamp(cosTheta, -1.0, 1.0);
    return std::acos(cosTheta) * (180.0 / PI);
}

double QualityAnalysis::QualityAnalysis::calculateSingleTriangleAspectRatio(Geometry::Point p1, Geometry::Point p2, Geometry::Point p3)
{
    ModifiedTriangulation mt;
    std::vector<double> vertex1 = mt.convertIndicesToDoublePoints(p1);
    std::vector<double> vertex2 = mt.convertIndicesToDoublePoints(p2);
    std::vector<double> vertex3 = mt.convertIndicesToDoublePoints(p3);

    double side1 = QualityAnalysis::QualityAnalysis::calculateDistanceBetweenPoints(vertex1, vertex2);
    double side2 = QualityAnalysis::QualityAnalysis::calculateDistanceBetweenPoints(vertex2, vertex3);
    double side3 = QualityAnalysis::QualityAnalysis::calculateDistanceBetweenPoints(vertex3, vertex1);

    double longestEdge = std::max(std::max(side1, side2), side3);
    double shortestEdge = std::min(std::min(side1, side2), side3);

    double aspectRatio = longestEdge / shortestEdge;
    return aspectRatio;
}

double QualityAnalysis::QualityAnalysis::calculateSingleTriangleInteriorAngle(Geometry::Point p1, Geometry::Point p2, Geometry::Point p3)
{
    ModifiedTriangulation mt;
    std::vector<double> vertex1 = mt.convertIndicesToDoublePoints(p1);
    std::vector<double> vertex2 = mt.convertIndicesToDoublePoints(p2);
    std::vector<double> vertex3 = mt.convertIndicesToDoublePoints(p3);

    std::vector<double> AB{ vertex2[0] - vertex1[0], vertex2[1] - vertex1[1], vertex2[2] - vertex1[2] };
    std::vector<double> BC{ vertex3[0] - vertex2[0], vertex3[1] - vertex2[1], vertex3[2] - vertex2[2] };
    std::vector<double> CA{ vertex1[0] - vertex3[0], vertex1[1] - vertex3[1], vertex1[2] - vertex3[2] };

    std::vector<double> negativeVectorCA = { -CA[0], -CA[1], -CA[2] };

    double angleA = calculateAngleBetweenVectors(AB, negativeVectorCA);
    double angleB = calculateAngleBetweenVectors(AB, BC);
    double angleC = calculateAngleBetweenVectors(BC, negativeVectorCA);

    double averageAngle = (angleA + angleB + angleC) / 3.0;

    return averageAngle;
}

double QualityAnalysis::QualityAnalysis::caculateTotalsurfaceArea(ModifiedTriangulation triangulation)
{
    double totalSurfaceArea = 0;
    ModifiedTriangulation mt;

    for (ModifiedTriangle triangle : triangulation.mTriangles)
    {
        std::vector<double> vertex1 = mt.convertIndicesToDoublePoints(triangle.P1());
        std::vector<double> vertex2 = mt.convertIndicesToDoublePoints(triangle.P2());
        std::vector<double> vertex3 = mt.convertIndicesToDoublePoints(triangle.P3());

        double side1 = QualityAnalysis::QualityAnalysis::calculateDistanceBetweenPoints(vertex1, vertex2);
        double side2 = QualityAnalysis::QualityAnalysis::calculateDistanceBetweenPoints(vertex2, vertex3);
        double side3 = QualityAnalysis::QualityAnalysis::calculateDistanceBetweenPoints(vertex3, vertex1);

        double semiPerimeter = (side1 + side2 + side3) / 2;
        double area = std::sqrt(semiPerimeter * (semiPerimeter - side1) * (semiPerimeter - side2) * (semiPerimeter - side3));
        totalSurfaceArea += area;
    }
    return totalSurfaceArea;
}

ModifiedTriangulation QualityAnalysis::QualityAnalysis::createOrthogonalityTriangulation(ModifiedTriangulation triangulation)
{
    ModifiedTriangulation orthogonalityTriangulation;
    double INTERIOR_ANGLE_THRESHOLD = 60;

    for (auto triangle : triangulation.mTriangles)
    {
        Geometry::Point p1 = triangle.P1();
        Geometry::Point p2 = triangle.P2();
        Geometry::Point p3 = triangle.P3();
        Geometry::Point normal; 
		double TriangleInteriorAngle = calculateSingleTriangleInteriorAngle(p1, p2, p3);

        if (TriangleInteriorAngle >= INTERIOR_ANGLE_THRESHOLD)
        {
            double color[3] = { 0, 1, 0 };
            orthogonalityTriangulation.mTriangles.push_back(ModifiedTriangle(normal, p1, p2, p3));
			orthogonalityTriangulation.mTriangles.back().setColor(color[0], color[1], color[2]); 
        }
        else
        {
            double color[3] = { 1, 0, 0 };
            orthogonalityTriangulation.mTriangles.push_back(ModifiedTriangle(normal, p1, p2, p3)); 
            orthogonalityTriangulation.mTriangles.back().setColor(color[0], color[1], color[2]); 

        }
    }
    return orthogonalityTriangulation;
}

ModifiedTriangulation QualityAnalysis::QualityAnalysis::createAspectRatioTriangulation(ModifiedTriangulation triangulation)
{
    ModifiedTriangulation aspectRatioTriangulation;
    double ASPECT_RATIO_THRESHOLD = 60;

    for (auto triangle : triangulation.mTriangles)
    { 
        Geometry::Point p1 = triangle.P1(); 
        Geometry::Point p2 = triangle.P2();  
        Geometry::Point p3 = triangle.P3(); 
        Geometry::Point normal; 
        if (calculateSingleTriangleAspectRatio(p1, p2, p3) >= ASPECT_RATIO_THRESHOLD)
        {
            double color[3] = { 0, 1, 0 };
            aspectRatioTriangulation.mTriangles.push_back(ModifiedTriangle(normal, p1, p2, p3)); 
            aspectRatioTriangulation.mTriangles.back().setColor(color[0], color[1], color[2]); 
        }
        else
        {
            double color[3] = { 1, 0, 0 };
            aspectRatioTriangulation.mTriangles.push_back(ModifiedTriangle(normal, p1, p2, p3)); 
            aspectRatioTriangulation.mTriangles.back().setColor(color[0], color[1], color[2]); 

        }
    }
    return aspectRatioTriangulation;
}
