#ifndef ENGINE_CLASS_FILE
#define ENGINE_CLASS_FILE

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

class VBO
{
    public:
    unsigned int vbo;
    VBO(GLsizeiptr size, const void* vertices);
    void Bind();
    void UnBind();
    void Delete();
};

class VAO
{
    public:
    unsigned int vao;
    VAO();
    void Bind();
    void LinkAttrib(VBO VBO, GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);
    void UnBind();
    void Delete();
};

class Shader
{
public:
    unsigned int ID;
    Shader(const char* vertexPath, const char* fragmentPath);
    void use();
    void setMat4(const char* uniform, glm::mat4 matrix);
    void setVec3(const char* vec3, glm::vec3 vector3);
    void checkCompileErrors(unsigned int shader, std::string type);
    void setFloat(const char* the_float, float value);
};

class EBO
{
    public:
    unsigned int ebo;
    EBO(GLsizeiptr size, const void* indices);
    void Bind();
    void UnBind();
    void Delete();
};

class Texture
{
    public:
    unsigned int texture;
    Texture(const char* filename);
    void Bind();
    void Delete();
};

class Camera
{
public:
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	bool firstClick = true;

	int width;
	int height;

	float speed = 0.01f;
	float sensitivity = 75.0f;

    float camFOV;
    float nearPlane;
    float farPlane;

	Camera(int width, int height, glm::vec3 position, float FOV, float clipMin, float clipMax);
	void updateObjectMatrix(Shader shader, const char* uniform);
	void Inputs(GLFWwindow* window);
    void printCamPos();
};

class Input
{
    public:
    bool GetKeyUp(GLFWwindow* window, int key);
    bool GetKeyDown(GLFWwindow* window, int key);
};

#endif