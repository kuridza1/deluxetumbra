#ifndef SHADOWS_GLSL
#define SHADOWS_GLSL


float computeShadow(
    vec3 fragPos,
    vec3 normal,
    vec3 lightCenter,
    vec3 lightRight,
    vec3 lightUp)
{
    vec2 samples[8] = vec2[](
        vec2(-0.375,-0.375),
        vec2(0.0,-0.375),
        vec2(0.375,-0.375),
        vec2(-0.375,0.0),
        vec2(0.375,0.0),
        vec2(-0.375,0.375),
        vec2(0.0,0.375),
        vec2(0.375,0.375)
    );


    float visibility = 0.0;


    for(int i = 0; i < 8; i++)
    {
        vec3 samplePos = lightCenter + samples[i].x * lightRight + samples[i].y * lightUp;

        vec3 toLight = samplePos - fragPos;

        float lightDistance = length(toLight);

        vec3 rayDir = normalize(toLight);


        float NdotL = max(dot(normal, rayDir), 0.05);

        float bias = min(0.02 / NdotL, 0.08);

        vec3 rayOrigin = fragPos + normal * bias;


        int hitObject;
        float hitDistance;
        vec3 hitNormal;

        bool blocked = traverseBVH(rayOrigin, rayDir, lightDistance, hitObject, hitDistance, hitNormal);


        if(!blocked)
            visibility += 1.0;
    }


    return visibility / 8.0;
}


#endif