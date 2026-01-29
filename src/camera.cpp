#include "../includes/camera.hpp"

Camera::Camera()
{
	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 3.0f;
	this->yRotation = -90.0f;
	this->xRotation = 0.0f;
	this->rotationSpeed = 80.0f;
	this->movementSpeed = 10.0f;
}

Camera::~Camera()
{
}

float Camera::getX() {
	return this->x;
}

float Camera::getY() {
	return this->y;
}

float Camera::getZ() {
	return this->z;
}

float Camera::getXRotation() {
	return this->xRotation;
}

float Camera::getYRotation() {
	return this->yRotation;
}

float Camera::getMovementSpeed() {
	return this->movementSpeed;
}

float Camera::getRotationSpeed() {
	return this->rotationSpeed;
}

void Camera::setX(float newX) {
	this->x = newX;
}

void Camera::setY(float newY) {
	this->y = newY;
}

void Camera::setZ(float newZ) {
	this->z = newZ;
}

void Camera::setXRotation(float newXRotation) {
	this->xRotation = newXRotation;
}

void Camera::setYRotation(float newYRotation) {
	this->yRotation = newYRotation;
}


std::ostream& operator<<(std::ostream &out, Camera &in) {
	out << "X = " << in.getX() << ", Y = " << in.getY() << ", Z = " << in.getZ() << std::endl;
	out << "xRotation = " << in.getXRotation() << ", yRotation = " << in.getYRotation() << std::endl;
	return out;
}