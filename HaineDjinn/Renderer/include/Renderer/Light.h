#pragma once

#include "Core/Maths.h"

struct Light
{
	Vector4 position = {};
	Vector4 ambient = {0.1f, 0.1f, 0.1f, 1.0f};
	Vector4 diffuse = {1.0f, 1.0f, 1.0f, 1.0f};
	Vector4 specular = {1.0f, 1.0f, 1.0f, 1.0f};
	Vector4 attenuation = { 0.1f, 0.01f, 0.01f, 1.0f };
	bool lightEnabled = false;
	Vector3 padding;
};