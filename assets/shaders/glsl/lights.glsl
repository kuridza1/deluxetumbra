#ifndef LIGHTS_GLSL
#define LIGHTS_GLSL

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

#endif