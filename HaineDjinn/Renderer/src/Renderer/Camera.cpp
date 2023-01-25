
#include <iostream>
#include "Renderer/Camera.h"
#include "../../../Engine/include/Game/InputSystem.h"
#include "Core/Constants.h"
#include "Core/Time.h"

Camera::Camera(int width, int height)
{
    aspect = (float)width / (float)height;

    position = { 9.897f, 3.095f, -4.730f, 0.f };
}

void Camera::FpsMove(float deltaTime, const CameraInputs& inputs)
{
    yaw -= inputs.deltaX * MOUSE_SENSITIVITY;
    pitch += inputs.deltaY * MOUSE_SENSITIVITY;

    //position = toFollow->position + offsetFromTarget;
    
    //toFollow->rotation.y = yaw;
}

void Camera::FreeMove(float deltaTime, const CameraInputs& inputs)
{
    if (!blocked)
    {
        Vector2 delta = InputSystem::GetMouseDelta();
        yaw += delta.x * MOUSE_SENSITIVITY;
        pitch += delta.y * MOUSE_SENSITIVITY;

    float forward_movement = -InputSystem::GetAxis("Vertical") * CAMERA_SPEED * (float)(Time::DeltaTime());
    float horizontal_movement = InputSystem::GetAxis("Horizontal") * CAMERA_SPEED * (float)(Time::DeltaTime());
    float vertical_movement = InputSystem::GetAxis("Upwards") * CAMERA_SPEED * (float)(Time::DeltaTime());

        Vector2 forwardVec = { 0.f, 1.f };
        Vector2 rightVec = { 1.f, 0.f };
        vectRotate(forwardVec, yaw);
        vectRotate(rightVec, yaw);
        position.x += (forwardVec.x * forward_movement) + (rightVec.x * horizontal_movement);
        position.z += (forwardVec.y * forward_movement) + (rightVec.y * horizontal_movement);

        position.y += vertical_movement;

    }
}

void Camera::FollowMove()
{
    //if (toFollow == nullptr) 
    //{
    //    std::cout << "a";
    //    return;
    //}

    //position = position + (((toFollow->position + offsetFromTarget) - position.xyz) * CAMERA_LERP);
}

void Camera::Update(float deltaTime, const CameraInputs& inputs)
{
    switch (state)
    {
    case CameraState::FREE:
        FreeMove(deltaTime, inputs);
        break;
    case CameraState::FIRST_PERSON:
        FpsMove(deltaTime, inputs);
        break;
    case CameraState::LOCK:
        break;
    case CameraState::FOLLOW:
        FollowMove();
        break;
    default:
        break;
    }

    pitch = Maths::clamp(pitch, -CAMERA_MAX_PITCH * M_PI / 180.f, CAMERA_MAX_PITCH * M_PI / 180.f);
    //model.position = {position.x, position.y, position.z};
    //model.rotation = { -pitch, yaw, 0.f};
}

mat4x4 Camera::GetViewMatrix()
{
    return  Maths::mat4::RotateX(pitch) * Maths::mat4::RotateY(-yaw) * Maths::mat4::Translate(-position.xyz);
    //return mat4::identity();
}

mat4x4 Camera::GetProjectionMatrix()
{
    //aspect = aspectRatio;

    if (isPerspective)
    {
        return Maths::mat4::perspective(fovY, aspect, nearVal, farVal);
    }
    else
        return Maths::mat4::orthographic(fovY, aspect, nearVal, farVal);
}
