#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <array>
#include <cmath>

#ifdef _WIN32
#define M_PI 3.14159265358979323846264338327950288
#endif

/**
 * Get line vector from two points
 * @param	A	First Point
 * @param	B	Second Point
 * @return		Vector from B to A
 */
std::array<int, 3> getVector(const int A[3], const int B[3]);

/**
 * Get line equation y = a * x + b by giving two points
 * @param	A	First Point
 * @param	B	Second Point
 * @return		a and b parameters of the equation y = a * x + b
 */
std::array<double, 2> getLineEquation(const int A[3], const int B[3]);

/**
 * Get angle between two lines
 * @param	A	First Point of First Line
 * @param	A_	Second Point of First Line
 * @param	B	First Point of Second Line
 * @param	B_	Second Point of Second Line
 * @return		Angle between two lines
 */
double getAngle(const int A[3], const int A_[3], const int B[3], const int B_[3]);

double getPointPointDistance(const std::array<double, 3> A, std::array<double, 3> B);

/**
 * Get plane equation a * x + b * y + c * z + d = 0 by giving three points
 * @param	A	First Point
 * @param	B	Second Point
 * @param	C	Third Point
 * @return		a, b, c and d parameters of the equation a * x + b * y + c * z + d = 0
 */
std::array<double, 4> getPlaneEquation(const int A[3], const int B[3], const int C[3]);

/**
 * Get line equation y = a * x + b by giving a plane and z value
 * @param	P	Plane
 * @param	Z	z value
 * @return		a and b parameters of the equation y = a * x + b
 */
std::array<double, 2> getLineEquationFromPlane(std::array<double, 4> P, int Z);

/**
 * Get distance between a point and a line defined by two points
 * @param	A	First Point of the Line
 * @param	A_	Second Point of the Line
 * @param	P	Point
 * @retur		Distance between the line and the point
 */
double getPointLineDistance(const int A[3], const int A_[3], const int P[3]);

std::array<double, 3> computePlanePointCoordinate(std::array<double, 3> Og, std::array<double, 3> P1, std::array<double, 3> P2, std::array<int, 2> planeSize, std::array<int, 2> pointXY);

#endif // GEOMETRY_H
