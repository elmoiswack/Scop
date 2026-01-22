#ifndef MATRIX_HPP
# define MATRIX_HPP

#include <math.h>
#include "./camera.hpp"

class Matrix
{
private:
	float model[16] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1	
	};
	float perspective[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	float view[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 

public:
	Matrix(const unsigned int width, const unsigned int height);
	~Matrix();

	void getForward(float *forward, Camera& cam);
	void buildViewMatrix(Camera& cam);

	float* getModelMatrix();
	float* getPerspectiveMatrix();
	float* getViewMatrix();
};



#endif