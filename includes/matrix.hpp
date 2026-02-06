#ifndef MATRIX_HPP
# define MATRIX_HPP

#include <math.h>
#include "./camera.hpp"

enum Rotation {
	X,
	Y,
	Z,
	CRAZY,
	NONE
};

class Matrix
{
private:
	float angle;
	bool rotate;
	float model[16]  = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	Rotation whichRotation;

	float identity[16] = {
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
	void normalize(float v[3]);
	void cross(float out[3], const float a[3], const float b[3]);
	float dot(const float a[3], const float b[3]);
	void buildViewMatrix(Camera& cam);

	float degreesToRadians(float degrees);
	void setModelToIdentity();
	void setModelToX();
	void setModelToY();
	void setModelToZ();
	void setModelToCrazy();
	void computeModelToX();
	void computeModelToY();
	void computeModelToZ();
	void computeModelToCrazy();

	void setAngle(float rotationSpeed);

	float* getModelMatrix();
	float* getPerspectiveMatrix();
	float* getViewMatrix();
	bool getRotate();
};



#endif