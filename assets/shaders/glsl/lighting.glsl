#ifndef LIGHTING_GLSL
#define LIGHTING_GLSL

struct LightingResult
{
    vec3 color;
    vec3 diffuse;
    vec3 specular;
};

struct EmissiveLight
{
    vec4 position;      
    vec4 colorEmission; 
};

layout(std430, binding = 4) buffer Lights
{
    EmissiveLight lights[];
};

uniform int lightCount;

LightingResult evaluateLighting(
    vec3 fragPos,
    vec3 normal,
    vec3 albedo,
    vec3 emission,
    float reflectivity,
    vec3 viewPos,
    float shadow,
    float ao)
{
    LightingResult result;
    result.diffuse  = vec3(0.0);
    result.specular = vec3(0.0);

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 direct  = vec3(0.0);

    for (int i = 0; i < lightCount; i++)
    {
        vec3 lightPos      = lights[i].position.xyz;
        vec3 lightColor    = lights[i].colorEmission.rgb;
        float lightEmission = lights[i].colorEmission.a;

        vec3 lightVector = lightPos - fragPos;
        float distance = length(lightVector);
        vec3 lightDir = normalize(lightVector);

        float ndotl = dot(normal, lightDir);
        float diff = smoothstep(-0.3, 0.8, ndotl);
        float attenuation = 1.0 / (1.0 + 0.35 * distance + 0.18 * distance * distance);

        vec3 diffuseTerm = diff * albedo * lightColor * lightEmission;

        vec3 halfwayDir = normalize(lightDir + viewDir);
        float shininess = 96.0;
        float specBase = pow(max(dot(normal, halfwayDir), 0.0), shininess);
        float specStrength = mix(0.15, 1.0, reflectivity);
        vec3 specularTerm = lightColor * specBase * specStrength * lightEmission;

        result.diffuse  += diffuseTerm;
        result.specular += specularTerm;

        direct += (diffuseTerm + specularTerm) * attenuation * 2.0;
    }

    float shadowSoft = smoothstep(0.15, 1.0, shadow);
    direct *= mix(0.05, 1.0, shadowSoft);

    result.color  = albedo * 0.12 * ao;
    result.color += direct;
    result.color += emission;

    return result;
}

#endif