#version 430 core
const int LIGHTS_NUMBER = 8; // Note: this value must be the same as in ShaderData (RendererBasic.hpp)

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;
layout(location = 3) in ivec4 boneId; 
layout(location = 4) in vec4 weight;

struct Light
{
    vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
    vec4 attenuation;
    bool enabled;
};

layout(std140, binding = 0) uniform shaderData
{
    Light lights[LIGHTS_NUMBER];
    mat4 projectionMat;
    mat4 viewMat;
    vec4 camPos;
};

// Uniforms
uniform mat4 uModelMat;

// Varyings (variables that are passed to fragment shader with perspective interpolation)
out vec2 vUV;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;

uniform mat4 finalBonesMatricesOld[MAX_BONES];

layout(std140, binding = 1) uniform shaderSkeleton
{
    mat4 finalBonesMatrices[MAX_BONES];
};

void main()
{
    vec4 totalPosition = vec4(0.0f);
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
    {
        if(boneId[i] == -1)
        {
            continue;
        }

        if(boneId[i] >=MAX_BONES) 
        {
            totalPosition = vec4(aPosition,1.0f);
            break;
        }

        vec4 localPosition = finalBonesMatrices[boneId[i]] * vec4(aPosition,1.0f);
        totalPosition += localPosition * weight[i];
        vec3 localNormal = mat3(finalBonesMatrices[boneId[i]]) * aNormal;
   }
	
    vUV = aUV;

    if (totalPosition == vec4(0.0f))
    {
        gl_Position = projectionMat * viewMat * uModelMat * vec4(aPosition, 1.0);
    }
    else
        gl_Position = projectionMat * (viewMat * uModelMat) * totalPosition;
}