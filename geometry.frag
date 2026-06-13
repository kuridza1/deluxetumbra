#version 450 core

in vec3 vPos;

out vec4 FragColor;

void main()
{
    FragColor = vec4(vPos * 0.5 + 0.5, 1.0);
}