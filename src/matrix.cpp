#include "../includes/matrix.hpp"

Matrix::Matrix()
{
}

Matrix::~Matrix()
{
}

void Matrix::multiplyMatrix(int rows, float (*matrixOne)[4], float (*matrixTwo)[4], float (*matrixResult)[4]) {
	int y = 0, x = 0;
	
	while (y < rows) {
		x = 0;
		while (x < 4)
		{
			float resultNumber = 0;
			int incrementingFactor = 0;

			while (incrementingFactor < 4) {
				resultNumber += (matrixOne[y][incrementingFactor] * matrixTwo[incrementingFactor][x]);
				incrementingFactor++;
			}
			matrixResult[y][x] = resultNumber;
			x++;
		}
		y++;
	}

}

void Matrix::convertVerticesToMatrix(std::vector<float> vertices, float (*matrix)[4]) {

	int y = 0, x = 0;
	int count = 0;
	for (auto it = vertices.begin(); it != vertices.end(); it++)
	{
		if (count == 3) {
			matrix[y][x] = 1;
			y++;
			x = 0;
			count = 0;
		}

		matrix[y][x] = (*it);
		x++;
		count++;
	}
	if (x == 3)
	{
		matrix[y][x] = 1;
	}
}