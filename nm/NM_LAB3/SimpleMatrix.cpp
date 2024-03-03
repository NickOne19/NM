#include "SimpleMatrix.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

SimpleMatrix::SimpleMatrix()
{
	height = 0;
	width = 0;
	data = nullptr;
}

SimpleMatrix::SimpleMatrix(int _height, int _width)
{
	height = _height;
	width = _width;

	data = new double* [height];
	for (int i = 0; i < height; i++)
		data[i] = new double[width];

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			data[i][j] = 0.0;
}

SimpleMatrix::SimpleMatrix(int _height, int _width, int border1, int border2)
{
	height = _height;
	width = _width;

	data = new double* [height];
	for (int i = 0; i < height; i++)
		data[i] = new double[width];

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			data[i][j] = border1 + static_cast<double>(rand() % (border2 - border1 + 1));
}

SimpleMatrix::SimpleMatrix(const SimpleMatrix& x)
{
	height = x.height;
	width = x.width;

	data = new double* [height];
	for (int i = 0; i < height; i++)
		data[i] = new double[width];

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			data[i][j] = x.data[i][j];
}

SimpleMatrix::~SimpleMatrix()
{
	for (int i = 0; i < height; i++)
		delete[] data[i];
	delete[] data;
}

SimpleMatrix& SimpleMatrix::operator=(const SimpleMatrix& x)
{
	if (&x != this)
	{
		height = x.height;
		width = x.width;

		data = new double* [height];
		for (int i = 0; i < height; i++)
			data[i] = new double[width];

		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
				data[i][j] = x.data[i][j];
	}
	return *this;
}

SimpleMatrix SimpleMatrix::operator+(const SimpleMatrix& x) const
{
	if (height != x.height || width != x.width)
	{
		std::cout << "Разные размерности. Сложение матриц невозможно.\n";
		return *this;
	}
	SimpleMatrix answ(height, width);

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			answ.data[i][j] = data[i][j] + x.data[i][j];

	return answ;
}

SimpleMatrix SimpleMatrix::operator-(const SimpleMatrix& x) const
{
	if (height != x.height || width != x.width)
	{
		std::cout << "Разные размерности. Вычитание матриц невозможно.\n";
		return *this;
	}
	SimpleMatrix answ(height, width);

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			answ.data[i][j] = data[i][j] - x.data[i][j];

	return answ;
}

SimpleMatrix SimpleMatrix::operator*(double x) const
{
	SimpleMatrix answ(height, width);

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			answ.data[i][j] = data[i][j] * x;

	return answ;
}

Vector SimpleMatrix::operator*(const Vector& x) const
{
	if (width != x.getSize())
	{
		std::cout << "Разные размерности. Умножение матрицы на вектор невозможно.\n";
		return x;
	}
	Vector answ(height);

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			answ.setValue(i, answ.getValue(i) + data[i][j] * x.getValue(j));

	return answ;
}

SimpleMatrix SimpleMatrix::operator*(const SimpleMatrix& x) const
{
	if (width != x.height)
	{
		std::cout << "Разные размерности. Умножение матриц невозможно.\n";
		return *this;
	}
	SimpleMatrix answ(height, x.width);

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < x.width; j++)
		{
			double sum = 0.0;
			for (int k = 0; k < width; k++)
				sum += data[i][k] * x.data[k][j];
			answ.data[i][j] = sum;
		}
	}
	return answ;
}

void SimpleMatrix::toEdentityMatrix()
{
	if (width != height)
	{
		std::cout << "Матрица не квадратная.Преобразование в единичную невозможно.\n";
	}
	
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			if (i != j)
				data[i][j] = 0.0;
			else
				data[i][j] = 1.0;
}

SimpleMatrix SimpleMatrix::transposition() const
{
	if (width != height)
	{
		std::cout << "Матрица не квадратная. Транспонирование невозможно.\n";
		return *this;
	}
	SimpleMatrix answ(height, width);

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			answ.data[i][j] = data[j][i];

	return answ;
}

void SimpleMatrix::setValue(int i, int j, double value)
{
	data[i][j] = value;
}

double SimpleMatrix::getValue(int i, int j) const
{
	return data[i][j];
}

void SimpleMatrix::print() const
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
			std::cout << std::setw(12) << std::setprecision(5) << data[i][j];
		std::cout << std::endl << std::endl;
	}
	std::cout << std::endl;
}

void SimpleMatrix::inputToFile(std::string fileName)
{
	std::ofstream fout(fileName + ".txt");

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
			fout << std::setw(12) << std::setprecision(5) << data[i][j] << " ";
		fout << std::endl << std::endl;
	}
	fout.close();
}

void SimpleMatrix::fromFile(std::string fileName)
{
	std::ifstream fin(fileName + ".txt");

	for (int i = 0; i < height; i++)
	{
		std::string s;
		getline(fin, s);
		if (s.empty())
			i--;
		else
		{
			int j = 0;
			for (int iter = 0; iter < s.length(); iter++)
			{
				std::string tmpinfo;
				while (s[iter] != ' ' && iter != s.length())
					tmpinfo.push_back(s[iter++]);

				if (!tmpinfo.empty())
					data[i][j++] = std::stod(tmpinfo);
			}
		}
	}
	fin.close();
}