#version 450 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;

uniform vec3 lightPos[4];
uniform vec3 viewPos;

void main()
{
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal  = normalize(texture(gNormal, TexCoords).rgb);
    vec3 Albedo  = texture(gAlbedo, TexCoords).rgb;

    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0.0);

    for(int i = 0; i < 4; i++)
    {
        vec3 lightDir = normalize(lightPos[i] - FragPos);

        float diff = max(dot(Normal, lightDir), 0.0);

        vec3 diffuse = diff * Albedo;

        result += diffuse;
    }

    FragColor = vec4(result, 1.0);
}