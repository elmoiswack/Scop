#include <iostream>
#include "includes/glad/glad.h"
#include <GLFW/glfw3.h>
#include <math.h>
#include "./includes/fileparser.hpp"
#include "./includes/shader.hpp"
#include "./includes/camera.hpp"
#include "./includes/matrix.hpp"

const unsigned int SCREEN_WIDTH = 1920;
const unsigned int SCREEN_HEIGHT= 1080;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	(void)window;
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window, Camera& cam, Matrix& matrix, Shader& shader, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
	}

	float forward[3];
	matrix.getForward(forward, cam);
	
	float velocity = cam.getMovementSpeed() * deltaTime;

    float right[3] = {
        forward[2],
        0.0f,
        -forward[0]
    };

	bool change = false;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cam.setX(cam.getX() + (forward[0] * velocity));
        cam.setY(cam.getY() + (forward[1] * velocity));
        cam.setZ(cam.getZ() + (forward[2] * velocity));
		change = true;
	}
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cam.setX(cam.getX() - (forward[0] * velocity));
        cam.setY(cam.getY() - (forward[1] * velocity));
        cam.setZ(cam.getZ() - (forward[2] * velocity));
		change = true;
	}
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cam.setX(cam.getX() + (right[0] * velocity));
        cam.setZ(cam.getZ() + (right[2] * velocity));
		change = true;
	}
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cam.setX(cam.getX() - (right[0] * velocity));
        cam.setZ(cam.getZ() - (right[2] * velocity));
		change = true;
	}
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		float newRotation = cam.getXRotation() + (cam.getRotationSpeed() * deltaTime);
		if (newRotation > 89.0f)
			newRotation = 89.0f;
		cam.setXRotation(newRotation);
		change = true;
	}
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		float newRotation = cam.getXRotation() - (cam.getRotationSpeed() * deltaTime);
		if (newRotation < -89.0f)
			newRotation = -89.0f;
		cam.setXRotation(newRotation);
		change = true;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		float newRotation = cam.getYRotation() + (cam.getRotationSpeed() * deltaTime);
		if (newRotation > 360.0f)
			newRotation = -360.0f;
		cam.setYRotation(newRotation);
		change = true;
	}
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		float newRotation = cam.getYRotation() - (cam.getRotationSpeed() * deltaTime);
		if (newRotation < -360.0f)
			newRotation = 360.0f;
		cam.setYRotation(newRotation);
		change = true;
	}
	if (change == true) {
		matrix.buildViewMatrix(cam);
		shader.setUniformMatrix4x4(matrix.getViewMatrix(), "view");
		change = false;
	}

	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
		shader.changeMode();
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		matrix.setModelToX();
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
		matrix.setModelToY();
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		matrix.setModelToZ();
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		matrix.setModelToIdentity();
		shader.setUniformMatrix4x4(matrix.getModelMatrix(), "model");
	}

	if (matrix.getRotate() == true) {
		matrix.setAngle(1.2f);
		shader.setUniformMatrix4x4(matrix.getModelMatrix(), "model");
	}

}

void displayControls() {
	std::cout << "CONTROLS" << std::endl;
	std::cout << "--------------------------------" << std::endl;
	std::cout << "MOVEMENT:" << std::endl;
	std::cout << "W = forward" << std::endl;
	std::cout << "A = left" << std::endl;
	std::cout << "S = back" << std::endl;
	std::cout << "D = right" << std::endl;
	std::cout << "Arrow Up = look up" << std::endl;
	std::cout << "Arrow Left = look left" << std::endl;
	std::cout << "Arrow Down = look down" << std::endl;
	std::cout << "Arrow Right = look right" << std::endl;
	std::cout << "--------------------------------" << std::endl;
	std::cout << "ROTATION:" << std::endl;
	std::cout << "R + X = rotation x-axis" << std::endl;
	std::cout << "R + Y = rotation y-axis" << std::endl;
	std::cout << "R + Z = rotation z-axis" << std::endl;
	std::cout << "R + C = rotation clear" << std::endl;
	std::cout << "--------------------------------" << std::endl;
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
	Matrix matrix = Matrix(SCREEN_WIDTH, SCREEN_HEIGHT);

	matrix.buildViewMatrix(camera);

	shader.useProgram();

	shader.setUniformMatrix4x4(matrix.getModelMatrix(), "model");
	shader.setUniformMatrix4x4(matrix.getViewMatrix(), "view");
	shader.setUniformMatrix4x4(matrix.getPerspectiveMatrix(), "perspective");

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	shader.setUniform1i("tex", 0);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);  
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	objectFile.computeVertexes();

	glBufferData(GL_ARRAY_BUFFER, objectFile.getComputedVertex().size() * sizeof(ComputedVertex), objectFile.getComputedVertex().data(), GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ComputedVertex), (void*)offsetof(ComputedVertex, pos));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ComputedVertex), (void*)offsetof(ComputedVertex, texture));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(ComputedVertex), (void*)offsetof(ComputedVertex, normal));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	float lastFrame = 0.0f;
	size_t vertexCount = objectFile.getComputedVertex().size();
	
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	float color[3] = {1.0f, 1.0f, 1.0f};
	// int colorIndex = 0;
	// bool tmp = false;

	glClearColor(0.15f, 0.15f, 0.15f, 1.0f);

	displayControls();

	float lastTime = glfwGetTime();
	int frames = 0;

	while(!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		frames++;
		if (lastTime + 1 < currentFrame)
		{
			std::cout << "FPS = " << frames << std::endl;
			lastTime = currentFrame;
			frames = 0;
		}

		processInput(window, camera, matrix, shader, deltaTime);
		

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.useProgram();

		// std::cout << "current color index = " << colorIndex << std::endl;
		// std::cout << "current value = " << color[0] << ", " << color[1] << ", " << color[2] << std::endl;

		// if (tmp == false)
		// {
		// 	if (color[colorIndex] > 1.0f)
		// 	{
		// 		color[colorIndex] = 1.0f;
		// 		colorIndex += 1;
		// 	}
	
		// 	if (colorIndex == 3)
		// 	{
		// 		colorIndex -= 1;
		// 		tmp = true;
		// 	} else {
		// 		color[colorIndex] += 1.0f / 255.0f;
		// 	}
		// } else {
		// 	if (color[colorIndex] < 0.0f)
		// 	{
		// 		color[colorIndex] = 0.0f;
		// 		colorIndex -= 1;
		// 	}
	
		// 	if (colorIndex == -1)
		// 	{
		// 		colorIndex += 1;
		// 		tmp = false;
		// 	} else {
		// 		color[colorIndex] -= 1.0f / 255.0f;
		// 	}
		// }

		// if (tmp == false)
		// {
		// 	if (color[0] > 1.0f)
		// 	{
		// 		for (int i = 0; i < 3; i++)
		// 			color[i] = 1.0f;
		// 		tmp = true;
		// 	}
		// 	else
		// 	{
		// 		for (int i = 0; i < 3; i++)
		// 			color[i] += 0.01f;
		// 	}
		// } else {
		// 	if (color[0] < 0.0f)
		// 	{
		// 		for (int i = 0; i < 3; i++)
		// 			color[i] = 0.0f;
		// 		tmp = false;
		// 	}
		// 	else
		// 	{
		// 		for (int i = 0; i < 3; i++)
		// 			color[i] -= 0.01f;
		// 	}
		// }

		shader.setUniform3f("aColor", color);

		glBindVertexArray(VAO);
		glPolygonMode(GL_FRONT_AND_BACK, shader.getMode());
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);

    	glfwSwapBuffers(window);
    	glfwPollEvents();
	}

	glfwTerminate();
    return 0;
}