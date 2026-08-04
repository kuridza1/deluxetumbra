#include "../include/Scene.h"
#include "../include/Shader.h"
#include <glm/ext/matrix_transform.hpp>

void Scene::destroy()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &quadVBO);

	glDeleteVertexArrays(1, &sphereVAO);
	glDeleteBuffers(1, &sphereVBO);
	glDeleteBuffers(1, &sphereEBO);
}

void Scene::createCube(float(&cubeVertices)[216])
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Scene::createQuad(float(&quadVertices)[24]) {
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);

    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Scene::createSphere(int stacks, int slices)
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    const float PI = 3.14159265359f;

    for (int i = 0;i <= stacks;i++)
    {
        float v = (float)i / stacks;
        float phi = v * PI;

        for (int j = 0;j <= slices;j++)
        {
            float u = (float)j / slices;
            float theta = u * 2.0f * PI;

            float x = sin(phi) * cos(theta);
            float y = cos(phi);
            float z = sin(phi) * sin(theta);

            vertices.push_back(x * 0.5f);
            vertices.push_back(y * 0.5f);
            vertices.push_back(z * 0.5f);

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }

    for (int i = 0;i < stacks;i++)
    {
        for (int j = 0;j < slices;j++)
        {
            int first = i * (slices + 1) + j;
            int second = first + slices + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }

    sphereIndexCount = (GLuint)indices.size();

    glGenVertexArrays(1, &sphereVAO);
    glGenBuffers(1, &sphereVBO);
    glGenBuffers(1, &sphereEBO);

    glBindVertexArray(sphereVAO);

    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}


void Scene::drawCube()
{
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Scene::drawSphere()
{
    glBindVertexArray(sphereVAO);
    glDrawElements(GL_TRIANGLES, sphereIndexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}


void Scene::buildScene(BVH& bvh)
{
    bvh.sceneObjects.clear();
    bvh.shadowObjects.clear();
    bvh.bvhNodes.clear();

    auto addObject = [&](const glm::mat4& model, const glm::vec3& color, float emission = 0.0f, float reflectivity = 0.0f, ObjectType type = Cube)
        {
            int id = bvh.sceneObjects.size();

            bvh.sceneObjects.push_back({ model, color, emission, reflectivity, type });

            ShadowObject shadow;
            shadow.model = model;
            shadow.bounds = bvh.computeBounds(model);
            shadow.id = id;

            bvh.shadowObjects.push_back(shadow);
        };

    glm::mat4 m;

    // Floor
    m = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.0f)), glm::vec3(5.0f, 0.02f, 5.0f));
    addObject(m, glm::vec3(0.8f), 0.0f, 0.3f);

    // Ceiling
    m = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 4.5f, 0.0f)), glm::vec3(5.0f, 0.02f, 5.0f));
    addObject(m, glm::vec3(1.0f), 0.0f, 0.3f);

    // Light
    m = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 4.5f, 0.0f)), glm::vec3(1.8f, 0.1f, 1.8f));
    addObject(m, glm::vec3(1.0f), 5.0f, 0.0f);

    // Left wall
    m = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(-2.5f, 2.0f, 0.0f)), glm::vec3(0.02f, 5.0f, 5.0f));
    addObject(m, glm::vec3(0.75f, 0.1f, 0.1f), 0.0f, 0.3f);

    // Right wall
    m = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(2.5f, 2.0f, 0.0f)), glm::vec3(0.02f, 5.0f, 5.0f));
    addObject(m, glm::vec3(0.1f, 0.75f, 0.1f), 0.0f, 0.3f);

    // Back wall
    m = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, -2.5f)), glm::vec3(5.0f, 5.0f, 0.02f));
    addObject(m, glm::vec3(0.8f), 0.0f, 0.3f);

    //Front wall
    /*m = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 2.5f)), glm::vec3(5.0f, 5.0f, 0.02f));
    addObject(m, glm::vec3(0.8f), 0.0f, 0.0f);*/

    // Small box
    //m = glm::mat4(1.0f);
    //m = glm::translate(m, glm::vec3(-0.9f, 0.27f, 1.0f));
    //m = glm::rotate(m, glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //m = glm::scale(m, glm::vec3(1.5f));
    //addObject(m, glm::vec3(0.85f), 0.0f, 0.0f);

    //// Large box
    //m = glm::mat4(1.0f);
    //m = glm::translate(m, glm::vec3(0.6f, 1.0f, -0.8f));
    //m = glm::rotate(m, glm::radians(-18.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //m = glm::scale(m, glm::vec3(1.3f, 3.0f, 1.3f));
    //addObject(m, glm::vec3(0.85f), 0.0f, 0.0f);

    // Smallest box
    //m = glm::mat4(1.0f);
    //m = glm::translate(m, glm::vec3(1.0f, 0.0f, 1.5f));
    //m = glm::rotate(m, glm::radians(-50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //m = glm::scale(m, glm::vec3(1.0f, 1.0f, 1.5f));
    //addObject(m, glm::vec3(0.85f), 0.0f, 0.0f);

    // Sphere 1
    m = glm::mat4(1.0f);
    m = glm::translate(m, glm::vec3(-1.0f, 0.0f, 1.0f));
    m = glm::scale(m, glm::vec3(1.0f));
    addObject(m, glm::vec3(0.0f, 1.0f, 1.0f), 0.0f, 0.5f, Sphere);

    // Sphere 2
    m = glm::mat4(1.0f);
    m = glm::translate(m, glm::vec3(0.8f, 0.15f, -0.8f));
    m = glm::scale(m, glm::vec3(1.3f));
    addObject(m, glm::vec3(0.0f, 0.0f, 1.0f), 0.0f, 0.5f, Sphere);

    // Sphere 3
    m = glm::mat4(1.0f);
    m = glm::translate(m, glm::vec3(1.3f, -0.12f, 1.4f));
    m = glm::scale(m, glm::vec3(0.7f));
    addObject(m, glm::vec3(1.0f, 0.0f, 1.0f), 0.0f, 0.5f, Sphere);

    bvh.buildBVH(0, bvh.shadowObjects.size());
    bvh.uploadBVH();
}

void Scene::setColor(GLuint shaderID, const glm::vec3& color)
{
    glUniform3f(glGetUniformLocation(shaderID, "objectColor"),
        color.x, color.y, color.z);
}

void Scene::setEmission(GLuint shaderID, float e)
{
    glUniform1f(glGetUniformLocation(shaderID, "emission"), e);
}

void Scene::setReflectivity(GLuint shaderID, float r)
{
    glUniform1f(glGetUniformLocation(shaderID, "reflectivity"), r);
}