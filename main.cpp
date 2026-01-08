#include <iostream>
#include "includes/glad/glad.h"
#include <GLFW/glfw3.h>
#include <math.h>
#include "./includes/fileparser.hpp"
#include "./includes/shader.hpp"
#include "./includes/matrix.hpp"


const unsigned int SCREEN_WIDTH = 2500;
const unsigned int SCREEN_HEIGHT= 1580;

enum VECTOR_INDEX {
	X = 0,
	Y = 1,
	Z = 2,
	W = 3
};

float FOCAL_POINT = 0.05;
float MOVEMENT_FACTOR = 0.01;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	(void)window;
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window, int rows, float (*result)[4]) //TODO: movement should increment or decrement a single vec pos, not inside the result matrix
{
	//ESC = close
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
	}

	//A = left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        for (int y = 0; y < rows; y++)
		{
			result[y][VECTOR_INDEX::X] += MOVEMENT_FACTOR;
		}
	}
	//D = right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        for (int y = 0; y < rows; y++)
		{
			result[y][VECTOR_INDEX::X] -= MOVEMENT_FACTOR;
		}
	}
	//W = up
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        for (int y = 0; y < rows; y++)
		{
			result[y][VECTOR_INDEX::Y] -= MOVEMENT_FACTOR;
		}
	}
	//S = down
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        for (int y = 0; y < rows; y++)
		{
			result[y][VECTOR_INDEX::Y] += MOVEMENT_FACTOR;
		}
	}

	//Arrow Up = zoom in
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        for (int y = 0; y < rows; y++)
		{
			result[y][VECTOR_INDEX::W] -= MOVEMENT_FACTOR;
		}
	}
	//Arrow Down = zoom out
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        for (int y = 0; y < rows; y++)
		{
			result[y][VECTOR_INDEX::W] += MOVEMENT_FACTOR;
		}
	}		
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
	
	// float aspectRatio = float(SCREEN_HEIGHT) / float(SCREEN_WIDTH);
	// float zNear = 0.0;
	// float zFar = 100.0;

	// float orthographicMatrix[4][4] = {
   	// 	{1.0, 0.0, 0.0, 0.0},
    //     {0.0, aspectRatio, 0.0, 0.0},
    //     {0.0, 0.0, float(1.0) / (zFar - zNear), -zNear / (zFar - zNear)},
    //     {0.0, 0.0, 0.0, 1.0},
	// };

	// for (int y = 0; y < rows; y++)
	// {
	// 	verticesMatrix[y][2] += 2;
	// 	for (int x = 0; x < 2; x++)
	// 		verticesMatrix[y][x] /= verticesMatrix[y][2] / FOCAL_POINT;
	// }

	float aspectRatio = float(SCREEN_HEIGHT) / float(SCREEN_WIDTH);
	float zNear = 0.1;
	float zFar = 100.0;
	float fov = 1 / tan(45.0 / 2);

	// float projectionMatrix[4][4] = {
   	// 	{aspectRatio * (1.0 / tan(fov / 2)), 0.0, 0.0, 0.0},
    //     {0.0, (1.0 / tan(fov / 2)), 0.0, 0.0},
    //     {0.0, 0.0, zFar / (zFar - zNear), (-zFar * zNear) / (zFar - zNear)},
    //     {0.0, 0.0, -1.0, 0.0},
	// };

	float tmp[4][4] = {
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
	};

    tmp[0][0] = fov / aspectRatio;
    tmp[1][1] = fov;
    tmp[2][2] = -(zFar + zNear) / (zFar - zNear);
    tmp[2][3] = -1.0f;
    tmp[3][2] = -(2 * zFar * zNear) / (zFar - zNear);

	// for (int y = 0; y < rows; y++)
	// {
	// 	for (int x = 0; x < 2; x++)
	// 		verticesMatrix[y][x] = verticesMatrix[y][x] / (verticesMatrix[y][2] / FOCAL_POINT);
	// }

	float result[rows][4];
	matrixCalculator.multiplyMatrix(rows, verticesMatrix, tmp, result);

	std::cout << "result: " << std::endl;
	for (int y = 0; y < rows; y++)
	{
		// if (result[rows][VECTOR_INDEX::W] != 0.0)
		// {
		// 	result[rows][VECTOR_INDEX::X] /= result[rows][VECTOR_INDEX::W];
		// 	result[rows][VECTOR_INDEX::Y] /= result[rows][VECTOR_INDEX::W];
		// 	result[rows][VECTOR_INDEX::Z] /= result[rows][VECTOR_INDEX::W];
		// }
		for (int i = 0; i < 4; i++)
		{
			std::cout  << result[y][i] << " ";
		}
		std::cout << std::endl;
	}

	Shader shader = Shader();

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);  
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	


	while(!glfwWindowShouldClose(window))
	{

		glBufferData(GL_ARRAY_BUFFER, rows * 4 * sizeof(float), result, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
        glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT);

		shader.useProgram();
		glDrawArrays(GL_LINE_LOOP, 0, rows);
		processInput(window, rows, result);

    	glfwSwapBuffers(window);
    	glfwPollEvents();
	}

	glfwTerminate();
    return 0;
}

// -nan -nan -nan -nan 
// -0.939241 0.0380174 -0.349499 -0.149 
// -0.456578 0.0162798 -0.496993 -0.2962 
// -0.289741 0.00897928 -0.64028 -0.4392 
// -0.201879 0.00539805 -0.77585 -0.5745 
// -0.14556 0.00334055 -0.899197 -0.6976 
// -0.104957 0.00207429 -1.00551 -0.8037 
// -0.0730795 0.00125046 -1.09078 -0.8888 
// -0.0463429 0.000707229 -1.1526 -0.9505 
// -0.022502 0.000317642 -1.18978 -0.9876 