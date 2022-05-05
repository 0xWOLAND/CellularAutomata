#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include "Texture.h"
#include <vector>
#include "Camera.h"

const int width = 800;
const int height = 800;

GLfloat vertices[] =
{
	-1.0f, -1.0f, 1.0f,	 0.5f, 0.5f,
	 1.0f, -1.0f, 1.0f,	 0.0f, 0.5f,
	-1.0f,  1.0f, 1.0f,	 0.5f, 0.0f,
	 1.0f,  1.0f, 1.0f,  0.0f, 0.0f
};

GLuint indices[] =
{
	0, 1, 3, 0,
	2, 3, 0
};

const float dimension = 2.0f;

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "CA", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();
	glViewport(0, 0, width, height);



	Shader shaderProgram("default.vert", "default.frag");

	float data[] = {
		0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,

	};

	Texture t(data, int(dimension), int(dimension) * int(dimension));
	t.BindTexture();
	t.applyTexture();

	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)(0));
	VAO1.LinkAttrib(VBO1, 1, 2, GL_FLOAT, int(dimension) * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();


	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 1.0f));
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shaderProgram.Activate();

		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");
		glUniform1f(glGetUniformLocation(shaderProgram.ID, "width"), dimension);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "cameraPosition"), camera.Position.x, camera.Position.y, camera.Position.z);
		glUniform2f(glGetUniformLocation(shaderProgram.ID, "u_resolution"), width, height);
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, 7, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}



	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}