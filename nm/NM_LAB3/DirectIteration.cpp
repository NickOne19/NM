#include "DirectIteration.h"
#include <iostream>
#include <iomanip>

DirectIteration::DirectIteration(int _size) : data(_size, _size), maxEigenVector(_size)
{
	size = _size;
	maxEigenValue = 0.0;
}

DirectIteration::DirectIteration(int _size, int border1, int border2, bool print) : data(_size, _size), maxEigenVector(_size)
{
	size = _size;

	// ��������� ����������� ��������

	Vector eigenValues(size);
	int diapazonValues = border2 - border1 + 1;
	if (diapazonValues < size)
	{
		//std::cout << "������������� �������� � ����������� ���������. ������ ��������� ���������: 1,2,.." << size << std::endl;
		for (int i = 0; i < size; i++)
			eigenValues.setValue(i, static_cast<double>(i) + 1);

		maxEigenValue = size;
	}
	else
	{
		for (int i = 0; i < size; i++) // �������� ��������� ����������� ��������
		{
			eigenValues.setValue(i, border1 + static_cast<double>(rand() % diapazonValues));

			auto isValueUsed = [](const Vector& ptr, int pos, double value)
			{
				for (int i = 0; i < pos; i++)
					if (abs(ptr.getValue(i)) == abs(value)) return true;
				return false;
			};

			while (isValueUsed(eigenValues, i, eigenValues.getValue(i)))
				eigenValues.setValue(i, border1 + static_cast<double>(rand() % diapazonValues));
		}

		// ������ ������������ ��������

		maxEigenValue = eigenValues.getValue(0);

		for (int i = 1; i < size; i++)
		{
			if (abs(eigenValues.getValue(i)) > abs(maxEigenValue))
				maxEigenValue = eigenValues.getValue(i);
		}
	}

	//��������� ����������� vectors

	Vector omega(size, border1, border2); // ��������� �����
	omega = omega / omega.getSecondNorm(); // ������������ �����

	SimpleMatrix E(size, size); // ��������� �������
	E.toEdentityMatrix();

	SimpleMatrix eigenVectors(size, size); // ������� ����������� ��������
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			eigenVectors.setValue(i, j, E.getValue(i, j) - 2 * omega.getValue(i) * omega.getValue(j));

	SimpleMatrix Lyabda(size, size); // ������������ ������� ������
	for (int i = 0; i < size; i++)
		Lyabda.setValue(i, i, eigenValues.getValue(i));

	data = eigenVectors * Lyabda * eigenVectors;

	// ����� ��������, ��������������� ������������ � ����������� ���������

	for (int i = 0; i < size; i++)
	{
		if (eigenValues.getValue(i) == maxEigenValue)
		{
			for (int j = 0; j < size; j++)
				maxEigenVector.setValue(j, eigenVectors.getValue(j, i));
		}
	}

	// ������ ��� �������� �����������������

	if (print)
	{
		std::cout << "����������� ��������: ";
		eigenValues.print();
		std::cout << "����������� �������:\n";
		eigenVectors.print();

		std::cout << "������������ ����������� ��������: " << maxEigenValue << "\n";
		std::cout << "������, ��������������� ������. ������������ ��������:\n";
		maxEigenVector.print();
	}
}

SimpleMatrix DirectIteration::getData()
{
	return data;
}

void DirectIteration::printMatrix() const
{
	data.print();
}

// ����� ���������� �������� �������� ���� ����� ��������� xk � xk1 � ��������� ����

double DirectIteration::findAngle(Vector& xk, const Vector& xk1) const
{
	double sum = 0.0;									//
	for (int i = 0; i < xk.getSize(); i++)			// ��������� ��������� ������������ ��� ����� ������������ ��������������� ���������
		sum += xk.getValue(i) * xk1.getValue(i);	//
	double xkNorm = xk.getSecondNorm();				// 
	xk = xk1; // ��� ����������� ��������

	double answer = sum / (xkNorm * xk1.getSecondNorm()); // ����������� ������� ���� �� ������� 

	return answer;
}

trio DirectIteration::findSecondMaxEigenValue(double eps, int M) const
{
	int numIteration = 0;		// ����� ����������� ��������
	Vector xk(size, 5, 10);		// ��������� ������ �
	Vector niulast(size, -4, 1);// ������ ����������� ��� ������� ����������� �������
	Vector niuNew(size, 1, 4);	// ������ � �������� ��������� ����������� ������ ������������� �����. ��-�
	double sigma = 0.0;			// ����� � �������� ��������� ������������ ����������� ��������
	
	SimpleMatrix A1(size, size); // ������� �1
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			A1.setValue(i, j, data.getValue(i, j) - maxEigenValue * maxEigenVector.getValue(i) * maxEigenVector.getValue(j));

	do
	{
		numIteration++;
		//if (print)
		//	std::cout << numIteration << " ��������:\n";

		niuNew = xk / xk.getSecondNorm();					// ������������ ������� �
		xk = A1 * niuNew;									// ��������� �(k+1)

		double sum = 0.0;
		for (int i = 0; i < size; i++)						// ��������� �����
			sum += niuNew.getValue(i) * xk.getValue(i);	//
		sigma = sum;										//

		//if (print)
		//{
		//	std::cout << "niuNew: "; niuNew.print();
		//	std::cout << "sigma: " << sigma << std::endl;
		//}

	} while (1 - abs(findAngle(niulast, niuNew)) > eps && numIteration < M);

	return trio{ sigma, niuNew, numIteration };
}