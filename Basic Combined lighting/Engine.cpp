#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Engine.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool GetKeyDown(GLFWwindow* window, int Key)
{
    if (glfwGetKey(window, Key) == GLFW_PRESS)
    {
        return true;
    }
}
bool GetKeyUp(GLFWwindow* window, int Key)
{
    if (glfwGetKey(window, Key) == GLFW_RELEASE)
    {
        return true;
    }
}

void frameSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void wireframeTickCallback(bool tick)
{
    if (tick)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else if (!tick)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

VAO::VAO()
{
    glGenVertexArrays(1, &vao);
}
void VAO::Bind()
{
    glBindVertexArray(vao);
}
void VAO::UnBind()
{
    glBindVertexArray(0);
}
void VAO::LinkAttrib(VBO VBO, GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer)
{
    VBO.Bind();
    glVertexAttribPointer(index, size, type, GL_FALSE, stride, pointer);
    glEnableVertexAttribArray(index);
    VBO.UnBind();
}
void VAO::Delete()
{
    glDeleteVertexArrays(1, &vao);
}

VBO::VBO(GLsizeiptr size, const void* vertices)
{
    glGenBuffers(1, &vbo);
    VBO::Bind();
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}
void VBO::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}
void VBO::UnBind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void VBO::Delete()
{
    glDeleteBuffers(1, &vbo);
}

EBO::EBO(GLsizeiptr size, const void* indices)
{
    glGenBuffers(1, &ebo);
    EBO::Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}
void EBO::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}
void EBO::UnBind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void EBO::Delete()
{
    glDeleteBuffers(1, &ebo);
}

Texture::Texture(const char* filename)
{
    glGenTextures(1, &texture);
    Texture::Bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);
}
void Texture::Bind()
{
    glBindTexture(GL_TEXTURE_2D, texture);
}
void Texture::Delete()
{
    glDeleteTextures(1, &texture);
}

Camera::Camera(int width, int height, glm::vec3 position, float FOV, float clipMin, float clipMax)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
    camFOV = FOV;
    nearPlane = clipMin;
    farPlane = clipMax;
}
void Camera::updateObjectMatrix(Shader shader, const char* uniform)
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	view = glm::lookAt(Position, Position + Orientation, Up);
    
	projection = glm::perspective(glm::radians(camFOV), (float)width / height, nearPlane, farPlane);

	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));
}
void Camera::Inputs(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		Position += speed * Orientation;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		Position += speed * -glm::normalize(glm::cross(Orientation, Up));
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		Position += speed * -Orientation;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		Position += speed * glm::normalize(glm::cross(Orientation, Up));
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		Position += speed * Up;
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		Position += speed * -Up;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		if (firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		double mouseX;
		double mouseY;
        
		glfwGetCursorPos(window, &mouseX, &mouseY);

		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

		if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			Orientation = newOrientation;
		}

		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstClick = true;
	}
}

void Shader::setMat4(const char* uniform, glm::mat4 matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(this->ID, uniform), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Camera::printCamPos()
{
    std::cout<<"[ "<<Position.x<<", "<<Position.y<<", "<<Position.z<<" ]\n";
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        
        vShaderFile.close();
        fShaderFile.close();
        
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char * fShaderCode = fragmentCode.c_str();
    
    unsigned int vertex, fragment;
    
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use()
{
    glUseProgram(ID); 
}

void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

void Shader::setVec3(const char* vec3, glm::vec3 vector3)
{
    glUniform3fv(glGetUniformLocation(this->ID, vec3), 1, glm::value_ptr(vector3));
}

void Shader::setFloat(const char* the_float, float value)
{
    glUniform1f(glGetUniformLocation(this->ID, the_float), value);
}

bool Input::GetKeyDown(GLFWwindow* window, int key)
{
    if (glfwGetKey(window, key) == GLFW_PRESS)
        return true;
    else
        return false;
}

bool Input::GetKeyUp(GLFWwindow* window, int key)
{
    if (glfwGetKey(window, key) == GLFW_RELEASE)
        return true;
    else
        return false;
}