#pragma once
#include <string>
#include <iostream> 
#include <iterator> 
#include "framework.h"
#include "Assets.h"
#include "Points.h"

void PhysicsUpdate();

class Rotation
{
public:
	float X;
	float Y;
	float Z;
	//Constructors
	Rotation(float x, float y, float z) : X(x), Y(y), Z(z) {};
	Rotation(Point3 point) : X(point.X), Y(point.Y), Z(point.Z) {};
private:
};

class Velocity
{
public:
	float X;	//position change per frame
	float Y;	//position change per frame
	float Z;	//position change per frame
	//Constructors
	Velocity(float x, float y, float z) : X(x), Y(y), Z(z) {};
	Velocity(Point3 point) : X(point.X), Y(point.Y), Z(point.Z) {};
private:
};

class Spin
{
public:
	float X;	//rotation change per frame
	float Y;	//rotation change per frame
	float Z;	//rotation change per frame
	//Constructors
	Spin(float x, float y, float z) : X(x), Y(y), Z(z) {};
	Spin(Point3 point) : X(point.X), Y(point.Y), Z(point.Z) {};
private:
};

class Position
{
private:

public:
	bool lockedXmov;
	bool lockedYmov;
	bool lockedZmov;
	bool lockedXspin;
	bool lockedYspin;
	bool lockedZspin;

	Point3 point;
	Rotation rotation;

	//Constructors
	Position(Point3 newPoint, Rotation newRotation) : point(newPoint), rotation(newRotation) 
	{
		lockedXmov = false, lockedYmov = false, lockedZmov = false, lockedXspin = false, lockedYspin = false, lockedZspin = false;
	};
	Position(Point3 newPoint, Rotation newRotation, bool xMov, bool yMov, bool zMov, bool xSpin, bool ySpin, bool zSpin) : point(newPoint), rotation(newRotation)
	{
		lockedXmov = xMov, lockedYmov = yMov, lockedZmov = zMov, lockedXspin = xSpin, lockedYspin = ySpin, lockedZspin = zSpin;
	};


	//Edit the position of a GameObject
	void EditPosition(Position newPos)
	{
		if (lockedXmov == false)
			point.X = newPos.point.X;
		if (lockedYmov == false)
			point.Y = newPos.point.Y;
		if (lockedZmov == false)
			point.Z = newPos.point.Z;
		if (lockedXspin == false)
			rotation.X = newPos.rotation.X;
		if (lockedYspin == false)
			rotation.Y = newPos.rotation.Y;
		if (lockedZspin == false)
			rotation.Z = newPos.rotation.Z;
	}

	//Overload that doesn't require a Position
	void EditPosition(Point3 newPoint, Rotation newRot)
	{
		if (lockedXmov == false)
			point.X = newPoint.X;
		if (lockedYmov == false)
			point.Y = newPoint.Y;
		if (lockedZmov == false)
			point.Z = newPoint.Z;
		if (lockedXspin == false)
			rotation.X = newRot.X;
		if (lockedYspin == false)
			rotation.Y = newRot.Y;
		if (lockedZspin == false)
			rotation.Z = newRot.Z;
	}

	//Overload that uses raw numbers
	void EditPosition(float x, float y, float z, float spinX, float spinY, float spinZ)
	{
		if (lockedXmov == false)
			point.X = x;
		if (lockedYmov == false)
			point.Y = y;
		if (lockedZmov == false)
			point.Z = z;
		if (lockedXspin == false)
			rotation.X = spinX;
		if (lockedYspin == false)
			rotation.Y = spinY;
		if (lockedZspin == false)
			rotation.Z = spinZ;
	}
};

class PhysicsObject
{
public:
	Position pos;
	Velocity vel;
	Spin spin;
private:
};

