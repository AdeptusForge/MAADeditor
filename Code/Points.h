#pragma once

class Point2
{
private:

public:
	float X;
	float Y;
	//Constructors
	Point2(float x, float y) : X(x), Y(y) {};

	////Conversions
	//Point2 operator=(const Point2Int a)
	//{
	//	X = (float)a.X;
	//	Y = (float)a.Y;
	//}

	//void operator=(Point3 a)
	//{
	//	X = a.X;
	//	Y = a.Y;
	//}

	//void operator=(Point3Int a)
	//{
	//	X = (float)a.X;
	//	Y = (float)a.Y;
	//}
};

class Point2Int
{
private:

public:
	int X;
	int Y;
	//Constructors
	Point2Int(int x, int y) : X(x), Y(y) {};

	////Conversions
	//Point2Int& operator=(const Point2 a)
	//{
	//	X = (int)a.X;
	//	Y = (int)a.Y;
	//}
	//
	//void operator=(Point3 a)
	//{
	//	X = (int)a.X;
	//	Y = (int)a.Y;
	//}	

	//void operator=(Point3Int a)
	//{
	//	X = a.X;
	//	Y = a.Y;
	//}
};

class Point3Int
{
private:

public:
	int X;
	int Y;
	int Z;

	//Constructors
	Point3Int(int x, int y, int z) : X(x), Y(y), Z(z) {};

	////Conversions
	//void operator=(Point2 a)
	//{
	//	X = (int)a.X;
	//	Y = (int)a.Y;
	//}

	//void operator=(Point2Int a)
	//{
	//	X = a.X;
	//	Y = a.Y;
	//}

	//void operator=(Point3 a)
	//{
	//	X = (int)a.X;
	//	Y = (int)a.Y;
	//	Z = (int)a.Z;
	//}


};

class Point3
{
private:

public:
	float X;
	float Y;
	float Z;
	//Constructors
	Point3(float x, float y, float z) : X(x), Y(y), Z(z) {};

	////Conversions
	//void operator=(Point2 a)
	//{
	//	X = a.X;
	//	Y = a.Y;
	//}

	//void operator=(Point2Int a)
	//{
	//	X = (float)a.X;
	//	Y = (float)a.Y;
	//}

	//void operator=(Point3Int a)
	//{
	//	X = (float)a.X;
	//	Y = (float)a.Y;
	//	Z = (float)a.Z;
	//}

};
