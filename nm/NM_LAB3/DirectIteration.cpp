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

	// получение собственных значений

	Vector eigenValues(size);
	int diapazonValues = border2 - border1 + 1;
	if (diapazonValues < size)
	{
		//std::cout << "Повторяющиеся значения в собственных значениях. Выбран фиктивный результат: 1,2,.." << size << std::endl;
		for (int i = 0; i < size; i++)
			eigenValues.setValue(i, static_cast<double>(i) + 1);

		maxEigenValue = size;
	}
	else
	{
		for (int i = 0; i < size; i++) // задаются различные собственные значения
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

		// ищется максимальное значение

		maxEigenValue = eigenValues.getValue(0);

		for (int i = 1; i < size; i++)
		{
			if (abs(eigenValues.getValue(i)) > abs(maxEigenValue))
				maxEigenValue = eigenValues.getValue(i);
		}
	}

	//получение собственных vectors

	Vector omega(size, border1, border2); // получение омеги
	omega = omega / omega.getSecondNorm(); // нормирование омеги

	SimpleMatrix E(size, size); // единичная матрица
	E.toEdentityMatrix();

	SimpleMatrix eigenVectors(size, size); // матрица собственных векторов
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			eigenVectors.setValue(i, j, E.getValue(i, j) - 2 * omega.getValue(i) * omega.getValue(j));

	SimpleMatrix Lyabda(size, size); // симметричная матрица Лямбда
	for (int i = 0; i < size; i++)
		Lyabda.setValue(i, i, eigenValues.getValue(i));

	data = eigenVectors * Lyabda * eigenVectors;

	// поиск векторов, соответствующих максимальным и минимальным значениям

	for (int i = 0; i < size; i++)
	{
		if (eigenValues.getValue(i) == maxEigenValue)
		{
			for (int j = 0; j < size; j++)
				maxEigenVector.setValue(j, eigenVectors.getValue(j, i));
		}
	}

	// печать для проверки работоспособности

	if (print)
	{
		std::cout << "Собственные значения: ";
		eigenValues.print();
		std::cout << "Собственные вектора:\n";
		eigenVectors.print();

		std::cout << "Максимальное собственные значения: " << maxEigenValue << "\n";
		std::cout << "Вектор, соответствующий максим. собственному значению:\n";
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

// метод возвращает значение косинуса угла между векторами xk и xk1 в радианной мере

double DirectIteration::findAngle(Vector& xk, const Vector& xk1) const
{
	double sum = 0.0;									//
	for (int i = 0; i < xk.getSize(); i++)			// считается скалярное произведение как сумма произведения соответствующих координат
		sum += xk.getValue(i) * xk1.getValue(i);	//
	double xkNorm = xk.getSecondNorm();				// 
	xk = xk1; // для продолжения итерации

	double answer = sum / (xkNorm * xk1.getSecondNorm()); // высчитываем косинус угла по формуле 

	return answer;
}

trio DirectIteration::findSecondMaxEigenValue(double eps, int M) const
{
	int numIteration = 0;		// число совершённых операций
	Vector xk(size, 5, 10);		// случайный вектор Х
	Vector niulast(size, -4, 1);// вектор необходимый для анализа приближения решения
	Vector niuNew(size, 1, 4);	// вектор к которому стремится собственный вектор максимального собст. зн-я
	double sigma = 0.0;			// число к которому стремится максимальное собственное значение
	
	SimpleMatrix A1(size, size); // матрица А1
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			A1.setValue(i, j, data.getValue(i, j) - maxEigenValue * maxEigenVector.getValue(i) * maxEigenVector.getValue(j));

	do
	{
		numIteration++;
		//if (print)
		//	std::cout << numIteration << " итерация:\n";

		niuNew = xk / xk.getSecondNorm();					// нормирование вектора х
		xk = A1 * niuNew;									// получение х(k+1)

		double sum = 0.0;
		for (int i = 0; i < size; i++)						// получение сигмы
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