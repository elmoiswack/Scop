#include <iostream>
#include "includes/glad/glad.h"
#include <GLFW/glfw3.h>
#include "./includes/fileparser.hpp"
#include "./includes/shader.hpp"
#include <math.h>

const unsigned int SCREEN_WIDTH = 1920;
const unsigned int SCREEN_HEIGHT= 1080;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	(void)window;
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
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

	Shader shader = Shader();

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);  
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glBufferData(GL_ARRAY_BUFFER, objectFile.getVertices().size() * sizeof(float), objectFile.getVertices().data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	float tmpTestMatrix[4][4] = {
		{3.5, 1.0, 2.0, 3.3},
		{4.3, 1.3, 0.3, 2.4},
		{2.3, 5.2, 0.3, 3.4},
		{1.0, 2.3, 1.0, 1.2}
	};

	float identityMatrix[4][4] = {
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	};

	int y = 0, x = 0, incrementingFactor = 0;
	float result[4][4];

	while (y < 4) {

		x = 0;
		while (x < 4)
		{
			float resultNumber = 0;
			incrementingFactor = 0;
			while (incrementingFactor < 4) {
				
				resultNumber += (tmpTestMatrix[y][incrementingFactor] * identityMatrix[incrementingFactor][x]);
				incrementingFactor++;
			}
			result[y][x] = resultNumber;
			x++;
		}
		y++;
	}
	
	std::cout << "result : " << std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int y = 0; y < 4; y++)
		{
			std::cout << result[i][y] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "expected : " << std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int y = 0; y < 4; y++)
		{
			std::cout << tmpTestMatrix[i][y] << " ";
		}
		std::cout << std::endl;
	}



	while(!glfwWindowShouldClose(window))
	{
        glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT);

		shader.useProgram();
		glDrawArrays(GL_LINE_LOOP, 0, objectFile.getVertices().size());

		processInput(window);

    	glfwSwapBuffers(window);
    	glfwPollEvents();
	}

	glfwTerminate();
    return 0;
}

