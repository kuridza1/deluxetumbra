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
    int isLight;
    float emission;
    float reflectivity;
};

layout(std430, binding = 3) buffer ObjectBuffer
{
    GPUObject objects[];
};



bool intersectAABB(vec3 ro, vec3 rd, vec3 bmin, vec3 bmax, out float tHit, out vec3 normal)
{
    vec3 invRd = 1.0 / rd;

    vec3 t0 = (bmin - ro) * invRd;
    vec3 t1 = (bmax - ro) * invRd;

    vec3 tMin = min(t0, t1);
    vec3 tMax = max(t0, t1);

    float tEnter = max(max(tMin.x, tMin.y), tMin.z);
    float tExit = min(min(tMax.x, tMax.y), tMax.z);

    if(tExit < max(tEnter, 0.0))
        return false;

    tHit = max(tEnter, 0.0);

    if(tMin.x > tMin.y && tMin.x > tMin.z)
        normal = vec3(invRd.x < 0.0 ? 1.0 : -1.0, 0.0, 0.0);
    else if(tMin.y > tMin.z)
        normal = vec3(0.0, invRd.y < 0.0 ? 1.0 : -1.0, 0.0);
    else
        normal = vec3(0.0, 0.0, invRd.z < 0.0 ? 1.0 : -1.0);

    return true;
}


bool intersectBox(vec3 rayOrigin, vec3 rayDir, mat4 invModel, out float tHit, out vec3 worldNormal)
{
    vec3 localOrigin = (invModel * vec4(rayOrigin,1.0)).xyz;
    vec3 localDir = normalize((invModel * vec4(rayDir,0.0)).xyz);

    vec3 localNormal;
    float localT;

    if(!intersectAABB(localOrigin, localDir, vec3(-0.5), vec3(0.5), localT, localNormal))
        return false;


    vec3 localHit = localOrigin + localDir * localT;
    vec3 worldHit = (inverse(invModel) * vec4(localHit,1.0)).xyz;

    tHit = length(worldHit - rayOrigin);

    worldNormal = normalize((transpose(invModel) * vec4(localNormal,0.0)).xyz);

    if(dot(worldNormal, rayDir) > 0.0)
        worldNormal = -worldNormal;

    return true;
}



bool intersectSphere(vec3 rayOrigin, vec3 rayDir, mat4 invModel, out float tHit, out vec3 worldNormal)
{
    vec3 ro = (invModel * vec4(rayOrigin,1.0)).xyz;
    vec3 rd = normalize((invModel * vec4(rayDir,0.0)).xyz);


    float a = dot(rd,rd);
    float b = 2.0 * dot(ro,rd);
    float c = dot(ro,ro)-0.25;


    float d = b*b-4.0*a*c;

    if(d < 0.0)
        return false;


    float sqrtD = sqrt(d);

    float t0 = (-b-sqrtD)/(2.0*a);
    float t1 = (-b+sqrtD)/(2.0*a);


    float t = t0;

    if(t < 0.0)
        t = t1;

    if(t < 0.0)
        return false;


    vec3 localHit = ro + rd*t;

    worldNormal = normalize((transpose(invModel)*vec4(normalize(localHit),0.0)).xyz);

    vec3 worldHit = (inverse(invModel)*vec4(localHit,1.0)).xyz;

    tHit = length(worldHit-rayOrigin);

    return true;
}



bool traverseBVH(vec3 rayOrigin, vec3 rayDir, float maxDistance, out int hitObject, out float closestHit, out vec3 hitNormal)
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
        vec3 dummyNormal;


        if(!intersectAABB(rayOrigin, rayDir, node.min.xyz, node.max.xyz, nodeHit, dummyNormal))
            continue;


        if(nodeHit > closestHit)
            continue;



        if(node.leaf == 1)
        {
            
            float objectHit;
            vec3 objectNormal;
            bool hit = false;

            if(objects[node.object].type == 0)
                hit = intersectBox(rayOrigin, rayDir, objects[node.object].inverseModel, objectHit, objectNormal);
            else
                hit = intersectSphere(rayOrigin, rayDir, objects[node.object].inverseModel, objectHit, objectNormal);

            if(hit && objectHit > 0.0 && objectHit < closestHit)
            {
                closestHit = objectHit;
                hitObject = node.object;
                hitNormal = objectNormal;
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

bool traverseBVHShadow(vec3 rayOrigin, vec3 rayDir, float maxDistance)
{
    int stack[64];
    int stackPtr = 0;

    stack[stackPtr++] = 0;

    float closestHit = maxDistance;

    while(stackPtr > 0)
    {
        int nodeIndex = stack[--stackPtr];
        BVHNode node = nodes[nodeIndex];

        float nodeHit;
        vec3 dummyNormal;

        if(!intersectAABB(rayOrigin, rayDir, node.min.xyz, node.max.xyz, nodeHit, dummyNormal))
            continue;

        if(nodeHit > closestHit)
            continue;

        if(node.leaf == 1)
        {
            if(objects[node.object].isLight == 1)
                continue;

            float objectHit;
            vec3 objectNormal;
            bool hit = false;

            if(objects[node.object].type == 0)
                hit = intersectBox(rayOrigin, rayDir, objects[node.object].inverseModel, objectHit, objectNormal);
            else
                hit = intersectSphere(rayOrigin, rayDir, objects[node.object].inverseModel, objectHit, objectNormal);

            if(hit && objectHit > 0.0 && objectHit < closestHit)
                return true;
        }
        else
        {
            if(node.left >= 0)
                stack[stackPtr++] = node.left;

            if(node.right >= 0)
                stack[stackPtr++] = node.right;
        }
    }

    return false;
}

#endif