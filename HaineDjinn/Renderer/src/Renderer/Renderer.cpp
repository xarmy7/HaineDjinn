
#include <vector>

#include "Renderer/Renderer.h"
#include "Core/Transform.h"
#include "Core/Debug.h"
#include "Core/Time.h"

#include <GLFW/glfw3.h>

Renderer::Renderer() : shader("Shaders/phong"), animShader("Shaders/animation"), outLineShader("Shader/white"), wireframeShader("Shaders/wireframePhys"), picking("Shaders/picking")
{
	// Generates ShaderData UBO
	glGenBuffers(1, &shaderDataUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, shaderDataUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(ShaderData), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glGenBuffers(1, &shaderSkeletonUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, shaderSkeletonUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(ShaderSkeleton), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//Create Frame Buffer (FBO)
	glGenFramebuffers(1, &pickingFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, pickingFBO);


	//Generate texture
	glGenTextures(1, &pickingCBO);
	glBindTexture(GL_TEXTURE_2D, pickingCBO);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pickingCBO, 0);

	glGenRenderbuffers(1, &pickingRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, pickingRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, pickingRBO);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Create Frame Buffer (FBO)
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);


	//Generate texture
	glGenTextures(1, &cbo);
	glBindTexture(GL_TEXTURE_2D, cbo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cbo, 0);

	//Create RenderBuffer
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	tex = new Texture("Textures/testBW.png");

	unsigned int shaderDataIndex = glGetUniformBlockIndex(shader.shaderProgram, "shaderData");
	glUniformBlockBinding(shader.shaderProgram, shaderDataIndex, 0);
	shaderDataIndex = glGetUniformBlockIndex(wireframeShader.shaderProgram, "shaderData");
	glUniformBlockBinding(wireframeShader.shaderProgram, shaderDataIndex, 0);
	
	shaderDataIndex = glGetUniformBlockIndex(animShader.shaderProgram, "shaderData");
	glUniformBlockBinding(animShader.shaderProgram, shaderDataIndex, 0);

	unsigned int shaderSkeletonIndex = glGetUniformBlockIndex(animShader.shaderProgram, "shaderSkeleton");
	glUniformBlockBinding(animShader.shaderProgram, shaderSkeletonIndex, 1);

	/*if (animation.exist != false)
	{
		unsigned int shaderDataIndexAnimation = glGetUniformBlockIndex(animShader.shaderProgram, "shaderDataAnim");
		glUniformBlockBinding(animShader.shaderProgram, shaderDataIndexAnimation, 0);
	}*/

	/*unsigned int outLineShaderDataIndex = glGetUniformBlockIndex(outLineShader.shaderProgram, "shaderData");
	glUniformBlockBinding(outLineShader.shaderProgram, outLineShaderDataIndex, 0);*/

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, shaderDataUBO, 0, sizeof(ShaderData));
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, shaderSkeletonUBO, 0, sizeof(ShaderSkeleton));

	// setup collider lines VAO
	glGenVertexArrays(1, &linesVAO);
	glGenBuffers(1, &linesVBO);
	glBindVertexArray(linesVAO);
	glBindBuffer(GL_ARRAY_BUFFER, linesVBO);

	glGenBuffers(1, &pickingEBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pickingEBO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
		sizeof(reactphysics3d::Vector3) + sizeof(reactphysics3d::uint32),
		(void*)offsetof(reactphysics3d::DebugRenderer::DebugLine, point1));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_FALSE,
		sizeof(reactphysics3d::Vector3) + sizeof(reactphysics3d::uint32),
		(void*)offsetof(reactphysics3d::DebugRenderer::DebugLine, color1));

	// setup collider lines VAO
	glGenVertexArrays(1, &trianglesVAO);
	glGenBuffers(1, &trianglesVBO);
	glBindVertexArray(trianglesVAO);
	glBindBuffer(GL_ARRAY_BUFFER, trianglesVBO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		sizeof(reactphysics3d::Vector3) + sizeof(reactphysics3d::uint32),
		(void*)offsetof(reactphysics3d::DebugRenderer::DebugTriangle, point1));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_FALSE,
		sizeof(reactphysics3d::Vector3) + sizeof(reactphysics3d::uint32),
		(void*)offsetof(reactphysics3d::DebugRenderer::DebugTriangle, color1));

}

Renderer::~Renderer()
{
	glDeleteBuffers(1, &shaderDataUBO);
	glDeleteVertexArrays(1, &trianglesVAO);
	glDeleteVertexArrays(1, &linesVAO);
	glDeleteBuffers(1, &trianglesVBO);
	glDeleteBuffers(1, &linesVBO);

	delete tex;
}

void Renderer::SetStaticModels(const std::vector<ModelInstance>& inStaticModels)
{
	std::cout << "renderer.SetStaticModels" << std::endl;
	staticModels = inStaticModels;

	// Calculate matrix if needed
	for (ModelInstance& modelInst : staticModels)
	{
		//if (!modelInst.dirtyFlag) continue;

		mat4x4 modelMat = mat4::Identity();
		modelMat = modelMat * mat4::Translate(modelInst.position) * mat4::Rotate3f(modelInst.eulerAngles) * mat4::Scale(modelInst.scale);

		for (Part& part : modelInst.model->parts)
			modelInst.worldMatricesPerParts.push_back(modelMat * part.localMatrix);

		modelInst.dirtyFlag = false;
	}
}

void Renderer::SetStaticLights(const std::vector<Light>& inStaticLights)
{
	std::cout << "renderer.SetDirectionalLights" << std::endl;
	staticLights = inStaticLights;
}

void renderQuad()
{
	// init quad
	unsigned int quadVBO;
	float quadVertices[] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};
	// setup plane VAO
	GLuint quadVAO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

Vector3 UInt32ColorToRGB(uint32_t uintColor)
{
	int red = (uintColor & 0xFF000000) >> 24;
	int green = (uintColor & 0x00FF0000) >> 16;
	int blue = (uintColor & 0x0000FF00) >> 8;

	Vector3 color = { red / 255.f, green / 255.f, blue / 255.f };
	//HD_LOG("In: " + std::to_string(uintColor) + ", Out: {" + std::to_string(color.r) + ", " + std::to_string(color.g) + ", " + std::to_string(color.b) + "}", LogSeverity::LOG_MESSAGE);

	return color;
}

void Renderer::RenderPhysics()
{
	int linesNum = physicsRenderer->getNbLines();
	int trianglesNum = physicsRenderer->getNbTriangles();

	if (linesNum <= 0 || trianglesNum <= 0)
		return;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glUseProgram(wireframeShader.shaderProgram);

	const reactphysics3d::DebugRenderer::DebugLine* linesArray = physicsRenderer->getLinesArray();
	glBindBuffer(GL_ARRAY_BUFFER, linesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(reactphysics3d::DebugRenderer::DebugLine) * linesNum, linesArray, GL_STATIC_DRAW);

	const reactphysics3d::DebugRenderer::DebugTriangle* trianglesArray = physicsRenderer->getTrianglesArray();
	glBindBuffer(GL_ARRAY_BUFFER, trianglesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(reactphysics3d::DebugRenderer::DebugTriangle) * trianglesNum, trianglesArray, GL_STATIC_DRAW);

	glLineWidth(1.0f);

	glBindVertexArray(linesVAO);
	glDrawArrays(GL_LINES, 0, linesNum * 2);

	glBindVertexArray(trianglesVAO);
	glDrawArrays(GL_TRIANGLES, 0, trianglesNum * 3);

	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::Render(const mat4x4& view, const mat4x4& proj, const Vector2& screenSize, std::deque<GameObject*>& dynamicObjects, const std::deque<Light>& dynamicLights, bool isPlaying)
{
	double deltaTime = Time::DeltaTime();

	glEnable(GL_CULL_FACE | GL_DEPTH_TEST);

	if (screenSize.x != vpSize.x || screenSize.y != vpSize.y)
		UpdateFramebuffer(screenSize);

	glUseProgram(animShader.shaderProgram);

	for (int i = 0; i < 8 && i < dynamicLights.size(); i++)
		shaderData.lights[i] = dynamicLights[i];

	shaderData.viewMatrix = view;
	shaderData.projectionMatrix = proj;
	shaderData.camPos = view.c[3];

	shaderData.lights[0].lightEnabled = true;
	shaderData.lights[0].position = { 0.5f, 0.5f, -0.4f, 1.f };
	shaderData.lights[0].diffuse = { 1.0f, 0.1f, 0.f, 1.0f };

	
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.1f, 0.1f, 0.1f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindBuffer(GL_UNIFORM_BUFFER, shaderDataUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ShaderData), &shaderData);


	//HD_LOG("FPS: " + std::to_string(1.f / Time::DeltaTime()), LogSeverity::LOG_MESSAGE);
	/*for (ModelInstance& model : staticModels)
	{
		for (Part& part : model.model->parts)
		{
			mat4x4 modelMat = mat4::Identity();
			modelMat = modelMat * mat4::Translate(model.position) * mat4::Rotate3f(model.eulerAngles) * mat4::Scale(model.scale);
			modelMat = modelMat * part.localMatrix;

	
			glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "uModelMat"), 1, GL_FALSE, modelMat.e);

			
			part.material->diffuseTexture->BindResource();
			part.mesh->BindResource();
			
			glDrawElements(GL_TRIANGLES, (int)part.mesh->indices.size(), GL_UNSIGNED_INT, 0);
		}
	}*/

	for (GameObject* objectptr : dynamicObjects)
	{
		GameObject& object = *objectptr;
		ModelInstance* model = static_cast<ModelInstance*>(object.GetComponent(ComponentType::MODELINSTANCE));

		if (model != nullptr)
		{
			Animator* animator = static_cast<Animator*>(object.GetComponent(ComponentType::ANIMATOR));
			if (animator && animator->HasAnimation())
			{
				glBindBuffer(GL_UNIFORM_BUFFER, shaderSkeletonUBO);
				glBufferSubData(GL_UNIFORM_BUFFER, 0, animator->GetFinalBoneMatrices().size() * sizeof(mat4x4), animator->GetFinalBoneMatrices().data());
				animator->UpdateAnimation(deltaTime);
			}

			for (Part& part : model->model->parts)
			{
				mat4x4 modelMat = mat4::Identity();
				modelMat = modelMat * mat4::Translate(object.transform->position) * mat4::RotateQuat(object.transform->rotation) * mat4::Scale(object.transform->scale);
				modelMat = modelMat * part.localMatrix;

				glUniformMatrix4fv(animShader.modelMatLoc, 1, GL_FALSE, modelMat.e);

				model->mat.BindResource();
				//part.material->BindResource();

				part.mesh->BindResource();

				glDrawElements(GL_TRIANGLES, (int)part.mesh->indices.size(), GL_UNSIGNED_INT, 0);
			}
		}
	}

	if (physicsRenderer != nullptr && !isPlaying)
		RenderPhysics();

	glUseProgram(animShader.shaderProgram);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	glBindTexture(GL_TEXTURE_2D, cbo);
	glUniformMatrix4fv(animShader.modelMatLoc, 1, GL_FALSE, mat4::Identity().e);
	glUniformMatrix4fv(glGetUniformLocation(animShader.shaderProgram, "viewProj"), 1, GL_FALSE, mat4::Identity().e);


	//cbo = model.parts[0].material->diffuseTexture->gpu->texture;
	/*tex->BindResource();

	if (model.materialLoaded == NULL || model.meshLoaded == NULL)
	{
		HD_LOG("No 3D object is loaded", LogSeverity::LOG_ERROR);
		return;
	}

	model.parts.back().mesh->BindResource();

	glDrawElements(GL_TRIANGLES, model.parts.back().mesh->indices.size(), GL_UNSIGNED_INT, 0);
	*/

	//DrawPicking(view, proj,dynamicObjects);
}

void Renderer::DrawPicking(const mat4x4& view, const mat4x4& proj,std::deque<GameObject*>& dynamicObjects)
{
	glUseProgram(picking.shaderProgram);
	glBindBuffer(GL_FRAMEBUFFER, pickingFBO);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnableVertexAttribArray(0);



	GameObject* goPTR;
	for (int i = 0; i < dynamicObjects.size(); i++)
	{
		goPTR = dynamicObjects[i];
		ModelInstance* model = static_cast<ModelInstance*>(goPTR->GetComponent(ComponentType::MODELINSTANCE));

		for (Part& part : model->model->parts)
		{
			mat4x4 modelMat = mat4::Translate(goPTR->transform->position) * mat4::RotateQuat(goPTR->transform->rotation) * mat4::Scale(goPTR->transform->scale);
			mat4x4 MVP = proj * view * modelMat;

			glUniformMatrix4fv(glGetUniformLocation(picking.shaderProgram, "MVP"), 1, GL_FALSE, MVP.e);

			int r = (goPTR->id & 0x000000FF) >> 0;
			int g = (goPTR->id & 0x0000FF00) >> 8;
			int b = (goPTR->id & 0x00FF0000) >> 16;

			glUniform4f(glGetUniformLocation(picking.shaderProgram, "PickingColor"), r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);

			//model->mat.BindResource();
			//part.material->BindResource();

			//part.mesh->BindResource();
			glBindVertexArray(pickingVAO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pickingEBO);
			//glBufferData(GL_ELEMENT_ARRAY_BUFFER, (int)part.mesh->indices.size() * sizeof(unsigned short), &(int)part.mesh->indices[0], GL_STATIC_DRAW);


			glDrawElements(GL_TRIANGLES, (int)part.mesh->indices.size(), GL_UNSIGNED_INT, 0);

		}
	
	}

	glDisableVertexAttribArray(0);


	// Wait until all the pending drawing commands are really done.
	// Ultra-mega-over slow ! 
	// There are usually a long time between glDrawElements() and
	// all the fragments completely rasterized.
	glFlush();
	glFinish();


	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned char data[4];
	glReadPixels(1024 / 2, 768 / 2, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

	// Convert the color back to an integer ID
	int pickedID =
		data[0] +
		data[1] * 256 +
		data[2] * 256 * 256;

	if (pickedID == 0x00ffffff) { // Full white, must be the background !
		HD_LOG("backGround", LogSeverity::LOG_MESSAGE);
	}
	else {
		HD_LOG("Find Object number " + pickedID, LogSeverity::LOG_MESSAGE);
	}


	glBindBuffer(GL_FRAMEBUFFER, 0);

}


void Renderer::UpdateFramebuffer(const Vector2& newVpSize)
{
	vpSize = newVpSize;

	glDeleteTextures(1, &cbo);
	glDeleteRenderbuffers(1, &rbo);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &cbo);
	glBindTexture(GL_TEXTURE_2D, cbo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)(vpSize.x), (GLsizei)(vpSize.y), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cbo, 0);

	//Create RenderBuffer
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (GLsizei)(vpSize.x), (GLsizei)(vpSize.y));
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		HD_LOG("Framebuffer incomplete", LogSeverity::LOG_ERROR);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
