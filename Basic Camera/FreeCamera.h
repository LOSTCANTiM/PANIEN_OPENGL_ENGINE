#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

class Camera
{
public:
	glm::vec3 camPos;
	float camSpeed;

	glm::vec3 camOri = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);

	int cam_height;
	int cam_width;

	glm::mat4 view;
	glm::mat4 proj;

	GLFWwindow* cam_window;

	Camera(GLFWwindow* window, int width, int height, float speed, glm::vec3 camPosition)
	{
		cam_height = height;
		cam_width = width;
		camPos = camPosition;
		cam_window = window;
		camSpeed = speed;
	}

	void createCamera(float FOV, Shader shader, float clipRangeMin = 0.1f, float clipRangeMax = 100.0f)
	{
		freeCamMovement();
		view = glm::lookAt(camPos, camPos + camOri, camUp);
		proj = glm::perspective(glm::radians(FOV), (float)(cam_width / cam_height), clipRangeMin, clipRangeMax);

		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "pers"), 1, GL_FALSE, glm::value_ptr(proj * view));
	}

private:
	void freeCamMovement()
	{
		GLFWwindow* window = cam_window;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camPos += camSpeed * camOri;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camPos -= camSpeed * camOri;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camPos -= glm::normalize(glm::cross(camOri, camUp)) * camSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camPos += glm::normalize(glm::cross(camOri, camUp)) * camSpeed;
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			camPos += camSpeed * camUp;
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			camPos -= camSpeed * camUp;
	}
};

#endif