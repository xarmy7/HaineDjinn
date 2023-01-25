#pragma once
#include <string>

void EditFileName(std::string& objName, char* buf, int size);

/*
static bool operator==(ImVec4 v1, ImVec4 v2)
{
	if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.w == v2.w)
		return true;
	else
		return false;
}
*/

/*
static void operator==(aiScene& obj1, aiScene& obj2)
{
	assert(false); // Si pas appelé, supprimer la fonction
	/*
	obj1.mAnimations = std::move(obj2.mAnimations);
	obj1.mCameras = std::move(obj2.mCameras);
	obj1.mFlags = obj2.mFlags;
	obj1.mLights = std::move(obj2.mLights);
	obj1.mMaterials = std::move(obj2.mMaterials);
	obj1.mMeshes = std::move(obj2.mMeshes);
	obj1.mMetaData = std::move(obj2.mMetaData);
	obj1.mName = obj2.mName;
	obj1.mNumAnimations = obj2.mNumAnimations;
	obj1.mNumCameras = obj2.mNumCameras;
	obj1.mNumLights = obj2.mNumLights;
	obj1.mNumMaterials = obj2.mNumMaterials;
	obj1.mNumMeshes = obj2.mNumMeshes;
	obj1.mNumTextures = obj2.mNumTextures;
	obj1.mPrivate = std::move(obj2.mPrivate);
	obj1.mRootNode = obj2.mRootNode;
	obj1.mTextures = std::move(obj2.mTextures);
	/
}*/