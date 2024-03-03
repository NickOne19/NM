#pragma once
#include "Vector.h"
#include "SimpleMatrix.h"

struct trio
{
	double maxValue;
	Vector maxVector;
	int iterationNum;
};

class DirectIteration
{
	int size;
	SimpleMatrix data;

	double maxEigenValue;
	Vector maxEigenVector;

public:
	DirectIteration(int _size);
	DirectIteration(int _size, int border1, int border2, bool print);

	SimpleMatrix getData();
	void printMatrix() const;
	trio findSecondMaxEigenValue(double eps, int M) const;
private:
	double findAngle(Vector& xk, const Vector& xk1) const;
};