#include <vector>
#ifndef MATRIX_HPP
# define MATRIX_HPP

class Matrix
{
private:

public:
	Matrix();
	~Matrix();

	void multiplyMatrix(int rows, float (*matrixOne)[4], float (*matrixTwo)[4],  float (*matrixResult)[4]); 
	void convertVerticesToMatrix(std::vector<float> vertices, float (*matrix)[4]);
};



#endif