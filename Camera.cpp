#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 position)
    : Position(position),
    Front(0.0f, 0.0f, -1.0f),
    Up(0.0f, 1.0f, 0.0f),
    Fov(45.0f)
{
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(
        Position,
        Position + Front,
        Up);
}

glm::mat4 Camera::GetProjectionMatrix(float aspect) const
{
    return glm::perspective(
        glm::radians(Fov),
        aspect,
        0.1f,
        100.0f);
}