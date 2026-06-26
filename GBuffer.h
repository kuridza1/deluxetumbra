#pragma once

#include <glad/glad.h>
#include <iostream>

class GBuffer
{
public:
    GLuint FBO       = 0;
    GLuint gPosition = 0;
    GLuint gNormal   = 0;
    GLuint gAlbedo   = 0;
    GLuint gEmission = 0;
    GLuint rboDepth  = 0;

    GBuffer() = default;

    bool init(int width, int height);

    void destroy();

    ~GBuffer() { destroy(); }
};
