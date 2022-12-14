#include "PerspectiveCamera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Application.h"
#include "GLFW/glfw3.h"

PerspectiveCamera::PerspectiveCamera(const Frustrum& frustrum, const glm::vec3& position, const glm::vec3& lookAt,
                                     const glm::vec3& upVector)
{
    RecalculateMatrix();
    Position = {0.0,14.5,0.01};
}

void PerspectiveCamera::RecalculateMatrix()
{
    Camera::RecalculateMatrix();

    ProjectionMatrix = glm::perspective(CameraFrustrum.angle,
                                        CameraFrustrum.width / CameraFrustrum.height, CameraFrustrum.near,
                                        CameraFrustrum.far);

    ViewMatrix = glm::lookAt(Position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}
