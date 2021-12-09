#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Engine.h"

#include <iostream>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 0.5f, 5.0f), 45.0f, 0.1f, 100.0f);
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

void wireframeTickCallback(bool tick);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
	gladLoadGL();
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader lightingShader("main.vert.fs", "main.frag.fs");
    Shader lightCubeShader("light.vert.fs", "light.frag.fs");

    float vertices[] = {
	    -0.5f, -0.5f, -0.5f,		0.0f,  0.0f, -1.0f,
	     0.5f, -0.5f, -0.5f,		0.0f,  0.0f, -1.0f, 
	     0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f, 
	     0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f, 
	    -0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f, 
	    -0.5f, -0.5f, -0.5f,		0.0f,  0.0f, -1.0f, 

	    -0.5f, -0.5f,  0.5f,		0.0f,  0.0f, 1.0f,
	     0.5f, -0.5f,  0.5f,		0.0f,  0.0f, 1.0f,
	     0.5f,  0.5f,  0.5f,		0.0f,  0.0f, 1.0f,
	     0.5f,  0.5f,  0.5f,		0.0f,  0.0f, 1.0f,
	    -0.5f,  0.5f,  0.5f,		0.0f,  0.0f, 1.0f,
	    -0.5f, -0.5f,  0.5f,		0.0f,  0.0f, 1.0f,

	    -0.5f,  0.5f,  0.5f,		1.0f,  0.0f,  0.0f,
	    -0.5f,  0.5f, -0.5f,		1.0f,  0.0f,  0.0f,
	    -0.5f, -0.5f, -0.5f,		1.0f,  0.0f,  0.0f,
	    -0.5f, -0.5f, -0.5f,		1.0f,  0.0f,  0.0f,
	    -0.5f, -0.5f,  0.5f,		1.0f,  0.0f,  0.0f,
	    -0.5f,  0.5f,  0.5f,		1.0f,  0.0f,  0.0f,

	     0.5f,  0.5f,  0.5f,		1.0f,  0.0f,  0.0f,
	     0.5f,  0.5f, -0.5f,		1.0f,  0.0f,  0.0f,
	     0.5f, -0.5f, -0.5f,		1.0f,  0.0f,  0.0f,
	     0.5f, -0.5f, -0.5f,		1.0f,  0.0f,  0.0f,
	     0.5f, -0.5f,  0.5f,		1.0f,  0.0f,  0.0f,
	     0.5f,  0.5f,  0.5f,		1.0f,  0.0f,  0.0f,

	    -0.5f, -0.5f, -0.5f,		0.0f, -1.0f,  0.0f,
	     0.5f, -0.5f, -0.5f,		0.0f, -1.0f,  0.0f,
	     0.5f, -0.5f,  0.5f,		0.0f, -1.0f,  0.0f,
	     0.5f, -0.5f,  0.5f,		0.0f, -1.0f,  0.0f,
	    -0.5f, -0.5f,  0.5f,		0.0f, -1.0f,  0.0f,
	    -0.5f, -0.5f, -0.5f,		0.0f, -1.0f,  0.0f,

	    -0.5f,  0.5f, -0.5f,		0.0f,  1.0f,  0.0f,
	     0.5f,  0.5f, -0.5f,		0.0f,  1.0f,  0.0f,
	     0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,
	     0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,
	    -0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,
	    -0.5f,  0.5f, -0.5f,		0.0f,  1.0f,  0.0f
	};
	
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

    Input Input;
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightingShader.use();
        camera.updateObjectMatrix(lightingShader, "pers");
		lightingShader.setVec3("lightPos", lightPos);
		lightingShader.setVec3("lightColor", lightColor);
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setVec3("material.ambient", (0.1f*lightColor)*glm::vec3(0.9f, 0.1f, 0.2f));
        lightingShader.setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
        lightingShader.setVec3("material.specular", glm::vec3(0.6f, 0.6f, 0.6f));
        lightingShader.setFloat("material.smoothness", 32.0f);
		
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

		lightCubeShader.use();
		camera.updateObjectMatrix(lightingShader, "pers");
        glm::mat4 model1 = glm::mat4(1.0f);
        model1 = glm::translate(model1, lightPos);
        model1 = glm::scale(model1, glm::vec3(0.05f));
        lightCubeShader.setMat4("model", model1);
		lightCubeShader.setVec3("lightColor", lightColor);

		camera.Inputs(window);
        wireframeTickCallback(Input.GetKeyDown(window, GLFW_KEY_ENTER));

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}
