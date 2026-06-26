#pragma once

#include <string>

class Shader
{
public:
    unsigned int ID;

    Shader(const char* vertexPath,
        const char* fragmentPath);

    Shader(const char* computePath);

    void use() const;
};