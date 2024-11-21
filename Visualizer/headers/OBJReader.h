#pragma once

#include "ModifiedTriangulation.h"
#include "Reader.h"
#include "Point.h"

using namespace MeshOperations;

class OBJReader : public Reader
{
public:
	OBJReader();
	~OBJReader();

	void read(const std::string& fileName, ModifiedTriangulation& tri);
	void helper(double xyz[3],std::vector<Geometry::Point>&, std::map<double, int, OBJReader>&, ModifiedTriangulation& triangulation);
	bool operator()(double a, double b) const;
	Geometry::Point vectorReader(const QStringList& lineList, std::map<double, int, OBJReader>& uniqueMap, Triangulation& tri);
};
