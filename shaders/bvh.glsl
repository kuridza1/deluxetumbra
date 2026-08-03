#ifndef BVH_GLSL
#define BVH_GLSL

struct BVHNode
{
    vec4 min;
    vec4 max;
    int left;
    int right;
    int object;
    int leaf;
};

layout(std430, binding = 2) buffer BVHBuffer
{
    BVHNode nodes[];
};


struct GPUObject
{
    mat4 inverseModel;
    vec4 color;
    int type;
};

layout(std430, binding = 3) buffer ObjectBuffer
{
    GPUObject objects[];
};


bool intersectAABB(vec3 ro, vec3 rd, vec3 bmin, vec3 bmax, out float tHit)
{
    vec3 invRd = 1.0 / rd;

    vec3 t0 = (bmin - ro) * invRd;
    vec3 t1 = (bmax - ro) * invRd;

    vec3 tMin = min(t0, t1);
    vec3 tMax = max(t0, t1);

    float tEnter = max(max(tMin.x, tMin.y), tMin.z);
    float tExit = min(min(tMax.x, tMax.y), tMax.z);

    tHit = tEnter;

    return tExit > max(tEnter, 0.0);
}


bool intersectBox(vec3 rayOrigin, vec3 rayDir, mat4 invModel, out float tHit)
{
    vec3 localOrigin = (invModel * vec4(rayOrigin, 1.0)).xyz;
    vec3 localDir = normalize((invModel * vec4(rayDir, 0.0)).xyz);

    return intersectAABB(localOrigin, localDir, vec3(-0.5), vec3(0.5), tHit);
}


bool intersectSphere(vec3 rayOrigin, vec3 rayDir, mat4 invModel, out float tHit)
{
    vec3 ro = (invModel * vec4(rayOrigin, 1.0)).xyz;
    vec3 rd = normalize((invModel * vec4(rayDir, 0.0)).xyz);

    float a = dot(rd, rd);
    float b = 2.0 * dot(ro, rd);
    float c = dot(ro, ro) - 0.25;

    float discriminant = b * b - 4.0 * a * c;

    if(discriminant < 0.0)
        return false;

    float s = sqrt(discriminant);

    float t0 = (-b - s) / (2.0 * a);
    float t1 = (-b + s) / (2.0 * a);

    tHit = t0 > 0.0 ? t0 : t1;

    return tHit > 0.0;
}


bool traverseBVH(vec3 rayOrigin, vec3 rayDir, float maxDistance, out int hitObject, out float closestHit)
{
    int stack[64];
    int stackPtr = 0;

    stack[stackPtr++] = 0;

    hitObject = -1;
    closestHit = maxDistance;


    while(stackPtr > 0)
    {
        int nodeIndex = stack[--stackPtr];

        BVHNode node = nodes[nodeIndex];

        float nodeHit;

        if(!intersectAABB(rayOrigin, rayDir, node.min.xyz, node.max.xyz, nodeHit))
            continue;

        if(nodeHit > closestHit)
            continue;


        if(node.leaf == 1)
        {
            float objectHit;
            bool hit;

            if(objects[node.object].type == 0)
                hit = intersectBox(rayOrigin, rayDir, objects[node.object].inverseModel, objectHit);
            else
                hit = intersectSphere(rayOrigin, rayDir, objects[node.object].inverseModel, objectHit);


            if(hit && objectHit > 0.0 && objectHit < closestHit)
            {
                closestHit = objectHit;
                hitObject = node.object;
            }
        }
        else
        {
            if(node.left >= 0)
                stack[stackPtr++] = node.left;

            if(node.right >= 0)
                stack[stackPtr++] = node.right;
        }
    }

    return hitObject >= 0;
}

#endif