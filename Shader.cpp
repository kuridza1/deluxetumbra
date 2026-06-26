#include "Shader.h"

#include <glad/glad.h>

#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* vertexPath,
    const char* fragmentPath)
{
    std::ifstream vertexFile(vertexPath);
    std::ifstream fragmentFile(fragmentPath);

    if (!vertexFile.is_open())
    {
        std::cerr << "Failed to open vertex shader: "
            << vertexPath << std::endl;
        return;
    }

    if (!fragmentFile.is_open())
    {
        std::cerr << "Failed to open fragment shader: "
            << fragmentPath << std::endl;
        return;
    }

    std::stringstream vertexStream;
    std::stringstream fragmentStream;

    vertexStream << vertexFile.rdbuf();
    fragmentStream << fragmentFile.rdbuf();

    std::string vertexCode = vertexStream.str();
    std::string fragmentCode = fragmentStream.str();

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    GLuint vertexShader =
        glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(
        vertexShader,
        1,
        &vShaderCode,
        nullptr);

    glCompileShader(vertexShader);

    GLint success;
    char infoLog[512];

    glGetShaderiv(
        vertexShader,
        GL_COMPILE_STATUS,
        &success);

    if (!success)
    {
        glGetShaderInfoLog(
            vertexShader,
            512,
            nullptr,
            infoLog);

        std::cerr
            << "Vertex shader compilation failed:\n"
            << infoLog << std::endl;
    }

    GLuint fragmentShader =
        glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(
        fragmentShader,
        1,
        &fShaderCode,
        nullptr);

    glCompileShader(fragmentShader);

    glGetShaderiv(
        fragmentShader,
        GL_COMPILE_STATUS,
        &success);

    if (!success)
    {
        glGetShaderInfoLog(
            fragmentShader,
            512,
            nullptr,
            infoLog);

        std::cerr
            << "Fragment shader compilation failed:\n"
            << infoLog << std::endl;
    }

    ID = glCreateProgram();

    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);

    glLinkProgram(ID);

    glGetProgramiv(
        ID,
        GL_LINK_STATUS,
        &success);

    if (!success)
    {
        glGetProgramInfoLog(
            ID,
            512,
            nullptr,
            infoLog);

        std::cerr
            << "Shader program linking failed:\n"
            << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::Shader(const char* computePath)
{
    std::ifstream file(computePath);

    std::stringstream stream;
    stream << file.rdbuf();

    std::string code = stream.str();
    const char* src = code.c_str();

    GLuint shader =
        glCreateShader(GL_COMPUTE_SHADER);

    glShaderSource(shader,
        1,
        &src,
        nullptr);

    glCompileShader(shader);

    ID = glCreateProgram();

    glAttachShader(ID, shader);
    glLinkProgram(ID);

    glDeleteShader(shader);
}

void Shader::use() const
{
    glUseProgram(ID);
}