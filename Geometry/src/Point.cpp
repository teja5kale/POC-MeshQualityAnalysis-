#include "Point.h"

Geometry::Point::Point() : mX(0), mY(0), mZ(0)
{
}

Geometry::Point::Point(int x, int y, int z)
	: mX(x), mY(y), mZ(z)
{
}

Geometry::Point::~Point()
{
}

int Geometry::Point::X() const
{
	return mX;
}

int Geometry::Point::Y() const
{
	return mY;
}

int Geometry::Point::Z() const
{
	return mZ;
}

bool Geometry::Point::operator<(const Geometry::Point& other) const
{
	if (mX != other.mX) 
	{
		return mX < other.mX;
	}
	if (mY != other.mY) 
	{
		return mY < other.mY;
	}
	return mZ < other.mZ;
}
