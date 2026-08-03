#version 450 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gEmission;
uniform sampler2D gReflectivity;
uniform sampler2D reflectionTexture;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform sampler2D shadowMask;
uniform vec3 lightColor;

uniform vec3 redWallColor;   // (0.75, 0.1, 0.1)
uniform vec3 greenWallColor; // (0.1, 0.75, 0.1)
uniform float redWallX;      // -2.5
uniform float greenWallX;    //  2.5
uniform float bleedStrength; // npr. 0.5 - podesi eksperimentalno

vec3 computeColorBleed(vec3 fragPos, vec3 normal)
{
    vec3 bleed = vec3(0.0);

    // Crveni zid je na x = redWallX, "gleda" u +x smeru
    float facingRed = 0.9 * max(dot(normal, vec3(-1.0,0.0,0.0)),0.0);    
    float distRed = abs(fragPos.x - redWallX);
    float falloffRed = 1.0 / (1.0 + 0.35 * distRed * distRed);
    bleed += redWallColor * facingRed * falloffRed;

    // Zeleni zid je na x = greenWallX, "gleda" u -x smeru
    float facingGreen = 0.9 * max(dot(normal, vec3(1.0,0.0,0.0)),0.0);    
    float distGreen = abs(fragPos.x - greenWallX);
    float falloffGreen = 1.0 / (1.0 + 0.35 * distGreen * distGreen);
    bleed += greenWallColor * facingGreen * falloffGreen;

    return bleed * bleedStrength;
}



void main()
{
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = normalize(texture(gNormal, TexCoords).rgb);
    vec3 Albedo = texture(gAlbedo, TexCoords).rgb;
    vec3 emission = texture(gEmission, TexCoords).rgb;

    vec3 result = Albedo * 0.12;

    vec3 lightVector = lightPos - FragPos;
    float distance = length(lightVector);
    vec3 lightDir = normalize(lightVector);

    float ndotl = dot(Normal, lightDir);
    float diff = smoothstep(-0.3, 0.8, ndotl);

    float attenuation = 1.0 / (1.0 + 0.35 * distance + 0.18 * distance * distance);

    vec3 diffuse = diff * Albedo * lightColor;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float shininess = 96.0;
    float specBase = pow(max(dot(Normal, halfwayDir), 0.0), shininess);

    float reflectivityMask = texture(gReflectivity, TexCoords).r;
    float specStrength = mix(0.15, 1.0, reflectivityMask);

    vec3 specular = lightColor * specBase * specStrength;

    vec3 direct = (diffuse + specular) * attenuation * 2.0;


    float shadow = texture(shadowMask, TexCoords).r;
    float shadowSoft = smoothstep(0.15, 1.0, shadow);

    direct *= mix(0.05, 1.0, shadowSoft);


    vec3 bleed = computeColorBleed(FragPos, Normal) * Albedo * 0.5;


    result += direct + emission + bleed;


    vec3 reflection = texture(reflectionTexture, TexCoords).rgb;
    float reflectivity = texture(gReflectivity, TexCoords).r;

    result = mix(result, reflection, reflectivity);


    FragColor = vec4(result, 1.0);
}