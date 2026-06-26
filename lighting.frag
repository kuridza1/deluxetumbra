#version 450 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gEmission;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform sampler2D shadowMask;

void main()
{
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal   = normalize(texture(gNormal, TexCoords).rgb);
    vec3 Albedo   = texture(gAlbedo, TexCoords).rgb;

    vec3 emission = texture(gEmission, TexCoords).rgb;

    vec3 result = Albedo * 0.08;

    vec3 lightVector = lightPos - FragPos;
    float distance = length(lightVector);
    vec3 lightDir = normalize(lightVector);

    float diff = max(dot(Normal, lightDir), 0.0);

    float attenuation =
        1.0 / (1.0 + 0.4 * distance + 0.25 * distance * distance);

    vec3 diffuse = diff * Albedo;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, Normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = vec3(0.15) * spec;

    result += (diffuse + specular) * attenuation * 8.0;

    result += emission;
    float shadow =
    texture(shadowMask, TexCoords).r;

    result *= shadow;
    FragColor = vec4(result, 1.0);
}