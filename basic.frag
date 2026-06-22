#version 450 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 viewPos;
uniform vec3 objectColor;

uniform vec3 lights[4];

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    if(dot(norm, viewDir) < 0.0)
        norm = -norm;
    vec3 result = vec3(0.0);

    for(int i = 0; i < 4; i++)
    {
        vec3 lightDir = normalize(lights[i] - FragPos);

        float diff = max(dot(norm, lightDir), 0.0);

        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0);

        float dist = length(lights[i] - FragPos);
        float attenuation = 1.0 / (dist * dist);

        vec3 ambient = 0.05 * objectColor;
        vec3 diffuse = diff * objectColor;
        vec3 specular = spec * vec3(1.0);

        result += (ambient + diffuse + specular) * attenuation;
    }

    FragColor = vec4(result, 1.0);
}