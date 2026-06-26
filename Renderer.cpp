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

    // G-buffer
    if (!gbuffer.init(width, height))
    {
        std::cerr << "GBuffer init failed\n";
        return false;
    }

    return true;
}

void Renderer::destroy()
{
    delete geometryShader; geometryShader = nullptr;
    delete lightingShader; lightingShader = nullptr;

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
                   glm::vec3(5.0f, 1.0f, 5.0f));
    drawObject(m, glm::vec3(0.8f));

    // Ceiling
    m = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 4.5f, 0.0f)),
                   glm::vec3(5.0f, 1.0f, 5.0f));
    drawObject(m, glm::vec3(0.8f));

    // Light panel (emissive)
    m = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 3.99f, 0.0f)),
                   glm::vec3(1.2f, 0.02f, 1.2f));
    drawObject(m, glm::vec3(1.0f), 15.0f);

    // Left wall (red)
    m = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(-2.5f, 2.0f, 0.0f)),
                   glm::vec3(1.0f, 5.0f, 5.0f));
    drawObject(m, glm::vec3(0.75f, 0.1f, 0.1f));

    // Right wall (green)
    m = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(2.5f, 2.0f, 0.0f)),
                   glm::vec3(1.0f, 5.0f, 5.0f));
    drawObject(m, glm::vec3(0.1f, 0.75f, 0.1f));

    // Back wall
    m = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, -2.5f)),
                   glm::vec3(5.0f, 5.0f, 1.0f));
    drawObject(m, glm::vec3(0.8f));

    // Small box
    m = glm::mat4(1.0f);
    m = glm::translate(m, glm::vec3(-0.9f, 0.5f, 0.8f));
    m = glm::rotate(m, glm::radians(-20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    m = glm::scale(m, glm::vec3(1.0f, 2.0f, 1.0f));
    drawObject(m, glm::vec3(0.85f));

    // Large box
    m = glm::mat4(1.0f);
    m = glm::translate(m, glm::vec3(1.0f, 1.0f, -0.6f));
    m = glm::rotate(m, glm::radians(18.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    m = glm::scale(m, glm::vec3(1.3f, 3.0f, 1.3f));
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

    glUniform3f(glGetUniformLocation(lightingShader->ID, "lightPos"),
                lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(glGetUniformLocation(lightingShader->ID, "viewPos"),
                viewPos.x, viewPos.y, viewPos.z);

    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, gbuffer.gPosition);
    glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, gbuffer.gNormal);
    glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, gbuffer.gAlbedo);
    glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, gbuffer.gEmission);

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
