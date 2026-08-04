#ifndef AO_GLSL
#define AO_GLSL

float computeAO(
    vec3 fragPos,
    vec3 normal)
{
    vec3 samples[16] = vec3[](
        vec3( 0.000, 1.000, 0.000),
        vec3( 0.500, 0.866, 0.000),
        vec3(-0.500, 0.866, 0.000),
        vec3( 0.000, 0.866, 0.500),
        vec3( 0.000, 0.866,-0.500),
        vec3( 0.354, 0.866, 0.354),
        vec3(-0.354, 0.866, 0.354),
        vec3( 0.354, 0.866,-0.354),
        vec3(-0.354, 0.866,-0.354),
        vec3( 0.707, 0.707, 0.000),
        vec3(-0.707, 0.707, 0.000),
        vec3( 0.000, 0.707, 0.707),
        vec3( 0.000, 0.707,-0.707),
        vec3( 0.500, 0.707, 0.500),
        vec3(-0.500, 0.707, 0.500),
        vec3( 0.000, 1.000, 0.000)
    );

    vec3 tangent = abs(normal.y) < 0.99
        ? normalize(cross(vec3(0.0,1.0,0.0), normal))
        : normalize(cross(vec3(1.0,0.0,0.0), normal));

    vec3 bitangent = cross(normal, tangent);

    mat3 TBN = mat3(tangent, bitangent, normal);

    float occlusion = 0.0;
    const float radius = 0.5;

    for(int i = 0; i < 16; i++)
    {
        vec3 rayDir = normalize(TBN * samples[i]);
        vec3 rayOrigin = fragPos + normal * 0.02;

        int hitObject;
        float hitDistance;
        vec3 hitNormal;

        bool blocked = traverseBVH(rayOrigin, rayDir, radius, hitObject, hitDistance, hitNormal);

        if(blocked)
            occlusion += 1.0;
    }

    return 1.0 - occlusion / 16.0;
}

#endif