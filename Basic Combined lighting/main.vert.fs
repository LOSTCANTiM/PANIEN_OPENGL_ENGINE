#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 pers;

void main()
{
    gl_Position = pers * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormals;
}