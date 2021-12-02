#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTextCord;

out vec2 mTextCords;

uniform mat4 model;
uniform mat4 pers;

void main()
{
    gl_Position = pers * model * vec4(aPos, 1.0);
    mTextCords = aTextCord;
}