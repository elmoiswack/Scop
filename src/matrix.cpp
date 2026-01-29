#include "../includes/matrix.hpp"
#include <string.h>

Matrix::Matrix(const unsigned int width, const unsigned int height) {
	float aspectRatio = float(width) / float(height);
	float zNear = 0.01f;
	float zFar = 100.0f;
	float fov = 45.0f * (3.1415926f / 180.0f);
	float f = 1.0f / tan(fov / 2.0f);

	this->perspective[0]  = f / aspectRatio;
	this->perspective[5]  = f;
	this->perspective[10] = (zFar + zNear) / (zNear - zFar);
	this->perspective[11] = -1.0f;
	this->perspective[14] = (2 * zFar * zNear) / (zNear - zFar);

	this->angle = 0.00f;
	this->rotate = false;
	this->whichRotation = Rotation::NONE;
}

Matrix::~Matrix() {}

void Matrix::getForward(float *forward, Camera& cam) {
    float yRad = cam.getYRotation() * (3.1415926f / 180.0f);
    float xRad = cam.getXRotation() * (3.1415926f / 180.0f);

    forward[0] = cosf(yRad) * cosf(xRad);
    forward[1] = sinf(xRad);
    forward[2] = sinf(yRad) * cosf(xRad);
}

void Matrix::buildViewMatrix(Camera& cam) {
	float forward[3];
	this->getForward(forward, cam);

	float eye[3] = {cam.getX(), cam.getY(), cam.getZ()};
	float center[3] = {cam.getX() + forward[0], cam.getY() + forward[1], cam.getZ() + forward[2]};
	float up[3] = {0.0f, 1.0f, 0.0f};

	float ftmp[3] = {center[0] - eye[0], center[1] - eye[1], center[2] - eye[2]};
	float flen = sqrtf(ftmp[0]*ftmp[0] + ftmp[1]*ftmp[1] + ftmp[2]*ftmp[2]);
	float f[3] = {ftmp[0]/flen, ftmp[1]/flen, ftmp[2]/flen};

	float stmp[3] = {(f[1] * up[2]) - (f[2] * up[1]), (f[2] * up[0]) - (f[0] * up[2]), (f[0] * up[1]) - (f[1] * up[0])};
	float slen = sqrtf(stmp[0]*stmp[0] + stmp[1]*stmp[1] + stmp[2]*stmp[2]);
	float s[3]= {stmp[0]/slen, stmp[1]/slen, stmp[2]/slen};

	float u[3] = {(s[1] * f[2]) - (s[2] * f[1]), (s[2] * f[0]) - (s[0] * f[2]), (s[0] * f[1]) - (s[1] * f[0])};

    this->view[0]  = s[0];
    this->view[1]  = u[0];
    this->view[2]  = -f[0];

    this->view[4]  = s[1];
    this->view[5]  = u[1];
    this->view[6]  = -f[1];

    this->view[8]  = s[2];
    this->view[9]  = u[2];
    this->view[10] = -f[2];

	this->view[12] = -(s[0]*eye[0] + s[1]*eye[1] + s[2]*eye[2]);
	this->view[13] = -(u[0]*eye[0] + u[1]*eye[1] + u[2]*eye[2]);
	this->view[14] = (f[0]*eye[0] + f[1]*eye[1] + f[2]*eye[2]);
	this->view[15] = 1.0f;
}

void Matrix::setModelToIdentity() {
	this->rotate = false;
	this->whichRotation = Rotation::NONE;
	this->angle = 0.00f;
	memcpy(this->model, this->identity, 16 * sizeof(float));
}

void Matrix::setModelToX() {
	this->rotate = true;
	this->whichRotation = Rotation::X;
}

void Matrix::setModelToY() {
	this->rotate = true;
	this->whichRotation = Rotation::Y;
}

void Matrix::setModelToZ() {
	this->rotate = true;
	this->whichRotation = Rotation::Z;
}

void Matrix::setModelToCrazy() {
	this->rotate = true;
	this->whichRotation = Rotation::CRAZY;
}

void  Matrix::computeModelToX() {
	memcpy(this->model, this->identity, 16 * sizeof(float));
	this->model[5] = cos(this->angle);
	this->model[6] = -sin(this->angle);
	this->model[9] = sin(this->angle);
	this->model[10] = cos(this->angle);
}

void  Matrix::computeModelToY() {
	memcpy(this->model, this->identity, 16 * sizeof(float));
	this->model[0] = cos(this->angle);
	this->model[2] = sin(this->angle);
	this->model[8] = -sin(this->angle);
	this->model[10] = cos(this->angle);
}

void  Matrix::computeModelToZ() {
	memcpy(this->model, this->identity, 16 * sizeof(float));
	this->model[0] = cos(this->angle);
	this->model[1] = -sin(this->angle);
	this->model[4] = sin(this->angle);
	this->model[5] = cos(this->angle);
}

void  Matrix::computeModelToCrazy() {
	memcpy(this->model, this->identity, 16 * sizeof(float));
	this->model[3] = cos(this->angle);
	this->model[7] = -sin(this->angle);
	this->model[2] = sin(this->angle);
	this->model[1] = -cos(this->angle);
	this->model[10] = cos(this->angle);
}

void Matrix::setAngle(float rotationSpeed) {
	if ((this->angle + rotationSpeed) > 359.9f)
		this->angle = 0.00f;
	this->angle += rotationSpeed;

	if (this->whichRotation == Rotation::X)
		this->computeModelToX();
	else if (this->whichRotation == Rotation::Y)
		this->computeModelToY();
	else if (this->whichRotation == Rotation::Z)
		this->computeModelToZ();
	else if (this->whichRotation == Rotation::CRAZY)
		this->computeModelToCrazy();
}

float* Matrix::getModelMatrix() {
	return this->model;
}

float* Matrix::getPerspectiveMatrix() {
	return this->perspective;
}

float* Matrix::getViewMatrix() {
	return this->view;
}

bool Matrix::getRotate() {
	return this->rotate;
}