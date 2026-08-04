#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <glad/glad.h>
#include "BVH.h"



class Scene {

public:
    void buildScene(BVH& bvh);
    void createSphere(int stacks = 32, int slices = 32);
    void createCube(float(&cubeVertices)[216]);
    void createQuad(float(&quadVertices)[24]);
    void destroy();

    void drawCube();
    void drawSphere();

    void setColor(GLuint shaderID, const glm::vec3& color);
    void setEmission(GLuint shaderID, float e);
    void setReflectivity(GLuint shaderID, float r);

    GLuint VAO = 0;
    GLuint VBO = 0;

    GLuint quadVAO = 0;
    GLuint quadVBO = 0;

    BVH bvh;

private:
    GLuint sphereVAO = 0;
    GLuint sphereVBO = 0;
    GLuint sphereEBO = 0;
    GLsizei sphereIndexCount = 0;
};