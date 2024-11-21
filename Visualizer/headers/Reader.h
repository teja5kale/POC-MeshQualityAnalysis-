#pragma once
#include<string>
#include "ModifiedTriangulation.h"

using namespace Geometry;
using namespace MeshOperations;

class Reader
{
	virtual void read(const std::string& fileName, ModifiedTriangulation& triangulation) = 0;
};
