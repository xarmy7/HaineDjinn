#include "Renderer/Bone.h"
#include "Core/Maths.h"

#include <vector>
#include <cassert>
#include <string>

/*reads keyframes from aiNodeAnim*/
Bone::Bone(const std::string& name, int ID, const aiNodeAnim* channel)
    :
    m_Name(name),
    m_ID(ID)
{
    m_NumPositions = channel->mNumPositionKeys;

    m_LocalTransform = mat4::Identity();

    for (int positionIndex = 0; positionIndex < m_NumPositions; ++positionIndex)
    {
        aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
        double timeStamp = channel->mPositionKeys[positionIndex].mTime;
        KeyPosition data;
        //data.position = AssimpGLMHelpers::GetGLMVec(aiPosition);
        data.position = GetVec3(aiPosition);
        data.timeStamp = timeStamp;
        m_Positions.push_back(data);
    }

    m_NumRotations = channel->mNumRotationKeys;
    for (int rotationIndex = 0; rotationIndex < m_NumRotations; ++rotationIndex)
    {
        aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
        double timeStamp = channel->mRotationKeys[rotationIndex].mTime;
        KeyRotation data;
        data.orientation = GetQuaternionFromAssimp(aiOrientation);
        data.timeStamp = timeStamp;
        m_Rotations.push_back(data);
    }

    m_NumScalings = channel->mNumScalingKeys;
    for (int keyIndex = 0; keyIndex < m_NumScalings; ++keyIndex)
    {
        aiVector3D scale = channel->mScalingKeys[keyIndex].mValue;
        double timeStamp = channel->mScalingKeys[keyIndex].mTime;
        KeyScale data;
        data.scale = GetVec3(scale);
        data.timeStamp = timeStamp;
        m_Scales.push_back(data);
    }
}

void Bone::Update(double animationTime)
{
    mat4x4 translation = InterpolatePosition(animationTime);
    mat4x4 rotation = InterpolateRotation(animationTime);
    mat4x4 scale = InterpolateScaling(animationTime);
    m_LocalTransform = translation * rotation * scale;
}

int Bone::GetPositionIndex(double animationTime)
{
    for (int index = 0; index < m_NumPositions - 1; ++index)
    {
        if (animationTime < m_Positions[static_cast<size_t>(index) + 1].timeStamp)
            return index;
    }
    assert(0);
    return 0;
}

int Bone::GetRotationIndex(double animationTime)
{
    for (int index = 0; index < m_NumRotations - 1; ++index)
    {
        if (animationTime < m_Rotations[static_cast<size_t>(index) +1].timeStamp)
            return index;
    }
    assert(0);
    return 0;
}

int Bone::GetScaleIndex(double animationTime)
{
    for (int index = 0; index < m_NumScalings - 1; ++index)
    {
        if (animationTime < m_Scales[static_cast<size_t>(index) + 1].timeStamp)
            return index;
    }
    assert(0);
    return 0;
}

double Bone::GetScaleFactor(double lastTimeStamp, double nextTimeStamp, double animationTime)
{
    double scaleFactor = 0.0f;
    double midWayLength = animationTime - lastTimeStamp;
    double framesDiff = nextTimeStamp - lastTimeStamp;
    scaleFactor = midWayLength / framesDiff;
    return scaleFactor;
}

mat4x4 Bone::InterpolatePosition(double animationTime)
{
    if (1 == m_NumPositions)
        return Maths::mat4::Translate(m_Positions[0].position);

    int p0Index = GetPositionIndex(animationTime);
    int p1Index = p0Index + 1;
    double scaleFactor = GetScaleFactor(m_Positions[p0Index].timeStamp,
        m_Positions[p1Index].timeStamp, animationTime);
    Vector3 finalPosition = lerp(m_Positions[p0Index].position,
        m_Positions[p1Index].position, scaleFactor);
    return Maths::mat4::Translate(finalPosition);
}

mat4x4 Bone::InterpolateRotation(double animationTime)
{
    if (1 == m_NumRotations)
    {
        Quaternion rotation = QuaternionNormalize(m_Rotations[0].orientation);
        return  Maths::mat4::RotateQuat(rotation);
    }

    int p0Index = GetRotationIndex(animationTime);
    int p1Index = p0Index + 1;
    double scaleFactor = GetScaleFactor(m_Rotations[p0Index].timeStamp,
        m_Rotations[p1Index].timeStamp, animationTime);
    Quaternion finalRotation = Slerp(m_Rotations[p0Index].orientation,
        m_Rotations[p1Index].orientation, scaleFactor);
    finalRotation = QuaternionNormalize(finalRotation);
    return Maths::mat4::RotateQuat(finalRotation);
}

mat4x4 Bone::InterpolateScaling(double animationTime)
{
    if (1 == m_NumScalings)
        return Maths::mat4::Scale(m_Scales[0].scale);

    int p0Index = GetScaleIndex(animationTime);
    int p1Index = p0Index + 1;
    double scaleFactor = GetScaleFactor(m_Scales[p0Index].timeStamp,
        m_Scales[p1Index].timeStamp, animationTime);
    Vector3 finalScale = lerp(m_Scales[p0Index].scale, m_Scales[p1Index].scale
        , scaleFactor);
    return Maths::mat4::Scale(finalScale);
}