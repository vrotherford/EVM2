#include "pch.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iomanip>
#include <list>

#define ROW 9
#define MAXVALUE 969
#define MINVALUE -969

//Является ли элемент размещенным
bool IsPlaced(int i, std::vector<std::vector<int>> &matrix)
{
	for (auto &row : matrix)
	{
		auto it = std::find(row.begin(), row.end(), i);
		if (it != row.end())
		{
			return true;
		}
	}
	return false;
}

//Сумма элементов строки
int Sum(std::vector<int> &row)
{
	int sum = 0;
	for (auto& n : row)
	{
		sum += n;
	}
	return sum;
}

//Сумма по размещенным элементам
int Sum(std::vector<int> &row, std::vector<std::vector<int>> &plateMatrix)
{
	int sum = 0;
	for (auto& placedRow : plateMatrix)
	{
		for (auto& placedV : placedRow)
		{
			if (placedV != MAXVALUE)
			{
				sum += row[placedV];
			}
		}
	}
	return sum;
}

//Размещает элемент на плате и возвращает false, если она заполнена
bool PlaceVInPlate(std::vector<std::vector<int>> &plateMatrix, int v)
{
	int front = MAXVALUE;
	int iToPlace = MAXVALUE, jToPlace = MAXVALUE;
	for (auto& plateRow : plateMatrix)
	{
		int j = &plateRow - &plateMatrix[0];
		auto it = std::find(plateRow.begin(), plateRow.end(), MAXVALUE);
		if (it != plateRow.end())
		{
			int i = std::distance(plateRow.begin(), it);
			if ( (i + j) < front || ( (i + j) == front && i == j) )
			{
				front = (i + j);
				iToPlace = i;
				jToPlace = j;
			}
		}
	}
	if (iToPlace != MAXVALUE || jToPlace != MAXVALUE)
	{
		plateMatrix[jToPlace][iToPlace] = v;
		return true;
	}
	return false;
}

//Возвращает индекс вершины на плате
std::pair<int, int> ReturnIndex(int v, std::vector<std::vector<int>> &plateMatrix)
{
	for (auto& plateRow : plateMatrix)
	{
		int j = &plateRow - &plateMatrix[0];
		auto it = std::find(plateRow.begin(), plateRow.end(), v);
		if (it != plateRow.end())
		{
			int i = std::distance(plateRow.begin(), it);
			return std::make_pair(j, i);
		}
	}
}

//Находим сумму связей вершин умноженную на длинну пути на плате
int Sum(std::vector<std::vector<int>> &matrix, std::vector<std::vector<int>> &plateMatrix, int v, int i, int j, std::list<int> &excluded)
{
	int sum = 0;
	for (auto& n : matrix[v])
	{
		int iMatrix = &n - &matrix[v][0];
		if (n != 0 && std::find(excluded.begin(), excluded.end(), iMatrix) == excluded.end())
		{
			std::pair<int, int> indexOfV = ReturnIndex(iMatrix, plateMatrix);
			sum += n * (abs(j - indexOfV.first) + abs(i - indexOfV.second));
		}
	}
	return sum;
}

//Находим Q
int Sum(std::vector<std::vector<int>> &matrix, std::vector<std::vector<int>> &plateMatrix)
{
	int sum = 0;
	std::list<int> excluded;
	for (auto& plateRow : plateMatrix)
	{
		int j = &plateRow - &plateMatrix[0];
		for (auto& v : plateRow)
		{
			excluded.push_back(v);
			auto it = std::find(plateRow.begin(), plateRow.end(), v);
			int i = std::distance(plateRow.begin(), it);
			sum += Sum(matrix, plateMatrix, v, i, j, excluded);
		}
	}
	return sum;
}

int SumByX(std::vector<std::vector<int>> &matrix, std::vector<std::vector<int>> &plateMatrix, int v, int i)
{
	int sum = 0;
	for (auto& n : matrix[v])
	{
		int iMatrix = &n - &matrix[v][0];
		if (n != 0)
		{
			std::pair<int, int> indexOfV = ReturnIndex(iMatrix, plateMatrix);
			sum += n * (abs(i - indexOfV.second) == 0 ? 1 : abs(i - indexOfV.second));
		}
	}
	return sum;
}

int SumByY(std::vector<std::vector<int>> &matrix, std::vector<std::vector<int>> &plateMatrix, int v, int j)
{
	int sum = 0;
	for (auto& n : matrix[v])
	{
		int iMatrix = &n - &matrix[v][0];
		if (n != 0)
		{
			std::pair<int, int> indexOfV = ReturnIndex(iMatrix, plateMatrix);
			sum += n * (abs(j - indexOfV.first) == 0 ? 1 : abs(j - indexOfV.first));
		}
	}
	return sum;
}

void printMatrix(std::vector<std::vector<int>> &matrix) {
	for (auto& rowM : matrix) {
		for (auto& n : rowM) {
			std::cout << std::setw(4) << n;
		}
		std::cout << std::endl;
	}
}

void printVector(std::vector<int> &V)
{
	for (auto& v : V)
	{
		std::cout << std::setw(3) << v;
	}
}

int main()
{
	std::vector<std::vector<int>> matrix(ROW);
	#pragma region MatrixDefinition
	/*matrix[0] = { 0, 0, 1, 3, 0, 3, 2, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 3, 0, 0, 3, 3, 0, 3, 0, 0, 0, 0, 1 };
	matrix[1] = { 0, 0, 0, 2, 4, 3, 4, 0, 1, 1, 0, 0, 0, 0, 2, 0, 1, 0, 0, 0, 2, 0, 4, 4, 0, 1, 3, 0, 2, 3 };
	matrix[2] = { 1, 0, 0, 0, 0, 0, 4, 0, 1, 0, 2, 0, 2, 0, 4, 1, 3, 0, 0, 0, 3, 0, 0, 3, 0, 2, 1, 2, 1, 0 };
	matrix[3] = { 3, 2, 0, 0, 1, 0, 0, 0, 0, 0, 2, 0, 1, 1, 0, 0, 4, 2, 1, 0, 2, 0, 1, 2, 2, 2, 3, 4, 3, 0 };
	matrix[4] = { 0, 4, 0, 1, 0, 0, 4, 3, 4, 3, 3, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 4, 1, 4, 0, 0, 4, 0, 1 };
	matrix[5] = { 3, 3, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 2, 4, 1, 2, 2, 0, 3, 3, 0, 3, 0, 3, 4, 0, 0, 0 };
	matrix[6] = { 2, 4, 4, 0, 4, 0, 0, 3, 0, 0, 1, 0, 2, 0, 4, 2, 0, 0, 0, 0, 0, 0, 0, 3, 4, 1, 0, 0, 0, 0 };
	matrix[7] = { 0, 0, 0, 0, 3, 3, 3, 0, 0, 3, 2, 2, 0, 0, 2, 0, 0, 0, 2, 0, 4, 4, 4, 2, 1, 0, 2, 2, 0, 0 };
	matrix[8] = { 0, 1, 1, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 3, 0, 1, 0, 0, 4, 0, 4, 2, 4, 0, 4, 0, 0, 0, 0, 0 };
	matrix[9] = { 0, 1, 0, 0, 3, 0, 0, 3, 4, 0, 1, 0, 0, 1, 2, 1, 0, 2, 0, 4, 3, 0, 3, 0, 0, 1, 0, 0, 0, 3 };
	matrix[10] = { 0, 0, 2, 2, 3, 0, 1, 2, 0, 1, 0, 1, 0, 2, 2, 3, 2, 3, 3, 0, 0, 0, 0, 1, 4, 4, 3, 0, 2, 0 };
	matrix[11] = { 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 1, 0, 0, 2, 4, 0, 1, 0, 3, 0, 0, 1, 0, 4, 1, 0, 4, 0, 4, 0 };
	matrix[12] = { 0, 0, 2, 1, 0, 0, 2, 0, 0, 0, 0, 0, 0, 3, 4, 3, 0, 0, 3, 2, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0 };
	matrix[13] = { 0, 0, 0, 1, 0, 0, 0, 0, 3, 1, 2, 2, 3, 0, 0, 1, 1, 3, 0, 0, 0, 0, 3, 0, 4, 1, 0, 0, 1, 4 };
	matrix[14] = { 1, 2, 4, 0, 0, 2, 4, 2, 0, 2, 2, 4, 4, 0, 0, 0, 4, 3, 0, 4, 0, 1, 2, 3, 1, 4, 4, 0, 4, 0 };
	matrix[15] = { 0, 0, 1, 0, 0, 4, 2, 0, 1, 1, 3, 0, 3, 1, 0, 0, 0, 0, 0, 2, 0, 0, 2, 2, 0, 2, 0, 3, 3, 2 };
	matrix[16] = { 0, 1, 3, 4, 0, 1, 0, 0, 0, 0, 2, 1, 0, 1, 4, 0, 0, 1, 1, 4, 4, 0, 2, 1, 0, 0, 0, 3, 0, 0 };
	matrix[17] = { 1, 0, 0, 2, 0, 2, 0, 0, 0, 2, 3, 0, 0, 3, 3, 0, 1, 0, 0, 0, 2, 0, 1, 2, 4, 1, 1, 0, 0, 3 };
	matrix[18] = { 3, 0, 0, 1, 3, 2, 0, 2, 4, 0, 3, 3, 3, 0, 0, 0, 1, 0, 0, 0, 2, 1, 3, 2, 4, 4, 0, 0, 3, 3 };
	matrix[19] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 2, 0, 4, 2, 4, 0, 0, 0, 4, 4, 3, 0, 4, 0, 0, 3, 0, 0 };
	matrix[20] = { 0, 2, 3, 2, 0, 3, 0, 4, 4, 3, 0, 0, 1, 0, 0, 0, 4, 2, 2, 4, 0, 0, 0, 0, 4, 0, 0, 2, 0, 4 };
	matrix[21] = { 3, 0, 0, 0, 0, 3, 0, 4, 2, 0, 0, 1, 3, 0, 1, 0, 0, 0, 1, 4, 0, 0, 0, 3, 2, 4, 0, 1, 2, 3 };
	matrix[22] = { 3, 4, 0, 1, 4, 0, 0, 4, 4, 3, 0, 0, 0, 3, 2, 2, 2, 1, 3, 3, 0, 0, 0, 3, 1, 0, 1, 4, 3, 0 };
	matrix[23] = { 0, 4, 3, 2, 1, 3, 3, 2, 0, 0, 1, 4, 0, 0, 3, 2, 1, 2, 2, 0, 0, 3, 3, 0, 4, 4, 2, 0, 1, 0 };
	matrix[24] = { 3, 0, 0, 2, 4, 0, 4, 1, 4, 0, 4, 1, 0, 4, 1, 0, 0, 4, 4, 4, 4, 2, 1, 4, 0, 3, 0, 0, 3, 1 };
	matrix[25] = { 0, 1, 2, 2, 0, 3, 1, 0, 0, 1, 4, 0, 0, 1, 4, 2, 0, 1, 4, 0, 0, 4, 0, 4, 3, 0, 0, 1, 0, 0 };
	matrix[26] = { 0, 3, 1, 3, 0, 4, 0, 2, 0, 0, 3, 4, 0, 0, 4, 0, 0, 1, 0, 0, 0, 0, 1, 2, 0, 0, 0, 4, 0, 0 };
	matrix[27] = { 0, 0, 2, 4, 4, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 3, 2, 1, 4, 0, 0, 1, 4, 0, 1, 0 };
	matrix[28] = { 0, 2, 1, 3, 0, 0, 0, 0, 0, 0, 2, 4, 0, 1, 4, 3, 0, 0, 3, 0, 0, 2, 3, 1, 3, 0, 0, 1, 0, 3 };
	matrix[29] = { 1, 3, 0, 0, 1, 0, 0, 0, 0, 3, 0, 0, 0, 4, 0, 2, 0, 3, 3, 0, 4, 3, 0, 0, 1, 0, 0, 0, 3, 0 };*/

	matrix[0] = { 0, 0, 0, 3, 0, 0, 2, 3, 0 };
	matrix[1] = { 0, 0, 2, 0, 2, 0, 0, 0, 0 };
	matrix[2] = { 0, 2, 0, 1, 0, 0, 0, 0, 0 };
	matrix[3] = { 3, 0, 1, 0, 0, 5, 0, 0, 0 };
	matrix[4] = { 0, 2, 0, 0, 0, 2, 0, 0, 4 };
	matrix[5] = { 0, 0, 0, 5, 2, 0, 5, 0, 0 };
	matrix[6] = { 2, 0, 0, 0, 0, 5, 0, 6, 2 };
	matrix[7] = { 3, 0, 0, 0, 0, 0, 6, 0, 0 };
	matrix[8] = { 0, 0, 0, 0, 4, 0, 2, 0, 0 };

#pragma endregion
	std::vector<int> sumVector(ROW);
	std::vector<int> kVector(ROW);
	std::vector<std::vector<int>> plateMatrix(3, std::vector<int>(3, MAXVALUE));
	bool isContinueToPlace = true;
	
	//Размещаем первый элемент
	plateMatrix[0][0] = 0;
	
	//Заполняем вектор сумм
	for (auto& row : matrix)
	{
		int j = &row - &matrix[0];
		sumVector[j] = Sum(row);
	}
	
	//Получаем опорное решение
	while (isContinueToPlace)
	{
		//Заполняем вектор коэффициентов К
		for (auto& k : kVector)
		{
			int kIndex = &k - &kVector[0];
			if (IsPlaced(kIndex, plateMatrix))
			{
				k = MINVALUE;
			}
			else
			{
				k = 2 * Sum(matrix[kIndex], plateMatrix) - sumVector[kIndex];
			}
		}

		auto indexOfMaxK = std::max_element(kVector.begin(), kVector.end());
		int maxK = *indexOfMaxK;
		std::list<int> VtoPlace;

		//Заполняем список элементов, которые необходимо разместить
		for (auto& k : kVector)
		{
			int kIndex = &k - &kVector[0];
			if (k == maxK)
			{
				VtoPlace.push_back(kIndex);
			}
		}

		//Размещаем элементы на плате
		for (auto& v : VtoPlace)
		{
			isContinueToPlace &= PlaceVInPlate(plateMatrix, v);
		}
		printMatrix(plateMatrix);
		std::cout << std::endl;
	}

	std::cout << "Q=" << Sum(matrix, plateMatrix) << std::endl;

	while (true)
	{
		std::vector<double> lVi(ROW);
		std::list<int> excluded;
		//Заполняем вектор lVi
		for (auto& row : plateMatrix)
		{
			int j = &row - &plateMatrix[0];
			for (auto& v : row)
			{
				excluded.push_back(v);
				auto it = std::find(row.begin(), row.end(), v);
				int i = std::distance(row.begin(), it);
				lVi[v] = (double)Sum(matrix, plateMatrix, v, i, j, excluded) / sumVector[v];
			}
		}

		auto itMaxLVi = std::max_element(lVi.begin(), lVi.end());
		int indexOfMaxLVi = std::distance(lVi.begin(), itMaxLVi);
		std::pair<int, int> indexOnPlate = ReturnIndex(indexOfMaxLVi, plateMatrix);

		int xVi = ceil((double)SumByX(matrix, plateMatrix, indexOfMaxLVi, indexOnPlate.second) / sumVector[indexOfMaxLVi]);
		int yVi = ceil((double)SumByY(matrix, plateMatrix, indexOfMaxLVi, indexOnPlate.first) / sumVector[indexOfMaxLVi]);

		int tempV = plateMatrix[yVi][xVi];
		plateMatrix[yVi][xVi] = indexOfMaxLVi;
		plateMatrix[indexOnPlate.first][indexOnPlate.second] = tempV;

		printMatrix(plateMatrix);
		std::cout << std::endl;
		std::cout << "Q=" << Sum(matrix, plateMatrix) << std::endl;

	}
}