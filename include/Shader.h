#pragma once

#include <string>

class Shader
{
private:
    std::string loadShaderFile(const char* path);

public:
    unsigned int ID;

    Shader(const char* vertexPath,
        const char* fragmentPath);

    Shader(const char* computePath);

    void use() const;
};