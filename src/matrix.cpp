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

void Matrix::normalize(float v[3])
{
    float len = std::sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    if (len == 0.0f) return;
    v[0] /= len;
    v[1] /= len;
    v[2] /= len;
}

void Matrix::cross(float out[3], const float a[3], const float b[3])
{
    out[0] = a[1]*b[2] - a[2]*b[1];
    out[1] = a[2]*b[0] - a[0]*b[2];
    out[2] = a[0]*b[1] - a[1]*b[0];
}

float Matrix::dot(const float a[3], const float b[3])
{
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

void Matrix::buildViewMatrix(Camera& cam)
{
    float eye[3] = { cam.getX(), cam.getY(), cam.getZ() };
    float up[3]  = { 0.0f, 1.0f, 0.0f };

    float forward[3];
    this->getForward(forward, cam);

    float f[3] = {
        forward[0],
        forward[1],
        forward[2]
    };
    this->normalize(f);

    float s[3];
    this->cross(s, f, up);
    this->normalize(s);

    float u[3];
    this->cross(u, s, f);

    view[0]  =  s[0];
    view[1]  =  u[0];
    view[2]  = -f[0];
    view[3]  =  0.0f;

    view[4]  =  s[1];
    view[5]  =  u[1];
    view[6]  = -f[1];
    view[7]  =  0.0f;

    view[8]  =  s[2];
    view[9]  =  u[2];
    view[10] = -f[2];
    view[11] =  0.0f;

    view[12] = -dot(s, eye);
    view[13] = -dot(u, eye);
    view[14] =  dot(f, eye);
    view[15] =  1.0f;
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