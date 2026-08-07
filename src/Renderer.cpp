#include "../include/Renderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <limits>
#include <algorithm>
#include <string>
#include "../external/imgui/imgui.h"
#include <filesystem>
#include <filesystem>

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

    -0.5f,-0.5f,-0.5f,    0.0f,-1.0f, 0.0f,
     0.5f,-0.5f,-0.5f,    0.0f,-1.0f, 0.0f,
     0.5f,-0.5f, 0.5f,    0.0f,-1.0f, 0.0f,
     0.5f,-0.5f, 0.5f,    0.0f,-1.0f, 0.0f,
    -0.5f,-0.5f, 0.5f,    0.0f,-1.0f, 0.0f,
    -0.5f,-0.5f,-0.5f,    0.0f,-1.0f, 0.0f,

    -0.5f, 0.5f,-0.5f,    0.0f, 1.0f, 0.0f,
     0.5f, 0.5f,-0.5f,    0.0f, 1.0f, 0.0f,
     0.5f, 0.5f, 0.5f,    0.0f, 1.0f, 0.0f,
     0.5f, 0.5f, 0.5f,    0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,    0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f,-0.5f,    0.0f, 1.0f, 0.0f
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

    scene.createCube(cubeVertices);
    scene.createQuad(quadVertices);
    scene.createSphere();

    geometryShader = new Shader("assets/shaders/geometry.vert", "assets/shaders/geometry.frag");
    lightingShader = new Shader("assets/shaders/lighting.vert",  "assets/shaders/lighting.frag");
    shadowShader = new Shader("assets/shaders/compute/shadow.comp");
    aoShader = new Shader("assets/shaders/compute/ao.comp");
    reflectionShader = new Shader("assets/shaders/compute/reflection.comp");
    denoiseShader = new Shader("assets/shaders/compute/denoise.comp");

    scene.buildScene(bvh);

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

    glGenTextures(1, &aoTexture);
    glBindTexture(GL_TEXTURE_2D, aoTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
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
    scene.destroy();
    bvh.destroy();
    if (shadowTexture)
    {
        glDeleteTextures(1, &shadowTexture);
    }
    
}


void Renderer::geometryPass(const glm::mat4& view, const glm::mat4& projection)
{
    glBindFramebuffer(GL_FRAMEBUFFER, gbuffer.FBO);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    geometryShader->use();

    glUniformMatrix4fv(glGetUniformLocation(geometryShader->ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(geometryShader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(scene.VAO);

    for (const SceneObject& obj : bvh.sceneObjects)
    {
        glUniformMatrix4fv(glGetUniformLocation(geometryShader->ID, "model"), 1, GL_FALSE, glm::value_ptr(obj.model));
        scene.setColor(geometryShader->ID, obj.color);
        scene.setEmission(geometryShader->ID, obj.emission);
        scene.setReflectivity(geometryShader->ID, obj.reflectivity);
        if (obj.type == Cube)
            scene.drawCube();
        else
            scene.drawSphere();
    }

    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



void Renderer::lightingPass( const glm::vec3& viewPos)
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
    glUniform1i(glGetUniformLocation(lightingShader->ID, "aoMask"), 7);

    glUniform1i(glGetUniformLocation(lightingShader->ID, "lightCount"), (int)bvh.emissiveLights.size());
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, bvh.lightSSBO);

    glUniform3f(glGetUniformLocation(lightingShader->ID, "viewPos"), viewPos.x, viewPos.y, viewPos.z);
    glUniform3f(glGetUniformLocation(lightingShader->ID, "lightColor"), 1.0f, 0.88f, 0.70f);

    glUniform3fv(glGetUniformLocation(lightingShader->ID, "redWallColor"), 1, glm::value_ptr(redWallColor));
    glUniform3fv(glGetUniformLocation(lightingShader->ID, "greenWallColor"), 1, glm::value_ptr(greenWallColor));
    glUniform1f(glGetUniformLocation(lightingShader->ID, "redWallX"), redWallX);
    glUniform1f(glGetUniformLocation(lightingShader->ID, "greenWallX"), greenWallX);
    glUniform1f(glGetUniformLocation(lightingShader->ID, "bleedStrength"), bleedStrength);

    glUniform1i(glGetUniformLocation(lightingShader->ID, "renderMode"), static_cast<int>(renderMode));

    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, gbuffer.gPosition);
    glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, gbuffer.gNormal);
    glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, gbuffer.gAlbedo);
    glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, gbuffer.gEmission);
    glActiveTexture(GL_TEXTURE4); glBindTexture(GL_TEXTURE_2D, shadowTexture);
    glActiveTexture(GL_TEXTURE5); glBindTexture(GL_TEXTURE_2D, denoisedReflectionTexture);
    glActiveTexture(GL_TEXTURE6); glBindTexture(GL_TEXTURE_2D, gbuffer.gReflectivity);
    glActiveTexture(GL_TEXTURE7);glBindTexture(GL_TEXTURE_2D, aoTexture);

    glBindVertexArray(scene.quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Renderer::shadowPass()
{
    shadowShader->use();

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, bvh.bvhSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, bvh.objectSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, bvh.lightSSBO);   

    glUniform1i(glGetUniformLocation(shadowShader->ID, "lightCount"), (int)bvh.emissiveLights.size()); 

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gbuffer.gPosition);

    glUniform1i(glGetUniformLocation(shadowShader->ID, "gPosition"), 0);
    glActiveTexture(GL_TEXTURE5);                       
    glBindTexture(GL_TEXTURE_2D, gbuffer.gNormal);
    glBindImageTexture(1, shadowTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);

    glDispatchCompute((screenWidth + 15) / 16, (screenHeight + 15) / 16, 1);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);
}

void Renderer::aoPass(const glm::vec3& lightPos)
{
    aoShader->use();

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, bvh.bvhSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, bvh.objectSSBO);

    glm::vec3 lightCenter = lightPos;
    glm::vec3 lightRight(1.0f, 0.0f, 0.0f);
    glm::vec3 lightUp(0.0f, 0.0f, 0.6f);

    glUniform3fv(glGetUniformLocation(aoShader->ID, "lightCenter"), 1, glm::value_ptr(lightCenter));
    glUniform3fv(glGetUniformLocation(aoShader->ID, "lightRight"), 1, glm::value_ptr(lightRight));
    glUniform3fv(glGetUniformLocation(aoShader->ID, "lightUp"), 1, glm::value_ptr(lightUp));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gbuffer.gPosition);

    glUniform1i(glGetUniformLocation(aoShader->ID, "gPosition"), 0);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, gbuffer.gNormal);
    glBindImageTexture(1, aoTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);

    glDispatchCompute((screenWidth + 15) / 16, (screenHeight + 15) / 16, 1);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);
}

void Renderer::reflectionPass(const glm::vec3& viewPos)
{
    reflectionShader->use();

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, bvh.bvhSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, bvh.objectSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, bvh.lightSSBO);

    glUniform1i(glGetUniformLocation(reflectionShader->ID, "lightCount"), (int)bvh.emissiveLights.size());
    glUniform3fv(glGetUniformLocation(reflectionShader->ID, "viewPos"), 1, glm::value_ptr(viewPos));

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

void Renderer::setRenderMode(RenderMode mode)
{
    renderMode = mode;
}

RenderMode Renderer::getRenderMode() const
{
    return renderMode;
}

const char* objectNames[] =
{
    "Floor",
    "Ceiling",
    "Light",
    "Left Wall",
    "Right Wall",
    "Back Wall",
    "Sphere 1",
    "Sphere 2",
    "Sphere 3"
};

void Renderer::drawMaterialUI()
{
    ImGui::Text("Cornell Box Controls");

    for (int i = 0; i < (int)bvh.sceneObjects.size(); i++)
    {
        SceneObject& obj = bvh.sceneObjects[i];

        ImGui::Text("%s", objectNames[i]);

        std::string reflectivityLabel = "Reflectivity##" + std::to_string(i);
        if (ImGui::SliderFloat(reflectivityLabel.c_str(), &obj.reflectivity, 0.0f, 1.0f))
        {
            bvh.uploadObjects();
        }

        std::string emissionLabel = "Emission##" + std::to_string(i);
        if (ImGui::SliderFloat(emissionLabel.c_str(), &obj.emission, 0.0f, 20.0f))
        {
            bvh.uploadObjects();
        }

        ImGui::Separator();
    }
}