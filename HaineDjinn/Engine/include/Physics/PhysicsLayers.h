#pragma once

#include <string>

enum class CollisionLayers // Using enum instead of enum class due to reactphysics' implementation of layers
{
	LAYER1 = 1,
	LAYER2 = 2,
	LAYER3 = 3,

	LAYERS_SIZE
};


// Note: Need to manually add strings here
inline std::string GetLayerAsString(const CollisionLayers layer)
{
	switch (layer)
	{
	case CollisionLayers::LAYER1:
		return "Layer 1";
	case CollisionLayers::LAYER2:
		return "Layer 2";
	case CollisionLayers::LAYER3:
		return "Layer 3";
	default:
		return "Layer";
	}
}