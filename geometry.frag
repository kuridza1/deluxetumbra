#version 330 core

layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec3 gAlbedo;
layout(location = 3) out vec3 gEmission;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 objectColor;
uniform float emission;

void main()
{
    gPosition = FragPos;
    gNormal   = normalize(Normal);
    gAlbedo   = objectColor;
    gEmission = objectColor * emission;
}