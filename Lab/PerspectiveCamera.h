#ifndef PERSPECTIVECAMERA_H_
#define PERSPECTIVECAMERA_H_

#include "Camera.h"

#include <array>

class PerspectiveCamera : public Camera
{
public:
    struct Frustrum
    {
        float angle;
        float width;
        float height;
        float near;
        float far;

        Frustrum()
        {
        }

        Frustrum(float angle, float width, float height, float near, float far)
            : angle(angle),
              width(width),
              height(height),
              near(near),
              far(far)
        {
        }
    };

public:
    PerspectiveCamera(const Frustrum& frustrum = {45.0f, -1.0f, 1.0f, 1.0f, -1.0f},
                      const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f),
                      const glm::vec3& lookAt = glm::vec3(-1.0f),
                      const glm::vec3& upVector = glm::vec3(0.0f, 1.0f, 0.0f));
    ~PerspectiveCamera() = default;

    PerspectiveCamera(const PerspectiveCamera& camera): Camera(camera)
    {
        this->LookAt = camera.LookAt;
        this->UpVector = camera.UpVector;
        this->CameraFrustrum = camera.CameraFrustrum;
    }

    void SetFrustrum(const Frustrum& frustrum)
    {
        this->CameraFrustrum = frustrum;
        this->RecalculateMatrix();
    }

    void SetLookAt(const glm::vec3& lookAt)
    {
        this->LookAt = lookAt;
        this->RecalculateMatrix();
    }

    void SetUpVector(const glm::vec3& upVector)
    {
        this->UpVector = upVector;
        this->RecalculateMatrix();
    }

    void SetMovement(const float deltaTime, const float velocity)
    {
        this->Movement.x += velocity * deltaTime;
        this->Movement.z += velocity * deltaTime;

        this->Position = glm::vec3(0.0f, sin(Movement.x) * radius, cos(Movement.z) * radius);
        // this->Position = glm::vec3(sin(Movement.x) * radius, 5.0f, cos(Movement.z) * radius);
        this->RecalculateMatrix();
    }

    void SetAngle(const float angle)
    {
        CameraFrustrum.angle = angle;
        this->RecalculateMatrix();
    }

    float GetAngle() const
    {
        return CameraFrustrum.angle;
    }


protected:
    void RecalculateMatrix() override;

protected:
    glm::vec3 LookAt;
    glm::vec3 UpVector;
    Frustrum CameraFrustrum;

    glm::vec3 Movement;
    float radius = 10.0f;
};

#endif // PERSPECTIVECAMERA_H_
