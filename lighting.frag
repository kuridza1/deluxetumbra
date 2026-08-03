#version 450 core

#include "lighting.glsl"

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gEmission;
uniform sampler2D gReflectivity;
uniform sampler2D shadowMask;
uniform sampler2D reflectionTexture;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

void main()
{
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = normalize(texture(gNormal, TexCoords).rgb);
    vec3 Albedo = texture(gAlbedo, TexCoords).rgb;
    vec3 emission = texture(gEmission, TexCoords).rgb;
    float reflectivity = texture(gReflectivity, TexCoords).r;
    float shadow = texture(shadowMask, TexCoords).r;

    LightingResult lighting = evaluateLighting(
        FragPos,
        Normal,
        Albedo,
        emission,
        reflectivity,
        lightPos,
        viewPos,
        lightColor,
        shadow
    );

    vec3 result = lighting.color;

    vec3 reflection = texture(reflectionTexture, TexCoords).rgb;

    result = mix(result, reflection, reflectivity);

    FragColor = vec4(result, 1.0);
}