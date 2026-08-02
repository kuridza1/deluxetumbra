#include "Renderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <limits>
#include <algorithm>

static float cubeVertices[] =
{
    // pos                // normal
    -0.5f,-0.5f,-0.5f,    0.0f, 0.0f,-1.0f,
     0.5f,-0.5f,-0.5f,    0.0f, 0.0f,-1.0f,
     0.5f, 0.5f,-0.5f,    0.0f, 0.0f,-1.0f,
     0.5f, 0.5f,-0.5f,    0.0f, 0.0f,-1.0f,
    -0.5f, 0.5f,-0.5f,    0.0f, 0.0f,-1.0f,
    -0.5f,-0.5f,-0.5f,    0.0f, 0.0f,-1.0f,

    -0.5f,-0.5f, 0.5f,    0.0f, 0.0f, 1.0f,
     0.5f,-0.5f, 0.5f,    0.0f, 0.0f, 1.0f,
     0.5f, 0.5f, 0.5f,    0.0f, 0.0f, 1.0f,
     0.5f, 0.5f, 0.5f,    0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f,    0.0f, 0.0f, 1.0f,
    -0.5f,-0.5f, 0.5f,    0.0f, 0.0f, 1.0f,

    -0.5f, 0.5f, 0.5f,   -1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f,-0.5f,   -1.0f, 0.0f, 0.0f,
    -0.5f,-0.5f,-0.5f,   -1.0f, 0.0f, 0.0f,
    -0.5f,-0.5f,-0.5f,   -1.0f, 0.0f, 0.0f,
    -0.5f,-0.5f, 0.5f,   -1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,   -1.0f, 0.0f, 0.0f,

     0.5f, 0.5f, 0.5f,    1.0f, 0.0f, 0.0f,
     0.5f, 0.5f,-0.5f,    1.0f, 0.0f, 0.0f,
     0.5f,-0.5f,-0.5f,    1.0f, 0.0f, 0.0f,
     0.5f,-0.5f,-0.5f,    1.0f, 0.0f, 0.0f,
     0.5f,-0.5f, 0.5f,    1.0f, 0.0f, 0.0f,
     0.5f, 0.5f, 0.5f,    1.0f, 0.0f, 0.0f,

    -0.5f,-0.5f,-0.5f,    0.0f,1.0f, 0.0f,
     0.5f,-0.5f,-0.5f,    0.0f,1.0f, 0.0f,
     0.5f,-0.5f, 0.5f,    0.0f,1.0f, 0.0f,
     0.5f,-0.5f, 0.5f,    0.0f,1.0f, 0.0f,
    -0.5f,-0.5f, 0.5f,    0.0f,1.0f, 0.0f,
    -0.5f,-0.5f,-0.5f,    0.0f,1.0f, 0.0f,

    -0.5f, 0.5f,-0.5f,    0.0f, -1.0f, 0.0f,
     0.5f, 0.5f,-0.5f,    0.0f, -1.0f, 0.0f,
     0.5f, 0.5f, 0.5f,    0.0f, -1.0f, 0.0f,
     0.5f, 0.5f, 0.5f,    0.0f, -1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,    0.0f, -1.0f, 0.0f,
    -0.5f, 0.5f,-0.5f,    0.0f, -1.0f, 0.0f
};

static float quadVertices[] =
{
    // pos        // uv
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f
};


bool Renderer::init(int width, int height)
{
    screenWidth  = width;
    screenHeight = height;

    // Cube VAO
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

    // Quad VAO
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
    createSphere();
    // Shaderi
    geometryShader = new Shader("geometry.vert", "geometry.frag");
    lightingShader = new Shader("lighting.vert",  "lighting.frag");
    shadowShader = new Shader("shadow.comp");
    reflectionShader = new Shader("reflection.comp");
    denoiseShader = new Shader("denoise.comp");
    buildScene();
    // G-buffer
    if (!gbuffer.init(width, height))
    {
        std::cerr << "GBuffer init failed\n";
        return false;
    }
    glGenTextures(1, &shadowTexture);
    glBindTexture( GL_TEXTURE_2D, shadowTexture);
    glTexImage2D( GL_TEXTURE_2D,0,GL_R32F,width,height, 0,GL_RED, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenTextures(1, &reflectionTexture);
    glBindTexture(GL_TEXTURE_2D, reflectionTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenTextures(1, &denoisedReflectionTexture);
    glBindTexture(GL_TEXTURE_2D, denoisedReflectionTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    return true;
}

void Renderer::destroy()
{
    delete geometryShader; geometryShader = nullptr;
    delete lightingShader; lightingShader = nullptr;
    delete shadowShader;
    shadowShader = nullptr;

    if (shadowTexture)
    {
        glDeleteTextures(1, &shadowTexture);
    }
    if (VAO)     { glDeleteVertexArrays(1, &VAO);     VAO     = 0; }
    if (VBO)     { glDeleteBuffers(1, &VBO);           VBO     = 0; }
    if (quadVAO) { glDeleteVertexArrays(1, &quadVAO); quadVAO = 0; }
    if (quadVBO) { glDeleteBuffers(1, &quadVBO);       quadVBO = 0; }
}

void Renderer::setReflectivity(float r)
{
    glUniform1f(glGetUniformLocation(geometryShader->ID, "reflectivity"), r);
}

void Renderer::buildScene()
{
    sceneObjects.clear();
    shadowObjects.clear();
    bvhNodes.clear();

    auto addObject = [&](const glm::mat4& model, const glm::vec3& color, float emission = 0.0f, float reflectivity = 0.0f, ObjectType type = Cube)
        {
            int id = sceneObjects.size();

            sceneObjects.push_back({ model, color, emission, reflectivity, type });

            ShadowObject shadow;
            shadow.model = model;
            shadow.bounds = computeBounds(model);
            shadow.id = id;

            shadowObjects.push_back(shadow);
        };

    glm::mat4 m;

    // Floor
    m = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.0f)), glm::vec3(5.0f, 0.02f, 5.0f));
    addObject(m, glm::vec3(0.8f), 0.0f, 0.0f);

    // Ceiling
    m = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 4.5f, 0.0f)), glm::vec3(5.0f, 0.02f, 5.0f));
    addObject(m, glm::vec3(1.0f), 0.0f, 0.0f);

    // Light
    m = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 4.40f, 0.0f)), glm::vec3(1.8f, 0.1f, 1.8f));
    addObject(m, glm::vec3(1.0f), 5.0f, 0.0f);

    // Left wall
    m = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(-2.5f, 2.0f, 0.0f)), glm::vec3(0.02f, 5.0f, 5.0f));
    addObject(m, glm::vec3(0.75f, 0.1f, 0.1f), 0.0f, 0.0f);

    // Right wall
    m = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(2.5f, 2.0f, 0.0f)), glm::vec3(0.02f, 5.0f, 5.0f));
    addObject(m, glm::vec3(0.1f, 0.75f, 0.1f), 0.0f, 0.0f);

    // Back wall
    m = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, -2.5f)), glm::vec3(5.0f, 5.0f, 0.02f));
    addObject(m, glm::vec3(0.8f), 0.0f, 0.0f);

	//Front wall
    m = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 2.5f)), glm::vec3(5.0f, 5.0f, 0.02f));
    addObject(m, glm::vec3(0.8f), 0.0f, 0.0f);

    //// Small box
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

    //// Smallest box
    //m = glm::mat4(1.0f);
    //m = glm::translate(m, glm::vec3(1.0f, 0.0f, 1.5f));
    //m = glm::rotate(m, glm::radians(-50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //m = glm::scale(m, glm::vec3(1.0f, 1.0f, 1.5f));
    //addObject(m, glm::vec3(0.85f), 0.0f, 0.0f);

    // Sphere 1
    m = glm::mat4(1.0f);
    m = glm::translate(m, glm::vec3(-1.0f, 0.0f, 1.0f));
    m = glm::scale(m, glm::vec3(1.0f));
    addObject(m, glm::vec3(0.85f), 0.0f, 1.0f, Sphere);

    // Sphere 2
    m = glm::mat4(1.0f);
    m = glm::translate(m, glm::vec3(0.8f, 0.9f, -0.8f));
    m = glm::scale(m, glm::vec3(1.3f));
    addObject(m, glm::vec3(0.0f, 0.0f, 1.0f), 0.0f, 1.0f, Sphere);

    // Sphere 3
    m = glm::mat4(1.0f);
    m = glm::translate(m, glm::vec3(1.3f, 0.5f, 1.4f));
    m = glm::scale(m, glm::vec3(0.7f));
    addObject(m, glm::vec3(0.85f), 0.0f, 1.0f, Sphere);

    buildBVH(0, shadowObjects.size());
    uploadBVH();
}

void Renderer::setColor(GLuint shaderID, const glm::vec3& color)
{
    glUniform3f(glGetUniformLocation(shaderID, "objectColor"),
                color.x, color.y, color.z);
}

void Renderer::setEmission(float e)
{
    glUniform1f(glGetUniformLocation(geometryShader->ID, "emission"), e);
}

void Renderer::drawCube()
{
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Renderer::drawSphere()
{
    glBindVertexArray(sphereVAO);
    glDrawElements(GL_TRIANGLES, sphereIndexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Renderer::createSphere(int stacks, int slices)
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

void Renderer::geometryPass(const glm::mat4& view, const glm::mat4& projection)
{
    glBindFramebuffer(GL_FRAMEBUFFER, gbuffer.FBO);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    geometryShader->use();

    glUniformMatrix4fv(glGetUniformLocation(geometryShader->ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(geometryShader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);

    for (const SceneObject& obj : sceneObjects)
    {
        setEmission(obj.emission);
        setReflectivity(obj.reflectivity);
        glUniformMatrix4fv(glGetUniformLocation(geometryShader->ID, "model"), 1, GL_FALSE, glm::value_ptr(obj.model));
        setColor(geometryShader->ID, obj.color);
        if (obj.type == Cube)
            drawCube();
        else
            drawSphere();
    }

    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



void Renderer::lightingPass(const glm::vec3& lightPos, const glm::vec3& viewPos)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);

    lightingShader->use();

    glUniform1i(glGetUniformLocation(lightingShader->ID, "gPosition"), 0);
    glUniform1i(glGetUniformLocation(lightingShader->ID, "gNormal"),   1);
    glUniform1i(glGetUniformLocation(lightingShader->ID, "gAlbedo"),   2);
    glUniform1i(glGetUniformLocation(lightingShader->ID, "gEmission"), 3);
    glUniform1i(glGetUniformLocation(lightingShader->ID, "shadowMask"),4);
    glUniform1i(glGetUniformLocation(lightingShader->ID, "reflectionTexture"), 5);
    glUniform1i(glGetUniformLocation(lightingShader->ID, "gReflectivity"), 6);
    glUniform3f(glGetUniformLocation(lightingShader->ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(glGetUniformLocation(lightingShader->ID, "viewPos"), viewPos.x, viewPos.y, viewPos.z);
    glUniform3f(glGetUniformLocation(lightingShader->ID, "lightColor"), 1.0f, 0.88f, 0.70f);

    glUniform3fv(glGetUniformLocation(lightingShader->ID, "redWallColor"), 1, glm::value_ptr(redWallColor));
    glUniform3fv(glGetUniformLocation(lightingShader->ID, "greenWallColor"), 1, glm::value_ptr(greenWallColor));
    glUniform1f(glGetUniformLocation(lightingShader->ID, "redWallX"), redWallX);
    glUniform1f(glGetUniformLocation(lightingShader->ID, "greenWallX"), greenWallX);
    glUniform1f(glGetUniformLocation(lightingShader->ID, "bleedStrength"), bleedStrength);

    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, gbuffer.gPosition);
    glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, gbuffer.gNormal);
    glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, gbuffer.gAlbedo);
    glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, gbuffer.gEmission);
    glActiveTexture(GL_TEXTURE4); glBindTexture(GL_TEXTURE_2D, shadowTexture);
    glActiveTexture(GL_TEXTURE5); glBindTexture(GL_TEXTURE_2D, denoisedReflectionTexture);
    glActiveTexture(GL_TEXTURE6); glBindTexture(GL_TEXTURE_2D, gbuffer.gReflectivity);

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Renderer::shadowPass(const glm::vec3& lightPos)
{
    shadowShader->use();

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, bvhSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, objectSSBO);

    glm::vec3 lightCenter = lightPos;
    glm::vec3 lightRight(1.0f, 0.0f, 0.0f);
    glm::vec3 lightUp(0.0f, 0.0f, 0.6f);

    glUniform3fv(glGetUniformLocation(shadowShader->ID, "lightCenter"), 1, glm::value_ptr(lightCenter));
    glUniform3fv(glGetUniformLocation(shadowShader->ID, "lightRight"), 1, glm::value_ptr(lightRight));
    glUniform3fv(glGetUniformLocation(shadowShader->ID, "lightUp"), 1, glm::value_ptr(lightUp));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gbuffer.gPosition);

    glUniform1i(glGetUniformLocation(shadowShader->ID, "gPosition"), 0);
    glActiveTexture(GL_TEXTURE5);                       
    glBindTexture(GL_TEXTURE_2D, gbuffer.gNormal);
    glBindImageTexture(1, shadowTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);

    glDispatchCompute((screenWidth + 15) / 16, (screenHeight + 15) / 16, 1);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);
}

void Renderer::reflectionPass(const glm::vec3& viewPos, const glm::vec3& lightPos)
{
    reflectionShader->use();

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, bvhSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, objectSSBO);

    glUniform3fv(glGetUniformLocation(reflectionShader->ID, "viewPos"), 1, glm::value_ptr(viewPos));
    glUniform3fv(glGetUniformLocation(reflectionShader->ID, "lightPos"), 1, glm::value_ptr(lightPos)); 

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gbuffer.gPosition);
    glUniform1i(glGetUniformLocation(reflectionShader->ID, "gPosition"), 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gbuffer.gNormal);
    glUniform1i(glGetUniformLocation(reflectionShader->ID, "gNormal"), 1);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, gbuffer.gReflectivity);
    glUniform1i(glGetUniformLocation(reflectionShader->ID, "gReflectivity"), 4);
    glBindImageTexture(0, reflectionTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA16F);
    glDispatchCompute((screenWidth + 15) / 16, (screenHeight + 15) / 16, 1);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);
}

void Renderer::denoisePass()
{
    denoiseShader->use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, reflectionTexture);
    glUniform1i(glGetUniformLocation(denoiseShader->ID, "reflectionTexture"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gbuffer.gPosition);
    glUniform1i(glGetUniformLocation(denoiseShader->ID, "gPosition"), 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gbuffer.gNormal);
    glUniform1i(glGetUniformLocation(denoiseShader->ID, "gNormal"), 2);

    glBindImageTexture(0, denoisedReflectionTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA16F);

    glDispatchCompute((screenWidth + 15) / 16, (screenHeight + 15) / 16, 1);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}
AABB Renderer::computeBounds(const glm::mat4& model)
{
    glm::vec3 corners[8] =
    {
        {-0.5f, -0.5f, -0.5f},
        { 0.5f, -0.5f, -0.5f},
        {-0.5f,  0.5f, -0.5f},
        { 0.5f,  0.5f, -0.5f},
        {-0.5f, -0.5f,  0.5f},
        { 0.5f, -0.5f,  0.5f},
        {-0.5f,  0.5f,  0.5f},
        { 0.5f,  0.5f,  0.5f}
    };

    glm::vec3 bmin(std::numeric_limits<float>::max());
    glm::vec3 bmax(std::numeric_limits<float>::lowest());

    for (int i = 0; i < 8; i++)
    {
        glm::vec3 p = glm::vec3(model * glm::vec4(corners[i], 1.0f));

        bmin = glm::min(bmin, p);
        bmax = glm::max(bmax, p);
    }

    return { bmin, bmax };
}

AABB mergeAABB(const AABB& a, const AABB& b)
{
    return {
        glm::min(a.min, b.min),
        glm::max(a.max, b.max)
    };
}

int Renderer::buildBVH(int begin, int end)
{
    if (begin >= end || begin < 0 || end > shadowObjects.size())
    {
        std::cerr << "Invalid BVH range: " << begin << " " << end << " size=" << shadowObjects.size() << std::endl;
        return -1;
    }

    BVHNode node;

    AABB bounds = shadowObjects[begin].bounds;

    for (int i = begin + 1; i < end; i++)
    {
        bounds = mergeAABB(bounds, shadowObjects[i].bounds);
    }

    node.bounds = bounds;

    int nodeIndex = bvhNodes.size();
    bvhNodes.push_back(node);


    int count = end - begin;


    // Leaf node
    if (count == 1)
    {
        bvhNodes[nodeIndex].leaf = true;
        bvhNodes[nodeIndex].object = shadowObjects[begin].id;
        return nodeIndex;
    }


    glm::vec3 size = bounds.max - bounds.min;

    int axis = 0;

    if (size.y > size.x && size.y > size.z)
        axis = 1;
    else if (size.z > size.x)
        axis = 2;


    std::sort(
        shadowObjects.begin() + begin,
        shadowObjects.begin() + end,
        [axis](const ShadowObject& a, const ShadowObject& b)
        {
            float ca = a.bounds.min[axis] + a.bounds.max[axis];
            float cb = b.bounds.min[axis] + b.bounds.max[axis];

            return ca < cb;
        }
    );


    int middle = (begin + end) / 2;


    int left = buildBVH(begin, middle);
    int right = buildBVH(middle, end);


    bvhNodes[nodeIndex].left = left;
    bvhNodes[nodeIndex].right = right;


    return nodeIndex;
}

void Renderer::uploadBVH()
{
    std::vector<GPUBVHNode> gpuNodes;

    gpuNodes.reserve(bvhNodes.size());

    for (const BVHNode& node : bvhNodes)
    {
        GPUBVHNode gpu;

        gpu.min = glm::vec4(node.bounds.min, 0.0f);
        gpu.max = glm::vec4(node.bounds.max, 0.0f);

        gpu.left = node.left;
        gpu.right = node.right;
        gpu.object = node.object;
        gpu.leaf = node.leaf ? 1 : 0;

        gpuNodes.push_back(gpu);
    }

    if (bvhSSBO == 0)
        glGenBuffers(1, &bvhSSBO);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, bvhSSBO);
    glBufferData( GL_SHADER_STORAGE_BUFFER, gpuNodes.size() * sizeof(GPUBVHNode), gpuNodes.data(),GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2,bvhSSBO );

    std::vector<GPUObject> gpuObjects;
    gpuObjects.reserve(sceneObjects.size());

    for (const SceneObject& obj : sceneObjects)
    {
        GPUObject gpu;
        gpu.inverseModel = glm::inverse(obj.model);
        gpu.color = glm::vec4(obj.color, 1.0f);
        gpu.type = obj.type;
        gpu.emission = obj.emission;

        gpuObjects.push_back(gpu);
    }

    if (objectSSBO == 0)
        glGenBuffers(1, &objectSSBO);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, objectSSBO);

    glBufferData(GL_SHADER_STORAGE_BUFFER, gpuObjects.size() * sizeof(GPUObject), gpuObjects.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER,3,objectSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}