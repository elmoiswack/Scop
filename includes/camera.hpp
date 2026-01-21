#include <iostream>

class Camera
{
private:
	float x, y, z;
	float yRotation, xRotation;
    float movementSpeed, rotationSpeed;
public:
	Camera();
	~Camera();

	float getX();
	float getY();
	float getZ();
	float getXRotation();
	float getYRotation();
	float getMovementSpeed();
	float getRotationSpeed();

	void setX(float newX);
	void setY(float newZ);
	void setZ(float newZ);
	void setXRotation(float newXRotation);
	void setYRotation(float newYRotation);

	friend std::ostream& operator<<(std::ostream &out, Camera &in);

};


