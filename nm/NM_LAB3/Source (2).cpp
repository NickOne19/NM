#include <iostream>
#include <iomanip>
#include <Windows.h>
#include "Vector.h"
#include "DirectIteration.h"
#include "SimpleMatrix.h"

void task();

int main() {
	srand(time(0));
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	DirectIteration matrix(4, -20, 20, true);
	std::cout << "Матрица А:\n";
	matrix.printMatrix();

	trio answer = matrix.findSecondMaxEigenValue(0.00001, 100);
	std::cout << "Второе максимальное по модулю собственное значение: " << answer.maxValue << std::endl;
	std::cout << "Второй собственный вектор: ";
	answer.maxVector.print();

	std::cout << "Мера точности полученной пары : " << (matrix.getData() * answer.maxVector - answer.maxVector * answer.maxValue).getFirstNorm() << std::endl;
	std::cout << "Число выполненных итераций: " << answer.iterationNum << std::endl << std::endl;

	task();
}


void task()
{
	std::string otstup = "  ";


	std::cout << "size" << otstup << "diapazon" << otstup << std::setw(8) << "eps" << otstup << "ср.знач" << otstup;
	std::cout << std::setw(8) << "r" << otstup << "ср.итер" << std::endl;
	for (int size = 10; size <= 50; size += 20)
	{
		bool flagPrint1 = true;
		for (int diapozon = 10; diapozon <= 100; diapozon *= 10) // изменил данные, ради чистоты эксперимента
		{
			bool flagPrint2 = true;
			for (double eps = 0.00001; eps >= 0.00000001; eps /= 1000)
			{	
				// функционал 

				double avgValue = 0.0;
				double r = 0.0;
				double avgIter = 0.0;

				DirectIteration matrix(size, -diapozon, diapozon, false);

				for (int count = 0; count < 10; count++) // все тесты проходят 10 раз, для получения максимально точного результата
				{
					trio answer2 = matrix.findSecondMaxEigenValue(eps, INT_MAX);

					avgValue += answer2.maxValue;
					r += (matrix.getData() * answer2.maxVector - answer2.maxVector * answer2.maxValue).getFirstNorm();
					avgIter += answer2.iterationNum;
				}

				// печать

				if (flagPrint1) std::cout << std::setw(4) << size << otstup;
				else std::cout << std::setw(4) << " " << otstup;

				if (flagPrint2) std::cout << std::setw(8) << diapozon << otstup;
				else std::cout << std::setw(8) << " " << otstup;

				std::cout << std::setw(8) << std::setprecision(5) << eps << otstup;

				std::cout << std::setw(7) << std::setprecision(4) << avgValue / 10 << otstup;
				std::cout << std::setw(8) << std::setprecision(4) << r / 10 << otstup << std::setw(7) << avgIter / 10 << std::endl;

				flagPrint1 = false;
				flagPrint2 = false;
			}
		}
	}
}