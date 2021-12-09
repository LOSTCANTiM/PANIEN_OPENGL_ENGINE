#version 430 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 pers;

void main()
{
    gl_Position = pers * model * vec4(aPos, 1.0);
}