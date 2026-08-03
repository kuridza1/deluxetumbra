#ifndef LIGHTING_GLSL
#define LIGHTING_GLSL

struct LightingResult
{
    vec3 color;
    vec3 diffuse;
    vec3 specular;
};

LightingResult evaluateLighting(
    vec3 fragPos,
    vec3 normal,
    vec3 albedo,
    vec3 emission,
    float reflectivity,
    vec3 lightPos,
    vec3 viewPos,
    vec3 lightColor,
    float shadow)
{
    LightingResult result;

    vec3 lightVector = lightPos - fragPos;
    float distance = length(lightVector);
    vec3 lightDir = normalize(lightVector);

    float ndotl = dot(normal, lightDir);
    float diff = smoothstep(-0.3, 0.8, ndotl);

    float attenuation = 1.0 / (1.0 + 0.35 * distance + 0.18 * distance * distance);

    result.diffuse = diff * albedo * lightColor;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float shininess = 96.0;
    float specBase = pow(max(dot(normal, halfwayDir), 0.0), shininess);

    float specStrength = mix(0.15, 1.0, reflectivity);

    result.specular = lightColor * specBase * specStrength;

    vec3 direct = (result.diffuse + result.specular) * attenuation * 2.0;

    float shadowSoft = smoothstep(0.15, 1.0, shadow);
    direct *= mix(0.05, 1.0, shadowSoft);

    result.color = albedo * 0.12;
    result.color += direct;
    result.color += emission;

    return result;
}

#endif