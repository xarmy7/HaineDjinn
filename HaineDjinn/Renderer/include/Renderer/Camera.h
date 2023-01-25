#pragma once

#include "Core/Maths.h"

struct CameraInputs
{
    float deltaX;
    float deltaY;
    bool moveForward;
    bool moveBackward;
    bool moveLeft;
    bool moveRight;
    bool moveUpwards;
    bool moveDownwards;
    // ... and more if needed
};

enum class CameraState
{
    FREE,
    FIRST_PERSON,
    LOCK,
    FOLLOW
};

struct Camera
{
    Camera(int width, int height);

    void Update(float deltaTime, const CameraInputs& inputs);
    void FreeMove(float deltaTime, const CameraInputs& inputs);
    void FpsMove(float deltaTime, const CameraInputs& inputs);
    void FollowMove();
    mat4x4 GetViewMatrix();
    mat4x4 GetProjectionMatrix();
    
    CameraState state = CameraState::FOLLOW;

    //Model model = Model();
    //Model* toFollow = nullptr;

    bool blocked = true;

    float yaw = 45;
    float pitch = 0.f;
    Vector4 position = { 0.f, 0.f, -1.f, 0.f };
    Vector3 offsetFromTarget = {0.f, 3.5f, 9.f};

    bool isMainCamera = false;
    std::string name;

    float aspect = 16.f/9.f;
    float fovY = 90.f;
    float nearVal = 0.01f; // TODO: if near is lower, everything breaks
    float farVal = 80000.f;

    bool isPerspective = true;
};