#version 330 core
out vec4 FragColor;

in vec3 mColor;
in vec2 mTextCords;

uniform sampler2D mTexture;

void main()
{
    FragColor = texture(mTexture, mTextCords) * vec4(mColor, 1.0f);
} 