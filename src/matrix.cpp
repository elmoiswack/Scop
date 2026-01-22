#include "../includes/matrix.hpp"

Matrix::Matrix(const unsigned int width, const unsigned int height)
{
	float aspectRatio = float(width) / float(height);
	float zNear = 0.1f;
	float zFar = 100.0f;
	float fov = 45.0f * (3.1415926f / 180.0f);
	float f = 1.0f / tan(fov / 2.0f);

	perspective[0]  = f / aspectRatio;
	perspective[5]  = f;
	perspective[10] = (zFar + zNear) / (zNear - zFar);
	perspective[11] = -1.0f;
	perspective[14] = (2 * zFar * zNear) / (zNear - zFar);
}

Matrix::~Matrix()
{
}

void Matrix::getForward(float *forward, Camera& cam) {
    float yawRad = cam.getYRotation() * (3.1415926f / 180.0f);
    float pitchRad = cam.getXRotation() * (3.1415926f / 180.0f);

    forward[0] = cosf(yawRad) * cosf(pitchRad);
    forward[1] = sinf(pitchRad);
    forward[2] = sinf(yawRad) * cosf(pitchRad);

    float len = sqrtf(forward[0]*forward[0] + forward[1]*forward[1] + forward[2]*forward[2]);
    forward[0] /= len;
    forward[1] /= len;
    forward[2] /= len;
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
	this->view[14] = -(f[0]*eye[0] + f[1]*eye[1] + f[2]*eye[2]);
	this->view[15] = 1.0f;
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