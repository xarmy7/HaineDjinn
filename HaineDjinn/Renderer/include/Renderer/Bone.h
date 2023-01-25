#pragma once

#include "Core/Maths.h"

#include <vector>
#include <string>

using namespace Maths;

struct KeyPosition
{
    Vector3 position = {};
    double timeStamp = 0.f;
};

struct KeyRotation
{
    Quaternion orientation = {};
    double timeStamp = 0.f;
};

struct KeyScale
{
    Vector3 scale = {};
    double timeStamp = 0.f;
};

class Bone
{
private:
    std::vector<KeyPosition> m_Positions = {};
    std::vector<KeyRotation> m_Rotations = {};
    std::vector<KeyScale> m_Scales = {};
    int m_NumPositions = 0;
    int m_NumRotations = 0;
    int m_NumScalings = 0;

    mat4x4 m_LocalTransform = {};
    std::string m_Name = {};
    int m_ID = 0;

public:
    const mat4x4& GetLocalTransform() const  { return m_LocalTransform; }
    const std::string& GetBoneName() const { return m_Name; }
    int GetBoneID() const { return m_ID; }

    Bone(const std::string& name, int ID, const aiNodeAnim* channel);

    /*interpolates  b/w positions,rotations & scaling keys based on the curren time of
    the animation and prepares the local transformation matrix by combining all keys
    tranformations*/
    void Update(double animationTime);

    /* Gets the current index on mKeyPositions to interpolate to based on
    the current animation time*/
    int GetPositionIndex(double animationTime);

    /* Gets the current index on mKeyRotations to interpolate to based on the
    current animation time*/
    int GetRotationIndex(double animationTime);

    /* Gets the current index on mKeyScalings to interpolate to based on the
    current animation time */
    int GetScaleIndex(double animationTime);

    /* Gets normalized value for Lerp & Slerp*/
    double GetScaleFactor(double lastTimeStamp, double nextTimeStamp, double animationTime);

    /*figures out which position keys to interpolate b/w and performs the interpolation
        and returns the translation matrix*/
    mat4x4 InterpolatePosition(double animationTime);

    /*figures out which rotations keys to interpolate b/w and performs the interpolation
        and returns the rotation matrix*/
    mat4x4 InterpolateRotation(double animationTime);

    /*figures out which scaling keys to interpolate b/w and performs the interpolation
        and returns the scale matrix*/
    mat4x4 InterpolateScaling(double animationTime);
};