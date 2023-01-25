#pragma once

#include <vector>

#include "Renderer/Light.h"
#include "Renderer/ModelInstance.h"
#include "Renderer/ModelLoader.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/Animator.h"

#include <reactphysics3d/reactphysics3d.h>

struct ShaderData
{
    Light lights[8];
    mat4x4 projectionMatrix;
    mat4x4 viewMatrix;
    Vector4 camPos;
};

struct ShaderSkeleton
{
    mat4x4 boneMat[100];
};

class Renderer
{
public:
	Renderer();
	~Renderer();

    void SetStaticModels(const std::vector<ModelInstance>& inStaticModels);
    void SetStaticLights(const std::vector<Light>& inStaticLights);
    void SetPhysicsRenderer(reactphysics3d::DebugRenderer* inRenderer) { physicsRenderer = inRenderer; }

    void ClearStaticModels() { staticModels.clear(); }
    void ClearStaticLights() { staticLights.clear(); }

    void Render(const mat4x4& view, const mat4x4& proj, const Vector2& screenSize, std::deque<GameObject*>& dynamicObjects, const std::deque<Light>& dynamicLights, bool isPlaying);
    void DrawPicking(const mat4x4& view, const mat4x4& proj,std::deque<GameObject*>& dynamicObjects);

    unsigned int cbo = 0;
    std::vector<Light> staticLights = {};
    std::vector<ModelInstance> staticModels = {};

    GLuint trianglesVAO, trianglesVBO = 0;
    GLuint linesVAO, linesVBO = 0;
    GLuint pickingVAO = 0;
    GLuint pickingVBO = 0;
    GLuint pickingEBO = 0;
    GLuint pickingFBO = 0;
    GLuint pickingCBO = 0;
    GLuint pickingRBO = 0;
private:
    reactphysics3d::DebugRenderer* physicsRenderer = nullptr;

    void UpdateFramebuffer(const Vector2& newVpSize);

    void RenderPhysics();

    ShaderData shaderData = {};
    unsigned int shaderDataUBO = 0;

    unsigned int fbo = 0;
    unsigned int rbo = 0;

    Vector2 vpSize = {1, 1};


    //Shader outLineShader;
    ShaderSkeleton shaderSkeleton = {};
    unsigned int shaderSkeletonUBO = 0;


    double lastFrame = 0.f;

    Shader shader;
    Shader animShader;
    Shader outLineShader;
    Shader wireframeShader;
    Shader picking;

    Texture* tex = nullptr;
};
