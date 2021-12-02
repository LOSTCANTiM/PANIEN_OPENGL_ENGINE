#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "FreeCamera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 800;

void resizeViewport(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main()
{
	if (!glfwInit())
		return -1;
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Learning", NULL, NULL);
	if (!window)
		return -1;

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		return -1;
	gladLoadGL();

	glfwSetFramebufferSizeCallback(window, resizeViewport);

	GLfloat vertices[] = {
		0.5f, 0.5f, 0.5f,			1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f,			0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f,			0.0f, 0.0f,
		0.5f, -0.5f, 0.5f,			1.0f, 0.0f,			//front face

		-0.5f, 0.5f, 0.5f,			0.0f, 0.0f,
		0.5f, 0.5f, 0.5f,			1.0f, 0.0f,
		0.5f, 0.5f, -0.5f,			1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f,			0.0f, 1.0f,			//top face

		0.5f, 0.5f, -0.5f,			1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f,			0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,		0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,			1.0f, 0.0f,			//back face

		-0.5f, -0.5f, 0.5f,			0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,		0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,			1.0f, 1.0f,
		0.5f, -0.5f, 0.5f,			1.0f, 0.0f,			//bottom face	

		0.5f, -0.5f, 0.5f,			0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,			1.0f, 0.0f,
		0.5f, 0.5f, -0.5f,			1.0f, 1.0f,
		0.5f, 0.5f, 0.5f,			0.0f, 1.0f,			//right face

		-0.5f, -0.5f, 0.5f,			0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,		1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f,			1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f,			0.0f, 1.0f			//left face
	};

	GLuint indices[] = {
		0, 1, 2,
		2, 3, 0,

		4, 5, 6,
		6, 7, 4,

		8, 9, 10,
		10, 11, 8,

		12, 13, 14,
		14, 15, 12,

		16, 17, 18,
		18, 19, 16,

		20, 21, 22,
		22, 23, 20
	};

	Shader Shader("default.vert.fs", "default.frag.fs");

	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	unsigned int texture;
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(VAO);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("textr.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(data);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 3.0f);

	Camera camera(WIDTH, HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));

	glEnable(GL_DEPTH_TEST);
	Shader.activate();
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.12f, 1.0f);

		if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		{
			glm::mat4 model = glm::mat4(1.0f);
			glUniformMatrix4fv(glGetUniformLocation(Shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		}
		camera.Inputs(window);
		camera.Matrix(45.0f, 0.1f, 100.0f, Shader, "pers");

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		Shader.activate();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteTextures(1, &texture);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
