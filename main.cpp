#include <iostream>
#include "includes/glad/glad.h"
#include <GLFW/glfw3.h>
#include <math.h>
#include "./includes/fileparser.hpp"
#include "./includes/shader.hpp"
#include "./includes/matrix.hpp"


const unsigned int SCREEN_WIDTH = 2500;
const unsigned int SCREEN_HEIGHT= 1580;
float FOCAL_POINT = 0.05;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	(void)window;
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		FOCAL_POINT += 0.05;
	}

	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		FOCAL_POINT -= 0.05;
	}
}




int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cout << "Pass a single object file with the executable! For example: ./Scop objects/42.obj" << std::endl; 
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

	// float identityMatrix[4][4] = {
	// 	{1, 0, 0, 0},
	// 	{0, 1, 0, 0},
	// 	{0, 0, 1, 0},
	// 	{0, 0, 0, 1}
	// };

	Matrix matrixCalculator = Matrix();

	int rows = objectFile.getVertices().size() / 3;
	float verticesMatrix[rows][4];
	matrixCalculator.convertVerticesToMatrix(objectFile.getVertices(), verticesMatrix);

	
	float aspectRatio = float(SCREEN_HEIGHT) / float(SCREEN_WIDTH);
	float zNear = 0.0;
	float zFar = 100.0;

	float orthographicMatrix[4][4] = {
   		{1.0, 0.0, 0.0, 0.0},
        {0.0, aspectRatio, 0.0, 0.0},
        {0.0, 0.0, float(1.0) / (zFar - zNear), -zNear / (zFar - zNear)},
        {0.0, 0.0, 0.0, 1.0},
	};

	for (int y = 0; y < rows; y++)
	{
		verticesMatrix[y][2] += 2;
		for (int x = 0; x < 2; x++)
			verticesMatrix[y][x] /= verticesMatrix[y][2] / FOCAL_POINT;
	}

	float result[rows][4];
	matrixCalculator.multiplyMatrix(rows, verticesMatrix, orthographicMatrix, result);

	std::cout << "result: " << std::endl;
	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			std::cout << result[y][x] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	Shader shader = Shader();

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);  
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glBufferData(GL_ARRAY_BUFFER, rows * 4 * sizeof(float), result, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	while(!glfwWindowShouldClose(window))
	{
        glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT);

		shader.useProgram();
		glDrawArrays(GL_LINE_LOOP, 0, rows);
		processInput(window);

    	glfwSwapBuffers(window);
    	glfwPollEvents();
	}

	glfwTerminate();
    return 0;
}

