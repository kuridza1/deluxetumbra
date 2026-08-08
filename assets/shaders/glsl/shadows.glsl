#ifndef SHADOWS_GLSL
#define SHADOWS_GLSL

#include "lights.glsl"

float computeShadow(vec3 fragPos, vec3 normal)
{
    if(lightCount <= 0)
        return 1.0;

    vec2 samples[8] = vec2[](
        vec2(-0.375, -0.375),
        vec2( 0.0,   -0.375),
        vec2( 0.375, -0.375),
        vec2(-0.375,  0.0),
        vec2( 0.375,  0.0),
        vec2(-0.375,  0.375),
        vec2( 0.0,    0.375),
        vec2( 0.375,  0.375)
    );

    float visibility = 0.0;

    for(int lightIndex = 0; lightIndex < lightCount; lightIndex++)
    {
        vec3 lightCenter = lights[lightIndex].position.xyz;
        vec3 lightRight = lights[lightIndex].right.xyz;
        vec3 lightUp = lights[lightIndex].up.xyz;

        float lightVisibility = 0.0;

        for(int i = 0; i < 8; i++)
        {
            vec3 samplePos = lightCenter + samples[i].x * lightRight + samples[i].y * lightUp;

            vec3 toLight = samplePos - fragPos;
            float lightDistance = length(toLight);

            if(lightDistance <= 0.0001)
            {
                lightVisibility += 1.0;
                continue;
            }

            vec3 rayDir = toLight / lightDistance;

            float NdotL = max(dot(normal, rayDir), 0.05);
            float bias = min(0.02 / NdotL, 0.03);

            vec3 rayOrigin = fragPos + normal * bias;

            int hitObject;
            float hitDistance;
            vec3 hitNormal;

            bool blocked = traverseBVHShadow(rayOrigin, rayDir, lightDistance - 0.001);

            if(!blocked)
                lightVisibility += 1.0;
        }

        visibility += lightVisibility / 8.0;
    }

    return visibility / float(lightCount);
}

#endif