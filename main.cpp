#include <iostream>
#include "includes/glad/glad.h"
#include <GLFW/glfw3.h>
#include <math.h>
#include "./includes/fileparser.hpp"
#include "./includes/shader.hpp"
#include "./includes/camera.hpp"


const unsigned int SCREEN_WIDTH = 2500;
const unsigned int SCREEN_HEIGHT= 1580;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	(void)window;
    glViewport(0, 0, width, height);
}

void getForward(float *forward, Camera& cam) {
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

void processInput(GLFWwindow *window, Camera& cam, float deltaTime) //TODO: movement should increment or decrement a single vec pos, not inside the result matrix
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
	}

	float forward[3];
	getForward(forward, cam);
	
	float velocity = cam.getMovementSpeed() * deltaTime;

    float right[3] = {
        forward[2],
        0.0f,
        -forward[0]
    };

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cam.setX(cam.getX() + (forward[0] * velocity));
        cam.setY(cam.getY() + (forward[1] * velocity));
        cam.setZ(cam.getZ() + (forward[2] * velocity));
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cam.setX(cam.getX() - (forward[0] * velocity));
        cam.setY(cam.getY() - (forward[1] * velocity));
        cam.setZ(cam.getZ() - (forward[2] * velocity));
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cam.setX(cam.getX() + (right[0] * velocity));
        cam.setZ(cam.getZ() + (right[2] * velocity));
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cam.setX(cam.getX() - (right[0] * velocity));
        cam.setZ(cam.getZ() - (right[2] * velocity));
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		float newRotation = cam.getXRotation() + (cam.getRotationSpeed() * deltaTime);
		if (newRotation > 90.0f)
			newRotation = 90.0f;
		cam.setXRotation(newRotation);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		float newRotation = cam.getXRotation() - (cam.getRotationSpeed() * deltaTime);
		if (newRotation < -90.0f)
			newRotation = -90.0f;
		cam.setXRotation(newRotation);
    }
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		float newRotation = cam.getYRotation() + (cam.getRotationSpeed() * deltaTime);
		if (newRotation > 360.0f)
			newRotation = -360.0f;
		cam.setYRotation(newRotation);
	}
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		float newRotation = cam.getYRotation() - (cam.getRotationSpeed() * deltaTime);
		if (newRotation < -360.0f)
			newRotation = 360.0f;
		cam.setYRotation(newRotation);
	}
}

void buildViewMatrix(float* view, Camera& cam) {
	float forward[3];
	getForward(forward, cam);

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

    view[0]  = s[0];
    view[1]  = u[0];
    view[2]  = -f[0];

    view[4]  = s[1];
    view[5]  = u[1];
    view[6]  = -f[1];

    view[8]  = s[2];
    view[9]  = u[2];
    view[10] = -f[2];

	view[12] = -((s[0]*eye[0]) + (s[1]*eye[1]) + (s[2]*eye[2]));
    view[13] = -((u[0]*eye[0]) + (u[1]*eye[1]) + (u[2]*eye[2]));
    view[14] = ((f[0]*eye[0]) + (f[1]*eye[1]) + (f[2]*eye[2]));
    view[15] = 1;
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cout << "Pass a single object file with the executable! For example: ./Scop models/42.obj" << std::endl; 
		return 1;
	}

	FileParser objectFile = FileParser(argv[1]);

    if (!glfwInit()) {
		std::cout << "Failed to init glfw for window creation!" << std::endl;
		return 1;
	};

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "SCOP", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}    

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	Shader shader = Shader();
	Camera camera = Camera();
	objectFile.computeVertexes();

	float aspectRatio = float(SCREEN_HEIGHT) / float(SCREEN_WIDTH);
	float zNear = -1.0;
	float zFar = 1.0;
	float fov = 1 / tan(45.0 / 2);

	float identity[16] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1	
	};
	shader.setUniformMatrix4x4(identity, "model");

	float perspective[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    perspective[0] = fov / aspectRatio;
    perspective[5] = fov;
    perspective[10] = (zFar + zNear) / (zNear - zFar);
    perspective[11] = -1.0f;
    perspective[14] = -(2 * zFar * zNear) / (zFar - zNear);

	float view[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	buildViewMatrix(view, camera);

	shader.useProgram();
	shader.setUniformMatrix4x4(perspective, "perspective");

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);  
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, objectFile.getComputedVertex().size() * sizeof(ComputedVertex), objectFile.getComputedVertex().data(), GL_DYNAMIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ComputedVertex), (void*)offsetof(ComputedVertex, pos));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ComputedVertex), (void*)offsetof(ComputedVertex, texture));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(ComputedVertex), (void*)offsetof(ComputedVertex, normal));
	
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	float lastFrame = 0.0f;
	while(!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window, camera, deltaTime);

		buildViewMatrix(view, camera);

		shader.useProgram();
		shader.setUniformMatrix4x4(view, "view");

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, objectFile.getComputedVertex().size());

    	glfwSwapBuffers(window);
    	glfwPollEvents();
	}

	glfwTerminate();
    return 0;
}