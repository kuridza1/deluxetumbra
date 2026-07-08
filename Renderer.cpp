#include "Renderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


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

    // Shaderi
    geometryShader = new Shader("geometry.vert", "geometry.frag");
    lightingShader = new Shader("lighting.vert",  "lighting.frag");
    shadowShader = new Shader("shadow.comp");
    // G-buffer
    if (!gbuffer.init(width, height))
    {
        std::cerr << "GBuffer init failed\n";
        return false;
    }
    glGenTextures(1, &shadowTexture);

    glBindTexture(
        GL_TEXTURE_2D,
        shadowTexture);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_R32F,
        width,
        height,
        0,
        GL_RED,
        GL_FLOAT,
        nullptr);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_NEAREST);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);
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
        glDeleteTextures(
            1,
            &shadowTexture);
    }
    if (VAO)     { glDeleteVertexArrays(1, &VAO);     VAO     = 0; }
    if (VBO)     { glDeleteBuffers(1, &VBO);           VBO     = 0; }
    if (quadVAO) { glDeleteVertexArrays(1, &quadVAO); quadVAO = 0; }
    if (quadVBO) { glDeleteBuffers(1, &quadVBO);       quadVBO = 0; }
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


void Renderer::geometryPass(const glm::mat4& view, const glm::mat4& projection)
{
    glBindFramebuffer(GL_FRAMEBUFFER, gbuffer.FBO);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    geometryShader->use();

    glUniformMatrix4fv(glGetUniformLocation(geometryShader->ID, "view"),
                       1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(geometryShader->ID, "projection"),
                       1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);

    auto drawObject = [&](const glm::mat4& model,
                          const glm::vec3& color,
                          float emission = 0.0f)
    {
        setEmission(emission);
        glUniformMatrix4fv(glGetUniformLocation(geometryShader->ID, "model"),
                           1, GL_FALSE, glm::value_ptr(model));
        setColor(geometryShader->ID, color);
        drawCube();
    };

    glm::mat4 m;

    // Floor
    m = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.0f)),
                   glm::vec3(5.0f, 0.02f, 5.0f));
    drawObject(m, glm::vec3(0.8f));

    // Ceiling
    m = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 4.5f, 0.0f)),
                   glm::vec3(5.0f, 0.02f, 5.0f));
    drawObject(m, glm::vec3(0.8f));

    // Light panel (emissive)
    m = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 4.49f, 0.0f)),
                   glm::vec3(1.2f, 0.02f, 1.2f));
    drawObject(m, glm::vec3(1.0f), 0.25f);

    // Left wall (red)
    m = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(-2.5f, 2.0f, 0.0f)),
                   glm::vec3(0.02f, 5.0f, 5.0f));
    drawObject(m, glm::vec3(0.75f, 0.1f, 0.1f));

    // Right wall (green)
    m = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(2.5f, 2.0f, 0.0f)),
                   glm::vec3(0.02f, 5.0f, 5.0f));
    drawObject(m, glm::vec3(0.1f, 0.75f, 0.1f));

    // Back wall
    m = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, -2.5f)),
                   glm::vec3(5.0f, 5.0f, 0.02f));
    drawObject(m, glm::vec3(0.8f));

    // Small box
    m = glm::mat4(1.0f);
    m = glm::translate(m, glm::vec3(-0.9f, 0.27f, 1.2f));
    m = glm::rotate(m, glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    m = glm::scale(m, glm::vec3(1.5f, 1.5f, 1.5f));

    smallBoxModel = m;

    drawObject(m, glm::vec3(0.85f));

    // Large box
    m = glm::mat4(1.0f);
    m = glm::translate(m, glm::vec3(0.6f, 1.0f, -0.8f));
    m = glm::rotate(m, glm::radians(-18.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    m = glm::scale(m, glm::vec3(1.3f, 3.0f, 1.3f));

	largeBoxModel = m;

    drawObject(m, glm::vec3(0.85f));

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
    glUniform3f(glGetUniformLocation(lightingShader->ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(glGetUniformLocation(lightingShader->ID, "viewPos"), viewPos.x, viewPos.y, viewPos.z);
    glUniform3f(glGetUniformLocation(lightingShader->ID, "lightColor"), 1.0f, 0.95f, 0.8f);

    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, gbuffer.gPosition);
    glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, gbuffer.gNormal);
    glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, gbuffer.gAlbedo);
    glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, gbuffer.gEmission);
    glActiveTexture(GL_TEXTURE4); glBindTexture(GL_TEXTURE_2D, shadowTexture);
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Renderer::shadowPass(const glm::vec3& lightPos)
{
    shadowShader->use();

	glUniformMatrix4fv(glGetUniformLocation(shadowShader->ID, "smallBoxModel"), 1, GL_FALSE, glm::value_ptr(smallBoxModel));
    glUniformMatrix4fv(glGetUniformLocation(shadowShader->ID, "largeBoxModel"), 1, GL_FALSE, glm::value_ptr(largeBoxModel));

    glUniform3f(glGetUniformLocation(shadowShader->ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gbuffer.gPosition);

    glUniform1i(glGetUniformLocation(shadowShader->ID, "gPosition"), 0);
    glBindImageTexture(1, shadowTexture,0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);
    glDispatchCompute((screenWidth + 15) / 16, (screenHeight + 15) / 16, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}