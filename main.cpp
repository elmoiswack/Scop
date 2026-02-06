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

void processInput(GLFWwindow *window, Camera& cam, Matrix& matrix, Shader& shader, bool textureAvailable, float deltaTime)
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
	if (change) {
		matrix.buildViewMatrix(cam);
		shader.setUniformMatrix4x4(matrix.getViewMatrix(), "view");
		change = false;
	}

	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
		shader.changeMode();
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		shader.setUniform1i("rotationBool", 1);
		matrix.setModelToX();
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
		shader.setUniform1i("rotationBool", 1);
		matrix.setModelToY();
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		shader.setUniform1i("rotationBool", 1);
		matrix.setModelToZ();
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		shader.setUniform1i("rotationBool", 1);
		matrix.setModelToCrazy();
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
		shader.setUniform1i("rotationBool", 0);
		matrix.setModelToIdentity();
		shader.setUniformMatrix4x4(matrix.getModelMatrix(), "model");
	}
	if (matrix.getRotate() == true) {
		matrix.setAngle(1.2f);
		shader.setUniformMatrix4x4(matrix.getModelMatrix(), "model");
	}

	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		shader.setUniform1i("colorPerFace", 1);
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
		shader.setUniform1i("colorPerFace", 0);
		shader.setColor((float[]){1.0f, 1.0f, 1.0f});
		shader.setUniform3f("aColor", shader.getColor());
		shader.setShowTexture(false);
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		shader.setUniform1i("colorPerFace", 0);
		shader.setColor((float[]){1.0f, 0.0f, 0.0f});
		shader.setUniform3f("aColor", shader.getColor());
		shader.setShowTexture(false);
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
		shader.setUniform1i("colorPerFace", 0);
		shader.setColor((float[]){0.0f, 1.0f, 0.0f});
		shader.setUniform3f("aColor", shader.getColor());
		shader.setShowTexture(false);
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
		shader.setUniform1i("colorPerFace", 0);
		shader.setColor((float[]){0.0f, 0.0f, 1.0f});
		shader.setUniform3f("aColor", shader.getColor());
		shader.setShowTexture(false);
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
		shader.setShowTexture(false);
		if (shader.getBreatheEffect() == true)
			shader.setBreatheEffect(false);
		else
			shader.setBreatheEffect(true);
	}

	if (shader.getFinishedApplyTexture() == true && glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && textureAvailable == true) {
		shader.setFinishedApplyTexture(false);
		if (shader.getTextureOpacity() == 0.0f)
			shader.setShowTexture(true);
		else
			shader.setShowTexture(false);
	}

	if (shader.getShowTexture() == true && shader.getFinishedApplyTexture() == false) {
		shader.incrementTextureOpacity(deltaTime);
		shader.setUniform1f("textureOpacity", shader.getTextureOpacity());
	}
	if (shader.getShowTexture() == false && shader.getFinishedApplyTexture() == false) {
		shader.decrementTextureOpacity(deltaTime);
		shader.setUniform1f("textureOpacity", shader.getTextureOpacity());
	}

	if (shader.getBreatheEffect() == true) {
		if (shader.getIncrementColorValues() == true)
			shader.incrementColor();
		else
			shader.decrementColor();
		shader.setUniform3f("aColor", shader.getColor());
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
	std::cout << "MODES:" << std::endl;
	std::cout << "M = change draw mode" << std::endl;
	std::cout << "--------------------------------" << std::endl;
	std::cout << "ROTATION:" << std::endl;
	std::cout << "R + X = rotation x-axis" << std::endl;
	std::cout << "R + Y = rotation y-axis" << std::endl;
	std::cout << "R + Z = rotation z-axis" << std::endl;
	std::cout << "R + I = rotation i don't know" << std::endl;
	std::cout << "R + N = rotation none" << std::endl;
	std::cout << "--------------------------------" << std::endl;
	std::cout << "COLOR:" << std::endl;
	std::cout << "C + F = color per face" << std::endl;
	std::cout << "C + V = color per vertex" << std::endl;
	std::cout << "C + R = color red" << std::endl;
	std::cout << "C + G = color green" << std::endl;
	std::cout << "C + B = color blue" << std::endl;
	std::cout << "C + H = color breathe effect" << std::endl;
	std::cout << "C + T = apply texture" << std::endl;
	std::cout << "--------------------------------" << std::endl;
}

int main(int argc, char *argv[])
{
	if (argc != 2 && argc != 3)
	{
		std::cout << "Pass a single object file with the executable! For example: ./Scop models/42.obj" << std::endl; 
		return 1;
	}

	FileParser objectFile(argc == 2 ? FileParser(argv[1]) : FileParser(argv[1], argv[2]));
    
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
		glfwTerminate();
		return -1;
	}    

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	Shader shader = Shader();
	Camera camera = Camera();
	Matrix matrix = Matrix(SCREEN_WIDTH, SCREEN_HEIGHT);

	unsigned int textureID;
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, objectFile.getTextureWidth(), objectFile.getTextureHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, objectFile.getTextureData());
	glGenerateMipmap(GL_TEXTURE_2D);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);  
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glClearColor(0.15f, 0.15f, 0.15f, 1.0f);

	displayControls();

	bool texture = objectFile.getTextureAvailable();
	
	matrix.buildViewMatrix(camera);

	objectFile.computeCenterVertex();
	objectFile.computeVertexes();

	shader.useProgram();
	shader.setUniformMatrix4x4(matrix.getModelMatrix(), "model");
	shader.setUniformMatrix4x4(matrix.getViewMatrix(), "view");
	shader.setUniformMatrix4x4(matrix.getPerspectiveMatrix(), "perspective");
	shader.setUniform1i("ourTexture", 0);
	shader.setUniform3f("aColor", shader.getColor());
	shader.setUniform1i("colorPerFace", 0);
	shader.setUniform3f("centerPos", objectFile.getCenterVertex());
	shader.setUniform1i("rotationBool", 0);
	shader.setUniform1f("textureOpacity", shader.getTextureOpacity());

	size_t vertexCount = objectFile.getComputedVertex().size();
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(ComputedVertex), objectFile.getComputedVertex().data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ComputedVertex), (void*)offsetof(ComputedVertex, pos));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ComputedVertex), (void*)offsetof(ComputedVertex, texture));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(ComputedVertex), (void*)offsetof(ComputedVertex, color));
		
	float lastFrame = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window, camera, matrix, shader, texture, deltaTime);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, textureID);
		glBindVertexArray(VAO);
		glPolygonMode(GL_FRONT_AND_BACK, shader.getMode());
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);

    	glfwSwapBuffers(window);
    	glfwPollEvents();
	}

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
	glfwTerminate();
    
	return 0;
}